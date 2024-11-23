// #include "base/abc/abc.h"
// #include "base/main/main.h"
// #include "base/main/mainInt.h"
// #include "proof/fraig/fraig.h"
// #include "opt/fxu/fxu.h"
// #include "opt/fxch/Fxch.h"
// #include "opt/cut/cut.h"
// #include "map/fpga/fpga.h"
// #include "map/if/if.h"
// #include "opt/sim/sim.h"
// #include "opt/res/res.h"
// #include "opt/lpk/lpk.h"
// #include "aig/gia/giaAig.h"
// #include "opt/dar/dar.h"
// #include "opt/mfs/mfs.h"
// #include "proof/fra/fra.h"
// #include "aig/saig/saig.h"
// #include "proof/int/int.h"
// #include "proof/dch/dch.h"
// #include "proof/ssw/ssw.h"
// #include "opt/cgt/cgt.h"
// #include "bool/kit/kit.h"
// #include "map/amap/amap.h"
// #include "opt/ret/retInt.h"
// #include "sat/xsat/xsat.h"
// #include "sat/satoko/satoko.h"
// #include "sat/cnf/cnf.h"
// #include "proof/cec/cec.h"
// #include "proof/acec/acec.h"
// #include "proof/pdr/pdr.h"
// #include "misc/tim/tim.h"
// #include "bdd/llb/llb.h"
// #include "bdd/bbr/bbr.h"
// #include "map/cov/cov.h"
// #include "base/cmd/cmd.h"
// #include "proof/abs/abs.h"
// #include "sat/bmc/bmc.h"
// #include "proof/ssc/ssc.h"
// #include "opt/sfm/sfm.h"
// #include "opt/sbd/sbd.h"
// #include "bool/rpo/rpo.h"
// #include "map/mpm/mpm.h"
// #include "opt/fret/fretime.h"
// #include "opt/nwk/nwkMerge.h"
// #include "base/acb/acbPar.h"
// #include "misc/extra/extra.h"

#include "abc_function.h"

namespace abc{
    extern Aig_Man_t * Abc_NtkToDar( Abc_Ntk_t * pNtk, int fExors, int fRegisters );
    extern Abc_Ntk_t * Abc_NtkFromAigPhase( Aig_Man_t * pMan );

    extern Vec_Ptr_t * Abc_NtkCollectCiNames( Abc_Ntk_t * pNtk );
    extern Vec_Ptr_t * Abc_NtkCollectCoNames( Abc_Ntk_t * pNtk );
int Abc_CommandPrintStats( Abc_Frame_t * pAbc, int argc, char ** argv )
{
    Abc_Ntk_t * pNtk = Abc_FrameReadNtk(pAbc);
    int fFactor;
    int fSaveBest;
    int fDumpResult;
    int fUseLutLib;
    int fPrintTime;
    int fPrintMuxes;
    int fPower;
    int fGlitch;
    int fSkipBuf;
    int fSkipSmall;
    int fPrintMem;
    int c;

    pNtk = Abc_FrameReadNtk(pAbc);

    // set the defaults
    fFactor   = 0;
    fSaveBest = 0;
    fDumpResult = 0;
    fUseLutLib = 0;
    fPrintTime = 0;
    fPrintMuxes = 0;
    fPower = 0;
    fGlitch = 0;
    fSkipBuf = 0;
    fSkipSmall = 0;
    fPrintMem = 0;
    Extra_UtilGetoptReset();
    while ( ( c = Extra_UtilGetopt( argc, argv, "fbdltmpgscuh" ) ) != EOF )
    {
        switch ( c )
        {
        case 'f':
            fFactor ^= 1;
            break;
        case 'b':
            fSaveBest ^= 1;
            break;
        case 'd':
            fDumpResult ^= 1;
            break;
        case 'l':
            fUseLutLib ^= 1;
            break;
        case 't':
            fPrintTime ^= 1;
            break;
        case 'm':
            fPrintMuxes ^= 1;
            break;
        case 'p':
            fPower ^= 1;
            break;
        case 'g':
            fGlitch ^= 1;
            break;
        case 's':
            fSkipBuf ^= 1;
            break;
        case 'c':
            fSkipSmall ^= 1;
            break;
        case 'u':
            fPrintMem ^= 1;
            break;
        case 'h':
            goto usage;
        default:
            goto usage;
        }
    }

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

usage:
    Abc_Print( -2, "usage: print_stats [-fbdltmpgscuh]\n" );
    Abc_Print( -2, "\t        prints the network statistics\n" );
    Abc_Print( -2, "\t-f    : toggles printing the literal count in the factored forms [default = %s]\n", fFactor? "yes": "no" );
    Abc_Print( -2, "\t-b    : toggles saving the best logic network in \"best.blif\" [default = %s]\n", fSaveBest? "yes": "no" );
    Abc_Print( -2, "\t-d    : toggles dumping statistics about the network into file [default = %s]\n", fDumpResult? "yes": "no" );
    Abc_Print( -2, "\t-l    : toggles printing delay of LUT mapping using LUT library [default = %s]\n", fSaveBest? "yes": "no" );
    Abc_Print( -2, "\t-t    : toggles printing runtime statistics [default = %s]\n", fPrintTime? "yes": "no" );
    Abc_Print( -2, "\t-m    : toggles printing MUX statistics [default = %s]\n", fPrintMuxes? "yes": "no" );
    Abc_Print( -2, "\t-p    : toggles printing power dissipation due to switching [default = %s]\n", fPower? "yes": "no" );
    Abc_Print( -2, "\t-g    : toggles printing percentage of increased power due to glitching [default = %s]\n", fGlitch? "yes": "no" );
    Abc_Print( -2, "\t-s    : toggles not counting single-output nodes as nodes [default = %s]\n", fSkipBuf? "yes": "no" );
    Abc_Print( -2, "\t-c    : toggles not counting constants and single-output nodes as nodes [default = %s]\n", fSkipSmall? "yes": "no" );
    Abc_Print( -2, "\t-u    : toggles printing memory usage [default = %s]\n", fPrintMem? "yes": "no" );
    Abc_Print( -2, "\t-h    : print the command usage\n");
    return 1;
}

int Abc_CommandAbc9Get( Abc_Frame_t * pAbc, int argc, char ** argv )
{
    extern Aig_Man_t * Abc_NtkToDarChoices( Abc_Ntk_t * pNtk );
    Abc_Ntk_t * pStrash;
    Aig_Man_t * pAig;
    Gia_Man_t * pGia, * pTemp;
    char * pInits;
    int c, fGiaSimple = 0, fMapped = 0, fNames = 0, fVerbose = 0;
    Extra_UtilGetoptReset();
    while ( ( c = Extra_UtilGetopt( argc, argv, "cmnvh" ) ) != EOF )
    {
        switch ( c )
        {
        case 'c':
            fGiaSimple ^= 1;
            break;
        case 'm':
            fMapped ^= 1;
            break;
        case 'n':
            fNames ^= 1;
            break;
        case 'v':
            fVerbose ^= 1;
            break;
        default:
            goto usage;
        }
    }
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

usage:
    Abc_Print( -2, "usage: &get [-cmnvh] <file>\n" );
    Abc_Print( -2, "\t         converts the current network into GIA and moves it to the &-space\n" );
    Abc_Print( -2, "\t         (if the network is a sequential logic network, normalizes the flops\n" );
    Abc_Print( -2, "\t         to have const-0 initial values, equivalent to \"undc; st; zero\")\n" );
    Abc_Print( -2, "\t-c     : toggles allowing simple GIA to be imported [default = %s]\n", fGiaSimple? "yes": "no" );
    Abc_Print( -2, "\t-m     : toggles preserving the current mapping [default = %s]\n", fMapped? "yes": "no" );
    Abc_Print( -2, "\t-n     : toggles saving CI/CO names of the AIG [default = %s]\n", fNames? "yes": "no" );
    Abc_Print( -2, "\t-v     : toggles additional verbose output [default = %s]\n", fVerbose? "yes": "no" );
    Abc_Print( -2, "\t-h     : print the command usage\n");
    Abc_Print( -2, "\t<file> : the file name\n");
    return 1;
}
int Abc_CommandAbc9Ps( Abc_Frame_t * pAbc, int argc, char ** argv )
{
    Gps_Par_t Pars, * pPars = &Pars;
    int c, fBest = 0;
    memset( pPars, 0, sizeof(Gps_Par_t) );
    Extra_UtilGetoptReset();
    while ( ( c = Extra_UtilGetopt( argc, argv, "Dtpcnlmaszxbh" ) ) != EOF )
    {
        switch ( c )
        {
        case 't':
            pPars->fTents ^= 1;
            break;
        case 'p':
            pPars->fSwitch ^= 1;
            break;
        case 'c':
            pPars->fCut ^= 1;
            break;
        case 'n':
            pPars->fNpn ^= 1;
            break;
        case 'l':
            pPars->fLutProf ^= 1;
            break;
        case 'm':
            pPars->fMuxXor ^= 1;
            break;
        case 'a':
            pPars->fMiter ^= 1;
            break;
        case 's':
            pPars->fSlacks ^= 1;
            break;
        case 'z':
            pPars->fSkipMap ^= 1;
            break;
        case 'x':
            pPars->fNoColor ^= 1;
            break;
        case 'D':
            if ( globalUtilOptind >= argc )
            {
                Abc_Print( -1, "Command line switch \"-D\" should be followed by a file name.\n" );
                goto usage;
            }
            pPars->pDumpFile = argv[globalUtilOptind];
            globalUtilOptind++;
            break;
        case 'b':
            fBest ^= 1;
            break;
        case 'h':
            goto usage;
        default:
            goto usage;
        }
    }
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

usage:
    Abc_Print( -2, "usage: &ps [-tpcnlmaszxbh] [-D file]\n" );
    Abc_Print( -2, "\t          prints stats of the current AIG\n" );
    Abc_Print( -2, "\t-t      : toggle printing BMC tents [default = %s]\n",                pPars->fTents? "yes": "no" );
    Abc_Print( -2, "\t-p      : toggle printing switching activity [default = %s]\n",       pPars->fSwitch? "yes": "no" );
    Abc_Print( -2, "\t-c      : toggle printing the size of frontier cut [default = %s]\n", pPars->fCut? "yes": "no" );
    Abc_Print( -2, "\t-n      : toggle printing NPN classes of functions [default = %s]\n", pPars->fNpn? "yes": "no" );
    Abc_Print( -2, "\t-l      : toggle printing LUT size profile [default = %s]\n",         pPars->fLutProf? "yes": "no" );
    Abc_Print( -2, "\t-m      : toggle printing MUX/XOR statistics [default = %s]\n",       pPars->fMuxXor? "yes": "no" );
    Abc_Print( -2, "\t-a      : toggle printing miter statistics [default = %s]\n",         pPars->fMiter? "yes": "no" );
    Abc_Print( -2, "\t-s      : toggle printing slack distribution [default = %s]\n",       pPars->fSlacks? "yes": "no" );
    Abc_Print( -2, "\t-z      : skip mapping statistics even if mapped [default = %s]\n",   pPars->fSkipMap? "yes": "no" );
    Abc_Print( -2, "\t-x      : toggle using no color in the printout [default = %s]\n",    pPars->fNoColor? "yes": "no" );
    Abc_Print( -2, "\t-b      : toggle printing saved AIG statistics [default = %s]\n",     fBest? "yes": "no" );
    Abc_Print( -2, "\t-D file : file name to dump statistics [default = none]\n" );
    Abc_Print( -2, "\t-h      : print the command usage\n");
    return 1;
}
int Abc_CommandStrash( Abc_Frame_t * pAbc, int argc, char ** argv )
{
    Abc_Ntk_t * pNtk, * pNtkRes;
    Abc_Obj_t * pObj;
    int c;
    int fAllNodes;
    int fRecord;
    int fCleanup;
    int fComplOuts;
    pNtk = Abc_FrameReadNtk(pAbc);
    // set defaults
    fAllNodes = 0;
    fCleanup  = 1;
    fRecord   = 0;
    fComplOuts= 0;
    Extra_UtilGetoptReset();
    while ( ( c = Extra_UtilGetopt( argc, argv, "acrih" ) ) != EOF )
    {
        switch ( c )
        {
        case 'a':
            fAllNodes ^= 1;
            break;
        case 'c':
            fCleanup ^= 1;
            break;
        case 'r':
            fRecord ^= 1;
            break;
        case 'i':
            fComplOuts ^= 1;
            break;
        case 'h':
            goto usage;
        default:
            goto usage;
        }
    }

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
    // Abc_FrameReplaceCurrentNetwork( pAbc, pNtkRes );
    Abc_FrameSetCurrentNetwork( pAbc, pNtkRes );
    return 0;

usage:
    Abc_Print( -2, "usage: strash [-acrih]\n" );
    Abc_Print( -2, "\t        transforms combinational logic into an AIG\n" );
    Abc_Print( -2, "\t-a    : toggles between using all nodes and DFS nodes [default = %s]\n", fAllNodes? "all": "DFS" );
    Abc_Print( -2, "\t-c    : toggles cleanup to remove the dagling AIG nodes [default = %s]\n", fCleanup? "all": "DFS" );
    Abc_Print( -2, "\t-r    : toggles using the record of AIG subgraphs [default = %s]\n", fRecord? "yes": "no" );
    Abc_Print( -2, "\t-i    : toggles complementing the POs of the AIG [default = %s]\n", fComplOuts? "yes": "no" );
    Abc_Print( -2, "\t-h    : print the command usage\n");
    return 1;
}

int Abc_CommandShow( Abc_Frame_t * pAbc, int argc, char ** argv )
{
    Abc_Ntk_t * pNtk = Abc_FrameReadNtk(pAbc);
    int c;
    int fSeq;
    int fGateNames;
    int fUseReverse;
    int fFlopDep;
    int fKeepDot;
    extern void Abc_NtkShow( Abc_Ntk_t * pNtk, int fGateNames, int fSeq, int fUseReverse, int fKeepDot );
    extern void Abc_NtkShowFlopDependency( Abc_Ntk_t * pNtk );

    // set defaults
    fSeq        = 0;
    fGateNames  = 0;
    fUseReverse = 1;
    fFlopDep    = 0;
    fKeepDot    = 0;
    Extra_UtilGetoptReset();
    while ( ( c = Extra_UtilGetopt( argc, argv, "rsgfdh" ) ) != EOF )
    {
        switch ( c )
        {
        case 'r':
            fUseReverse ^= 1;
            break;
        case 's':
            fSeq ^= 1;
            break;
        case 'g':
            fGateNames ^= 1;
            break;
        case 'f':
            fFlopDep ^= 1;
            break;
        case 'd':
            fKeepDot ^= 1;
            break;
        default:
            goto usage;
        }
    }

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

usage:
    Abc_Print( -2, "usage: show [-srgfdh]\n" );
    Abc_Print( -2, "       visualizes the network structure using DOT and GSVIEW\n" );
#ifdef WIN32
    Abc_Print( -2, "       \"dot.exe\" and \"gsview32.exe\" should be set in the paths\n" );
    Abc_Print( -2, "       (\"gsview32.exe\" may be in \"C:\\Program Files\\Ghostgum\\gsview\\\")\n" );
#endif
    Abc_Print( -2, "\t-s    : toggles visualization of sequential networks [default = %s].\n", fSeq? "yes": "no" );
    Abc_Print( -2, "\t-r    : toggles ordering nodes in reverse order [default = %s].\n", fUseReverse? "yes": "no" );
    Abc_Print( -2, "\t-g    : toggles printing gate names for mapped network [default = %s].\n", fGateNames? "yes": "no" );
    Abc_Print( -2, "\t-f    : toggles visualizing flop dependency graph [default = %s].\n", fFlopDep? "yes": "no" );
    Abc_Print( -2, "\t-d    : toggles keeping the .dot file used to produce the .ps file [default = %s].\n", fKeepDot? "yes": "no" );
    Abc_Print( -2, "\t-h    : print the command usage\n");
    return 1;
}
}