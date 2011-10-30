/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

#ifndef BJT
#define BJT

#include "devdefs.h"

/* structures to describe Bipolar Junction Transistors */

/* data needed to describe a single instance */

typedef struct sBJTinstance {

    struct sBJTmodel *BJTmodPtr;    /* backpointer to model */
    struct sBJTinstance *BJTnextInstance;   /* pointer to next instance of
                                             * current model */
    IFuid BJTname;        /* pointer to char string naming this instance */
    int BJTstate;         /* pointer to start of state vector for bjt */
    int BJTcolNode;       /* number of collector node of bjt */
    int BJTbaseNode;      /* number of base node of bjt */
    int BJTemitNode;      /* number of emitter node of bjt */
    int BJTsubstNode;     /* number of substrate node of bjt */
    int BJTcolPrimeNode;  /* number of internal collector node of bjt */
    int BJTbasePrimeNode; /* number of internal base node of bjt */
    int BJTemitPrimeNode; /* number of internal emitter node of bjt */

    double BJTarea;       /* area factor for the bjt */
    double BJTicVBE;      /* initial condition voltage B-E */
    double BJTicVCE;      /* initial condition voltage C-E */
    double BJTtemp;       /* instance temperature */
    double BJTtSatCur;    /* temperature adjusted saturation current */
    double BJTtBetaF;     /* temperature adjusted forward beta */
    double BJTtBetaR;     /* temperature adjusted reverse beta */
    double BJTtBEleakCur; /* temperature adjusted B-E leakage current */
    double BJTtBCleakCur; /* temperature adjusted B-C leakage current */
    double BJTtBEcap;     /* temperature adjusted B-E capacitance */
    double BJTtBEpot;     /* temperature adjusted B-E potential */
    double BJTtBCcap;     /* temperature adjusted B-C capacitance */
    double BJTtBCpot;     /* temperature adjusted B-C potential */
    double BJTtDepCap;    /* temperature adjusted join point in diode curve */
    double BJTtf1;        /* temperature adjusted polynomial coefficient */
    double BJTtf4;        /* temperature adjusted polynomial coefficient */
    double BJTtf5;        /* temperature adjusted polynomial coefficient */
    double BJTtVcrit;     /* temperature adjusted critical voltage */
    double BJTcapbe;
    double BJTcapbc;
    double BJTcapcs;
    double BJTcapbx;
    double BJTcc;
    double BJTcb;
    double BJTgpi;
    double BJTgmu;
    double BJTgm;
    double BJTgo;
    double BJTgx;
    double BJTgeqcb;
    double BJTgccs;
    double BJTgeqbx;

    double *BJTcolColPrimePtr;   /* pointer to sparse matrix at
                                  * (collector,collector prime) */
    double *BJTbaseBasePrimePtr; /* pointer to sparse matrix at
                                  * (base,base prime) */
    double *BJTemitEmitPrimePtr; /* pointer to sparse matrix at
                                  * (emitter,emitter prime) */
    double *BJTcolPrimeColPtr;   /* pointer to sparse matrix at
                                  * (collector prime,collector) */
    double *BJTcolPrimeBasePrimePtr; /* pointer to sparse matrix at
                                  * (collector prime,base prime) */
    double *BJTcolPrimeEmitPrimePtr; /* pointer to sparse matrix at
                                  * (collector prime,emitter prime) */
    double *BJTbasePrimeBasePtr; /* pointer to sparse matrix at
                                  * (base prime,base ) */
    double *BJTbasePrimeColPrimePtr; /* pointer to sparse matrix at
                                  * (base prime,collector prime) */
    double *BJTbasePrimeEmitPrimePtr; /* pointer to sparse matrix at
                                  * (base primt,emitter prime) */
    double *BJTemitPrimeEmitPtr; /* pointer to sparse matrix at
                                  * (emitter prime,emitter) */
    double *BJTemitPrimeColPrimePtr; /* pointer to sparse matrix at
                                  * (emitter prime,collector prime) */
    double *BJTemitPrimeBasePrimePtr; /* pointer to sparse matrix at
                                  * (emitter prime,base prime) */
    double *BJTcolColPtr;        /* pointer to sparse matrix at
                                  * (collector,collector) */
    double *BJTbaseBasePtr;      /* pointer to sparse matrix at
                                  * (base,base) */
    double *BJTemitEmitPtr;      /* pointer to sparse matrix at
                                  * (emitter,emitter) */
    double *BJTcolPrimeColPrimePtr; /* pointer to sparse matrix at
                                  * (collector prime,collector prime) */
    double *BJTbasePrimeBasePrimePtr; /* pointer to sparse matrix at
                                  * (base prime,base prime) */
    double *BJTemitPrimeEmitPrimePtr; /* pointer to sparse matrix at
                                  * (emitter prime,emitter prime) */
    double *BJTsubstSubstPtr;    /* pointer to sparse matrix at
                                  * (substrate,substrate) */
    double *BJTcolPrimeSubstPtr; /* pointer to sparse matrix at
                                  * (collector prime,substrate) */
    double *BJTsubstColPrimePtr; /* pointer to sparse matrix at
                                  * (substrate,collector prime) */
    double *BJTbaseColPrimePtr;  /* pointer to sparse matrix at
                                  * (base,collector prime) */
    double *BJTcolPrimeBasePtr;  /* pointer to sparse matrix at
                                  * (collector prime,base) */

/*
 * distortion stuff
 * the following naming convention is used:
 * x = vbe
 * y = vbc
 * z = vbb
 * w = vbed (vbe delayed for the linear gm delay)
 * therefore ic_xyz stands for the coefficient of the vbe*vbc*vbb
 * term in the multidimensional Taylor expansion for ic; and ibb_x2y
 * for the coeff. of the vbe*vbe*vbc term in the ibb expansion.
 */

#define BJTNDCOEFFS 65

#ifndef NODISTO
    double BJTdCoeffs[BJTNDCOEFFS];
#else /* NODISTO */
    double *BJTdCoeffs;
#endif /* NODISTO */

#ifndef CONFIG

#define    ic_x        BJTdCoeffs[0]
#define    ic_y        BJTdCoeffs[1]
#define    ic_xd       BJTdCoeffs[2]
#define    ic_x2       BJTdCoeffs[3]
#define    ic_y2       BJTdCoeffs[4]
#define    ic_w2       BJTdCoeffs[5]
#define    ic_xy       BJTdCoeffs[6]
#define    ic_yw       BJTdCoeffs[7]
#define    ic_xw       BJTdCoeffs[8]
#define    ic_x3       BJTdCoeffs[9]
#define    ic_y3       BJTdCoeffs[10]
#define    ic_w3       BJTdCoeffs[11]
#define    ic_x2w      BJTdCoeffs[12]
#define    ic_x2y      BJTdCoeffs[13]
#define    ic_y2w      BJTdCoeffs[14]
#define    ic_xy2      BJTdCoeffs[15]
#define    ic_xw2      BJTdCoeffs[16]
#define    ic_yw2      BJTdCoeffs[17]
#define    ic_xyw      BJTdCoeffs[18]

#define    ib_x        BJTdCoeffs[19]
#define    ib_y        BJTdCoeffs[20]
#define    ib_x2       BJTdCoeffs[21]
#define    ib_y2       BJTdCoeffs[22]
#define    ib_xy       BJTdCoeffs[23]
#define    ib_x3       BJTdCoeffs[24]
#define    ib_y3       BJTdCoeffs[25]
#define    ib_x2y      BJTdCoeffs[26]
#define    ib_xy2      BJTdCoeffs[27]

#define    ibb_x       BJTdCoeffs[28]
#define    ibb_y       BJTdCoeffs[29]
#define    ibb_z       BJTdCoeffs[30]
#define    ibb_x2      BJTdCoeffs[31]
#define    ibb_y2      BJTdCoeffs[32]
#define    ibb_z2      BJTdCoeffs[33]
#define    ibb_xy      BJTdCoeffs[34]
#define    ibb_yz      BJTdCoeffs[35]
#define    ibb_xz      BJTdCoeffs[36]
#define    ibb_x3      BJTdCoeffs[37]
#define    ibb_y3      BJTdCoeffs[38]
#define    ibb_z3      BJTdCoeffs[39]
#define    ibb_x2z     BJTdCoeffs[40]
#define    ibb_x2y     BJTdCoeffs[41]
#define    ibb_y2z     BJTdCoeffs[42]
#define    ibb_xy2     BJTdCoeffs[43]
#define    ibb_xz2     BJTdCoeffs[44]
#define    ibb_yz2     BJTdCoeffs[45]
#define    ibb_xyz     BJTdCoeffs[46]

#define    qbe_x       BJTdCoeffs[47]
#define    qbe_y       BJTdCoeffs[48]
#define    qbe_x2      BJTdCoeffs[49]
#define    qbe_y2      BJTdCoeffs[50]
#define    qbe_xy      BJTdCoeffs[51]
#define    qbe_x3      BJTdCoeffs[52]
#define    qbe_y3      BJTdCoeffs[53]
#define    qbe_x2y     BJTdCoeffs[54]
#define    qbe_xy2     BJTdCoeffs[55]

#define    capbc1      BJTdCoeffs[56]
#define    capbc2      BJTdCoeffs[57]
#define    capbc3      BJTdCoeffs[58]

#define    capbx1      BJTdCoeffs[59]
#define    capbx2      BJTdCoeffs[60]
#define    capbx3      BJTdCoeffs[61]

#define    capsc1      BJTdCoeffs[62]
#define    capsc2      BJTdCoeffs[63]
#define    capsc3      BJTdCoeffs[64]

#endif

/* indices to array of BJT noise sources */

#define BJTRCNOIZ    0
#define BJTRBNOIZ    1
#define BJT_RE_NOISE 2
#define BJTICNOIZ    3
#define BJTIBNOIZ    4
#define BJTFLNOIZ    5
#define BJTTOTNOIZ   6

#define BJTNSRCS     7     /* the number of BJT noise sources */

#ifndef NONOISE
      double BJTnVar[NSTATVARS][BJTNSRCS];
#else /*NONOISE*/
      double **BJTnVar;
#endif /*NONOISE*/
/* the above to avoid allocating memory when it is not needed */


    unsigned BJToff         :1;  /* 'off' flag for bjt */
    unsigned BJTtempGiven   :1;  /* temperature given  for bjt instance */
    unsigned BJTareaGiven   :1;  /* flag to indicate area was specified */
    unsigned BJTicVBEGiven  :1;  /* flag to indicate VBE init. cond. given */
    unsigned BJTicVCEGiven  :1;  /* flag to indicate VCE init. cond. given */

} BJTinstance ;


/* entries in the state vector for bjt: */
#define BJTvbe       BJTstate
#define BJTvbc       BJTstate+1
#define BJTvbx       BJTstate+2
#define BJTvcs       BJTstate+3
#define BJTqbe       BJTstate+4
#define BJTcqbe      BJTstate+5
#define BJTqbc       BJTstate+6
#define BJTcqbc      BJTstate+7
#define BJTqcs       BJTstate+8
#define BJTcqcs      BJTstate+9
#define BJTqbx       BJTstate+10
#define BJTcqbx      BJTstate+11
#define BJTcexbc     BJTstate+12
#define BJTnumStates 13


/* per model data */
typedef struct sBJTmodel {          /* model structure for a bjt */
    int BJTmodType;                 /* type index of this device type */
    struct sBJTmodel *BJTnextModel; /* pointer to next possible model in
                                     * linked list */
    BJTinstance * BJTinstances;     /* pointer to list of instances
                                     * that have this model */
    IFuid BJTmodName; /* pointer to character string naming this model */
    int BJTtype;

    double BJTtnom;            /* nominal temperature */
    double BJTsatCur;          /* input - don't use */
    double BJTbetaF;           /* input - don't use */
    double BJTemissionCoeffF;
    double BJTearlyVoltF;
    double BJTrollOffF;
    double BJTleakBEcurrent;   /* input - don't use */
    double BJTleakBEemissionCoeff;
    double BJTbetaR;           /* input - don't use */
    double BJTemissionCoeffR;
    double BJTearlyVoltR;
    double BJTrollOffR;
    double BJTleakBCcurrent;   /* input - don't use */
    double BJTleakBCemissionCoeff;
    double BJTbaseResist;
    double BJTbaseCurrentHalfResist;
    double BJTminBaseResist;
    double BJTemitterResist;
    double BJTcollectorResist;
    double BJTdepletionCapBE;  /* input - don't use */
    double BJTpotentialBE;     /* input - don't use */
    double BJTjunctionExpBE;
    double BJTtransitTimeF;
    double BJTtransitTimeBiasCoeffF;
    double BJTtransitTimeFVBC;
    double BJTtransitTimeHighCurrentF;
    double BJTexcessPhase;
    double BJTdepletionCapBC;  /* input - don't use */
    double BJTpotentialBC;     /* input - don't use */
    double BJTjunctionExpBC;
    double BJTbaseFractionBCcap;
    double BJTtransitTimeR;
    double BJTcapCS;
    double BJTpotentialSubstrate;
    double BJTexponentialSubstrate;
    double BJTbetaExp;
    double BJTenergyGap;
    double BJTtempExpIS;
    double BJTdepletionCapCoeff;
    double BJTfNcoef;
    double BJTfNexp;
    
    double BJTinvEarlyVoltF;   /* inverse of BJTearlyVoltF */
    double BJTinvEarlyVoltR;   /* inverse of BJTearlyVoltR */
    double BJTinvRollOffF;     /* inverse of BJTrollOffF */
    double BJTinvRollOffR;     /* inverse of BJTrollOffR */
    double BJTcollectorConduct; /* collector conductance */
    double BJTemitterConduct;  /* emitter conductance */
    double BJTtransitTimeVBCFactor; /* */
    double BJTexcessPhaseFactor;
    double BJTf2;
    double BJTf3;
    double BJTf6;
    double BJTf7;

    unsigned BJTtnomGiven : 1;
    unsigned BJTsatCurGiven : 1;
    unsigned BJTbetaFGiven : 1;
    unsigned BJTemissionCoeffFGiven : 1;
    unsigned BJTearlyVoltFGiven : 1;
    unsigned BJTrollOffFGiven : 1;
    unsigned BJTleakBEcurrentGiven : 1;
    unsigned BJTleakBEemissionCoeffGiven : 1;
    unsigned BJTbetaRGiven : 1;
    unsigned BJTemissionCoeffRGiven : 1;
    unsigned BJTearlyVoltRGiven : 1;
    unsigned BJTrollOffRGiven : 1;
    unsigned BJTleakBCcurrentGiven : 1;
    unsigned BJTleakBCemissionCoeffGiven : 1;
    unsigned BJTbaseResistGiven : 1;
    unsigned BJTbaseCurrentHalfResistGiven : 1;
    unsigned BJTminBaseResistGiven : 1;
    unsigned BJTemitterResistGiven : 1;
    unsigned BJTcollectorResistGiven : 1;
    unsigned BJTdepletionCapBEGiven : 1;
    unsigned BJTpotentialBEGiven : 1;
    unsigned BJTjunctionExpBEGiven : 1;
    unsigned BJTtransitTimeFGiven : 1;
    unsigned BJTtransitTimeBiasCoeffFGiven : 1;
    unsigned BJTtransitTimeFVBCGiven : 1;
    unsigned BJTtransitTimeHighCurrentFGiven : 1;
    unsigned BJTexcessPhaseGiven : 1;
    unsigned BJTdepletionCapBCGiven : 1;
    unsigned BJTpotentialBCGiven : 1;
    unsigned BJTjunctionExpBCGiven : 1;
    unsigned BJTbaseFractionBCcapGiven : 1;
    unsigned BJTtransitTimeRGiven : 1;
    unsigned BJTcapCSGiven : 1;
    unsigned BJTpotentialSubstrateGiven : 1;
    unsigned BJTexponentialSubstrateGiven : 1;
    unsigned BJTbetaExpGiven : 1;
    unsigned BJTenergyGapGiven : 1;
    unsigned BJTtempExpISGiven : 1;
    unsigned BJTdepletionCapCoeffGiven : 1;
    unsigned BJTfNcoefGiven : 1;
    unsigned BJTfNexpGiven :1;
} BJTmodel;


#ifndef NPN
#define NPN 1
#define PNP -1
#endif /*NPN*/

/* device parameters */
#define BJT_AREA      1
#define BJT_OFF       2
#define BJT_IC_VBE    3
#define BJT_IC_VCE    4
#define BJT_IC        5
#define BJT_TEMP      6

/* model parameters */
#define BJT_MOD_NPN   101
#define BJT_MOD_PNP   102
#define BJT_MOD_IS    103
#define BJT_MOD_BF    104
#define BJT_MOD_NF    105
#define BJT_MOD_VAF   106  /* 206 (error) in UCB 3f2 dist. */
#define BJT_MOD_IKF   107
#define BJT_MOD_ISE   108
#define BJT_MOD_NE    109
#define BJT_MOD_BR    110
#define BJT_MOD_NR    111
#define BJT_MOD_VAR   112
#define BJT_MOD_IKR   113
#define BJT_MOD_ISC   114
#define BJT_MOD_NC    115
#define BJT_MOD_RB    116
#define BJT_MOD_IRB   117
#define BJT_MOD_RBM   118
#define BJT_MOD_RE    119
#define BJT_MOD_RC    120
#define BJT_MOD_CJE   121
#define BJT_MOD_VJE   122
#define BJT_MOD_MJE   123
#define BJT_MOD_TF    124
#define BJT_MOD_XTF   125
#define BJT_MOD_VTF   126
#define BJT_MOD_ITF   127
#define BJT_MOD_PTF   128
#define BJT_MOD_CJC   129
#define BJT_MOD_VJC   130
#define BJT_MOD_MJC   131
#define BJT_MOD_XCJC  132
#define BJT_MOD_TR    133
#define BJT_MOD_CJS   134
#define BJT_MOD_VJS   135
#define BJT_MOD_MJS   136
#define BJT_MOD_XTB   137
#define BJT_MOD_EG    138
#define BJT_MOD_XTI   139
#define BJT_MOD_FC    140
#define BJT_MOD_TNOM  141
#define BJT_MOD_AF    142
#define BJT_MOD_KF    143

/* device questions */
#define BJT_QUEST_FT             201
#define BJT_QUEST_COLNODE        202
#define BJT_QUEST_BASENODE       203
#define BJT_QUEST_EMITNODE       204
#define BJT_QUEST_SUBSTNODE      205
#define BJT_QUEST_COLPRIMENODE   206
#define BJT_QUEST_BASEPRIMENODE  207
#define BJT_QUEST_EMITPRIMENODE  208
#define BJT_QUEST_VBE            209
#define BJT_QUEST_VBC            210
#define BJT_QUEST_CC             211
#define BJT_QUEST_CB             212
#define BJT_QUEST_GPI            213
#define BJT_QUEST_GMU            214
#define BJT_QUEST_GM             215
#define BJT_QUEST_GO             216
#define BJT_QUEST_QBE            217
#define BJT_QUEST_CQBE           218
#define BJT_QUEST_QBC            219
#define BJT_QUEST_CQBC           220
#define BJT_QUEST_QCS            221
#define BJT_QUEST_CQCS           222
#define BJT_QUEST_QBX            223
#define BJT_QUEST_CQBX           224
#define BJT_QUEST_GX             225
#define BJT_QUEST_CEXBC          226
#define BJT_QUEST_GEQCB          227
#define BJT_QUEST_GCCS           228
#define BJT_QUEST_GEQBX          229
#define BJT_QUEST_CE             230
#define BJT_QUEST_CS             231
#define BJT_QUEST_POWER          232
#define BJT_QUEST_CPI            233
#define BJT_QUEST_CMU            234
#define BJT_QUEST_CBX            235
#define BJT_QUEST_CCS            236

/* model questions */
#define BJT_MOD_INVEARLYF             301
#define BJT_MOD_INVEARLYR             302
#define BJT_MOD_INVROLLOFFF           303
#define BJT_MOD_INVROLLOFFR           304
#define BJT_MOD_COLCONDUCT            305
#define BJT_MOD_EMITTERCONDUCT        306
#define BJT_MOD_TRANSVBCFACT          307
#define BJT_MOD_EXCESSPHASEFACTOR     308
#define BJT_MOD_TYPE                  309


#ifdef __STDC__

extern int  BJTacLoad(GENmodel *,CKTcircuit*);
extern int  BJTask(CKTcircuit *,GENinstance*,int,IFvalue*,IFvalue*);
extern int  BJTconvTest(GENmodel*,CKTcircuit*);
extern int  BJTdisto(int,GENmodel*,CKTcircuit*);
extern int  BJTgetic(GENmodel*,CKTcircuit*);
extern int  BJTload(GENmodel*,CKTcircuit*);
extern int  BJTmAsk(CKTcircuit*,GENmodel*,int,IFvalue*);
extern int  BJTmParam(int,IFvalue*,GENmodel*);
extern int  BJTnoise(int,int,GENmodel*,CKTcircuit*,GENERIC*,double*);
extern int  BJTparam(CKTcircuit*,int,IFvalue*,GENinstance*,IFvalue*);
extern void BJTparse(int,GENERIC*,GENERIC*,GENERIC*);
extern int  BJTpzLoad(GENmodel*,CKTcircuit*,SPcomplex*);
extern int  BJTsetup(SMPmatrix*,GENmodel*,CKTcircuit*,int*);
extern int  BJTtemp(GENmodel*,CKTcircuit*);
extern int  BJTtrunc(GENmodel*,CKTcircuit*,double*);

#else /* stdc */

extern int  BJTacLoad();
extern int  BJTask();
extern int  BJTconvTest();
extern int  BJTdisto();
extern int  BJTgetic();
extern int  BJTload();
extern int  BJTnoise();
extern int  BJTmAsk();
extern int  BJTmParam();
extern int  BJTparam();
extern void BJTparse();
extern int  BJTpzLoad();
extern int  BJTsetup();
extern int  BJTtemp();
extern int  BJTtrunc();

#endif /* stdc */


#endif /*BJT*/
