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

//print state of current gia
int ABC_function::ps_gia(Gia_Man_t * pGia ){

    if(pGia == NULL){
        std::cout<<"empty Gia"<<std::endl;
        return 1;
    }
    Gps_Par_t Pars, * pPars = &Pars;
    memset( pPars, 0, sizeof(Gps_Par_t) );
    Gia_ManPrintStats( pGia, pPars );

    return 0;

}

//given abc ntk return it's gia
Gia_Man_t * ABC_function::get_gia(Abc_Ntk_t *pNtk, int fGiaSimple, int fMapped, int fNames, int fVerbose){
    Abc_Ntk_t * pStrash;
    Aig_Man_t * pAig;
    Gia_Man_t * pGia, * pTemp;
    char * pInits;
    // int c, fGiaSimple = 0, fMapped = 0, fNames = 0, fVerbose = 0;

   
    if ( pNtk == NULL )
    {
        // Abc_Print( -1, "There is no current network\n" );
        std::cout<<"empty network"<<std::endl;
        return nullptr;
    }
    if ( !Abc_NtkIsStrash( pNtk ) )
    {
        if ( fGiaSimple || fMapped )
        {
            assert( Abc_NtkIsLogic(pNtk) );
            Abc_NtkToAig( pNtk );
            pGia = Abc_NtkAigToGia( pNtk, fGiaSimple );
        }
        else
        {
            // derive comb GIA
            pStrash = Abc_NtkStrash( pNtk, 0, 1, 0 );
            pAig = Abc_NtkToDar( pStrash, 0, 0 );
            Abc_NtkDelete( pStrash );
            pGia = Gia_ManFromAig( pAig );
            Aig_ManStop( pAig );
            // perform undc/zero
            pInits = Abc_NtkCollectLatchValuesStr( pNtk );
            pGia = Gia_ManDupZeroUndc( pTemp = pGia, pInits, 0, 0, fVerbose );
            Gia_ManStop( pTemp );
            ABC_FREE( pInits );
        }
    }
    else
    {
        if ( Abc_NtkGetChoiceNum(pNtk) )
            pAig = Abc_NtkToDarChoices( pNtk );
        else
            pAig = Abc_NtkToDar( pNtk, 0, 1 );
        pGia = Gia_ManFromAig( pAig );
        Aig_ManStop( pAig );
    }
    // copy names
    if ( fNames )
    {
        pGia->vNamesIn  = Abc_NtkCollectCiNames( pNtk );
        pGia->vNamesOut = Abc_NtkCollectCoNames( pNtk );
    }
    // copy user timing information
    if ( pNtk->pManTime != NULL )
    {
        // Abc_Ntk_t * pNtk = pAbc->pNtkCur;
        Vec_FltFreeP( &pGia->vInArrs );
        Vec_FltFreeP( &pGia->vOutReqs );
        pGia->DefInArrs  = Abc_NtkReadDefaultArrivalWorst(pNtk);
        pGia->DefOutReqs = Abc_NtkReadDefaultRequiredWorst(pNtk);
        pGia->vInArrs  = Vec_FltAllocArray( Abc_NtkGetCiArrivalFloats(pNtk), Abc_NtkCiNum(pNtk) );
        pGia->vOutReqs = Vec_FltAllocArray( Abc_NtkGetCoRequiredFloats(pNtk), Abc_NtkCoNum(pNtk) );
        pGia->And2Delay = pNtk->AndGateDelay;
    }
    // Abc_FrameUpdateGia( pAbc, pGia );
    return pGia;

}

Abc_Ntk_t* ABC_function::put_gia(Abc_Ntk_t* pNtk, Gia_Man_t *pGia){
    extern Abc_Ntk_t * Abc_NtkFromDarChoices( Abc_Ntk_t * pNtkOld, Aig_Man_t * pMan );
    extern void Abc_NtkRedirectCiCo( Abc_Ntk_t * pNtk );
    extern Abc_Ntk_t * Abc_NtkFromCellMappedGia( Gia_Man_t * p, int fUseBuffs );
    extern Abc_Ntk_t * Abc_NtkFromMappedGia( Gia_Man_t * p, int fFindEnables, int fUseBuffs );
    extern Abc_Ntk_t * Abc_NtkFromAigPhase( Aig_Man_t * pMan );

    Aig_Man_t * pMan;
    // Abc_Ntk_t * pNtk = Abc_FrameReadNtk(pAbc);
    int fStatusClear = 1;
    int fFindEnables = 0;
    int fUseBuffs    = 0;
    int c, fVerbose  = 0;


    // if ( pAbc->pGia == NULL )
    // {
    //     Abc_Print( -1, "Empty network.\n" );
    //     return 1;
    // }
    if ( fFindEnables )
        pNtk = Abc_NtkFromMappedGia( pGia, 1, fUseBuffs );
    else if ( Gia_ManHasCellMapping(pGia) )
        pNtk = Abc_NtkFromCellMappedGia( pGia, fUseBuffs );
    else if ( Gia_ManHasMapping(pGia) || pGia->pMuxes )
        pNtk = Abc_NtkFromMappedGia( pGia, 0, fUseBuffs );
    else if ( Gia_ManHasDangling(pGia) == 0 )
    {
        pMan = Gia_ManToAig( pGia, 0 );
        pNtk = Abc_NtkFromAigPhase( pMan );
        pNtk->pName = Extra_UtilStrsav(pMan->pName);
        Aig_ManStop( pMan );
    }
    else
    {
        Abc_Ntk_t * pNtkNoCh;
        // Abc_Print( -1, "Transforming AIG with %d choice nodes.\n", Gia_ManEquivCountClasses(pAbc->pGia) );
        // create network without choices
        pMan = Gia_ManToAig( pGia, 0 );
        pNtkNoCh = Abc_NtkFromAigPhase( pMan );
        pNtkNoCh->pName = Extra_UtilStrsav(pMan->pName);
        Aig_ManStop( pMan );
        // derive network with choices
        pMan = Gia_ManToAig( pGia, 1 );
        pNtk = Abc_NtkFromDarChoices( pNtkNoCh, pMan );
        Abc_NtkDelete( pNtkNoCh );
        Aig_ManStop( pMan );
    }
    // // transfer PI names to pNtk
    // if ( pGia->vNamesIn )
    // {
    //     Abc_Obj_t * pObj;
    //     int i;
    //     Abc_NtkForEachCi( pNtk, pObj, i ) {
    //         if (i < Vec_PtrSize(pGia->vNamesIn)) {
    //             Nm_ManDeleteIdName(pNtk->pManName, pObj->Id);
    //             Abc_ObjAssignName( pObj, (char *)Vec_PtrEntry(pGia->vNamesIn, i), NULL );
    //         }
    //     }
    // }
    // // transfer PO names to pNtk
    // if ( pGia->vNamesOut )
    // {
    //     char pSuffix[1000];
    //     Abc_Obj_t * pObj; int i;
    //     unsigned char nDigits = (unsigned char)Abc_Base10Log( Abc_NtkLatchNum(pNtk) );
    //     Abc_NtkForEachCo( pNtk, pObj, i ) {
    //         if (i < Vec_PtrSize(pGia->vNamesOut)) {
    //             Nm_ManDeleteIdName(pNtk->pManName, pObj->Id);
    //             if ( Abc_ObjIsPo(pObj) )
    //                 Abc_ObjAssignName( pObj, (char *)Vec_PtrEntry(pGia->vNamesOut, i), NULL );
    //             else
    //             {
    //                 assert( i >= Abc_NtkPoNum(pNtk) );
    //                 sprintf( pSuffix, "_li%0*d", nDigits, i-Abc_NtkPoNum(pNtk) );
    //                 Abc_ObjAssignName( pObj, (char *)Vec_PtrEntry(pGia->vNamesOut, i), pSuffix );
    //             }
    //         }
    //     }
    // }
    // if ( pGia->vNamesNode )
    //     Abc_Print( 0, "Internal nodes names are not transferred.\n" );

    // // decouple CI/CO with the same name
    // if ( !Abc_NtkIsStrash(pNtk) && (pGia->vNamesIn || pGia->vNamesOut) )
    //     Abc_NtkRedirectCiCo( pNtk );

    // // transfer timing information
    // if ( pGia->vInArrs || pGia->vOutReqs )
    // {
    //     Abc_Obj_t * pObj; int i;
    //     Abc_NtkTimeInitialize( pNtk, NULL );
    //     Abc_NtkTimeSetDefaultArrival( pNtk, pGia->DefInArrs, pGia->DefInArrs );
    //     Abc_NtkTimeSetDefaultRequired( pNtk, pGia->DefOutReqs, pGia->DefOutReqs );
    //     if ( pGia->vInArrs )
    //         Abc_NtkForEachCi( pNtk, pObj, i )
    //             Abc_NtkTimeSetArrival( pNtk, Abc_ObjId(pObj), Vec_FltEntry(pGia->vInArrs, i), Vec_FltEntry(pGia->vInArrs, i) );
    //     if ( pGia->vOutReqs )
    //         Abc_NtkForEachCo( pNtk, pObj, i )
    //             Abc_NtkTimeSetRequired( pNtk, Abc_ObjId(pObj), Vec_FltEntry(pGia->vOutReqs, i), Vec_FltEntry(pGia->vOutReqs, i) );
    // }

    // replace the current network
    // Abc_FrameReplaceCurrentNetwork( pAbc, pNtk );
    // if ( fStatusClear )
        // Abc_FrameClearVerifStatus( pAbc );
    return pNtk;
}

Gia_Man_t * ABC_function::deepsyn_int(Gia_Man_t *pGia){
    
    Gia_Man_t * pTemp; 
    int c, nIters = 1, nNoImpr = ABC_INFINITY, TimeOut = 0, nAnds = 0, Seed = 0, fUseTwo = 0, fVerbose = 0;
    
    if ( pGia == NULL )
    {
        Abc_Print( -1, "Abc_CommandAbc9DeepSyn(): There is no AIG.\n" );
        return 0;
    }
    // pTemp = Gia_ManDeepSyn( pGia, nIters, nNoImpr, TimeOut, nAnds, Seed, fUseTwo, fVerbose );

    // Gia_Man_t * pInit = Gia_ManDup(pGia);
    // Gia_Man_t * pBest = Gia_ManDup(pGia);
    // Gia_Man_t * pThis;
    // int i;
    // for ( i = 0; i < nIters; i++ )
    // {
    //     // Abc_FrameUpdateGia( Abc_FrameGetGlobalFrame(), Gia_ManDup(pInit) );
    //     pThis = Gia_ManDeepSynOne( nNoImpr, TimeOut, nAnds, Seed+i, fUseTwo, fVerbose );
    //     if ( Gia_ManAndNum(pBest) > Gia_ManAndNum(pThis) ) 
    //     {
    //         Gia_ManStop( pBest );
    //         pBest = pThis;
    //     }
    //     else 
    //         Gia_ManStop( pThis );
        
    // }
    // Gia_ManStop( pInit );
    return pTemp;
}

Abc_Ntk_t * ABC_function::deepsyn(Abc_Ntk_t *pNtk){

    //&get
    Gia_Man_t *pGia = get_gia(pNtk, 0, 0, 0, 0);
    if(ps_gia(pGia)){
        std::cout<<"something wrong"<<std::endl;
        return nullptr;
    }
    //&deepsyn -T 10

    //&put
    if(put_gia(pNtk, pGia)){
        std::cout<<"put fail"<<std::endl;
        return nullptr;
    }

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