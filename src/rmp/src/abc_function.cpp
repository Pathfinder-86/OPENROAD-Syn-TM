#include "abc_function.h"
#include "map/scl/sclSize.h"
#include "map/scl/sclLib.h"
#include <iostream>

namespace abc {
void* Abc_FrameReadLibGen();
extern Aig_Man_t * Abc_NtkToDarChoices( Abc_Ntk_t * pNtk );
extern Aig_Man_t * Abc_NtkToDar( Abc_Ntk_t * pNtk, int fExors, int fRegisters );
extern Vec_Ptr_t * Abc_NtkCollectCiNames( Abc_Ntk_t * pNtk );
extern Vec_Ptr_t * Abc_NtkCollectCoNames( Abc_Ntk_t * pNtk );
extern Gia_Man_t * Gia_ManDeepSyn( Gia_Man_t * pGia, int nIters, int nNoImpr, int TimeOut, int nAnds, int Seed, int fUseTwo, int fVerbose );


int ABC_function::JH_ps(Abc_Ntk_t * pNtk,int fFactor, int fSaveBest, int fDumpResult, int fUseLutLib, 
int fPrintMuxes, int fPower, int fGlitch, int fSkipBuf, int fSkipSmall, int fPrintMem){  
  if(pNtk==NULL){
    std::cout<<"There is no current network."<<std::endl;
    return 1;
  }
  Abc_NtkPrintStats( pNtk, fFactor, fSaveBest, fDumpResult, fUseLutLib, fPrintMuxes, fPower, fGlitch, fSkipBuf, fSkipSmall, fPrintMem );
  return 0;
}

Abc_Ntk_t * ABC_function::JH_topo(Abc_Ntk_t * pNtk,int fVerbose){
  if ( pNtk == NULL )
  {
      Abc_Print( -1, "Empty network.\n" );
      return pNtk;
  }
  if ( !Abc_NtkIsLogic(pNtk) )
  {
      Abc_Print( -1, "This command can only be applied to a logic network.\n" );
      return pNtk;
  }

  // modify the current network
  return Abc_NtkDupDfs( pNtk );
}

int ABC_function::JH_stime(Abc_Ntk_t * pNtk,int fShowAll,int fUseWireLoads ,int fPrintPath ,int fDumpStats ,int nTreeCRatio )
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

    std::cout<<"Abc_SclTimePerform"<<std::endl;
    Abc_SclTimePerform( sc_lib, pNtk, nTreeCRatio, fUseWireLoads, fShowAll, fPrintPath, fDumpStats );
    return 0;

}

Abc_Ntk_t * ABC_function::JH_buffer( Abc_Ntk_t * pNtk,int GainRatio,int Slew,int nDegree,int fSizeOnly,
int fAddBufs,int fBufPis,int fUseWireLoads,int fVerbose,int fVeryVerbose)
{
    SC_BusPars Pars, * pPars = &Pars;    
    memset( pPars, 0, sizeof(SC_BusPars) );
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
    if ( !Abc_NtkIsLogic(pNtk) )
    {
        std::cout<<"This command can only be applied to a logic network."<<std::endl;
        return pNtk;
    }
    if ( !pPars->fSizeOnly && !pPars->fAddBufs && pNtk->vPhases == NULL )
    {
        std::cout<<"Fanin phase information is not available."<<std::endl;
        return pNtk;
    }

    SC_Lib * sc_lib = Abc_SclReadFromGenlib(Abc_FrameReadLibGen());
    if( sc_lib == NULL )
    {
        std::cout<<"There is no SC Liberty library available."<<std::endl;
        return pNtk;
    }

    if ( !Abc_SclHasDelayInfo(sc_lib) )
    {
        std::cout<<"Library delay info is not available."<<std::endl;
        return pNtk;
    }
    // modify the current network
    std::cout<<"Abc_SclBufferingPerform"<<std::endl;
    return Abc_SclBufferingPerform( pNtk, sc_lib, pPars );
}



int ABC_function::JH_upsize( Abc_Ntk_t * pNtk, int nIters, int nIterNoChange, int Window, int Ratio, int Notches, 
double DelayUser, double DelayGap, int TimeOut, int BuffTreeEst, int BypassFreq, int fUseDept, int fUseWireLoads, 
int fDumpStats, int fVerbose, int fVeryVerbose)
{    
    SC_SizePars Pars, * pPars = &Pars;    
    memset( pPars, 0, sizeof(SC_SizePars) );
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

    std::cout<<"Abc_SclUpsizePerform"<<std::endl;
    Abc_SclUpsizePerform( sc_lib, pNtk, pPars, NULL );
    return 0;
}


int ABC_function::JH_dnsize( Abc_Ntk_t * pNtk, int nIters, int nIterNoChange, int Window, int Ratio, int Notches, 
double DelayUser, double DelayGap, int TimeOut, int BuffTreeEst, int BypassFreq, int fUseDept, int fUseWireLoads, 
int fDumpStats, int fVerbose, int fVeryVerbose)
{
    SC_SizePars Pars, * pPars = &Pars;    
    memset( pPars, 0, sizeof(SC_SizePars) );
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

    std::cout<<"Abc_SclDnsizePerform"<<std::endl;
    Abc_SclDnsizePerform( sc_lib, pNtk, pPars, NULL );    
    return 0;
}

Abc_Ntk_t * ABC_function::JH_resyn2(Abc_Ntk_t * pNtk){
  if ( pNtk == NULL )
  {
      std::cout<<"Empty network."<<std::endl;
      return pNtk;
  }
  // balance, rewrite, refactor, balance, rewrite
  // rewrite -z, balance, refactor -z, rewrite -z balance
  Abc_Ntk_t * temp;
  temp = JH_balance( pNtk );
  JH_ps(temp);
  temp = JH_rewrite( temp);
  JH_ps(temp);
  temp = JH_refactor( temp);
  JH_ps(temp);
  temp = JH_balance( temp);
  JH_ps(temp);
  temp = JH_rewrite( temp);
  JH_ps(temp);
  temp = JH_rewrite( temp, 1);
  temp = JH_balance( temp);
  temp = JH_refactor( temp, 1);
  temp = JH_rewrite( temp, 1);
  pNtk = JH_balance( temp );
  return pNtk;
}

Abc_Ntk_t * ABC_function::JH_balance(Abc_Ntk_t * pNtk)
{
    Abc_Ntk_t * pNtkRes, * pNtkTemp;    
    int fDuplicate;
    int fSelective;
    int fUpdateLevel;
    int fExor;
    int fVerbose;

    // set defaults
    fDuplicate   = 0;
    fSelective   = 0;
    fUpdateLevel = 1;
    fExor        = 0;
    fVerbose     = 0;


    if ( pNtk == NULL )
    {
        Abc_Print( -1, "Empty network.\n" );
        return pNtk;
    }
    // get the new network
    if ( Abc_NtkIsStrash(pNtk) )
    {        
        if ( fExor )
            pNtkRes = Abc_NtkBalanceExor( pNtk, fUpdateLevel, fVerbose );
        else
            pNtkRes = Abc_NtkBalance( pNtk, fDuplicate, fSelective, fUpdateLevel );
    }
    else
    {
        pNtkTemp = Abc_NtkStrash( pNtk, 0, 0, 0 );
        if ( pNtkTemp == NULL )
        {
            Abc_Print( -1, "Strashing before balancing has failed.\n" );
            return pNtkTemp;
        }
        if ( fExor )
            pNtkRes = Abc_NtkBalanceExor( pNtkTemp, fUpdateLevel, fVerbose );
        else
            pNtkRes = Abc_NtkBalance( pNtkTemp, fDuplicate, fSelective, fUpdateLevel );
        Abc_NtkDelete( pNtkTemp );
    }

    // check if balancing worked
    if ( pNtkRes == NULL )
    {
        Abc_Print( -1, "Balancing has failed.\n" );
        return pNtk;
    }

    // replace the current network        
    pNtk = pNtkRes;
    pNtkRes = NULL;        
    return pNtk;
}

// fUseZeros = -z
Abc_Ntk_t * ABC_function::JH_rewrite(Abc_Ntk_t * pNtk, int fUseZeros)
{
    Abc_Ntk_t * pDup;
    int RetValue;
    int fUpdateLevel;
    int fPrecompute;    
    int fVerbose;
    int fVeryVerbose;
    int fPlaceEnable;
    // external functions
    extern void Rwr_Precompute();

    // set defaults
    fUpdateLevel = 1;
    fPrecompute  = 0;    
    fVerbose     = 0;
    fVeryVerbose = 0;
    fPlaceEnable = 0;
    Extra_UtilGetoptReset();

    if ( fPrecompute )
    {
        Rwr_Precompute();
        return pNtk;
    }

    if ( pNtk == NULL )
    {
        Abc_Print( -1, "Empty network.\n" );
        return NULL;
    }
    if ( !Abc_NtkIsStrash(pNtk) )
    {
        Abc_Print( -1, "This command can only be applied to an AIG (run \"strash\").\n" );
        return pNtk;
    }
    if ( Abc_NtkGetChoiceNum(pNtk) )
    {
        Abc_Print( -1, "AIG resynthesis cannot be applied to AIGs with choice nodes.\n" );
        return pNtk;
    }

    // modify the current network
    pDup = Abc_NtkDup( pNtk );
    RetValue = Abc_NtkRewrite( pNtk, fUpdateLevel, fUseZeros, fVerbose, fVeryVerbose, fPlaceEnable );
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
            return pNtk;
        }
    }
    return pNtk;
}

Abc_Ntk_t * ABC_function::JH_refactor(Abc_Ntk_t * pNtk, int fUseZeros)
{
    Abc_Ntk_t * pDup;
    int RetValue;
    int nNodeSizeMax;
    int nMinSaved;
    int nConeSizeMax;
    int fUpdateLevel;
    int fUseDcs;
    int fVerbose;
    extern int Abc_NtkRefactor( Abc_Ntk_t * pNtk, int nNodeSizeMax, int nMinSaved, int nConeSizeMax, int fUpdateLevel, int fUseZeros, int fUseDcs, int fVerbose );

    // set defaults
    nNodeSizeMax = 10;
    nMinSaved    =  1;
    nConeSizeMax = 16;
    fUpdateLevel =  1;
    fUseDcs      =  0;
    fVerbose     =  0;
    Extra_UtilGetoptReset();

    if ( fUseZeros )
        nMinSaved = 0;
    if ( nMinSaved == 0 )
        fUseZeros = 1;

    if ( pNtk == NULL )
    {
        Abc_Print( -1, "Empty network.\n" );
        return pNtk;
    }
    if ( !Abc_NtkIsStrash(pNtk) )
    {
        Abc_Print( -1, "This command can only be applied to an AIG (run \"strash\").\n" );
        return pNtk;
    }
    if ( Abc_NtkGetChoiceNum(pNtk) )
    {
        Abc_Print( -1, "AIG resynthesis cannot be applied to AIGs with choice nodes.\n" );
        return pNtk;
    }
    if ( nNodeSizeMax > 15 )
    {
        Abc_Print( -1, "The cone size cannot exceed 15.\n" );
        return pNtk;
    }

    if ( fUseDcs && nNodeSizeMax >= nConeSizeMax )
    {
        Abc_Print( -1, "For don't-care to work, containing cone should be larger than collapsed node.\n" );
        return pNtk;
    }

    // modify the current network
    pDup = Abc_NtkDup( pNtk );
    RetValue = Abc_NtkRefactor( pNtk, nNodeSizeMax, nMinSaved, nConeSizeMax, fUpdateLevel, fUseZeros, fUseDcs, fVerbose );
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
            return pNtk;
        }
    }
    return pNtk;
}



}//namespace abc