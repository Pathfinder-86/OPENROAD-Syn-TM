#include "abc_function.h"
#include "map/scl/sclSize.h"
#include "map/scl/sclLib.h"
#include <iostream>

namespace abc {
void* Abc_FrameReadLibGen();


int ABC_function::JH_ps( Abc_Ntk_t * pNtk,  JH_ps_params params) {  
    if(pNtk == NULL) {
        std::cout << "There is no current network." << std::endl;
        return 1;
    }
    Abc_NtkPrintStats(pNtk, params.fFactor, params.fSaveBest, params.fDumpResult, 
                      params.fUseLutLib, params.fPrintMuxes, params.fPower, params.fGlitch, 
                      params.fSkipBuf, params.fSkipSmall, params.fPrintMem);
    return 0;
}

int ABC_function::JH_topo(Abc_Ntk_t * &pNtk){
  if ( pNtk == NULL )
  {
      Abc_Print( -1, "Empty network.\n" );
      return 1;
  }
  if ( !Abc_NtkIsLogic(pNtk) )
  {
      Abc_Print( -1, "This command can only be applied to a logic network.\n" );
      return 1;
  }

  // modify the current network
  pNtk =  Abc_NtkDupDfs( pNtk );
  return 0;
}

int ABC_function::JH_stime( Abc_Ntk_t * &pNtk,  JH_stime_params params)
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

    SC_Lib * sc_lib = Abc_SclReadFromGenlib(Abc_FrameReadLibGen());
    if( sc_lib == NULL )
    {
        std::cout<<"There is no SC Liberty library available."<<std::endl;
        return 1;
    }
    
    Abc_SclTimePerform( sc_lib, pNtk, params.nTreeCRatio, params.fUseWireLoads,
                       params.fShowAll, params.fPrintPath, params.fDumpStats);
    return 0;

}

int ABC_function::JH_buffer( Abc_Ntk_t * &pNtk,  JH_buffer_params params)
{
    SC_BusPars Pars, * pPars = &Pars;    
    memset( pPars, 0, sizeof(SC_BusPars) );
    pPars->GainRatio     =  params.GainRatio;
    pPars->Slew          = params.Slew;
    pPars->nDegree       = params.nDegree;
    pPars->fSizeOnly     = params.fSizeOnly;
    pPars->fAddBufs      = params.fAddBufs;
    pPars->fBufPis       = params.fBufPis;
    pPars->fUseWireLoads = params.fUseWireLoads;
    pPars->fVerbose      = params.fVerbose;
    pPars->fVeryVerbose  = params.fVeryVerbose;

    if ( pNtk == NULL )
    {
        std::cout<<"Empty network."<<std::endl;
        return 1;
    }
    if ( !Abc_NtkIsLogic(pNtk) )
    {
        std::cout<<"This command can only be applied to a logic network."<<std::endl;
        return 1;
    }
    if ( !pPars->fSizeOnly && !pPars->fAddBufs && pNtk->vPhases == NULL )
    {
        std::cout<<"Fanin phase information is not available."<<std::endl;
        return 1;
    }

    SC_Lib * sc_lib = Abc_SclReadFromGenlib(Abc_FrameReadLibGen());
    if( sc_lib == NULL )
    {
        std::cout<<"There is no SC Liberty library available."<<std::endl;
        return 1;
    }

    if ( !Abc_SclHasDelayInfo(sc_lib) )
    {
        std::cout<<"Library delay info is not available."<<std::endl;
        return 1;
    }
    // modify the current network
    pNtk =  Abc_SclBufferingPerform( pNtk, sc_lib, pPars );
    return 0;
}



int ABC_function::JH_upsize( Abc_Ntk_t * &pNtk,  JH_upsize_params params)
{    
    SC_SizePars Pars, * pPars = &Pars;    
    memset( pPars, 0, sizeof(SC_SizePars) );
    pPars->nIters        = params.nIters;
    pPars->nIterNoChange = params.nIterNoChange;
    pPars->Window        = params.Window;
    pPars->Ratio         = params.Ratio;
    pPars->Notches       = params.Notches;
    pPars->DelayUser     = params.DelayUser;
    pPars->DelayGap      = params.DelayGap;
    pPars->TimeOut       = params.TimeOut;
    pPars->BuffTreeEst   = params.BuffTreeEst;
    pPars->BypassFreq    = params.BypassFreq;
    pPars->fUseDept      = params.fUseDept; 
    pPars->fUseWireLoads = params.fUseWireLoads;
    pPars->fDumpStats    = params.fDumpStats;
    pPars->fVerbose      = params.fVerbose;
    pPars->fVeryVerbose  = params.fVeryVerbose;

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

    SC_Lib * sc_lib = Abc_SclReadFromGenlib(Abc_FrameReadLibGen());
    if( sc_lib == NULL )
    {
        std::cout<<"There is no SC Liberty library available."<<std::endl;
        return 1;
    }

    Abc_SclUpsizePerform( sc_lib, pNtk, pPars, NULL );
    return 0;
}


int ABC_function::JH_dnsize( Abc_Ntk_t * &pNtk,  JH_dnsize_params params)
{
    SC_SizePars Pars, * pPars = &Pars;    
    memset( pPars, 0, sizeof(SC_SizePars) );
    pPars->nIters        = params.nIters;
    pPars->nIterNoChange = params.nIterNoChange;
    pPars->Window        = params.Window;
    pPars->Ratio         = params.Ratio;
    pPars->Notches       = params.Notches;
    pPars->DelayUser     = params.DelayUser;
    pPars->DelayGap      = params.DelayGap;
    pPars->TimeOut       = params.TimeOut;
    pPars->BuffTreeEst   = params.BuffTreeEst;
    pPars->BypassFreq    = params.BypassFreq;
    pPars->fUseDept      = params.fUseDept; 
    pPars->fUseWireLoads = params.fUseWireLoads;
    pPars->fDumpStats    = params.fDumpStats;
    pPars->fVerbose      = params.fVerbose;
    pPars->fVeryVerbose  = params.fVeryVerbose;

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

    SC_Lib * sc_lib = Abc_SclReadFromGenlib(Abc_FrameReadLibGen());
    if( sc_lib == NULL )
    {
        std::cout<<"There is no SC Liberty library available."<<std::endl;
        return 1;
    }

    Abc_SclDnsizePerform( sc_lib, pNtk, pPars, NULL );    
    return 0;
}

int ABC_function::JH_resyn2(Abc_Ntk_t * &pNtk){
  if ( pNtk == NULL )
  {
      std::cout<<"Empty network."<<std::endl;
      return 1;
  }
  // balance, rewrite, refactor, balance, rewrite
  // rewrite -z, balance, refactor -z, rewrite -z balance
  JH_rewrite_params rewrite_z_params;
  rewrite_z_params.fUseZeros = 1;  
  JH_refactor_params refactor_z_params;
  refactor_z_params.fUseZeros = 1;

// any better way to do this? 
if (JH_balance(pNtk)) return 1;
if (JH_ps(pNtk)) return 1;
if (JH_rewrite(pNtk)) return 1;
if (JH_ps(pNtk)) return 1;
if (JH_refactor(pNtk)) return 1;
if (JH_ps(pNtk)) return 1;
if (JH_balance(pNtk)) return 1;
if (JH_ps(pNtk)) return 1;
if (JH_rewrite(pNtk)) return 1;
if (JH_ps(pNtk)) return 1;
if (JH_rewrite(pNtk, rewrite_z_params)) return 1;
if (JH_balance(pNtk)) return 1;
if (JH_refactor(pNtk, refactor_z_params)) return 1;
if (JH_rewrite(pNtk, rewrite_z_params)) return 1;
if (JH_balance(pNtk)) return 1;
return 0;
}

int ABC_function::JH_balance(Abc_Ntk_t * &pNtk,  JH_balance_params params)
{
    if ( pNtk == NULL )
    {
        Abc_Print( -1, "Empty network.\n" );
        return 1;
    }

    // get the new network
    if ( Abc_NtkIsStrash(pNtk) )
    {        
        if ( params.fExor )
            pNtk = Abc_NtkBalanceExor( pNtk, params.fUpdateLevel, params.fVerbose );
        else
            pNtk = Abc_NtkBalance( pNtk, params.fDuplicate, params.fSelective, params.fUpdateLevel );
    }
    else
    {
        Abc_Ntk_t * pNtkStrash = Abc_NtkStrash( pNtk, 0, 0, 0 );
        if ( pNtkStrash == NULL )
        {
            Abc_Print( -1, "Strashing before balancing has failed.\n" );
            return 1;
        }
        if ( params.fExor )
            pNtk = Abc_NtkBalanceExor( pNtkStrash, params.fUpdateLevel, params.fVerbose );
        else
            pNtk = Abc_NtkBalance( pNtkStrash, params.fDuplicate, params.fSelective, params.fUpdateLevel );
        Abc_NtkDelete( pNtkStrash );
    }

    // check if balancing worked
    if ( pNtk == NULL )
    {
        Abc_Print( -1, "Balancing has failed.\n" );
        return 1;
    }

    return 0;
}

// fUseZeros = -z
int ABC_function::JH_rewrite(Abc_Ntk_t * &pNtk,  JH_rewrite_params params)
{
    Abc_Ntk_t * pDup;
    // external functions
    extern void Rwr_Precompute();
    
    Extra_UtilGetoptReset();

    if ( params.fPrecompute )
    {
        Rwr_Precompute();
        return 0;
    }

    if ( pNtk == NULL )
    {
        Abc_Print( -1, "Empty network.\n" );
        return 1;
    }
    if ( !Abc_NtkIsStrash(pNtk) )
    {
        Abc_Print( -1, "This command can only be applied to an AIG (run \"strash\").\n" );
        return 1;
    }
    if ( Abc_NtkGetChoiceNum(pNtk) )
    {
        Abc_Print( -1, "AIG resynthesis cannot be applied to AIGs with choice nodes.\n" );
        return 1;
    }

    // modify the current network
    pDup = Abc_NtkDup( pNtk );
    int RetValue = Abc_NtkRewrite( pNtk, params.fUpdateLevel, params.fUseZeros, params.fVerbose, params.fVeryVerbose, params.fPlaceEnable );
    if ( RetValue == -1 )
    {
        pNtk = pDup;
        printf( "An error occurred during computation. The original network is restored.\n" );
    }
    else
    {
        Abc_NtkDelete( pDup );
        if ( RetValue == 0 )
        {
            Abc_Print( 0, "Rewriting has failed.\n" );
            return 1;
        }
    }
    return 0;
}


int ABC_function::JH_refactor(Abc_Ntk_t * &pNtk,  JH_refactor_params params)
{
    int RetValue;

    Extra_UtilGetoptReset();
    
    Abc_Ntk_t * pDup;
    extern int Abc_NtkRefactor( Abc_Ntk_t * pNtk, int nNodeSizeMax, int nMinSaved, int nConeSizeMax, int fUpdateLevel, int fUseZeros, int fUseDcs, int fVerbose );

    if ( params.fUseZeros )
        params.nMinSaved = 0;
    if ( params.nMinSaved == 0 )
        params.fUseZeros = 1;

    if ( pNtk == NULL )
    {
        Abc_Print( -1, "Empty network.\n" );
        return 1;
    }
    if ( !Abc_NtkIsStrash(pNtk) )
    {
        Abc_Print( -1, "This command can only be applied to an AIG (run \"strash\").\n" );
        return 1;
    }
    if ( Abc_NtkGetChoiceNum(pNtk) )
    {
        Abc_Print( -1, "AIG resynthesis cannot be applied to AIGs with choice nodes.\n" );
        return 1;
    }
    if ( params.nNodeSizeMax > 15 )
    {
        Abc_Print( -1, "The cone size cannot exceed 15.\n" );
        return 1;
    }

    if ( params.fUseDcs && (params.nNodeSizeMax >= params.nConeSizeMax ))
    {
        Abc_Print( -1, "For don't-care to work, containing cone should be larger than collapsed node.\n" );
        return 1;
    }

    // modify the current network
    pDup = Abc_NtkDup( pNtk );
    RetValue = Abc_NtkRefactor( pNtk, params.nNodeSizeMax, params.nMinSaved, params.nConeSizeMax, params.fUpdateLevel, params.fUseZeros, params.fUseDcs, params.fVerbose );
    if ( RetValue == -1 )
    {
        pNtk = pDup;
        printf( "An error occurred during computation. The original network is restored.\n" );
    }
    else
    {
        Abc_NtkDelete( pDup );
        if ( RetValue == 0 )
        {
            Abc_Print( 0, "Refactoring has failed.\n" );
            return 1;
        }
    }
    return 0;
}


int ABC_function::JH_map(Abc_Ntk_t * &pNtk,  JH_map_params params)
{
    Abc_Ntk_t * pNtkRes;    

    extern Abc_Ntk_t * Abc_NtkMap(Abc_Ntk_t * pNtk, double DelayTarget, double AreaMulti, double DelayMulti, float LogFan, float Slew, float Gain, int nGatesMin, int fRecovery, int fSwitching, int fSkipFanout, int fUseProfile, int fUseBuffs, int fVerbose);
    extern int Abc_NtkFraigSweep(Abc_Ntk_t * pNtk, int fUseInv, int fExdc, int fVerbose, int fVeryVerbose);

    if (pNtk == NULL)
    {
        Abc_Print(-1, "Empty network.\n");
        return 1;
    }

    if (params.fAreaOnly)
        params.DelayTarget = ABC_INFINITY;

    if (!Abc_NtkIsStrash(pNtk)){
        Abc_Print(-1, "Strashing before mapping has failed.\n");
        return 1;
    }
    else{
        // get the new network
        pNtkRes = Abc_NtkMap(pNtk, params.DelayTarget, params.AreaMulti, params.DelayMulti, params.LogFan, params.Slew, params.Gain, params.nGatesMin, params.fRecovery, params.fSwitching, params.fSkipFanout, params.fUseProfile, params.fUseBuffs, params.fVerbose);
        if (pNtkRes == NULL)
        {
            Abc_Print(-1, "Mapping has failed.\n");
            return 1;
        }
    }

    if (params.fSweep)
    {
        Abc_NtkFraigSweep(pNtkRes, 0, 0, 0, 0);
        if (Abc_NtkHasMapping(pNtkRes))
        {
            pNtkRes = Abc_NtkDupDfs(pNtk = pNtkRes);
            Abc_NtkDelete(pNtk);
        }
    }
    
    // replace the current network
    pNtk = pNtkRes;
    return 0;    
}

}//namespace abc