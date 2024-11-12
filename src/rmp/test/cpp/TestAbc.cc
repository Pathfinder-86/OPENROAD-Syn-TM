// Copyright 2023 Google LLC
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#include <tcl.h>
#include <unistd.h>

#include <array>
#include <filesystem>
#include <map>
#include <memory>
#include <mutex>
#include <set>

#include "abc_library_factory.h"
#include "base/abc/abc.h"
#include "base/io/ioAbc.h"
#include "base/main/abcapis.h"
#include "db_sta/MakeDbSta.hh"
#include "db_sta/dbReadVerilog.hh"
#include "db_sta/dbSta.hh"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "logic_extractor.h"
#include "map/mio/mio.h"
#include "map/scl/sclLib.h"
#include "odb/lefin.h"
#include "sta/FuncExpr.hh"
#include "sta/Graph.hh"
#include "sta/Liberty.hh"
#include "sta/NetworkClass.hh"
#include "sta/Sta.hh"
#include "sta/Units.hh"
#include "utl/Logger.h"
#include "utl/deleter.h"
#include "map/scl/sclSize.h"
#include "map/scl/sclLib.h"

//#include "base/abci/abcMap.c"


// Headers have duplicate declarations so we include
// a forward one to get at this function without angering
// gcc.
namespace abc {
void* Abc_FrameReadLibGen();
Abc_Ntk_t * Abc_NtkMap( Abc_Ntk_t * pNtk, double DelayTarget, double AreaMulti, double DelayMulti, float LogFan, float Slew, float Gain, int nGatesMin, int fRecovery, int fSwitching, int fSkipFanout, int fUseProfile, int fUseBuffs, int fVerbose );

}

namespace rmp {

using ::testing::Contains;

std::once_flag init_sta_flag;

class AbcTest : public ::testing::Test
{
 protected:
  void SetUp() override
  {
    db_ = utl::deleted_unique_ptr<odb::dbDatabase>(odb::dbDatabase::create(),
                                                   &odb::dbDatabase::destroy);
    std::call_once(init_sta_flag, []() {
      sta::initSta();
      abc::Abc_Start();
    });
    sta_ = std::unique_ptr<sta::dbSta>(ord::makeDbSta());
    sta_->initVars(Tcl_CreateInterp(), db_.get(), &logger_);
    auto path = std::filesystem::canonical("./Nangate45/Nangate45_fast.lib");
    library_ = sta_->readLiberty(path.string().c_str(),
                                 sta_->findCorner("default"),
                                 /*min_max=*/nullptr,
                                 /*infer_latches=*/false);

    odb::lefin lef_reader(
        db_.get(), &logger_, /*ignore_non_routing_layers=*/false);

    auto tech_lef
        = std::filesystem::canonical("./Nangate45/Nangate45_tech.lef");
    auto stdcell_lef
        = std::filesystem::canonical("./Nangate45/Nangate45_stdcell.lef");
    odb::dbTech* tech
        = lef_reader.createTech("nangate45", tech_lef.string().c_str());
    odb::dbLib* lib
        = lef_reader.createLib(tech, "nangate45", stdcell_lef.string().c_str());

    sta_->postReadLef(/*tech=*/nullptr, lib);

    sta::Units* units = library_->units();
    power_unit_ = units->powerUnit();
  }

  void LoadVerilog(const std::string& file_name, const std::string& top = "top")
  {
    // Assumes module name is "top" and clock name is "clk"
    sta::dbNetwork* network = sta_->getDbNetwork();
    ord::dbVerilogNetwork verilog_network;
    verilog_network.init(network);
    ord::dbReadVerilog(file_name.c_str(), &verilog_network);
    ord::dbLinkDesign(top.c_str(),
                      &verilog_network,
                      db_.get(),
                      &logger_,
                      /*hierarchy = */ false);

    sta_->postReadDb(db_.get());

    sta::Cell* top_cell = network->cell(network->topInstance());
    sta::Port* clk_port = network->findPort(top_cell, "clk");
    sta::Pin* clk_pin = network->findPin(network->topInstance(), clk_port);

    sta::PinSet* pinset = new sta::PinSet(network);
    pinset->insert(clk_pin);

    float period = 2.0;
    sta::FloatSeq* waveform = new sta::FloatSeq;
    waveform->push_back(0);
    waveform->push_back(period / 2.0);

    sta_->makeClock("clk",
                    pinset,
                    /*add_to_pins=*/false,
                    /*period=*/2.0,
                    waveform,
                    /*comment=*/nullptr);

    sta_->ensureGraph();
    sta_->ensureLevelized();
  }

  utl::deleted_unique_ptr<odb::dbDatabase> db_;
  sta::Unit* power_unit_;
  std::unique_ptr<sta::dbSta> sta_;
  sta::LibertyLibrary* library_;
  utl::Logger logger_;
};

TEST_F(AbcTest, CellPropertiesMatchOpenSta)
{
  AbcLibraryFactory factory(&logger_);
  factory.AddDbSta(sta_.get());
  AbcLibrary abc_library = factory.Build();

  for (size_t i = 0; i < Vec_PtrSize(&abc_library.abc_library()->vCells); i++) {
    abc::SC_Cell* abc_cell = static_cast<abc::SC_Cell*>(
        abc::Vec_PtrEntry(&abc_library.abc_library()->vCells, i));
    sta::LibertyCell* sta_cell = library_->findLibertyCell(abc_cell->pName);
    EXPECT_NE(nullptr, sta_cell);
    // Expect area matches
    EXPECT_FLOAT_EQ(abc_cell->area, sta_cell->area());

    float leakage_power = -1;
    bool exists;
    sta_cell->leakagePower(leakage_power, exists);
    if (exists) {
      EXPECT_FLOAT_EQ(abc_cell->leakage, power_unit_->staToUser(leakage_power));
    }
  }
}

TEST_F(AbcTest, DoesNotContainPhysicalCells)
{
  AbcLibraryFactory factory(&logger_);
  factory.AddDbSta(sta_.get());
  AbcLibrary abc_library = factory.Build();

  std::set<std::string> abc_cells;
  using ::testing::Contains;
  using ::testing::Not;

  for (size_t i = 0; i < Vec_PtrSize(&abc_library.abc_library()->vCells); i++) {
    abc::SC_Cell* abc_cell = static_cast<abc::SC_Cell*>(
        abc::Vec_PtrEntry(&abc_library.abc_library()->vCells, i));
    abc_cells.emplace(abc_cell->pName);
  }

  EXPECT_THAT(abc_cells, Not(Contains("ANTENNA_X1")));
  EXPECT_THAT(abc_cells, Not(Contains("FILLCELL_X1")));
}

TEST_F(AbcTest, DoesNotContainSequentialCells)
{
  AbcLibraryFactory factory(&logger_);
  factory.AddDbSta(sta_.get());
  AbcLibrary abc_library = factory.Build();

  std::set<std::string> abc_cells;
  using ::testing::Contains;
  using ::testing::Not;

  for (size_t i = 0; i < Vec_PtrSize(&abc_library.abc_library()->vCells); i++) {
    abc::SC_Cell* abc_cell = static_cast<abc::SC_Cell*>(
        abc::Vec_PtrEntry(&abc_library.abc_library()->vCells, i));
    abc_cells.emplace(abc_cell->pName);
  }

  EXPECT_THAT(abc_cells, Not(Contains("DFFRS_X2")));
}

TEST_F(AbcTest, ContainsLogicCells)
{
  AbcLibraryFactory factory(&logger_);
  factory.AddDbSta(sta_.get());
  AbcLibrary abc_library = factory.Build();

  std::set<std::string> abc_cells;
  using ::testing::Contains;
  using ::testing::Not;

  for (size_t i = 0; i < Vec_PtrSize(&abc_library.abc_library()->vCells); i++) {
    abc::SC_Cell* abc_cell = static_cast<abc::SC_Cell*>(
        abc::Vec_PtrEntry(&abc_library.abc_library()->vCells, i));
    abc_cells.emplace(abc_cell->pName);
  }

  EXPECT_THAT(abc_cells, Contains("AND2_X1"));
  EXPECT_THAT(abc_cells, Contains("AND2_X2"));
  EXPECT_THAT(abc_cells, Contains("AND2_X4"));
  EXPECT_THAT(abc_cells, Contains("AOI21_X1"));
}

// Create standard cell library from dbsta. Then create an
// abc network with a single and gate, and make sure that it
// simulates correctly.
TEST_F(AbcTest, TestLibraryInstallation)
{
  AbcLibraryFactory factory(&logger_);
  factory.AddDbSta(sta_.get());
  AbcLibrary abc_library = factory.Build();

  // When you set these params to zero they are essentially turned off.
  abc::Abc_SclInstallGenlib(
      abc_library.abc_library(), /*Slew=*/0, /*Gain=*/0, /*nGatesMin=*/0);
  abc::Mio_LibraryTransferCellIds();
  abc::Mio_Library_t* lib
      = static_cast<abc::Mio_Library_t*>(abc::Abc_FrameReadLibGen());

  std::map<std::string, abc::Mio_Gate_t*> gates;
  abc::Mio_Gate_t* gate = abc::Mio_LibraryReadGates(lib);
  while (gate) {
    gates[abc::Mio_GateReadName(gate)] = gate;
    gate = abc::Mio_GateReadNext(gate);
  }

  utl::deleted_unique_ptr<abc::Abc_Ntk_t> network(
      abc::Abc_NtkAlloc(abc::Abc_NtkType_t::ABC_NTK_NETLIST,
                        abc::Abc_NtkFunc_t::ABC_FUNC_MAP,
                        /*fUseMemMan=*/1),
      &abc::Abc_NtkDelete);
  abc::Abc_NtkSetName(network.get(), strdup("test_module"));

  abc::Abc_Obj_t* input_1 = abc::Abc_NtkCreatePi(network.get());
  abc::Abc_Obj_t* input_1_net = abc::Abc_NtkCreateNet(network.get());
  abc::Abc_Obj_t* input_2 = abc::Abc_NtkCreatePi(network.get());
  abc::Abc_Obj_t* input_2_net = abc::Abc_NtkCreateNet(network.get());
  abc::Abc_Obj_t* output = abc::Abc_NtkCreatePo(network.get());
  abc::Abc_Obj_t* output_net = abc::Abc_NtkCreateNet(network.get());
  abc::Abc_Obj_t* and_gate = abc::Abc_NtkCreateNode(network.get());

  abc::Abc_ObjSetData(and_gate, gates["AND2_X1"]);

  abc::Abc_ObjAddFanin(input_1_net, input_1);
  abc::Abc_ObjAddFanin(input_2_net, input_2);

  // Gate order is technically dependent on the order in which the port
  // appears in the Mio_Gate_t struct. In practice you should go a build
  // a port_name -> index map type thing to make sure the right ports
  // are connected.
  abc::Abc_ObjAddFanin(and_gate, input_1_net);  // A
  abc::Abc_ObjAddFanin(and_gate, input_2_net);  // B

  std::string output_name = "out";
  abc::Abc_ObjAssignName(output_net, output_name.data(), /*pSuffix=*/nullptr);
  abc::Abc_ObjAddFanin(output_net, and_gate);

  abc::Abc_ObjAddFanin(output, output_net);

  utl::deleted_unique_ptr<abc::Abc_Ntk_t> logic_network(
      abc::Abc_NtkToLogic(network.get()), &abc::Abc_NtkDelete);

  std::array<int, 2> input_vector = {1, 1};
  utl::deleted_unique_ptr<int> output_vector(
      abc::Abc_NtkVerifySimulatePattern(logic_network.get(),
                                        input_vector.data()),
      &free);

  EXPECT_EQ(output_vector.get()[0], 1);  // Expect that 1 & 1 == 1
}

TEST_F(AbcTest, ExtractsAndGateCorrectly)
{
  AbcLibraryFactory factory(&logger_);
  factory.AddDbSta(sta_.get());
  AbcLibrary abc_library = factory.Build();

  LoadVerilog("simple_and_gate_extract.v");

  sta::dbNetwork* network = sta_->getDbNetwork();
  sta::Vertex* flop_input_vertex = nullptr;
  for (sta::Vertex* vertex : *sta_->endpoints()) {
    if (std::string(vertex->name(network)) == "output_flop/D") {
      flop_input_vertex = vertex;
    }
  }
  EXPECT_NE(flop_input_vertex, nullptr);

  LogicExtractorFactory logic_extractor(sta_.get());
  logic_extractor.AppendEndpoint(flop_input_vertex);
  LogicCut cut = logic_extractor.BuildLogicCut(abc_library);

  EXPECT_EQ(cut.cut_instances().size(), 1);
  EXPECT_EQ(std::string(network->name(*cut.cut_instances().begin())), "_403_");
}

TEST_F(AbcTest, ExtractsEmptyCutSetCorrectly)
{
  AbcLibraryFactory factory(&logger_);
  factory.AddDbSta(sta_.get());
  AbcLibrary abc_library = factory.Build();

  LoadVerilog("empty_cut_set.v");

  sta::dbNetwork* network = sta_->getDbNetwork();
  sta::Vertex* flop_input_vertex = nullptr;
  for (sta::Vertex* vertex : *sta_->endpoints()) {
    if (std::string(vertex->name(network)) == "output_flop/D") {
      flop_input_vertex = vertex;
    }
  }
  EXPECT_NE(flop_input_vertex, nullptr);

  LogicExtractorFactory logic_extractor(sta_.get());
  logic_extractor.AppendEndpoint(flop_input_vertex);
  LogicCut cut = logic_extractor.BuildLogicCut(abc_library);

  EXPECT_TRUE(cut.IsEmpty());
}

TEST_F(AbcTest, ExtractSideOutputsCorrectly)
{
  AbcLibraryFactory factory(&logger_);
  factory.AddDbSta(sta_.get());
  AbcLibrary abc_library = factory.Build();

  LoadVerilog("side_outputs_extract.v");

  sta::dbNetwork* network = sta_->getDbNetwork();
  sta::Vertex* flop_input_vertex = nullptr;
  for (sta::Vertex* vertex : *sta_->endpoints()) {
    if (std::string(vertex->name(network)) == "output_flop/D") {
      flop_input_vertex = vertex;
    }
  }
  EXPECT_NE(flop_input_vertex, nullptr);

  LogicExtractorFactory logic_extractor(sta_.get());
  logic_extractor.AppendEndpoint(flop_input_vertex);
  LogicCut cut = logic_extractor.BuildLogicCut(abc_library);

  std::unordered_set<std::string> primary_output_names;
  for (sta::Pin* pin : cut.primary_outputs()) {
    primary_output_names.insert(network->name(pin));
  }

  EXPECT_EQ(cut.primary_outputs().size(), 2);
  EXPECT_THAT(primary_output_names, Contains("output_flop/D"));
  EXPECT_THAT(primary_output_names, Contains("output_flop2/D"));
}

int JH_ps(abc::Abc_Ntk_t * pNtk,int fFactor=0, int fSaveBest=0, int fDumpResult=0, int fUseLutLib=0, int fPrintMuxes=0, int fPower=0, int fGlitch=0, int fSkipBuf=0, int fSkipSmall=0, int fPrintMem=0){  
  if(pNtk==NULL){
    std::cout<<"There is no current network."<<std::endl;
    return 1;
  }
  abc::Abc_NtkPrintStats( pNtk, fFactor, fSaveBest, fDumpResult, fUseLutLib, fPrintMuxes, fPower, fGlitch, fSkipBuf, fSkipSmall, fPrintMem );
  return 0;
}

abc::Abc_Ntk_t * JH_topo(abc::Abc_Ntk_t * pNtk,int fVerbose=0){
  if ( pNtk == NULL )
  {
      abc::Abc_Print( -1, "Empty network.\n" );
      return pNtk;
  }
  if ( !abc::Abc_NtkIsLogic(pNtk) )
  {
      abc::Abc_Print( -1, "This command can only be applied to a logic network.\n" );
      return pNtk;
  }

  // modify the current network
  return abc::Abc_NtkDupDfs( pNtk );
}

int JH_stime(abc::Abc_Ntk_t * pNtk,int fShowAll=0,int fUseWireLoads = 1,int fPrintPath = 0,int fDumpStats =0,int nTreeCRatio =0)
{
    if ( pNtk == NULL )
    {
        std::cout<<"There is no current network."<<std::endl;
        return 1;
    }
    if ( !Abc_NtkHasMapping(pNtk) )
    {
        std::cout<<"The current network is not mapped."<<std::endl;
        return 1;
    }
    if ( !Abc_SclCheckNtk(pNtk, 0) )
    {
        std::cout<<"The current network is not in a topo order (run \"topo\")."<<std::endl;
        return 1;
    }

    abc::SC_Lib * sc_lib = abc::Abc_SclReadFromGenlib(abc::Abc_FrameReadLibGen());
    if( sc_lib == NULL )
    {
        std::cout<<"There is no SC Liberty library available."<<std::endl;
        return 1;
    }

    std::cout<<"Abc_SclTimePerform"<<std::endl;
    Abc_SclTimePerform( sc_lib, pNtk, nTreeCRatio, fUseWireLoads, fShowAll, fPrintPath, fDumpStats );
    return 0;

}

abc::Abc_Ntk_t * JH_buffer( abc::Abc_Ntk_t * pNtk,int GainRatio=300,int Slew=100,int nDegree=10,int fSizeOnly=0,
int fAddBufs=1,int fBufPis=0,int fUseWireLoads=1,int fVerbose=1,int fVeryVerbose=0)
{
    abc::SC_BusPars Pars, * pPars = &Pars;    
    memset( pPars, 0, sizeof(abc::SC_BusPars) );
    pPars->GainRatio     =  GainRatio;
    pPars->Slew          = Slew;
    pPars->nDegree       = nDegree;
    pPars->fSizeOnly     = fSizeOnly;
    pPars->fAddBufs      = fAddBufs;
    pPars->fBufPis       = fBufPis;
    pPars->fUseWireLoads = fUseWireLoads;
    pPars->fVerbose      = fVerbose;
    pPars->fVeryVerbose  = fVeryVerbose;

    if ( pNtk == NULL )
    {
        std::cout<<"Empty network."<<std::endl;
        return pNtk;
    }
    if ( !abc::Abc_NtkIsLogic(pNtk) )
    {
        std::cout<<"This command can only be applied to a logic network."<<std::endl;
        return pNtk;
    }
    if ( !pPars->fSizeOnly && !pPars->fAddBufs && pNtk->vPhases == NULL )
    {
        std::cout<<"Fanin phase information is not available."<<std::endl;
        return pNtk;
    }

    abc::SC_Lib * sc_lib = abc::Abc_SclReadFromGenlib(abc::Abc_FrameReadLibGen());
    if( sc_lib == NULL )
    {
        std::cout<<"There is no SC Liberty library available."<<std::endl;
        return pNtk;
    }

    if ( !abc::Abc_SclHasDelayInfo(sc_lib) )
    {
        std::cout<<"Library delay info is not available."<<std::endl;
        return pNtk;
    }
    // modify the current network
    std::cout<<"Abc_SclBufferingPerform"<<std::endl;
    return Abc_SclBufferingPerform( pNtk, sc_lib, pPars );
}



int JH_upsize( abc::Abc_Ntk_t * pNtk, int nIters= 1000, int nIterNoChange=50, int Window=1, int Ratio=10, int Notches=1000, 
double DelayUser=0, double DelayGap=0, int TimeOut=0, int BuffTreeEst=0, int BypassFreq=0, int fUseDept=1, int fUseWireLoads=1, 
int fDumpStats=1, int fVerbose=1, int fVeryVerbose=0)
{    
    abc::SC_SizePars Pars, * pPars = &Pars;    
    memset( pPars, 0, sizeof(abc::SC_SizePars) );
    pPars->nIters        = nIters;
    pPars->nIterNoChange = nIterNoChange;
    pPars->Window        = Window;
    pPars->Ratio         = Ratio;
    pPars->Notches       = Notches;
    pPars->DelayUser     = DelayUser;
    pPars->DelayGap      = DelayGap;
    pPars->TimeOut       = TimeOut;
    pPars->BuffTreeEst   = BuffTreeEst;
    pPars->BypassFreq    = BypassFreq;
    pPars->fUseDept      = fUseDept; 
    pPars->fUseWireLoads = fUseWireLoads;
    pPars->fDumpStats    = fDumpStats;
    pPars->fVerbose      = fVerbose;
    pPars->fVeryVerbose  = fVeryVerbose;

    if ( pNtk == NULL )
    {
        std::cout<<"There is no current network."<<std::endl;
        return 1;
    }
    if ( !abc::Abc_NtkHasMapping(pNtk) )
    {
        std::cout<<"The current network is not mapped."<<std::endl;
        return 1;
    }
    if ( !abc::Abc_SclCheckNtk(pNtk, 0) )
    {
        std::cout<<"The current network is not in a topo order (run \"topo\")."<<std::endl;
        return 1;
    }

    abc::SC_Lib * sc_lib = abc::Abc_SclReadFromGenlib(abc::Abc_FrameReadLibGen());
    if( sc_lib == NULL )
    {
        std::cout<<"There is no SC Liberty library available."<<std::endl;
        return 1;
    }

    std::cout<<"Abc_SclUpsizePerform"<<std::endl;
    Abc_SclUpsizePerform( sc_lib, pNtk, pPars, NULL );
    return 0;
}


int JH_dnsize( abc::Abc_Ntk_t * pNtk, int nIters= 1000, int nIterNoChange=50, int Window=1, int Ratio=10, int Notches=1000, 
double DelayUser=0, double DelayGap=0, int TimeOut=0, int BuffTreeEst=0, int BypassFreq=0, int fUseDept=1, int fUseWireLoads=1, 
int fDumpStats=1, int fVerbose=1, int fVeryVerbose=0)
{
    abc::SC_SizePars Pars, * pPars = &Pars;    
    memset( pPars, 0, sizeof(abc::SC_SizePars) );
    pPars->nIters        = nIters;
    pPars->nIterNoChange = nIterNoChange;
    pPars->Window        = Window;
    pPars->Ratio         = Ratio;
    pPars->Notches       = Notches;
    pPars->DelayUser     = DelayUser;
    pPars->DelayGap      = DelayGap;
    pPars->TimeOut       = TimeOut;
    pPars->BuffTreeEst   = BuffTreeEst;
    pPars->BypassFreq    = BypassFreq;
    pPars->fUseDept      = fUseDept; 
    pPars->fUseWireLoads = fUseWireLoads;
    pPars->fDumpStats    = fDumpStats;
    pPars->fVerbose      = fVerbose;
    pPars->fVeryVerbose  = fVeryVerbose;

    if ( pNtk == NULL )
    {
        std::cout<<"There is no current network."<<std::endl;
        return 1;
    }
    if ( !abc::Abc_NtkHasMapping(pNtk) )
    {
        std::cout<<"The current network is not mapped."<<std::endl;
        return 1;
    }
    if ( !abc::Abc_SclCheckNtk(pNtk, 0) )
    {
        std::cout<<"The current network is not in a topo order (run \"topo\")."<<std::endl;
        return 1;
    }

    abc::SC_Lib * sc_lib = abc::Abc_SclReadFromGenlib(abc::Abc_FrameReadLibGen());
    if( sc_lib == NULL )
    {
        std::cout<<"There is no SC Liberty library available."<<std::endl;
        return 1;
    }

    std::cout<<"Abc_SclDnsizePerform"<<std::endl;
    Abc_SclDnsizePerform( sc_lib, pNtk, pPars, NULL );    
    return 0;
}


TEST_F(AbcTest, BuildAbcMappedNetworkFromLogicCut)
{
  AbcLibraryFactory factory(&logger_);
  factory.AddDbSta(sta_.get());
  AbcLibrary abc_library = factory.Build();

  LoadVerilog("side_outputs_extract_logic_depth.v");

  sta::dbNetwork* network = sta_->getDbNetwork();
  sta::Vertex* flop_input_vertex = nullptr;
  for (sta::Vertex* vertex : *sta_->endpoints()) {
    if (std::string(vertex->name(network)) == "output_flop/D") {
      flop_input_vertex = vertex;
    }
  }
  EXPECT_NE(flop_input_vertex, nullptr);

  LogicExtractorFactory logic_extractor(sta_.get());
  logic_extractor.AppendEndpoint(flop_input_vertex);
  LogicCut cut = logic_extractor.BuildLogicCut(abc_library);

  utl::deleted_unique_ptr<abc::Abc_Ntk_t> abc_network
      = cut.BuildMappedAbcNetwork(abc_library, network, &logger_);

  abc::Abc_NtkSetName(abc_network.get(), strdup("temp_network_name"));

  utl::deleted_unique_ptr<abc::Abc_Ntk_t> logic_network(
      abc::Abc_NtkToLogic(abc_network.get()), &abc::Abc_NtkDelete);


  //map 
  std::cout<<"Abc_NtkStrash:"<<std::endl;
  logic_network.reset( abc::Abc_NtkStrash(logic_network.get(), 0, 0, 0));  
  JH_ps(logic_network.get());
  // set the library
  //abc::Abc_SclInstallGenlib(abc_library.abc_library(), /*Slew=*/0, /*Gain=*/0, /*nGatesMin=*/0);
  abc::Abc_SclInstallGenlib(abc_library.abc_library(), 0, 0, 0);

  logic_network.reset(Abc_NtkMap(logic_network.get(), 1, 0, 0, 0, 0, 250, 0, 1, 0, 0, 0, 0, 0));
  std::cout<<"AFTER MAP PS:"<<std::endl;
  JH_ps(logic_network.get());


  //topo
  logic_network.reset(JH_topo(logic_network.get()));
  if(logic_network.get()==NULL){
    std::cout<<"TOPO FAILED"<<std::endl;
  }else{
    std::cout<<"AFTER TOPO:"<<std::endl;
    JH_ps(logic_network.get());
  }
  
  //stime
  if(JH_stime(logic_network.get())){
    std::cout<<"STIME FAILED"<<std::endl;
  }else{
    std::cout<<"AFTER STIME:"<<std::endl;
    JH_ps(logic_network.get());
  }

  // buffer
  logic_network.reset(JH_buffer(logic_network.get()));
  if(logic_network.get()==NULL){
    std::cout<<"BUFFER FAILED"<<std::endl;
  }else{
    std::cout<<"AFTER BUFFER PS:"<<std::endl;
    JH_ps(logic_network.get());
  }
  
  //stime
  if(JH_stime(logic_network.get())){
    std::cout<<"STIME FAILED"<<std::endl;
  }else{
    std::cout<<"AFTER STIME:"<<std::endl;
    JH_ps(logic_network.get());
  }

  //upsizing
  if(JH_upsize(logic_network.get())){
    std::cout<<"UPSIZING FAILED"<<std::endl;
  }else{
    std::cout<<"AFTER UPSIZE PS:"<<std::endl;
    JH_ps(logic_network.get());
  }

  //stime
  if(JH_stime(logic_network.get())){
    std::cout<<"STIME FAILED"<<std::endl;
  }else{
    std::cout<<"AFTER STIME:"<<std::endl;
    JH_ps(logic_network.get());
  }

  //dnsizing
  if(JH_dnsize(logic_network.get())){
    std::cout<<"DNSIZING FAILED"<<std::endl;
  }else{
    std::cout<<"AFTER DNSIZE PS:"<<std::endl;
    JH_ps(logic_network.get());
  }

  //stime
  if(JH_stime(logic_network.get())){
    std::cout<<"STIME FAILED"<<std::endl;
  }else{
    std::cout<<"AFTER STIME:"<<std::endl;
    JH_ps(logic_network.get());
  }









  // abc --> openroad
  std::array<int, 2> input_vector = {1, 1};
  utl::deleted_unique_ptr<int> output_vector(
      abc::Abc_NtkVerifySimulatePattern(logic_network.get(),
                                        input_vector.data()),
      &free);

  // Both outputs are just the and gate.
  EXPECT_EQ(output_vector.get()[0], 0);  // Expect that !(1 & 1) == 0
  EXPECT_EQ(output_vector.get()[1], 0);  // Expect that !(1 & 1) == 0
}

TEST_F(AbcTest, BuildComplexLogicCone)
{
  AbcLibraryFactory factory(&logger_);
  factory.AddDbSta(sta_.get());
  AbcLibrary abc_library = factory.Build();

  LoadVerilog("aes_nangate45.v", /*top=*/"aes_cipher_top");

  sta::dbNetwork* network = sta_->getDbNetwork();
  sta::Vertex* flop_input_vertex = nullptr;
  for (sta::Vertex* vertex : *sta_->endpoints()) {
    if (std::string(vertex->name(network)) == "_32989_/D") {
      flop_input_vertex = vertex;
    }
  }
  EXPECT_NE(flop_input_vertex, nullptr);

  LogicExtractorFactory logic_extractor(sta_.get());
  logic_extractor.AppendEndpoint(flop_input_vertex);
  LogicCut cut = logic_extractor.BuildLogicCut(abc_library);

  EXPECT_NO_THROW(cut.BuildMappedAbcNetwork(abc_library, network, &logger_));
}
}  // namespace rmp
