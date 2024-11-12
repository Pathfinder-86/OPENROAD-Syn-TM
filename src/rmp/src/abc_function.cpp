#include "abc_function.h"
#include "map/scl/sclSize.h"
#include "map/scl/sclLib.h"
#include <iostream>

namespace abc {
void* Abc_FrameReadLibGen();
}


namespace rmp{

int ABC_function::JH_ps(abc::Abc_Ntk_t * pNtk,int fFactor, int fSaveBest, int fDumpResult, int fUseLutLib, 
int fPrintMuxes, int fPower, int fGlitch, int fSkipBuf, int fSkipSmall, int fPrintMem){  
  if(pNtk==NULL){
    std::cout<<"There is no current network."<<std::endl;
    return 1;
  }
  abc::Abc_NtkPrintStats( pNtk, fFactor, fSaveBest, fDumpResult, fUseLutLib, fPrintMuxes, fPower, fGlitch, fSkipBuf, fSkipSmall, fPrintMem );
  return 0;
}

abc::Abc_Ntk_t * ABC_function::JH_topo(abc::Abc_Ntk_t * pNtk,int fVerbose){
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

int ABC_function::JH_stime(abc::Abc_Ntk_t * pNtk,int fShowAll,int fUseWireLoads ,int fPrintPath ,int fDumpStats ,int nTreeCRatio )
{
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

    std::cout<<"Abc_SclTimePerform"<<std::endl;
    abc::Abc_SclTimePerform( sc_lib, pNtk, nTreeCRatio, fUseWireLoads, fShowAll, fPrintPath, fDumpStats );
    return 0;

}

abc::Abc_Ntk_t * ABC_function::JH_buffer( abc::Abc_Ntk_t * pNtk,int GainRatio,int Slew,int nDegree,int fSizeOnly,
int fAddBufs,int fBufPis,int fUseWireLoads,int fVerbose,int fVeryVerbose)
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
    return abc::Abc_SclBufferingPerform( pNtk, sc_lib, pPars );
}



int ABC_function::JH_upsize( abc::Abc_Ntk_t * pNtk, int nIters, int nIterNoChange, int Window, int Ratio, int Notches, 
double DelayUser, double DelayGap, int TimeOut, int BuffTreeEst, int BypassFreq, int fUseDept, int fUseWireLoads, 
int fDumpStats, int fVerbose, int fVeryVerbose)
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
    abc::Abc_SclUpsizePerform( sc_lib, pNtk, pPars, NULL );
    return 0;
}


int ABC_function::JH_dnsize( abc::Abc_Ntk_t * pNtk, int nIters, int nIterNoChange, int Window, int Ratio, int Notches, 
double DelayUser, double DelayGap, int TimeOut, int BuffTreeEst, int BypassFreq, int fUseDept, int fUseWireLoads, 
int fDumpStats, int fVerbose, int fVeryVerbose)
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
    abc::Abc_SclDnsizePerform( sc_lib, pNtk, pPars, NULL );    
    return 0;
}

}