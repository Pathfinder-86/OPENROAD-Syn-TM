#include <base/abc/abc.h>

namespace abc {

struct JH_ps_params {
  int fFactor;
  int fSaveBest;
  int fDumpResult;
  int fUseLutLib;
  int fPrintMuxes;
  int fPower;
  int fGlitch;
  int fSkipBuf;
  int fSkipSmall;
  int fPrintMem;

  JH_ps_params() : fFactor(0), fSaveBest(0), fDumpResult(0), fUseLutLib(0), fPrintMuxes(0), fPower(0), fGlitch(0), fSkipBuf(0), fSkipSmall(0), fPrintMem(0) {}
};

struct JH_buffer_params {
  int GainRatio;
  int Slew;
  int nDegree;
  int fSizeOnly;
  int fAddBufs;
  int fBufPis;
  int fUseWireLoads;
  int fVerbose;
  int fVeryVerbose;

  JH_buffer_params() : GainRatio(300), Slew(100), nDegree(10), fSizeOnly(0), fAddBufs(1), fBufPis(0), fUseWireLoads(1), fVerbose(0), fVeryVerbose(0) {}
};

struct JH_stime_params {
  int fShowAll;
  int fUseWireLoads;
  int fPrintPath;
  int fDumpStats;
  int nTreeCRatio;

  JH_stime_params() : fShowAll(0), fUseWireLoads(1), fPrintPath(0), fDumpStats(0), nTreeCRatio(0) {}
};

struct JH_dnsize_params {
  int nIters;
  int nIterNoChange;
  int Window;
  int Ratio;
  int Notches;
  double DelayUser;
  double DelayGap;
  int TimeOut;
  int BuffTreeEst;
  int BypassFreq;
  int fUseDept;
  int fUseWireLoads;
  int fDumpStats;
  int fVerbose;
  int fVeryVerbose;

  JH_dnsize_params() : nIters(1000), nIterNoChange(50), Window(1), Ratio(10), Notches(1000), DelayUser(0), DelayGap(0), TimeOut(0), BuffTreeEst(0), BypassFreq(0), fUseDept(1), fUseWireLoads(1), fDumpStats(1), fVerbose(0), fVeryVerbose(0) {}
};

struct JH_upsize_params {
  int nIters;
  int nIterNoChange;
  int Window;
  int Ratio;
  int Notches;
  double DelayUser;
  double DelayGap;
  int TimeOut;
  int BuffTreeEst;
  int BypassFreq;
  int fUseDept;
  int fUseWireLoads;
  int fDumpStats;
  int fVerbose;
  int fVeryVerbose;

  JH_upsize_params() : nIters(1000), nIterNoChange(50), Window(1), Ratio(10), Notches(1000), DelayUser(0), DelayGap(0), TimeOut(0), BuffTreeEst(0), BypassFreq(0), fUseDept(1), fUseWireLoads(1), fDumpStats(1), fVerbose(0), fVeryVerbose(0) {}
};

struct JH_map_params {
  double DelayTarget;
  double AreaMulti;
  double DelayMulti;
  float LogFan;
  float Slew;
  float Gain;
  int nGatesMin;
  int fAreaOnly;
  int fRecovery;
  int fSweep;
  int fSwitching;
  int fSkipFanout;
  int fUseProfile;
  int fUseBuffs;
  int fVerbose;

  JH_map_params() : DelayTarget(-1), AreaMulti(0), DelayMulti(0), LogFan(0), Slew(0), Gain(250), nGatesMin(0), fAreaOnly(0), fRecovery(1), fSweep(0), fSwitching(0), fSkipFanout(0), fUseProfile(0), fUseBuffs(0), fVerbose(1) {}
};


struct JH_refactor_params {
  int RetValue;
  int nNodeSizeMax;
  int nMinSaved;
  int nConeSizeMax;
  int fUpdateLevel;
  int fUseDcs;
  int fVerbose;
  int fUseZeros;

  JH_refactor_params() : RetValue(0), nNodeSizeMax(10), nMinSaved(1), nConeSizeMax(16), fUpdateLevel(1), fUseDcs(0), fVerbose(0), fUseZeros(0) {}
};

struct JH_rewrite_params {
  int RetValue;
  int fUpdateLevel;
  int fPrecompute;
  int fVerbose;
  int fVeryVerbose;
  int fPlaceEnable;
  int fUseZeros;

  JH_rewrite_params() : RetValue(0), fUpdateLevel(1), fPrecompute(0), fVerbose(0), fVeryVerbose(0), fPlaceEnable(0), fUseZeros(0) {}
};

struct JH_balance_params {
  int fDuplicate;
  int fSelective;
  int fUpdateLevel;
  int fExor;
  int fVerbose;

  JH_balance_params() : fDuplicate(0), fSelective(0), fUpdateLevel(1), fExor(0), fVerbose(0) {}
};

class ABC_function {
  public:
    static int JH_ps(Abc_Ntk_t * pNtk, JH_ps_params params = JH_ps_params());
    static int JH_topo(Abc_Ntk_t * &pNtk);
    static int JH_buffer(Abc_Ntk_t * &pNtk, JH_buffer_params params = JH_buffer_params());
    static int JH_stime(Abc_Ntk_t * &pNtk, JH_stime_params params = JH_stime_params());
    static int JH_dnsize(Abc_Ntk_t * &pNtk, JH_dnsize_params params = JH_dnsize_params());
    static int JH_upsize(Abc_Ntk_t * &pNtk, JH_upsize_params params = JH_upsize_params());
    static int JH_resyn2(Abc_Ntk_t * &pNtk);
    static int JH_rewrite(Abc_Ntk_t * &pNtk, JH_rewrite_params params = JH_rewrite_params());
    static int JH_refactor(Abc_Ntk_t * &pNtk,JH_refactor_params params = JH_refactor_params());
    static int JH_balance(Abc_Ntk_t * &pNtk, JH_balance_params params = JH_balance_params());
    static int JH_map(Abc_Ntk_t * &pNtk, JH_map_params params = JH_map_params());
};

}  // namespace rmp