// #include <base/abc/abc.h>
// #include <base/main/mainInt.h>
// #include <base/main/main.h>
// #include <aig/gia/gia.h>
// #include <aig/gia/giaAig.h>


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

  static int ps_gia(Gia_Man_t * pGia );
  static Gia_Man_t * get_gia(Abc_Ntk_t *pNtk, int fGiaSimple, int fMapped, int fNames, int fVerbose);
  static Abc_Ntk_t* put_gia(Abc_Ntk_t* pNtk, Gia_Man_t *pGia);
  static Gia_Man_t * deepsyn_int(Gia_Man_t *pGia);
  static Abc_Ntk_t * deepsyn(Abc_Ntk_t *pNtk);

  // int Abc_CommandPrintStats( Abc_Frame_t * pAbc, int argc, char ** argv );
  // int Abc_CommandAbc9Ps( Abc_Frame_t * pAbc, int argc, char ** argv );
  // int Abc_CommandAbc9Get( Abc_Frame_t * pAbc, int argc, char ** argv );

};

}  // namespace rmp
