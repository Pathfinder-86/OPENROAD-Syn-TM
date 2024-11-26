
#include <base/abc/abc.h>
#include <base/main/main.h>
#include <base/main/mainInt.h>
#include <proof/fraig/fraig.h>
#include <opt/fxu/fxu.h>
#include <opt/fxch/Fxch.h>
#include <opt/cut/cut.h>
#include <map/fpga/fpga.h>
#include <map/if/if.h>
#include <opt/sim/sim.h>
#include <opt/res/res.h>
#include <opt/lpk/lpk.h>
#include <aig/gia/giaAig.h>
#include <opt/dar/dar.h>
#include <opt/mfs/mfs.h>
#include <proof/fra/fra.h>
#include <aig/saig/saig.h>
#include <proof/int/int.h>
#include <proof/dch/dch.h>
#include <proof/ssw/ssw.h>
#include <opt/cgt/cgt.h>
#include <bool/kit/kit.h>
#include <map/amap/amap.h>
#include <opt/ret/retInt.h>
#include <sat/xsat/xsat.h>
#include <sat/satoko/satoko.h>
#include <sat/cnf/cnf.h>
#include <proof/cec/cec.h>
#include <proof/acec/acec.h>
#include <proof/pdr/pdr.h>
#include <misc/tim/tim.h>
#include <bdd/llb/llb.h>
#include <bdd/bbr/bbr.h>
#include <map/cov/cov.h>
#include <base/cmd/cmd.h>
#include <proof/abs/abs.h>
#include <sat/bmc/bmc.h>
#include <proof/ssc/ssc.h>
#include <opt/sfm/sfm.h>
#include <opt/sbd/sbd.h>
#include <bool/rpo/rpo.h>
#include <map/mpm/mpm.h>
#include <opt/fret/fretime.h>
#include <opt/nwk/nwkMerge.h>
#include <base/acb/acbPar.h>
#include <misc/extra/extra.h>
#include <map/scl/sclCon.h>
#include <map/scl/sclSize.h>

#include<iostream>



namespace abc {

class ABC_function {
public:
  static int JH_ps(Abc_Ntk_t * pNtk,int fFactor=0, int fSaveBest=0, int fDumpResult=0, int fUseLutLib=0, int fPrintMuxes=0, int fPower=0, int fGlitch=0, int fSkipBuf=0, int fSkipSmall=0, int fPrintMem=0);
  static Abc_Ntk_t * JH_topo(Abc_Ntk_t * pNtk,int fVerbose=0);
  static Abc_Ntk_t * JH_buffer( Abc_Ntk_t * pNtk,int GainRatio=300,int Slew=100,int nDegree=10,int fSizeOnly=0,int fAddBufs=1,int fBufPis=0,int fUseWireLoads=1,int fVerbose=0,int fVeryVerbose=0);
  static int JH_stime(Abc_Ntk_t * pNtk,int fShowAll=0,int fUseWireLoads = 1,int fPrintPath = 0,int fDumpStats =0,int nTreeCRatio =0);
  static int JH_dnsize( Abc_Ntk_t * pNtk, int nIters= 1000, int nIterNoChange=50, int Window=1, int Ratio=10, int Notches=1000, double DelayUser=0, double DelayGap=0, int TimeOut=0, int BuffTreeEst=0, int BypassFreq=0, int fUseDept=1, int fUseWireLoads=1, int fDumpStats=1, int fVerbose=0, int fVeryVerbose=0);
  static int JH_upsize( Abc_Ntk_t * pNtk, int nIters= 1000, int nIterNoChange=50, int Window=1, int Ratio=10, int Notches=1000, double DelayUser=0, double DelayGap=0, int TimeOut=0, int BuffTreeEst=0, int BypassFreq=0, int fUseDept=1, int fUseWireLoads=1, int fDumpStats=1, int fVerbose=0, int fVeryVerbose=0);
  // resyn2 command
  //alias resyn2      "b; rw; rf; b; rw; rwz; b; rfz; rwz; b"
  // balance, rewrite, refactor, balance, rewrite
  // rewrite -z, balance, refactor -z, rewrite -z balance
  static Abc_Ntk_t * JH_resyn2(Abc_Ntk_t * pNtk);  
  static Abc_Ntk_t * JH_rewrite(Abc_Ntk_t * pNtk,int fUseZeros = 0);
  static Abc_Ntk_t * JH_refactor(Abc_Ntk_t * pNtk,int fUseZeros = 0);
  static Abc_Ntk_t * JH_balance(Abc_Ntk_t * pNtk);



  //abc commands
  static int Abc_CommandPrintStats( Abc_Frame_t * pAbc, int fFactor = 0,int fSaveBest = 0,int fDumpResult = 0,int fUseLutLib = 0,int fPrintTime = 0,int fPrintMuxes = 0,int fPower = 0,int fGlitch = 0,int fSkipBuf = 0,int fSkipSmall = 0,int fPrintMem = 0 );
  static int Abc_CommandPrintCone( Abc_Frame_t * pAbc, int fUseLibrary = 1);
  static int Abc_CommandPrintMiter( Abc_Frame_t * pAbc, int fUseLibrary = 1);
  static int Abc_CommandPrintStatus( Abc_Frame_t * pAbc, int fOutStatus = 0, int fShort = 1, char * pLogFileName = NULL);
  static int Abc_CommandShow( Abc_Frame_t * pAbc, int fSeq= 0, int fGateNames= 0, int fUseReverse= 1, int fFlopDep= 0, int fKeepDot= 0);
  
  static int Abc_CommandCollapse( Abc_Frame_t * pAbc,int fVerbose = 0, int fBddSizeMax= ABC_INFINITY, int fDualRail= 0, int fReorder= 1, int fReverse= 0, int fDumpOrder= 0);
  static int Abc_CommandStrash( Abc_Frame_t * pAbc, int fAllNodes= 0, int fRecord= 0, int fCleanup= 1, int fComplOuts= 0);
  static int Abc_CommandLogic( Abc_Frame_t * pAbc);
  static int Abc_CommandBalance( Abc_Frame_t * pAbc, int fDuplicate= 0, int fSelective= 0, int fUpdateLevel= 1, int fExor= 0, int fVerbose= 0);
  static int Abc_CommandSweep( Abc_Frame_t * pAbc, int fSingle = 0, int fVerbose = 0);
  static int Abc_CommandRewrite( Abc_Frame_t * pAbc, int fUpdateLevel= 1, int fPrecompute= 0, int fUseZeros= 0, int fVerbose= 0, int fVeryVerbose= 0, int fPlaceEnable= 0);
  static int Abc_CommandRefactor( Abc_Frame_t * pAbc, int nNodeSizeMax= 10, int nMinSaved=  1, int nConeSizeMax= 16, int fUpdateLevel=  1, int fUseZeros=  0, int fUseDcs=  0, int fVerbose=  0);
  static int Abc_CommandResubstitute( Abc_Frame_t * pAbc, int RS_CUT_MIN =  4, int RS_CUT_MAX = 16, int nCutsMax=  8, int nNodesMax=  1, int nLevelsOdc=  0, int nMinSaved=  1, int fUpdateLevel=  1, int fUseZeros=  0, int fVerbose=  0, int fVeryVerbose= 0);
  static int Abc_CommandResubCheck( Abc_Frame_t * pAbc, char * pFileR=NULL, char *pFileS=NULL, int fVerbose=0);
  static int Abc_CommandMiter( Abc_Frame_t * pAbc, char* file1, char* file2, int fCheck= 1, int fComb= 0, int fImplic= 0, int fMulti= 0, int nPartSize= 0, int fTrans= 0, int fIgnoreNames= 0);
  static int Abc_CommandSop( Abc_Frame_t * pAbc, int fCubeSort = 1, int fMode = -1, int nCubeLimit= 1000000);
  static int Abc_CommandBdd( Abc_Frame_t * pAbc, int fReorder = 1, int fBdd2Sop = 0);//bdd
  static int Abc_CommandReorder( Abc_Frame_t * pAbc, int fVerbose = 0); //bdd
  static int Abc_CommandMap( Abc_Frame_t * pAbc, double DelayTarget= -1, double AreaMulti= 0, double DelayMulti= 0, float LogFan = 0, float Slew = 0, float Gain = 250, int nGatesMin = 0, int fAreaOnly= 1, int fRecovery= 0, int fSweep= 0, int fSwitching= 0, int fSkipFanout= 0, int fUseProfile= 0, int fUseBuffs= 0, int fVerbose= 0);
  static int Abc_CommandAmap( Abc_Frame_t * pAbc, float fADratio, int fSweep = 0, int nIterFlow= 1, int nIterArea= 4, int nCutsMax= 500, int fUseMuxes = 0, int fUseXors = 1, int fFreeInvs = 0, float fEpsilon = 0.001, int fVerbose = 0);
  static int Abc_CommandCec( Abc_Frame_t * pAbc, Abc_Ntk_t * pNtk1, Abc_Ntk_t * pNtk2, int fSat=  0, int fVerbose=  0, int nSeconds=  20, int nPartSize=  0, int nConfLimit=  0, int nInsLimit=  10000, int fPartition=  0, int fIgnoreNames=  0);
  
  //gia
  static int Abc_CommandAbc9Get( Abc_Frame_t * pAbc, int fGiaSimple = 0, int fMapped = 0, int fNames = 0, int fVerbose = 0);
  static int Abc_CommandAbc9Put( Abc_Frame_t * pAbc, int fStatusClear = 1, int fFindEnables = 0, int fUseBuffs    = 0, int fVerbose  = 0);
  static int Abc_CommandAbc9Ps( Abc_Frame_t * pAbc, int fBest = 0, int fTents = 0, int fSwitch = 0, int fCut = 0, int fNpn = 0, int fLutProf = 0, int fMuxXor = 0, int fMiter = 0, int fSlacks = 0, int fSkipMap = 0, int fNoColor = 0, char * pDumpFile = NULL);
  static int Abc_CommandAbc9Resub( Abc_Frame_t * pAbc, int nNodes = 0, int nSupp =  0, int nDivs =  0, int fVerbose = 0, int fVeryVerbose = 0);
  static int Abc_CommandAbc9Syn2( Abc_Frame_t * pAbc, int fVerbose  =  0, int fOldAlgo =  0, int fCoarsen =  1, int fCutMin =  0, int nRelaxRatio = 20, int fDelayMin  = 0, int fVeryVerbose = 0);
  static int Abc_CommandAbc9Syn3( Abc_Frame_t * pAbc, int fVerbose = 0, int fVeryVerbose = 0);
  static int Abc_CommandAbc9DeepSyn( Abc_Frame_t * pAbc, int nIters = 1, int nNoImpr = ABC_INFINITY, int TimeOut = 0, int nAnds = 0, int Seed = 0, int fUseTwo = 0, int fVerbose = 0);
  static int Abc_CommandAbc9StochSyn( Abc_Frame_t * pAbc, int nMaxSize = 1000, int nIters = 10, int TimeOut = 0, int Seed = 0, int nProcs = 1, int fVerbose = 0, char * pScript = NULL);

  //scl
  static int Scl_CommandLeak2Area( Abc_Frame_t * pAbc, float A = 1, float B = 1, int fVerbose = 0);
  static int Scl_CommandTopo( Abc_Frame_t * pAbc, int fVerbose = 0);
  static int Scl_CommandUpsize( Abc_Frame_t * pAbc);//check par
  static int Scl_CommandDnsize( Abc_Frame_t * pAbc);//check par
  static int Scl_CommandUnBuffer( Abc_Frame_t * pAbc, int fRemInv = 0, int fVerbose = 0);
  static int Scl_CommandBuffer( Abc_Frame_t * pAbc);//check par


};
 //abc commands

}  // namespace rmp
