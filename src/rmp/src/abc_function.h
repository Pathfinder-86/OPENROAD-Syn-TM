#include <base/abc/abc.h>

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
};

}  // namespace rmp
