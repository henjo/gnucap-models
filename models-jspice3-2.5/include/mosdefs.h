/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1989 Takayasu Sakurai
         1993 Stephen R. Whiteley
****************************************************************************/

#ifndef MOS
#define MOS

#include "devdefs.h"

    /* declarations for analytical MOSFETs */

/* information needed for each instance */

typedef struct sMOSinstance {
    struct sMOSmodel *MOSmodPtr;  /* backpointer to model */
    struct sMOSinstance *MOSnextInstance;  /* pointer to next instance of 
                                            * current model */
    IFuid MOSname; /* pointer to character string naming this instance */
    int MOSstates; /* index into state table for this device */

    int MOSdNode;  /* number of the gate node of the mosfet */
    int MOSgNode;  /* number of the gate node of the mosfet */
    int MOSsNode;  /* number of the source node of the mosfet */
    int MOSbNode;  /* number of the bulk node of the mosfet */
    int MOSdNodePrime; /* number of the internal drain node of the mosfet */
    int MOSsNodePrime; /* number of the internal source node of the mosfet */

    int MOSmode;   /* device mode : 1 = normal, -1 = inverse */

    double MOSl;                 /* the length of the channel region */
    double MOSw;                 /* the width of the channel region */
    double MOSdrainArea;         /* the area of the drain diffusion */
    double MOSsourceArea;        /* the area of the source diffusion */
    double MOSdrainSquares;      /* the length of the drain in squares */
    double MOSsourceSquares;     /* the length of the source in squares */
    double MOSdrainPerimeter;
    double MOSsourcePerimeter;
    double MOSsourceConductance; /* conductance of source, set in setup */
    double MOSdrainConductance;  /* conductance of drain, set in setup */
    double MOStemp;              /* operating temperature of this instance */

    /* temperature corrected ... */
    double MOStTransconductance; /* transconductance */
    double MOStSurfMob;    /* surface mobility */
    double MOStPhi;        /* Phi */
    double MOStVto;        /* Vto */
    double MOStSatCur;     /* saturation Cur. */
    double MOStSatCurDens; /* saturation Cur. density*/
    double MOStCbd;        /* B-D Capacitance */
    double MOStCbs;        /* B-S Capacitance */
    double MOStCj;         /* Bulk bottom Capacitance */
    double MOStCjsw;       /* Bulk side Capacitance */
    double MOStBulkPot;    /* Bulk potential */
    double MOStDepCap;     /* transition point in the curve matching Fc*Vj */
    double MOStVbi;        /* Vbi */
    double MOStDrainSatCur;
    double MOStSourceSatCur;

    double MOSicVBS;       /* initial condition B-S voltage */
    double MOSicVDS;       /* initial condition D-S voltage */
    double MOSicVGS;       /* initial condition G-S voltage */
    double MOSvon;
    double MOSvdsat;
    double MOSsourceVcrit; /* vcrit for pos. vds */
    double MOSdrainVcrit;  /* vcrit for neg. vds */
    double MOScd;
    double MOScbs;
    double MOScbd;
    double MOSgmbs;
    double MOSgm;
    double MOSgds;
    double MOSgbd;
    double MOSgbs;
    double MOScapbd;
    double MOScapbs;
    double MOSCbd;
    double MOSCbdsw;
    double MOSCbs;
    double MOSCbssw;
    double MOSf2d;
    double MOSf3d;
    double MOSf4d;
    double MOSf2s;
    double MOSf3s;
    double MOSf4s;

    double MOSeffectiveLength;
    double MOSgateSourceOverlapCap;
    double MOSgateDrainOverlapCap;
    double MOSgateBulkOverlapCap;
    double MOSbeta;
    double MOSoxideCap;

    double *MOSDdPtr;      /* pointer to sparse matrix element at
                            * (Drain node,drain node) */
    double *MOSGgPtr;      /* pointer to sparse matrix element at
                            * (gate node,gate node) */
    double *MOSSsPtr;      /* pointer to sparse matrix element at
                            * (source node,source node) */
    double *MOSBbPtr;      /* pointer to sparse matrix element at
                            * (bulk node,bulk node) */
    double *MOSDPdpPtr;    /* pointer to sparse matrix element at
                            * (drain prime node,drain prime node) */
    double *MOSSPspPtr;    /* pointer to sparse matrix element at
                            * (source prime node,source prime node) */
    double *MOSDdpPtr;     /* pointer to sparse matrix element at
                            * (drain node,drain prime node) */
    double *MOSGbPtr;      /* pointer to sparse matrix element at
                            * (gate node,bulk node) */
    double *MOSGdpPtr;     /* pointer to sparse matrix element at
                            * (gate node,drain prime node) */
    double *MOSGspPtr;     /* pointer to sparse matrix element at
                            * (gate node,source prime node) */
    double *MOSSspPtr;     /* pointer to sparse matrix element at
                            * (source node,source prime node) */
    double *MOSBdpPtr;     /* pointer to sparse matrix element at
                            * (bulk node,drain prime node) */
    double *MOSBspPtr;     /* pointer to sparse matrix element at
                            * (bulk node,source prime node) */
    double *MOSDPspPtr;    /* pointer to sparse matrix element at
                            * (drain prime node,source prime node) */
    double *MOSDPdPtr;     /* pointer to sparse matrix element at
                            * (drain prime node,drain node) */
    double *MOSBgPtr;      /* pointer to sparse matrix element at
                            * (bulk node,gate node) */
    double *MOSDPgPtr;     /* pointer to sparse matrix element at
                            * (drain prime node,gate node) */

    double *MOSSPgPtr;     /* pointer to sparse matrix element at
                            * (source prime node,gate node) */
    double *MOSSPsPtr;     /* pointer to sparse matrix element at
                            * (source prime node,source node) */
    double *MOSDPbPtr;     /* pointer to sparse matrix element at
                            * (drain prime node,bulk node) */
    double *MOSSPbPtr;     /* pointer to sparse matrix element at
                            * (source prime node,bulk node) */
    double *MOSSPdpPtr;    /* pointer to sparse matrix element at
                            * (source prime node,drain prime node) */

    /* distortion stuff */
/*
 * naming convention:
 * x = vgs
 * y = vbs
 * z = vds
 * cdr = cdrain
 */


#define MOSNDCOEFFS 30

#ifndef NODISTO
    double MOSdCoeffs[MOSNDCOEFFS];
#else /* NODISTO */
    double *MOSdCoeffs;
#endif /* NODISTO */

#ifndef CONFIG

#define capbs2   MOSdCoeffs[0]
#define capbs3   MOSdCoeffs[1]
#define capbd2   MOSdCoeffs[2]
#define capbd3   MOSdCoeffs[3]
#define gbs2     MOSdCoeffs[4]
#define gbs3     MOSdCoeffs[5]
#define gbd2     MOSdCoeffs[6]
#define gbd3     MOSdCoeffs[7]
#define capgb2   MOSdCoeffs[8]
#define capgb3   MOSdCoeffs[9]
#define cdr_x2   MOSdCoeffs[10]
#define cdr_y2   MOSdCoeffs[11]
#define cdr_z2   MOSdCoeffs[12]
#define cdr_xy   MOSdCoeffs[13]
#define cdr_yz   MOSdCoeffs[14]
#define cdr_xz   MOSdCoeffs[15]
#define cdr_x3   MOSdCoeffs[16]
#define cdr_y3   MOSdCoeffs[17]
#define cdr_z3   MOSdCoeffs[18]
#define cdr_x2z  MOSdCoeffs[19]
#define cdr_x2y  MOSdCoeffs[20]
#define cdr_y2z  MOSdCoeffs[21]
#define cdr_xy2  MOSdCoeffs[22]
#define cdr_xz2  MOSdCoeffs[23]
#define cdr_yz2  MOSdCoeffs[24]
#define cdr_xyz  MOSdCoeffs[25]
#define capgs2   MOSdCoeffs[26]
#define capgs3   MOSdCoeffs[27]
#define capgd2   MOSdCoeffs[28]
#define capgd3   MOSdCoeffs[29]

#endif

    /*  end distortion coeffs. */

/* indices to the array of MOSFET(3) noise sources */

#define MOSRDNOIZ  0
#define MOSRSNOIZ  1
#define MOSIDNOIZ  2
#define MOSFLNOIZ  3
#define MOSTOTNOIZ 4

#define MOSNSRCS   5     /* the number of MOSFET(3) noise sources */

#ifndef NONOISE
    double MOSnVar[NSTATVARS][MOSNSRCS];
#else /* NONOISE */
    double **MOSnVar;
#endif /* NONOISE */

    unsigned MOSoff :1;        /* device is off for dc analysis */
    unsigned MOStempGiven :1;  /* instance temperature specified */
    unsigned MOSlGiven :1;
    unsigned MOSwGiven :1;
    unsigned MOSdrainAreaGiven :1;
    unsigned MOSsourceAreaGiven :1;
    unsigned MOSdrainSquaresGiven :1;
    unsigned MOSsourceSquaresGiven :1;
    unsigned MOSdrainPerimeterGiven :1;
    unsigned MOSsourcePerimeterGiven :1;
    unsigned MOSdNodePrimeSet :1;
    unsigned MOSsNodePrimeSet :1;
    unsigned MOSicVBSGiven :1;
    unsigned MOSicVDSGiven :1;
    unsigned MOSicVGSGiven :1;
    unsigned MOSvonGiven :1;
    unsigned MOSvdsatGiven :1;
    unsigned MOSmodeGiven :1;

} MOSinstance ;

#define MOSvbd   MOSstates
#define MOSvbs   MOSstates + 1
#define MOSvgs   MOSstates + 2
#define MOSvds   MOSstates + 3

/* meyer capacitances */
#define MOScapgs MOSstates + 4   /* gate-source capacitor value */
#define MOSqgs   MOSstates + 5   /* gate-source capacitor charge */
#define MOScqgs  MOSstates + 6   /* gate-source capacitor current */

#define MOScapgd MOSstates + 7   /* gate-drain capacitor value */
#define MOSqgd   MOSstates + 8   /* gate-drain capacitor charge */
#define MOScqgd  MOSstates + 9   /* gate-drain capacitor current */

#define MOScapgb MOSstates + 10  /* gate-bulk capacitor value */
#define MOSqgb   MOSstates + 11  /* gate-bulk capacitor charge */
#define MOScqgb  MOSstates + 12  /* gate-bulk capacitor current */

/* diode capacitances */
#define MOSqbd   MOSstates + 13  /* bulk-drain capacitor charge */
#define MOScqbd  MOSstates + 14  /* bulk-drain capacitor current */

#define MOSqbs   MOSstates + 15  /* bulk-source capacitor charge */
#define MOScqbs  MOSstates + 16  /* bulk-source capacitor current */ 

#define MOSnumStates 17


/* per model data */

    /* NOTE:  parameters marked 'input - use xxxx' are paramters for
     * which a temperature correction is applied in MOStemp, thus
     * the MOSxxxx value in the per-instance structure should be used
     * instead in all calculations 
     */

typedef struct sMOSmodel {      /* model structure for a resistor */
    int MOSmodType;             /* type index of this device type */
    struct sMOSmodel *MOSnextModel; /* pointer to next possible model 
                                     * in linked list */
    MOSinstance * MOSinstances; /* pointer to list of instances 
                                 * that have this model */
    IFuid MOSmodName;  /* pointer to character string naming this model */
    int MOStype;       /* device type : 1 = nmos,  -1 = pmos */
    int MOSlevel;      /* UCB model complexity level, 1-3 */
    int MOSgateType;
    double MOSoxideCapFactor;
    double MOStnom;    /* temperature at which parameters measured */
    double MOSvt0;               /* input - use tVto */
    double MOStransconductance;  /* input - use tTransconductance */
    double MOSgamma;
    double MOSphi;               /* input - use tPhi */
    double MOSdrainResistance;
    double MOSsourceResistance;
    double MOScapBD;             /* input - use tCbs */
    double MOScapBS;             /* input - use tCbd */
    double MOSjctSatCur;         /* input - use tSatCur instead */
    double MOSbulkJctPotential;  /* input - use tBulkPot */
    double MOSgateSourceOverlapCapFactor;
    double MOSgateDrainOverlapCapFactor;
    double MOSgateBulkOverlapCapFactor;
    double MOSbulkCapFactor;     /* input - use tCj */
    double MOSbulkJctBotGradingCoeff;
    double MOSsideWallCapFactor; /* input - use tCjsw */
    double MOSbulkJctSideGradingCoeff;
    double MOSjctSatCurDensity;  /* input - use tSatCurDens*/
    double MOSoxideThickness;
    double MOSlatDiff;
    double MOSsheetResistance;
    double MOSsurfaceMobility;   /* input - use tSurfMob */
    double MOSfwdCapDepCoeff;
    double MOSsurfaceStateDensity;
    double MOSsubstrateDoping;
    double MOSfNcoef;
    double MOSfNexp;

    /* levels 1,2 and 6 */
    double MOSlambda;

    /* level 2 */
    double MOScritFieldExp;      /* uexp */
    double MOSchannelCharge;     /* neff */
    double MOScritField;         /* ucrit */

    /* levels 2 and 3 */
    double MOSfastSurfaceStateDensity; /* nfs */
    double MOSnarrowFactor;      /* delta */
    double MOSmaxDriftVel;       /* vmax */
    double MOSjunctionDepth;
    double MOSxd;

    /* level 3 */
    double MOSeta;
    double MOStheta;             /* theta */
    double MOSkappa;             /* kappa */
    double MOSdelta;             /* input delta */
    double MOSalpha;             /* alpha */

    /* level 6 */
    double MOSkv;       /* input - use tKv */
    double MOSnv;       /* drain linear conductance factor*/
    double MOSkc;       /* input - use tKc */
    double MOSnc;       /* saturation current coeff.*/
    double MOSgamma1;   /* secondary back-gate effect parametr */
    double MOSsigma;
    double MOSlamda0;
    double MOSlamda1;

    /* level 8, CryoMOS by Luong Huynh 1/19/95 */
    double MOSa1;
    double MOSa2;
    double MOSk1;
    double MOSk2;
    double MOSk3;
    double MOSk4;
    double MOSpclm;
    double MOSem;

    unsigned MOStypeGiven :1;
    unsigned MOSgateTypeGiven :1;
    unsigned MOSlevelGiven :1;
    unsigned MOStnomGiven :1;
    unsigned MOSvt0Given :1;
    unsigned MOStransconductanceGiven :1;
    unsigned MOSgammaGiven :1;
    unsigned MOSphiGiven :1;
    unsigned MOSdrainResistanceGiven :1;
    unsigned MOSsourceResistanceGiven :1;
    unsigned MOScapBDGiven :1;
    unsigned MOScapBSGiven :1;
    unsigned MOSjctSatCurGiven :1;
    unsigned MOSbulkJctPotentialGiven :1;
    unsigned MOSgateSourceOverlapCapFactorGiven :1;
    unsigned MOSgateDrainOverlapCapFactorGiven :1;
    unsigned MOSgateBulkOverlapCapFactorGiven :1;
    unsigned MOSbulkCapFactorGiven :1;
    unsigned MOSbulkJctBotGradingCoeffGiven :1;
    unsigned MOSsideWallCapFactorGiven :1;
    unsigned MOSbulkJctSideGradingCoeffGiven :1;
    unsigned MOSjctSatCurDensityGiven :1;
    unsigned MOSoxideThicknessGiven :1;
    unsigned MOSlatDiffGiven :1;
    unsigned MOSsheetResistanceGiven :1;
    unsigned MOSsurfaceMobilityGiven :1;
    unsigned MOSfwdCapDepCoeffGiven :1;
    unsigned MOSsurfaceStateDensityGiven :1;
    unsigned MOSsubstrateDopingGiven :1;
    unsigned MOSfNcoefGiven :1;
    unsigned MOSfNexpGiven :1;

    /* levels 1,2, and 6 */
    unsigned MOSlambdaGiven :1;

    /* level 2 */
    unsigned MOScritFieldExpGiven :1;   /* uexp */
    unsigned MOSchannelChargeGiven :1;  /* neff */
    unsigned MOScritFieldGiven :1;      /* ucrit */

    /* levels 2 and 3 */
    unsigned MOSfastSurfaceStateDensityGiven :1; /* nfs */
    unsigned MOSnarrowFactorGiven :1;   /* delta */
    unsigned MOSmaxDriftVelGiven :1;    /* vmax */
    unsigned MOSjunctionDepthGiven :1;

    /* level 3 */
    unsigned MOSetaGiven :1;
    unsigned MOSthetaGiven :1;          /* theta */
    unsigned MOSkappaGiven :1;          /* kappa */
    unsigned MOSdeltaGiven :1;          /* delta */

    /* level 6 */
    unsigned MOSkvGiven :1;
    unsigned MOSnvGiven :1;
    unsigned MOSkcGiven :1;
    unsigned MOSncGiven :1;
    unsigned MOSgamma1Given :1;
    unsigned MOSsigmaGiven :1;
    unsigned MOSlamda0Given :1;
    unsigned MOSlamda1Given :1;

    /* level 8, CryoMOS by Luong Huynh 1/19/95 */
    unsigned MOSa1Given :1;
    unsigned MOSa2Given :1;
    unsigned MOSk1Given :1;
    unsigned MOSk2Given :1;
    unsigned MOSk3Given :1;
    unsigned MOSk4Given :1;
    unsigned MOSpclmGiven :1;
    unsigned MOSemGiven :1;

} MOSmodel;

#ifndef NMOS
#define NMOS 1
#define PMOS -1
#endif /*NMOS*/

/* device parameters */
#define MOS_TEMP            1
#define MOS_L               2
#define MOS_W               3
#define MOS_AD              4
#define MOS_AS              5
#define MOS_PD              6
#define MOS_PS              7
#define MOS_NRD             8
#define MOS_NRS             9
#define MOS_OFF             10   
#define MOS_IC_VDS          11   
#define MOS_IC_VGS          12   
#define MOS_IC_VBS          13   
#define MOS_IC              14   
#define MOS_CGD             15   
#define MOS_CGS             16   
#define MOS_DNODE           17   
#define MOS_GNODE           18   
#define MOS_SNODE           19   
#define MOS_BNODE           20   
#define MOS_DNODEPRIME      21   
#define MOS_SNODEPRIME      22   
#define MOS_DRAINCONDUCT    23   
#define MOS_SOURCECONDUCT   24   
#define MOS_VON             25   
#define MOS_VDSAT           26   
#define MOS_DRAINVCRIT      27   
#define MOS_SOURCEVCRIT     28   
#define MOS_CD              29   
#define MOS_CBD             30   
#define MOS_CBS             31   
#define MOS_GMBS            32   
#define MOS_GM              33   
#define MOS_GDS             34   
#define MOS_GBD             35   
#define MOS_GBS             36   
#define MOS_CAPBD           37   
#define MOS_CAPBS           38   
#define MOS_CAPZEROBIASBD   39   
#define MOS_CAPZEROBIASBDSW 40    
#define MOS_CAPZEROBIASBS   41   
#define MOS_CAPZEROBIASBSSW 42   
#define MOS_VBD             43   
#define MOS_VBS             44   
#define MOS_VGS             45   
#define MOS_VDS             46   
#define MOS_CAPGD           47   
#define MOS_QGD             48   
#define MOS_CQGD            49   
#define MOS_CAPGS           50   
#define MOS_QGS             51   
#define MOS_CQGS            52   
#define MOS_CAPGB           53   
#define MOS_QGB             54   
#define MOS_CQGB            55   
#define MOS_QBD             56   
#define MOS_CQBD            57   
#define MOS_QBS             58   
#define MOS_CQBS            59   
#define MOS_CG              60   
#define MOS_CS              61   
#define MOS_CB              62   
#define MOS_POWER           63   
#define MOS_DRAINRESIST     64   
#define MOS_SOURCERESIST    65   

/* model parameters */
#define MOS_MOD_TYPE        100   
#define MOS_MOD_LEVEL       101   
#define MOS_MOD_TNOM        102   
#define MOS_MOD_VTO         103   
#define MOS_MOD_KP          104   
#define MOS_MOD_GAMMA       105   
#define MOS_MOD_PHI         106   
#define MOS_MOD_RD          107   
#define MOS_MOD_RS          108   
#define MOS_MOD_CBD         109   
#define MOS_MOD_CBS         110   
#define MOS_MOD_IS          111   
#define MOS_MOD_PB          112   
#define MOS_MOD_CGSO        113   
#define MOS_MOD_CGDO        114   
#define MOS_MOD_CGBO        115   
#define MOS_MOD_CJ          116   
#define MOS_MOD_MJ          117   
#define MOS_MOD_CJSW        118   
#define MOS_MOD_MJSW        119   
#define MOS_MOD_JS          120   
#define MOS_MOD_TOX         121   
#define MOS_MOD_LD          122   
#define MOS_MOD_RSH         123   
#define MOS_MOD_U0          124   
#define MOS_MOD_FC          125   
#define MOS_MOD_NSS         126   
#define MOS_MOD_NSUB        127   
#define MOS_MOD_TPG         128   
#define MOS_MOD_NMOS        129   
#define MOS_MOD_PMOS        130   
#define MOS_MOD_KF          131   
#define MOS_MOD_AF          132   
#define MOS_MOD_LAMBDA      133   
#define MOS_MOD_UEXP        134   
#define MOS_MOD_NEFF        135   
#define MOS_MOD_UCRIT       136   
#define MOS_MOD_NFS         137   
#define MOS_MOD_DELTA       138   
#define MOS_MOD_VMAX        139   
#define MOS_MOD_XJ          140   
#define MOS_MOD_ETA         141   
#define MOS_MOD_THETA       142   
#define MOS_MOD_ALPHA       143   
#define MOS_MOD_KAPPA       144   
#define MOS_MOD_XD          145   
#define MOS_DELTA           146   
#define MOS_MOD_KV          147
#define MOS_MOD_NV          148
#define MOS_MOD_KC          149
#define MOS_MOD_NC          150
#define MOS_MOD_GAMMA1      151
#define MOS_MOD_SIGMA       152
#define MOS_MOD_LAMDA0      153
#define MOS_MOD_LAMDA1      154
/* level 8, CryoMOS by Luong Huynh, 1/19/95 */
#define MOS_MOD_A1          155
#define MOS_MOD_A2          156
#define MOS_MOD_K1          157
#define MOS_MOD_K2          158
#define MOS_MOD_K3          159
#define MOS_MOD_K4          160
#define MOS_MOD_PCLM        161
#define MOS_MOD_EM          162


#ifdef __STDC__

extern int  MOSacLoad(GENmodel*,CKTcircuit*);
extern int  MOSask(CKTcircuit*,GENinstance*,int,IFvalue*,IFvalue*);
extern int  MOSconvTest(GENmodel*,CKTcircuit*);
extern int  MOSdisto(int,GENmodel*,CKTcircuit*);
extern int  MOSgetic(GENmodel*,CKTcircuit*);
extern int  MOSload(GENmodel*,CKTcircuit*);
extern int  MOSmAsk(CKTcircuit*,GENmodel*,int,IFvalue*);
extern int  MOSmParam(int,IFvalue*,GENmodel*);
extern int  MOSnoise(int,int,GENmodel*,CKTcircuit*,GENERIC*,double*);
extern int  MOSparam(CKTcircuit*,int,IFvalue*,GENinstance*,IFvalue*);
extern int  MOSpzLoad(GENmodel*,CKTcircuit*,SPcomplex*);
extern int  MOSsetup(SMPmatrix*,GENmodel*,CKTcircuit*,int*);
extern int  MOStemp(GENmodel*,CKTcircuit*);
extern int  MOStrunc(GENmodel*,CKTcircuit*,double*);

#else /* stdc */

extern int  MOSacLoad();
extern int  MOSask();
extern int  MOSconvTest();
extern int  MOSdisto();
extern int  MOSgetic();
extern int  MOSload();
extern int  MOSmAsk();
extern int  MOSmParam();
extern int  MOSnoise();
extern int  MOSparam();
extern int  MOSpzLoad();
extern int  MOSsetup();
extern int  MOStemp();
extern int  MOStrunc();

#endif /* stdc */


/* store some things to pass to functions */
struct mosstuff {
    double ms_vt;
    double ms_von;
    double ms_vdsat;
    double ms_vgs;
    double ms_vds;
    double ms_vbs;
    double ms_vbd;
    double ms_vgb;
    double ms_vgd;
    double ms_ceqgs;
    double ms_ceqgd;
    double ms_ceqgb;
    double ms_cdrain;
    double ms_gcgs;
    double ms_gcgd;
    double ms_gcgb;
    double ms_capgs;
    double ms_capgd;
    double ms_capgb;
};

/* semi-private functions... */
#ifdef __STDC__
extern void MOSfd(MOSmodel*,MOSinstance*);
extern void MOSfs(MOSmodel*,MOSinstance*);
extern double MOSeq1(MOSmodel*,MOSinstance*,struct mosstuff*);
extern double MOSeq2(MOSmodel*,MOSinstance*,struct mosstuff*);
extern double MOSeq3(MOSmodel*,MOSinstance*,struct mosstuff*);
extern double MOSeq6(MOSmodel*,MOSinstance*,struct mosstuff*);
/* level 8, CryoMOS by Luong Huynh 1/19/95 */
extern double cryoMOSeq(MOSmodel*,MOSinstance*,struct mosstuff*);
/**/
#else
extern void MOSfd();
extern void MOSfs();
extern double MOSeq1();
extern double MOSeq2();
extern double MOSeq3();
extern double MOSeq6();
/* level 8, CryoMOS by Luong Huynh 1/19/95 */
extern double cryoMOSeq();
/**/
#endif

/* ...and a macro */
#define SARGS(arg,bot,side,sarg,sargsw) { \
  if(bot==side) \
    { if(bot==.5) sarg=sargsw=1/sqrt(arg); \
    else sarg=sargsw=exp(-bot*log(arg)); } \
  else \
    { if(bot==.5) sarg=1/sqrt(arg); \
    else sarg=exp(-bot*log(arg)); \
    if(side==.5) sargsw=1/sqrt(arg); \
    else sargsw=exp(-side*log(arg)); } \
}

#endif /*MOS*/
