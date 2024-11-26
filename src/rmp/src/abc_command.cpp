#include "abc_function.h"

namespace abc{
    extern Aig_Man_t * Abc_NtkToDar( Abc_Ntk_t * pNtk, int fExors, int fRegisters );
    extern Abc_Ntk_t * Abc_NtkFromAigPhase( Aig_Man_t * pMan );
    extern Vec_Ptr_t * Abc_NtkCollectCiNames( Abc_Ntk_t * pNtk );
    extern Vec_Ptr_t * Abc_NtkCollectCoNames( Abc_Ntk_t * pNtk );

//printing
int ABC_function::Abc_CommandPrintStats( Abc_Frame_t * pAbc, int fFactor ,int fSaveBest ,int fDumpResult ,int fUseLutLib ,int fPrintTime ,int fPrintMuxes ,int fPower ,int fGlitch ,int fSkipBuf ,int fSkipSmall ,int fPrintMem )
{
    Abc_Ntk_t * pNtk = Abc_FrameReadNtk(pAbc);

    pNtk = Abc_FrameReadNtk(pAbc);


    if ( pNtk == NULL )
    {
        Abc_Print( -1, "Empty network.\n" );
        return 1;
    }
    if ( !Abc_NtkIsLogic(pNtk) && fUseLutLib )
    {
        Abc_Print( -1, "Cannot print LUT delay for a non-logic network.\n" );
        return 1;
    }
    Abc_NtkPrintStats( pNtk, fFactor, fSaveBest, fDumpResult, fUseLutLib, fPrintMuxes, fPower, fGlitch, fSkipBuf, fSkipSmall, fPrintMem );
    if ( fPrintTime )
    {
        pAbc->TimeTotal += pAbc->TimeCommand;
        Abc_Print( 1, "elapse: %3.2f seconds, total: %3.2f seconds\n", pAbc->TimeCommand, pAbc->TimeTotal );
        pAbc->TimeCommand = 0.0;
    }
    return 0;

}

int ABC_function::Abc_CommandPrintCone( Abc_Frame_t * pAbc, int fUseLibrary)
{
    Abc_Ntk_t * pNtk = Abc_FrameReadNtk(pAbc);
    

    if ( pNtk == NULL )
    {
        Abc_Print( -1, "Empty network.\n" );
        return 1;
    }
    if ( Abc_NtkLatchNum(pNtk) == 0 )
    {
        Abc_Print( -1, "The network is combinational.\n" );
        return 1;
    }
    Abc_NtkDarPrintCone( pNtk );
    return 0;

}

int ABC_function::Abc_CommandPrintMiter( Abc_Frame_t * pAbc, int fUseLibrary)
{
    Abc_Ntk_t * pNtk = Abc_FrameReadNtk(pAbc);
    int c;

    extern void Abc_NtkPrintMiter( Abc_Ntk_t * pNtk );

    if ( pNtk == NULL )
    {
        Abc_Print( -1, "Empty network.\n" );
        return 1;
    }
    if ( !Abc_NtkIsStrash(pNtk) )
    {
        Abc_Print( -1, "The network is should be structurally hashed.\n" );
        return 1;
    }
    Abc_NtkPrintMiter( pNtk );
    return 0;

}

int ABC_function::Abc_CommandPrintStatus( Abc_Frame_t * pAbc, int fOutStatus, int fShort, char * pLogFileName)
{
    extern void Abc_NtkDumpOneCexSpecial( FILE * pFile, Abc_Ntk_t * pNtk, Abc_Cex_t * pCex );
    extern void Abc_NtkPrintPoEquivs( Abc_Ntk_t * pNtk );
    Abc_Ntk_t * pNtk = Abc_FrameReadNtk(pAbc);

    if ( fOutStatus )
    {
        if ( pNtk == NULL )
        {
            Abc_Print( -1, "Empty network.\n" );
            return 1;
        }
        Abc_NtkPrintPoEquivs( pNtk );
        return 0;
    }
    if ( !pAbc->vStatuses )
        Abc_Print( 1,"Status = %d  Frames = %d   ", pAbc->Status, pAbc->nFrames );
    if ( pAbc->pCex == NULL && pAbc->vCexVec == NULL )
        Abc_Print( 1,"Cex is not defined.\n" );
    else
    {
        if ( pAbc->pCex )
            Abc_CexPrintStats( pAbc->pCex );
        if ( pAbc->vCexVec )
        {
            Abc_Cex_t * pTemp;
            int nCexes = 0;
            int Counter = 0;
            //printf( "\n" );
            int i;
            Vec_PtrForEachEntry( Abc_Cex_t *, pAbc->vCexVec, pTemp, i )
            {
                if ( pTemp == (void *)(ABC_PTRINT_T)1 )
                {
                    Counter++;
                    continue;
                }
                if ( pTemp )
                {
                    printf( "%4d : ", ++nCexes );
                    Abc_CexPrintStats( pTemp );
                }
            }
        }
    }
    if ( pAbc->vStatuses )
    {
        if ( pLogFileName )
        {
            Abc_Cex_t * pTemp = NULL;
            FILE * pFile = fopen( pLogFileName, "wb" );
            if ( pFile == NULL )
            {
                printf( "Cannot open file \"%s\" for writing.\n", pLogFileName );
                return 0;
            }
            int i;
            Vec_PtrForEachEntry( Abc_Cex_t *, pAbc->vCexVec, pTemp, i )
            {
                int Status = Vec_IntEntry( pAbc->vStatuses, i );
                if ( Status == -1 ) // undec
                    fprintf( pFile, "STATUS: ABORTED " );
                else if ( Status == 0 )
                    fprintf( pFile, "STATUS: SAT " );
                else if ( Status == 1 )
                    fprintf( pFile, "STATUS: UNSAT " );
                fprintf( pFile, "%s\n", Abc_ObjName(Abc_NtkPo(pNtk, i)) );
                if ( Status != 0 )
                    continue;
                Abc_NtkDumpOneCexSpecial( pFile, pNtk, pTemp );
            }
            fclose( pFile );
        }
        else if ( fShort )
        {
            printf( "Status array contains %d SAT, %d UNSAT, and %d UNDEC entries (out of %d).",
                Vec_IntCountEntry(pAbc->vStatuses, 0), Vec_IntCountEntry(pAbc->vStatuses, 1),
                Vec_IntCountEntry(pAbc->vStatuses, -1), Vec_IntSize(pAbc->vStatuses) );
        }
        else
        {
            int i, Entry;
            Vec_IntForEachEntry( pAbc->vStatuses, Entry, i )
                printf( "%d=%d  ", i, Entry );
        }
        printf( "\n" );
    }
    return 0;

}

int ABC_function::Abc_CommandShow( Abc_Frame_t * pAbc, int fSeq, int fGateNames, int fUseReverse, int fFlopDep, int fKeepDot)
{
    Abc_Ntk_t * pNtk = Abc_FrameReadNtk(pAbc);

    extern void Abc_NtkShow( Abc_Ntk_t * pNtk, int fGateNames, int fSeq, int fUseReverse, int fKeepDot );
    extern void Abc_NtkShowFlopDependency( Abc_Ntk_t * pNtk );


    if ( pNtk == NULL )
    {
        Abc_Print( -1, "Empty network.\n" );
        return 1;
    }

    if ( fFlopDep )
        Abc_NtkShowFlopDependency( pNtk );
    else
        Abc_NtkShow( pNtk, fGateNames, fSeq, fUseReverse, fKeepDot );
    return 0;

}

int ABC_function::Abc_CommandCollapse( Abc_Frame_t * pAbc, int fVerbose, int fBddSizeMax, int fDualRail, int fReorder, int fReverse, int fDumpOrder)
{
    Abc_Ntk_t * pNtk, * pNtkRes;

    int c;
    char * pLogFileName = NULL;
    pNtk = Abc_FrameReadNtk(pAbc);

    // set defaults

    if ( pNtk == NULL )
    {
        Abc_Print( -1, "Empty network.\n" );
        return 1;
    }

    if ( !Abc_NtkIsLogic(pNtk) && !Abc_NtkIsStrash(pNtk) )
    {
        Abc_Print( -1, "Can only collapse a logic network or an AIG.\n" );
        return 1;
    }

    // get the new network
    if ( Abc_NtkIsStrash(pNtk) )
        pNtkRes = Abc_NtkCollapse( pNtk, fBddSizeMax, fDualRail, fReorder, fReverse, fDumpOrder, fVerbose );
    else
    {
        pNtk = Abc_NtkStrash( pNtk, 0, 0, 0 );
        pNtkRes = Abc_NtkCollapse( pNtk, fBddSizeMax, fDualRail, fReorder, fReverse, fDumpOrder, fVerbose );
        Abc_NtkDelete( pNtk );
    }
    if ( pNtkRes == NULL )
    {
        Abc_Print( -1, "Collapsing has failed.\n" );
        return 1;
    }
    // replace the current network
    Abc_FrameReplaceCurrentNetwork( pAbc, pNtkRes );
    if ( pLogFileName )
    {
        pAbc->pCex = NULL;
        pAbc->nFrames = -1;
        if ( Abc_NtkNodeNum(pNtkRes) == 0 ) 
            pAbc->Status =  1; // UNSAT
        else
            pAbc->Status = -1; // UNDEC
        Abc_NtkWriteLogFile( pLogFileName, pAbc->pCex, pAbc->Status, pAbc->nFrames, "collapse" );
    }
    return 0;

}

int ABC_function::Abc_CommandStrash( Abc_Frame_t * pAbc, int fAllNodes, int fRecord, int fCleanup, int fComplOuts)
{
    Abc_Ntk_t * pNtk, * pNtkRes;
    Abc_Obj_t * pObj;
    int c;
    pNtk = Abc_FrameReadNtk(pAbc);
    // set defaults

    if ( pNtk == NULL )
    {
        Abc_Print( -1, "Empty network.\n" );
        return 1;
    }

    // get the new network
    pNtkRes = Abc_NtkStrash( pNtk, fAllNodes, fCleanup, fRecord );
    if ( pNtkRes == NULL )
    {
        Abc_Print( -1, "Strashing has failed.\n" );
        return 1;
    }
    if ( fComplOuts )
    Abc_NtkForEachPo( pNtkRes, pObj, c )
        Abc_ObjXorFaninC( pObj, 0 );
    // replace the current network
    Abc_FrameReplaceCurrentNetwork( pAbc, pNtkRes );
    return 0;

}

int ABC_function::Abc_CommandLogic( Abc_Frame_t * pAbc)
{
    Abc_Ntk_t * pNtk, * pNtkRes;

    pNtk = Abc_FrameReadNtk(pAbc);
    // set defaults

    if ( pNtk == NULL )
    {
        Abc_Print( -1, "Empty network.\n" );
        return 1;
    }

    if ( !Abc_NtkIsStrash( pNtk ) )
    {
        Abc_Print( -1, "This command is only applicable to strashed networks.\n" );
        return 1;
    }

    // get the new network
    pNtkRes = Abc_NtkToLogic( pNtk );
    if ( pNtkRes == NULL )
    {
        Abc_Print( -1, "Converting to a logic network has failed.\n" );
        return 1;
    }
    // replace the current network
    Abc_FrameReplaceCurrentNetwork( pAbc, pNtkRes );
    return 0;


}

int ABC_function::Abc_CommandBalance( Abc_Frame_t * pAbc, int fDuplicate, int fSelective, int fUpdateLevel, int fExor, int fVerbose)
{
    Abc_Ntk_t * pNtk, * pNtkRes, * pNtkTemp;
    int c;
    pNtk = Abc_FrameReadNtk(pAbc);

    // set defaults

    if ( pNtk == NULL )
    {
        Abc_Print( -1, "Empty network.\n" );
        return 1;
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
            return 1;
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
        return 1;
    }
    // replace the current network
    Abc_FrameReplaceCurrentNetwork( pAbc, pNtkRes );
    return 0;

}

int ABC_function::Abc_CommandSweep( Abc_Frame_t * pAbc, int fSingle , int fVerbose )
{
    Abc_Ntk_t * pNtk = Abc_FrameReadNtk(pAbc);
    int c;
    if ( pNtk == NULL )
    {
        Abc_Print( -1, "Empty network.\n" );
        return 1;
    }
    if ( !Abc_NtkIsLogic(pNtk) )
    {
        Abc_Print( -1, "The classical (SIS-like) sweep can only be performed on a logic network.\n" );
        return 1;
    }
    // modify the current network
    if ( fSingle )
        Abc_NtkSweepBufsInvs( pNtk, fVerbose );
    else
        Abc_NtkSweep( pNtk, fVerbose );
    return 0;

}

int ABC_function::Abc_CommandRewrite( Abc_Frame_t * pAbc, int fUpdateLevel, int fPrecompute, int fUseZeros, int fVerbose, int fVeryVerbose, int fPlaceEnable)
{   
    Abc_Ntk_t * pNtk = Abc_FrameReadNtk(pAbc), * pDup;
    // external functions
    extern void Rwr_Precompute();
    int c, RetValue;


    if ( fPrecompute )
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
    RetValue = Abc_NtkRewrite( pNtk, fUpdateLevel, fUseZeros, fVerbose, fVeryVerbose, fPlaceEnable );
    if ( RetValue == -1 )
    {
        Abc_FrameReplaceCurrentNetwork( pAbc, pDup );
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

int ABC_function::Abc_CommandRefactor( Abc_Frame_t * pAbc, int nNodeSizeMax, int nMinSaved, int nConeSizeMax, int fUpdateLevel, int fUseZeros, int fUseDcs, int fVerbose)
{
    Abc_Ntk_t * pNtk = Abc_FrameReadNtk(pAbc), * pDup;
    int c, RetValue;
    extern int Abc_NtkRefactor( Abc_Ntk_t * pNtk, int nNodeSizeMax, int nMinSaved, int nConeSizeMax, int fUpdateLevel, int fUseZeros, int fUseDcs, int fVerbose );

    if ( fUseZeros )
        nMinSaved = 0;
    if ( nMinSaved == 0 )
        fUseZeros = 1;

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
    if ( nNodeSizeMax > 15 )
    {
        Abc_Print( -1, "The cone size cannot exceed 15.\n" );
        return 1;
    }

    if ( fUseDcs && nNodeSizeMax >= nConeSizeMax )
    {
        Abc_Print( -1, "For don't-care to work, containing cone should be larger than collapsed node.\n" );
        return 1;
    }

    // modify the current network
    pDup = Abc_NtkDup( pNtk );
    RetValue = Abc_NtkRefactor( pNtk, nNodeSizeMax, nMinSaved, nConeSizeMax, fUpdateLevel, fUseZeros, fUseDcs, fVerbose );
    if ( RetValue == -1 )
    {
        Abc_FrameReplaceCurrentNetwork( pAbc, pDup );
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

int ABC_function::Abc_CommandResubstitute( Abc_Frame_t * pAbc, int RS_CUT_MIN, int RS_CUT_MAX, int nCutsMax, int nNodesMax, int nLevelsOdc, int nMinSaved, int fUpdateLevel, int fUseZeros, int fVerbose, int fVeryVerbose)
{
    Abc_Ntk_t * pNtk = Abc_FrameReadNtk(pAbc);
    int c;
    extern int Abc_NtkResubstitute( Abc_Ntk_t * pNtk, int nCutsMax, int nNodesMax, int nMinSaved, int nLevelsOdc, int fUpdateLevel, int fVerbose, int fVeryVerbose );

    
    if ( fUseZeros )
        nMinSaved = 0;
    if ( nMinSaved == 0 )
        fUseZeros = 1;

    if ( pNtk == NULL )
    {
        Abc_Print( -1, "Empty network.\n" );
        return 1;
    }
    if ( nCutsMax < RS_CUT_MIN || nCutsMax > RS_CUT_MAX )
    {
        Abc_Print( -1, "Can only compute cuts for %d <= K <= %d.\n", RS_CUT_MIN, RS_CUT_MAX );
        return 1;
    }
    if ( nNodesMax < 0 || nNodesMax > 3 )
    {
        Abc_Print( -1, "Can only resubstitute at most 3 nodes.\n" );
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
    if ( !Abc_NtkResubstitute( pNtk, nCutsMax, nNodesMax, nMinSaved, nLevelsOdc, fUpdateLevel, fVerbose, fVeryVerbose ) )
    {
        Abc_Print( -1, "Refactoring has failed.\n" );
        return 1;
    }
    return 0;

}

int ABC_function::Abc_CommandResubCheck( Abc_Frame_t * pAbc, char * pFileR , char *pFileS, int fVerbose)
{
    extern void Res6_ManResubCheck( char * pFileNameRes, char * pFileNameSol, int fVerbose );
    Res6_ManResubCheck( pFileR, pFileS, fVerbose );
    return 0;


}

int ABC_function::Abc_CommandMiter( Abc_Frame_t * pAbc, char* file1, char* file2, int fCheck, int fComb, int fImplic, int fMulti, int nPartSize, int fTrans, int fIgnoreNames)
{
    char Buffer[32];
    Abc_Ntk_t * pNtk, * pNtk1, * pNtk2, * pNtkRes;
    int fDelete1, fDelete2;
    char ** pArgvNew;
    int nArgcNew;
    int c;

    pNtk = Abc_FrameReadNtk(pAbc);


    if ( fTrans )
    {
        if ( (Abc_NtkPoNum(pNtk) & 1) == 1 )
        {
            Abc_Print( -1, "Abc_CommandMiter(): The number of outputs should be even.\n" );
            return 0;
        }
        // replace the current network
        pNtkRes = Abc_NtkDupTransformMiter( pNtk );
        Abc_FrameReplaceCurrentNetwork( pAbc, pNtkRes );
        Abc_Print( 1, "The miter (current network) is transformed by XORing POs pair-wise.\n" );
        return 0;
    }

    //do something
    // pArgvNew = argv + globalUtilOptind;
    // nArgcNew = argc - globalUtilOptind;
    if ( !Abc_NtkPrepareTwoNtks( stdout, pNtk, pArgvNew, nArgcNew, &pNtk1, &pNtk2, &fDelete1, &fDelete2, 1 ) )
        return 1;

    if ( fIgnoreNames )
    {
        if ( !fDelete1 )
        {
            pNtk1 = Abc_NtkStrash( pNtk1, 0, 1, 0 );
            fDelete1 = 1;
        }
        if ( !fDelete2 )
        {
            pNtk2 = Abc_NtkStrash( pNtk2, 0, 1, 0 );
            fDelete2 = 1;
        }
        Abc_NtkShortNames( pNtk1 );
        Abc_NtkShortNames( pNtk2 );
    }
    // compute the miter
    pNtkRes = Abc_NtkMiter( pNtk1, pNtk2, fComb, nPartSize, fImplic, fMulti );
    if ( fDelete1 ) Abc_NtkDelete( pNtk1 );
    if ( fDelete2 ) Abc_NtkDelete( pNtk2 );

    // get the new network
    if ( pNtkRes == NULL )
    {
        Abc_Print( -1, "Miter computation has failed.\n" );
        return 0;
    }
    // replace the current network
    Abc_FrameReplaceCurrentNetwork( pAbc, pNtkRes );
    return 0;

}

int ABC_function::Abc_CommandSop( Abc_Frame_t * pAbc, int fCubeSort, int fMode, int nCubeLimit)
{
    extern void Abc_NtkFaninSort( Abc_Ntk_t * pNtk );
    Abc_Ntk_t * pNtk = Abc_FrameReadNtk(pAbc);
    int c;

    // set defaults
    if ( pNtk == NULL )
    {
        Abc_Print( -1, "Empty network.\n" );
        return 1;
    }
    if ( !Abc_NtkIsLogic(pNtk) )
    {
        Abc_Print( -1, "Converting to SOP is possible only for logic networks.\n" );
        return 1;
    }
    if ( !fCubeSort && Abc_NtkHasBdd(pNtk) && !Abc_NtkBddToSop(pNtk, -1, ABC_INFINITY, 0) )
    {
        Abc_Print( -1, "Converting to SOP has failed.\n" );
        return 0;
    }
    if ( !Abc_NtkToSop(pNtk, fMode, nCubeLimit) )
    {
        Abc_Print( -1, "Converting to SOP has failed.\n" );
        return 0;
    }
    if ( !fCubeSort )
        Abc_NtkFaninSort( pNtk );
    return 0;

}

int ABC_function::Abc_CommandBdd( Abc_Frame_t * pAbc, int fReorder, int fBdd2Sop)
{
    Abc_Ntk_t * pNtk = Abc_FrameReadNtk(pAbc);
    int c;

    if ( pNtk == NULL )
    {
        Abc_Print( -1, "Empty network.\n" );
        return 1;
    }
    if ( !Abc_NtkIsLogic(pNtk) )
    {
        Abc_Print( -1, "Converting to BDD is possible only for logic networks.\n" );
        return 1;
    }
    if ( fBdd2Sop && Abc_NtkHasSop(pNtk) )
        return !Abc_NtkSopToBdd(pNtk);
    if ( Abc_NtkIsBddLogic(pNtk) )
    {
        Abc_Print( -1, "The logic network is already in the BDD form.\n" );
        return 0;
    }
    if ( !Abc_NtkToBdd(pNtk) )
    {
        Abc_Print( -1, "Converting to BDD has failed.\n" );
        return 1;
    }
    return 0;

}

int ABC_function::Abc_CommandReorder( Abc_Frame_t * pAbc, int fVerbose)
{
    Abc_Ntk_t * pNtk = Abc_FrameReadNtk(pAbc);
    int c;
    extern void Abc_NtkBddReorder( Abc_Ntk_t * pNtk, int fVerbose );


    if ( pNtk == NULL )
    {
        Abc_Print( -1, "Empty network.\n" );
        return 1;
    }

    // get the new network
    if ( !Abc_NtkIsBddLogic(pNtk) )
    {
        Abc_Print( -1, "Variable reordering is possible when node functions are BDDs (run \"bdd\").\n" );
        return 1;
    }
    Abc_NtkBddReorder( pNtk, fVerbose );
    return 0;

}

int ABC_function::Abc_CommandMap( Abc_Frame_t * pAbc, double DelayTarget, double AreaMulti, double DelayMulti, float LogFan, float Slew, float Gain, int nGatesMin, int fAreaOnly, int fRecovery, int fSweep, int fSwitching, int fSkipFanout, int fUseProfile, int fUseBuffs, int fVerbose)
{
    Abc_Ntk_t * pNtk, * pNtkRes;
    char Buffer[100];
    int c;
    extern Abc_Ntk_t * Abc_NtkMap( Abc_Ntk_t * pNtk, double DelayTarget, double AreaMulti, double DelayMulti, float LogFan, float Slew, float Gain, int nGatesMin, int fRecovery, int fSwitching, int fSkipFanout, int fUseProfile, int fUseBuffs, int fVerbose );
    extern int Abc_NtkFraigSweep( Abc_Ntk_t * pNtk, int fUseInv, int fExdc, int fVerbose, int fVeryVerbose );

    pNtk = Abc_FrameReadNtk(pAbc);

    if ( pNtk == NULL )
    {
        Abc_Print( -1, "Empty network.\n" );
        return 1;
    }

    if ( fAreaOnly )
        DelayTarget = ABC_INFINITY;

    if ( !Abc_NtkIsStrash(pNtk) )
    {
        pNtk = Abc_NtkStrash( pNtk, 0, 0, 0 );
        if ( pNtk == NULL )
        {
            Abc_Print( -1, "Strashing before mapping has failed.\n" );
            return 1;
        }
        pNtk = Abc_NtkBalance( pNtkRes = pNtk, 0, 0, 1 );
        Abc_NtkDelete( pNtkRes );
        if ( pNtk == NULL )
        {
            Abc_Print( -1, "Balancing before mapping has failed.\n" );
            return 1;
        }
        Abc_Print( 0, "The network was strashed and balanced before mapping.\n" );
        // get the new network
        pNtkRes = Abc_NtkMap( pNtk, DelayTarget, AreaMulti, DelayMulti, LogFan, Slew, Gain, nGatesMin, fRecovery, fSwitching, fSkipFanout, fUseProfile, fUseBuffs, fVerbose );
        if ( pNtkRes == NULL )
        {
            Abc_NtkDelete( pNtk );
            Abc_Print( -1, "Mapping has failed.\n" );
            return 1;
        }
        Abc_NtkDelete( pNtk );
    }
    else
    {
        // get the new network
        pNtkRes = Abc_NtkMap( pNtk, DelayTarget, AreaMulti, DelayMulti, LogFan, Slew, Gain, nGatesMin, fRecovery, fSwitching, fSkipFanout, fUseProfile, fUseBuffs, fVerbose );
        if ( pNtkRes == NULL )
        {
            Abc_Print( -1, "Mapping has failed.\n" );
            return 1;
        }
    }

    if ( fSweep )
    {
        Abc_NtkFraigSweep( pNtkRes, 0, 0, 0, 0 );
        if ( Abc_NtkHasMapping(pNtkRes) )
        {
            pNtkRes = Abc_NtkDupDfs( pNtk = pNtkRes );
            Abc_NtkDelete( pNtk );
        }
    }

    // replace the current network
    Abc_FrameReplaceCurrentNetwork( pAbc, pNtkRes );
    return 0;
}

int ABC_function::Abc_CommandAmap( Abc_Frame_t * pAbc, float fADratio, int fSweep , int nIterFlow, int nIterArea, int nCutsMax, int fUseMuxes, int fUseXors, int fFreeInvs, float fEpsilon, int fVerbose)
{
    Amap_Par_t Pars, * pPars = &Pars;
    Abc_Ntk_t * pNtk, * pNtkRes;
    int c;
    extern Abc_Ntk_t * Abc_NtkDarAmap( Abc_Ntk_t * pNtk, Amap_Par_t * pPars );
    extern int Abc_NtkFraigSweep( Abc_Ntk_t * pNtk, int fUseInv, int fExdc, int fVerbose, int fVeryVerbose );

    pNtk = Abc_FrameReadNtk(pAbc);
    // set defaults
    Amap_ManSetDefaultParams( pPars );
    pPars->nIterFlow = nIterFlow;
    pPars->nIterArea = nIterArea;
    pPars->nCutsMax = nCutsMax;
    pPars->fEpsilon = fEpsilon;
    pPars->fADratio = fADratio;
    pPars->fUseMuxes ^= fUseMuxes;
    pPars->fUseXors ^= fUseXors;
    pPars->fFreeInvs ^= fFreeInvs;
    pPars->fVerbose ^= fVerbose;

    if ( pNtk == NULL )
    {
        Abc_Print( -1, "Empty network.\n" );
        return 1;
    }

    if ( !Abc_NtkIsStrash(pNtk) )
    {
        pNtk = Abc_NtkStrash( pNtk, 0, 0, 0 );
        if ( pNtk == NULL )
        {
            Abc_Print( -1, "Strashing before mapping has failed.\n" );
            return 1;
        }
        pNtk = Abc_NtkBalance( pNtkRes = pNtk, 0, 0, 1 );
        Abc_NtkDelete( pNtkRes );
        if ( pNtk == NULL )
        {
            Abc_Print( -1, "Balancing before mapping has failed.\n" );
            return 1;
        }
        Abc_Print( 0, "The network was strashed and balanced before mapping.\n" );
        // get the new network
        pNtkRes = Abc_NtkDarAmap( pNtk, pPars );
        if ( pNtkRes == NULL )
        {
            Abc_NtkDelete( pNtk );
            Abc_Print( -1, "Mapping has failed.\n" );
            return 1;
        }
        Abc_NtkDelete( pNtk );
    }
    else
    {
        // get the new network
        pNtkRes = Abc_NtkDarAmap( pNtk, pPars );
        if ( pNtkRes == NULL )
        {
            Abc_Print( -1, "Mapping has failed.\n" );
            return 1;
        }
    }

    if ( fSweep )
    {
        Abc_NtkFraigSweep( pNtkRes, 0, 0, 0, 0 );
        if ( Abc_NtkHasMapping(pNtkRes) )
        {
            pNtkRes = Abc_NtkDupDfs( pNtk = pNtkRes );
            Abc_NtkDelete( pNtk );
        }
    }
    // replace the current network
    Abc_FrameReplaceCurrentNetwork( pAbc, pNtkRes );
    return 0;

}

int ABC_function::Abc_CommandCec( Abc_Frame_t * pAbc, Abc_Ntk_t * pNtk1, Abc_Ntk_t * pNtk2, int fSat, int fVerbose, int nSeconds, int nPartSize, int nConfLimit, int nInsLimit, int fPartition, int fIgnoreNames)
{
    // char Buffer[16];
    int c;

    extern void Abc_NtkCecSat( Abc_Ntk_t * pNtk1, Abc_Ntk_t * pNtk2, int nConfLimit, int nInsLimit );
    extern void Abc_NtkCecFraig( Abc_Ntk_t * pNtk1, Abc_Ntk_t * pNtk2, int nSeconds, int fVerbose );
    extern void Abc_NtkCecFraigPart( Abc_Ntk_t * pNtk1, Abc_Ntk_t * pNtk2, int nSeconds, int nPartSize, int fVerbose );
    extern void Abc_NtkCecFraigPartAuto( Abc_Ntk_t * pNtk1, Abc_Ntk_t * pNtk2, int nSeconds, int fVerbose );

    // pNtk = Abc_FrameReadNtk(pAbc);
    // set defaults
    

    



    if ( fIgnoreNames )
    {
        if(!Abc_NtkIsStrash(pNtk1)){
            pNtk1 = Abc_NtkStrash( pNtk1, 0, 1, 0 );
        }
        if(!Abc_NtkIsStrash(pNtk2)){
            pNtk2 = Abc_NtkStrash( pNtk2, 0, 1, 0 );
        }
        Abc_NtkShortNames( pNtk1 );
        Abc_NtkShortNames( pNtk2 );
    }

    // perform equivalence checking
    if ( fPartition )
        Abc_NtkCecFraigPartAuto( pNtk1, pNtk2, nSeconds, fVerbose );
    else if ( nPartSize )
        Abc_NtkCecFraigPart( pNtk1, pNtk2, nSeconds, nPartSize, fVerbose );
    else if ( fSat )
        Abc_NtkCecSat( pNtk1, pNtk2, nConfLimit, nInsLimit );
    else
        Abc_NtkCecFraig( pNtk1, pNtk2, nSeconds, fVerbose );

    return 0;

}

//gia
int ABC_function::Abc_CommandAbc9Get( Abc_Frame_t * pAbc, int fGiaSimple, int fMapped, int fNames, int fVerbose)
{
    extern Aig_Man_t * Abc_NtkToDarChoices( Abc_Ntk_t * pNtk );
    Abc_Ntk_t * pStrash;
    Aig_Man_t * pAig;
    Gia_Man_t * pGia, * pTemp;
    char * pInits;
    int c;

    if ( pAbc->pNtkCur == NULL )
    {
        Abc_Print( -1, "There is no current network\n" );
        return 1;
    }
    if ( !Abc_NtkIsStrash( pAbc->pNtkCur ) )
    {
        if ( fGiaSimple || fMapped )
        {
            assert( Abc_NtkIsLogic(pAbc->pNtkCur) );
            Abc_NtkToAig( pAbc->pNtkCur );
            pGia = Abc_NtkAigToGia( pAbc->pNtkCur, fGiaSimple );
        }
        else
        {
            // derive comb GIA
            pStrash = Abc_NtkStrash( pAbc->pNtkCur, 0, 1, 0 );
            pAig = Abc_NtkToDar( pStrash, 0, 0 );
            Abc_NtkDelete( pStrash );
            pGia = Gia_ManFromAig( pAig );
            Aig_ManStop( pAig );
            // perform undc/zero
            pInits = Abc_NtkCollectLatchValuesStr( pAbc->pNtkCur );
            pGia = Gia_ManDupZeroUndc( pTemp = pGia, pInits, 0, 0, fVerbose );
            Gia_ManStop( pTemp );
            ABC_FREE( pInits );
        }
    }
    else
    {
        if ( Abc_NtkGetChoiceNum(pAbc->pNtkCur) )
            pAig = Abc_NtkToDarChoices( pAbc->pNtkCur );
        else
            pAig = Abc_NtkToDar( pAbc->pNtkCur, 0, 1 );
        pGia = Gia_ManFromAig( pAig );
        Aig_ManStop( pAig );
    }
    // copy names
    if ( fNames )
    {
        pGia->vNamesIn  = Abc_NtkCollectCiNames( pAbc->pNtkCur );
        pGia->vNamesOut = Abc_NtkCollectCoNames( pAbc->pNtkCur );
    }
    // copy user timing information
    if ( pAbc->pNtkCur->pManTime != NULL )
    {
        Abc_Ntk_t * pNtk = pAbc->pNtkCur;
        Vec_FltFreeP( &pGia->vInArrs );
        Vec_FltFreeP( &pGia->vOutReqs );
        pGia->DefInArrs  = Abc_NtkReadDefaultArrivalWorst(pNtk);
        pGia->DefOutReqs = Abc_NtkReadDefaultRequiredWorst(pNtk);
        pGia->vInArrs  = Vec_FltAllocArray( Abc_NtkGetCiArrivalFloats(pNtk), Abc_NtkCiNum(pNtk) );
        pGia->vOutReqs = Vec_FltAllocArray( Abc_NtkGetCoRequiredFloats(pNtk), Abc_NtkCoNum(pNtk) );
        pGia->And2Delay = pNtk->AndGateDelay;
    }
    Abc_FrameUpdateGia( pAbc, pGia );
    return 0;

}

int ABC_function::Abc_CommandAbc9Put( Abc_Frame_t * pAbc, int fStatusClear, int fFindEnables, int fUseBuffs, int fVerbose)
{
    extern Abc_Ntk_t * Abc_NtkFromDarChoices( Abc_Ntk_t * pNtkOld, Aig_Man_t * pMan );
    extern void Abc_NtkRedirectCiCo( Abc_Ntk_t * pNtk );
    extern Abc_Ntk_t * Abc_NtkFromCellMappedGia( Gia_Man_t * p, int fUseBuffs );
    extern Abc_Ntk_t * Abc_NtkFromMappedGia( Gia_Man_t * p, int fFindEnables, int fUseBuffs );

    Aig_Man_t * pMan;
    Abc_Ntk_t * pNtk = Abc_FrameReadNtk(pAbc);

    int c;

    if ( pAbc->pGia == NULL )
    {
        Abc_Print( -1, "Empty network.\n" );
        return 1;
    }
    if ( fFindEnables )
        pNtk = Abc_NtkFromMappedGia( pAbc->pGia, 1, fUseBuffs );
    else if ( Gia_ManHasCellMapping(pAbc->pGia) )
        pNtk = Abc_NtkFromCellMappedGia( pAbc->pGia, fUseBuffs );
    else if ( Gia_ManHasMapping(pAbc->pGia) || pAbc->pGia->pMuxes )
        pNtk = Abc_NtkFromMappedGia( pAbc->pGia, 0, fUseBuffs );
    else if ( Gia_ManHasDangling(pAbc->pGia) == 0 )
    {
        pMan = Gia_ManToAig( pAbc->pGia, 0 );
        pNtk = Abc_NtkFromAigPhase( pMan );
        pNtk->pName = Extra_UtilStrsav(pMan->pName);
        Aig_ManStop( pMan );
    }
    else
    {
        Abc_Ntk_t * pNtkNoCh;
        Abc_Print( -1, "Transforming AIG with %d choice nodes.\n", Gia_ManEquivCountClasses(pAbc->pGia) );
        // create network without choices
        pMan = Gia_ManToAig( pAbc->pGia, 0 );
        pNtkNoCh = Abc_NtkFromAigPhase( pMan );
        pNtkNoCh->pName = Extra_UtilStrsav(pMan->pName);
        Aig_ManStop( pMan );
        // derive network with choices
        pMan = Gia_ManToAig( pAbc->pGia, 1 );
        pNtk = Abc_NtkFromDarChoices( pNtkNoCh, pMan );
        Abc_NtkDelete( pNtkNoCh );
        Aig_ManStop( pMan );
    }
    // transfer PI names to pNtk
    if ( pAbc->pGia->vNamesIn )
    {
        Abc_Obj_t * pObj;
        int i;
        Abc_NtkForEachCi( pNtk, pObj, i ) {
            if (i < Vec_PtrSize(pAbc->pGia->vNamesIn)) {
                Nm_ManDeleteIdName(pNtk->pManName, pObj->Id);
                Abc_ObjAssignName( pObj, (char *)Vec_PtrEntry(pAbc->pGia->vNamesIn, i), NULL );
            }
        }
    }
    // transfer PO names to pNtk
    if ( pAbc->pGia->vNamesOut )
    {
        char pSuffix[1000];
        Abc_Obj_t * pObj; int i;
        unsigned char nDigits = (unsigned char)Abc_Base10Log( Abc_NtkLatchNum(pNtk) );
        Abc_NtkForEachCo( pNtk, pObj, i ) {
            if (i < Vec_PtrSize(pAbc->pGia->vNamesOut)) {
                Nm_ManDeleteIdName(pNtk->pManName, pObj->Id);
                if ( Abc_ObjIsPo(pObj) )
                    Abc_ObjAssignName( pObj, (char *)Vec_PtrEntry(pAbc->pGia->vNamesOut, i), NULL );
                else
                {
                    assert( i >= Abc_NtkPoNum(pNtk) );
                    sprintf( pSuffix, "_li%0*d", nDigits, i-Abc_NtkPoNum(pNtk) );
                    Abc_ObjAssignName( pObj, (char *)Vec_PtrEntry(pAbc->pGia->vNamesOut, i), pSuffix );
                }
            }
        }
    }
    if ( pAbc->pGia->vNamesNode )
        Abc_Print( 0, "Internal nodes names are not transferred.\n" );

    // decouple CI/CO with the same name
    if ( pAbc->pGia->vNamesIn || pAbc->pGia->vNamesOut )
        Abc_NtkRedirectCiCo( pNtk );

    // transfer timing information
    if ( pAbc->pGia->vInArrs || pAbc->pGia->vOutReqs )
    {
        Abc_Obj_t * pObj; int i;
        Abc_NtkTimeInitialize( pNtk, NULL );
        Abc_NtkTimeSetDefaultArrival( pNtk, pAbc->pGia->DefInArrs, pAbc->pGia->DefInArrs );
        Abc_NtkTimeSetDefaultRequired( pNtk, pAbc->pGia->DefOutReqs, pAbc->pGia->DefOutReqs );
        if ( pAbc->pGia->vInArrs )
            Abc_NtkForEachCi( pNtk, pObj, i )
                Abc_NtkTimeSetArrival( pNtk, Abc_ObjId(pObj), Vec_FltEntry(pAbc->pGia->vInArrs, i), Vec_FltEntry(pAbc->pGia->vInArrs, i) );
        if ( pAbc->pGia->vOutReqs )
            Abc_NtkForEachCo( pNtk, pObj, i )
                Abc_NtkTimeSetRequired( pNtk, Abc_ObjId(pObj), Vec_FltEntry(pAbc->pGia->vOutReqs, i), Vec_FltEntry(pAbc->pGia->vOutReqs, i) );
    }

    // replace the current network
    Abc_FrameReplaceCurrentNetwork( pAbc, pNtk );
    if ( fStatusClear )
        Abc_FrameClearVerifStatus( pAbc );
    return 0;

}

int ABC_function::Abc_CommandAbc9Ps( Abc_Frame_t * pAbc, int fBest, int fTents, int fSwitch, int fCut, int fNpn, int fLutProf, int fMuxXor, int fMiter, int fSlacks, int fSkipMap, int fNoColor, char * pDumpFile)
{
    Gps_Par_t Pars, * pPars = &Pars;
    int c;
    memset( pPars, 0, sizeof(Gps_Par_t) );
    pPars->fTents = fTents;
    pPars->fSwitch = fSwitch;
    pPars->fCut = fCut;
    pPars->fNpn = fNpn;
    pPars->fLutProf = fLutProf;
    pPars->fMuxXor = fMuxXor;
    pPars->fMiter = fMiter;
    pPars->fSlacks = fSlacks;
    pPars->fSkipMap = fSkipMap;
    pPars->fNoColor = fNoColor;
    pPars->pDumpFile = pDumpFile;
    fBest = fBest;
    if ( fBest )
    {
        if ( pAbc->pGiaBest == NULL )
        {
            Abc_Print( -1, "Abc_CommandAbc9Ps(): There is no AIG.\n" );
            return 1;
        }
        Gia_ManPrintStats( pAbc->pGiaBest, pPars );
    }
    else
    {
        if ( pAbc->pGia == NULL )
        {
            Abc_Print( -1, "Abc_CommandAbc9Ps(): There is no AIG.\n" );
            return 1;
        }
        Gia_ManPrintStats( pAbc->pGia, pPars );
    }
    return 0;

}

int ABC_function::Abc_CommandAbc9Resub( Abc_Frame_t * pAbc, int nNodes ,int nSupp , int nDivs , int fVerbose , int fVeryVerbose )
{
    extern Gia_Man_t * Gia_ManResub1( char * pFileName, int nNodes, int nSupp, int nDivs, int iChoice, int fUseXor, int fVerbose, int fVeryVerbose );
    extern Gia_Man_t * Gia_ManResub2( Gia_Man_t * pGia, int nNodes, int nSupp, int nDivs, int iChoice, int fUseXor, int fVerbose, int fVeryVerbose );
    Gia_Man_t * pTemp;
    int c;
    
    // if ( argc == globalUtilOptind + 1 )
    // {
    //     pTemp = Gia_ManResub1( argv[globalUtilOptind], nNodes, nSupp, nDivs, 0, 0, fVerbose, fVeryVerbose );
    //     Abc_FrameUpdateGia( pAbc, pTemp );
    //     return 0;
    // }
    if ( pAbc->pGia == NULL )
    {
        Abc_Print( -1, "Abc_CommandAbc9Resub(): There is no AIG.\n" );
        return 1;
    }
    //?? ask Alan
    pTemp = Gia_ManResub2( pAbc->pGia, nNodes, nSupp, nDivs, 0, 0, fVerbose, fVeryVerbose );
    if(pTemp == NULL){
        Abc_Print( -1, "resub fail.\n" );
        return 1;
    }
    Abc_FrameUpdateGia( pAbc, pTemp );
    return 0;

}

int ABC_function::Abc_CommandAbc9Syn2( Abc_Frame_t * pAbc, int fVerbose, int fOldAlgo, int fCoarsen, int fCutMin, int nRelaxRatio, int fDelayMin, int fVeryVerbose)
{
    Gia_Man_t * pTemp;
    int c;
    if ( pAbc->pGia == NULL )
    {
        Abc_Print( -1, "Abc_CommandAbc9Syn2(): There is no AIG.\n" );
        return 1;
    }
    if ( fDelayMin )
    {
        If_DsdMan_t * p = (If_DsdMan_t *)Abc_FrameReadManDsd();
        if ( p && If_DsdManVarNum(p) < 6 )
        {
            printf( "DSD manager has incompatible number of variables. Delay minimization is not performed.\n" );
            fDelayMin = 0;
        }
    }
    pTemp = Gia_ManAigSyn2( pAbc->pGia, fOldAlgo, fCoarsen, fCutMin, nRelaxRatio, fDelayMin, fVerbose, fVeryVerbose );
    Abc_FrameUpdateGia( pAbc, pTemp );
    return 0;

}

int ABC_function::Abc_CommandAbc9Syn3( Abc_Frame_t * pAbc, int fVerbose, int fVeryVerbose)
{
    Gia_Man_t * pTemp;
    int c;
    
    if ( pAbc->pGia == NULL )
    {
        Abc_Print( -1, "Abc_CommandAbc9Syn3(): There is no AIG.\n" );
        return 1;
    }
    pTemp = Gia_ManAigSyn3( pAbc->pGia, fVerbose, fVeryVerbose );
    Abc_FrameUpdateGia( pAbc, pTemp );
    return 0;

}

int ABC_function::Abc_CommandAbc9DeepSyn( Abc_Frame_t * pAbc, int nIters, int nNoImpr, int TimeOut, int nAnds, int Seed, int fUseTwo, int fVerbose)
{
    extern Gia_Man_t * Gia_ManDeepSyn( Gia_Man_t * pGia, int nIters, int nNoImpr, int TimeOut, int nAnds, int Seed, int fUseTwo, int fVerbose );
    Gia_Man_t * pTemp; int c;
    if ( pAbc->pGia == NULL )
    {
        Abc_Print( -1, "Abc_CommandAbc9DeepSyn(): There is no AIG.\n" );
        return 0;
    }
    pTemp = Gia_ManDeepSyn( pAbc->pGia, nIters, nNoImpr, TimeOut, nAnds, Seed, fUseTwo, fVerbose );
    Abc_FrameUpdateGia( pAbc, pTemp );
    return 0;

}

int ABC_function::Abc_CommandAbc9StochSyn( Abc_Frame_t * pAbc, int nMaxSize, int nIters, int TimeOut, int Seed, int nProcs, int fVerbose, char * pScript)
{
    extern void Gia_ManStochSyn( int nMaxSize, int nIters, int TimeOut, int Seed, int fVerbose, char * pScript, int nProcs );
    int c;
    if ( pAbc->pGia == NULL )
    {
        Abc_Print( -1, "Abc_CommandAbc9StochSyn(): There is no AIG.\n" );
        return 0;
    }
    pScript = Abc_UtilStrsav(pScript);
    Gia_ManStochSyn( nMaxSize, nIters, TimeOut, Seed, fVerbose, pScript, nProcs );
    ABC_FREE( pScript );
    return 0;

}

int ABC_function::Scl_CommandLeak2Area( Abc_Frame_t * pAbc, float A, float B, int fVerbose)
{

    int c;
    if ( pAbc->pLibScl == NULL )
    {
        fprintf( pAbc->Err, "There is no Liberty library available.\n" );
        return 1;
    }
    // update the current library
    Abc_SclConvertLeakageIntoArea( (SC_Lib *)pAbc->pLibScl, A, B );
    return 0;

}

int ABC_function::Scl_CommandTopo( Abc_Frame_t * pAbc, int fVerbose)
{
    Abc_Ntk_t * pNtk = Abc_FrameReadNtk(pAbc);
    Abc_Ntk_t * pNtkRes;
    int c;

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
    pNtkRes = Abc_NtkDupDfs( pNtk );
    if ( pNtkRes == NULL )
    {
        Abc_Print( -1, "The command has failed.\n" );
        return 1;
    }
    // replace the current network
    Abc_FrameReplaceCurrentNetwork( pAbc, pNtkRes );
    return 0;

}


int ABC_function::Scl_CommandUpsize( Abc_Frame_t * pAbc)
{
    SC_SizePars Pars, * pPars = &Pars;
    Abc_Ntk_t * pNtk = Abc_FrameReadNtk(pAbc);
    int c;
    memset( pPars, 0, sizeof(SC_SizePars) );
    pPars->nIters        = 1000;
    pPars->nIterNoChange =   50;
    pPars->Window        =    1;
    pPars->Ratio         =   10;
    pPars->Notches       = 1000;
    pPars->DelayUser     =    0;
    pPars->DelayGap      =    0;
    pPars->TimeOut       =    0;
    pPars->BuffTreeEst   =    0;
    pPars->BypassFreq    =    0;
    pPars->fUseDept      =    1;
    pPars->fUseWireLoads =    0;
    pPars->fDumpStats    =    0;
    pPars->fVerbose      =    0;
    pPars->fVeryVerbose  =    0;

    if ( Abc_FrameReadNtk(pAbc) == NULL )
    {
        fprintf( pAbc->Err, "There is no current network.\n" );
        return 1;
    }
    if ( !Abc_NtkHasMapping(Abc_FrameReadNtk(pAbc)) )
    {
        fprintf( pAbc->Err, "The current network is not mapped.\n" );
        return 1;
    }
    if ( !Abc_SclCheckNtk(Abc_FrameReadNtk(pAbc), 0) )
    {
        fprintf( pAbc->Err, "The current network is not in a topo order (run \"topo\").\n" );
        return 1;
    }
    if ( !pAbc->pLibScl || !Abc_SclHasDelayInfo(pAbc->pLibScl) )
    {
        Abc_Print( -1, "Library delay info is not available.\n" );
        return 1;
    }

    Abc_SclUpsizePerform( (SC_Lib *)pAbc->pLibScl, pNtk, pPars, NULL );
    return 0;

}

int ABC_function::Scl_CommandDnsize( Abc_Frame_t * pAbc)
{
    SC_SizePars Pars, * pPars = &Pars;
    Abc_Ntk_t * pNtk = Abc_FrameReadNtk(pAbc);
    int c;
    memset( pPars, 0, sizeof(SC_SizePars) );
    pPars->nIters        =    5;
    pPars->nIterNoChange =   50;
    pPars->Notches       = 1000;
    pPars->DelayUser     =    0;
    pPars->DelayGap      = 1000;
    pPars->TimeOut       =    0;
    pPars->BuffTreeEst   =    0;
    pPars->fUseDept      =    1;
    pPars->fUseWireLoads =    0;
    pPars->fDumpStats    =    0;
    pPars->fVerbose      =    0;
    pPars->fVeryVerbose  =    0;

    if ( Abc_FrameReadNtk(pAbc) == NULL )
    {
        fprintf( pAbc->Err, "There is no current network.\n" );
        return 1;
    }
    if ( !Abc_NtkHasMapping(Abc_FrameReadNtk(pAbc)) )
    {
        fprintf( pAbc->Err, "The current network is not mapped.\n" );
        return 1;
    }
    if ( !Abc_SclCheckNtk(Abc_FrameReadNtk(pAbc), 0) )
    {
        fprintf( pAbc->Err, "The current network is not in a topo order (run \"topo\").\n" );
        return 1;
    }
    if ( !pAbc->pLibScl || !Abc_SclHasDelayInfo(pAbc->pLibScl) )
    {
        Abc_Print( -1, "Library delay info is not available.\n" );
        return 1;
    }

    Abc_SclDnsizePerform( (SC_Lib *)pAbc->pLibScl, pNtk, pPars, NULL );
    return 0;

}

int ABC_function::Scl_CommandUnBuffer( Abc_Frame_t * pAbc, int fRemInv, int fVerbose)
{
    Abc_Ntk_t * pNtkRes, * pNtk = Abc_FrameReadNtk(pAbc);
    int c;
    

    if ( pNtk == NULL )
    {
        fprintf( pAbc->Err, "There is no current network.\n" );
        return 1;
    }
    if ( !Abc_NtkIsLogic(pNtk) )
    {
        fprintf( pAbc->Err, "The current network is not a logic network.\n" );
        return 1;
    }
    if ( fRemInv )
        pNtkRes = Abc_SclUnBufferPhase( pNtk, fVerbose );
    else
        pNtkRes = Abc_SclUnBufferPerform( pNtk, fVerbose );
    if ( pNtkRes == NULL )
    {
        Abc_Print( -1, "The command has failed.\n" );
        return 1;
    }
    Abc_FrameReplaceCurrentNetwork( pAbc, pNtkRes );
    return 0;

}

int ABC_function::Scl_CommandBuffer( Abc_Frame_t * pAbc)
{
    SC_BusPars Pars, * pPars = &Pars;
    Abc_Ntk_t * pNtkRes, * pNtk = Abc_FrameReadNtk(pAbc);
    int c;
    memset( pPars, 0, sizeof(SC_BusPars) );
    pPars->GainRatio     =  300;
    pPars->Slew          = pAbc->pLibScl ? Abc_SclComputeAverageSlew((SC_Lib *)pAbc->pLibScl) : 100;
    pPars->nDegree       =   10;
    pPars->fSizeOnly     =    0;
    pPars->fAddBufs      =    1;
    pPars->fBufPis       =    0;
    pPars->fUseWireLoads =    0;
    pPars->fVerbose      =    0;
    pPars->fVeryVerbose  =    0;

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
    if ( !pPars->fSizeOnly && !pPars->fAddBufs && pNtk->vPhases == NULL )
    {
        Abc_Print( -1, "Fanin phase information is not available.\n" );
        return 1;
    }
    if ( !pAbc->pLibScl || !Abc_SclHasDelayInfo(pAbc->pLibScl) )
    {
        Abc_Print( -1, "Library delay info is not available.\n" );
        return 1;
    }
    // modify the current network
    pNtkRes = Abc_SclBufferingPerform( pNtk, (SC_Lib *)pAbc->pLibScl, pPars );
    if ( pNtkRes == NULL )
    {
        Abc_Print( -1, "The command has failed.\n" );
        return 1;
    }
    // replace the current network
    Abc_FrameReplaceCurrentNetwork( pAbc, pNtkRes );
    return 0;

}

int ABC_function::Abc_CommandPrintGates( Abc_Frame_t * pAbc,int fUseLibrary, int fUpdateProfile)
{
    Abc_Ntk_t * pNtk = Abc_FrameReadNtk(pAbc);  
    extern void Abc_NtkPrintGates( Abc_Ntk_t * pNtk, int fUseLibrary, int fUpdateProfile );
    Extra_UtilGetoptReset();
    if ( pNtk == NULL )
    {
        Abc_Print( -1, "Empty network.\n" );
        return 1;
    }
    if ( Abc_NtkHasAig(pNtk) )
    {
        Abc_Print( -1, "Printing gates does not work for AIGs and sequential AIGs.\n" );
        return 1;
    }    
    Abc_NtkPrintGates( pNtk, fUseLibrary, fUpdateProfile );    
    return 0;
}


}