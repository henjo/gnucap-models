/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Authors: 1993 Stephen R. Whiteley
****************************************************************************/

#ifndef SFFT
#define SFFT

#include "devdefs.h"

            /* data structures used to describe sfft */


/* information needed per instance */

typedef struct sSFFTinstance {
    struct sSFFTmodel *SFFTmodPtr;    /* backpointer to model */
    struct sSFFTinstance *SFFTnextInstance; /* pointer to next sfft instance */
    IFuid SFFTname;      /* pointer to string naming this instance */
    int SFFTstate;       /* pointer to start of state vector for sfft */
    int SFFTposNode;     /* number of positive node of sfft */
    int SFFTnegNode;     /* number of negative node of sfft */
    int SFFTcposNode;    /* number of positive control node */
    int SFFTcnegNode;    /* number of negative control node */
    int SFFTi1Eq;        /* internal branch for control current */
    int SFFTi2Eq;        /* internal branch for output current */
    int SFFTvprime;      /* internal node */

    double *SFFTcposCposptr; /* pointers to the various matrix locations */
    double *SFFTcnegCnegptr;
    double *SFFTposPosptr;
    double *SFFTnegNegptr;

    double *SFFTcposPosptr;
    double *SFFTposCposptr;
    double *SFFTcnegNegptr;
    double *SFFTnegCnegptr;

    double *SFFTcposI1ptr;
    double *SFFTi1Cposptr;
    double *SFFTcnegI1ptr;
    double *SFFTi1Cnegptr;
    double *SFFTi1I1ptr;

    double *SFFTposI2ptr;
    double *SFFTi2Posptr;
    double *SFFTnegI2ptr;
    double *SFFTi2Negptr;
    double *SFFTi2Vprimeptr;
    double *SFFTi2I2ptr;
    double *SFFTi1I2ptr;
    double *SFFTi2I1ptr;

    double *SFFTvprimeVprimeptr;
    double *SFFTvprimeI1ptr;
    double *SFFTvprimeI2ptr;


    /* flags to indicate... */
    unsigned SFFToff : 1;          /* 'off' flag for sfft was specified */
    unsigned SFFTlenGiven : 1;     /* length was specified */
    unsigned SFFTwidGiven : 1;     /* width was specified */
    unsigned SFFTinitV1Given : 1;  /* Vd+c+ was specified */
    unsigned SFFTinitV2Given : 1;  /* Vd-c- was specified */
    unsigned SFFTinitI1Given : 1;  /* Ic was specified */
    unsigned SFFTinitI2Given : 1;  /* Id was specified */

    double SFFTlength;   /* length factor for the sfft */
    double SFFTwidth;    /* width factor for the sfft */
    double SFFTinitV1;   /* initial condition */
    double SFFTinitV2;   /* initial condition */
    double SFFTinitI1;   /* initial condition */
    double SFFTinitI2;   /* initial condition */
    double SFFTic;       /* critical current */
    double SFFTicpos;    /* pos critical current exp decay factor */
    double SFFTicneg;    /* neg critical current exp decay factor */
    double SFFTisat;     /* fluxon velocity saturation current */
    double SFFTinsat;    /* fluxon number saturation current */
    double SFFTr;        /* resistance */
    double SFFTl1;       /* control line self inductance */
    double SFFTl2;       /* SFFT body self inductance */
    double SFFTm12;      /* mutual inductance */
    double SFFTc1;       /* control positive to body positive capacitance */
    double SFFTc2;       /* control negative to body negative capacitaqnce */
    double SFFTtc;       /* transit time constant */
    double SFFTgeq1;     /* integration parameters... */
    double SFFTgeq2;
    double SFFTgeq3;
    double SFFTceq1;
    double SFFTceq2;
    double SFFTceq3;
    double SFFTreq1;
    double SFFTreq2;
    double SFFTveq1;
    double SFFTveq2; 

    double SFFTv;        /* voltage across device */
    double SFFTd1;       /* dVoltage/dControl */
    double SFFTd2;       /* dVoltage/dCurrent */

} SFFTinstance ;


#define SFFTi1       SFFTstate
#define SFFTi2       SFFTstate+1
#define SFFTflux1    SFFTstate+2
#define SFFTvolt1    SFFTstate+3
#define SFFTflux2    SFFTstate+4
#define SFFTvolt2    SFFTstate+5
#define SFFTqcap1    SFFTstate+6
#define SFFTccap1    SFFTstate+7
#define SFFTqcap2    SFFTstate+8
#define SFFTccap2    SFFTstate+9
#define SFFTqcap3    SFFTstate+10
#define SFFTccap3    SFFTstate+11

#define SFFTnumStates 12

/* per model data */

typedef struct sSFFTmodel {       /* model structure for a sfft */
    int SFFTmodType; /* type index of this device type */
    struct sSFFTmodel *SFFTnextModel; /* pointer to next possible model in 
                                     * linked list */
    SFFTinstance * SFFTinstances; /* pointer to list of instances 
                                 * that have this model */
    IFuid SFFTmodName; /* pointer to character string naming this model */

    /* flags to indicate... */
    unsigned SFFTnumSectsGiven : 1;/* number of sections was specified */
    unsigned SFFTicGiven : 1;      /* critical current was specified */
    unsigned SFFTicposGiven : 1;   /* pos exp current was specified */
    unsigned SFFTicnegGiven : 1;   /* neg exp current was specified */
    unsigned SFFTisatGiven : 1;    /* fluxon sat. velocity current specd. */
    unsigned SFFTinsatGiven : 1;   /* fluxon sat. number current specd. */
    unsigned SFFTgainGiven : 1;    /* gain parameter specd. */
    unsigned SFFTrGiven : 1;       /* normal resistance specified */
    unsigned SFFTl1Given : 1;      /* control self inductance specd. */
    unsigned SFFTl2Given : 1;      /* body self inductance specd. */
    unsigned SFFTfactorGiven : 1;  /* coupling factor specd. */
    unsigned SFFTc1Given : 1;      /* cap. control+ to drain specd. */
    unsigned SFFTc2Given : 1;      /* cap. control- to source specd. */
    unsigned SFFTtcGiven : 1;      /* transit time constant specd. */

    int SFFTnumSects;    /* number of sections */
    double SFFTic;       /* critical current */
    double SFFTicpos;    /* pos critical current exp factor */
    double SFFTicneg;    /* neg critical current exp factor */
    double SFFTisat;     /* fluxon velocity saturation current */
    double SFFTinsat;    /* fluxon number saturation current */
    double SFFTgain;     /* gain parameter */
    double SFFTr;        /* resistance */
    double SFFTl1;       /* control line self inductance */
    double SFFTl2;       /* body inductance */
    double SFFTfactor;   /* coupling factor */
    double SFFTc1;       /* pos cont line to gate cap */
    double SFFTc2;       /* neg cont line to source cap */
    double SFFTtc;       /* transit time constant */

} SFFTmodel;


/* device parameters */
#define SFFT_LEN             1 
#define SFFT_WID             2
#define SFFT_ICV1            3
#define SFFT_ICV2            4
#define SFFT_ICI1            5
#define SFFT_ICI2            6
#define SFFT_OFF             7

/* model parameters */
#define SFFT_MOD_NSEC        101
#define SFFT_MOD_IC          102
#define SFFT_MOD_ICPOS       103
#define SFFT_MOD_ICNEG       104
#define SFFT_MOD_ISAT        105
#define SFFT_MOD_INSAT       106
#define SFFT_MOD_GAIN        107
#define SFFT_MOD_R           108
#define SFFT_MOD_L1          109
#define SFFT_MOD_L2          110
#define SFFT_MOD_K           111
#define SFFT_MOD_C1          112
#define SFFT_MOD_C2          113
#define SFFT_MOD_TC          114
#define SFFT_MOD_SFFT        115

/* device questions */
#define SFFT_QUEST_IC        201
#define SFFT_QUEST_ICPOS     202
#define SFFT_QUEST_ICNEG     203
#define SFFT_QUEST_ISAT      204
#define SFFT_QUEST_INSAT     205
#define SFFT_QUEST_R         206
#define SFFT_QUEST_L1        207
#define SFFT_QUEST_L2        208
#define SFFT_QUEST_MUT       209
#define SFFT_QUEST_C1        210
#define SFFT_QUEST_C2        211
#define SFFT_QUEST_TC        212


#ifdef __STDC__

extern int  SFFTacLoad(GENmodel*,CKTcircuit*);
extern int  SFFTask(CKTcircuit*,GENinstance*,int,IFvalue*,IFvalue*);
extern int  SFFTconvTest(GENmodel *,CKTcircuit*);
extern int  SFFTdisto(int,GENmodel*,CKTcircuit*);
extern int  SFFTgetic(GENmodel*,CKTcircuit*);
extern int  SFFTload(GENmodel*,CKTcircuit*);
extern int  SFFTmAsk(CKTcircuit*,GENmodel*,int,IFvalue*);
extern int  SFFTmParam(int,IFvalue*,GENmodel*);
extern int  SFFTnoise(int,int,GENmodel*,CKTcircuit*,GENERIC*,double*);
extern int  SFFTparam(CKTcircuit*,int,IFvalue*,GENinstance*,IFvalue*);
extern void SFFTparse(int,GENERIC*,GENERIC*,GENERIC*);
extern int  SFFTpzLoad(GENmodel*,CKTcircuit*,SPcomplex*);
extern int  SFFTsetup(SMPmatrix*,GENmodel*,CKTcircuit*,int*);
extern int  SFFTtemp(GENmodel*,CKTcircuit*);
extern int  SFFTtrunc(GENmodel*,CKTcircuit*,double*);

#else /* stdc */

extern int  SFFTacLoad();
extern int  SFFTask();
extern int  SFFTconvTest();
extern int  SFFTdisto();
extern int  SFFTgetic();
extern int  SFFTload();
extern int  SFFTmAsk();
extern int  SFFTmParam();
extern int  SFFTnoise();
extern int  SFFTparam();
extern void SFFTparse();
extern int  SFFTpzLoad();
extern int  SFFTsetup();
extern int  SFFTtemp();
extern int  SFFTtrunc();

#endif /* stdc */


#endif /*SFFT*/

