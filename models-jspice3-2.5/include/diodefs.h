/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

#ifndef DIO
#define DIO

#include "devdefs.h"

            /* data structures used to describe diodes */


/* information needed per instance */

typedef struct sDIOinstance {
    struct sDIOmodel *DIOmodPtr;    /* backpointer to model */
    struct sDIOinstance *DIOnextInstance;   /* pointer to next instance of 
                                             * current model*/
    IFuid DIOname;      /* pointer to character string naming this instance */
    int DIOstate;       /* pointer to start of state vector for diode */
    int DIOposNode;     /* number of positive node of diode */
    int DIOnegNode;     /* number of negative node of diode */
    int DIOposPrimeNode;/* number of positive prime node of diode */
    double *DIOposPosPrimePtr;      /* pointer to sparse matrix at 
                                     * (positive,positive prime) */
    double *DIOnegPosPrimePtr;      /* pointer to sparse matrix at 
                                     * (negative,positive prime) */
    double *DIOposPrimePosPtr;      /* pointer to sparse matrix at 
                                     * (positive prime,positive) */
    double *DIOposPrimeNegPtr;      /* pointer to sparse matrix at 
                                     * (positive prime,negative) */
    double *DIOposPosPtr;   /* pointer to sparse matrix at 
                             * (positive,positive) */
    double *DIOnegNegPtr;   /* pointer to sparse matrix at 
                             * (negative,negative) */
    double *DIOposPrimePosPrimePtr; /* pointer to sparse matrix at 
                                     * (positive prime,positive prime) */

    unsigned DIOoff : 1;          /* 'off' flag for diode */
    unsigned DIOareaGiven : 1;    /* flag to indicate area was specified */
    unsigned DIOinitCondGiven : 1;/* flag to indicate ic was specified */
    unsigned DIOtempGiven : 1;    /* flag to indicate temperature was specified */


    double DIOarea;     /* area factor for the diode */
    double DIOinitCond; /* initial condition */
    double DIOtemp;     /* temperature of the instance */
    double DIOtJctPot;  /* temperature adjusted junction potential */
    double DIOtJctCap;  /* temperature adjusted junction capacitance */
    double DIOtDepCap;  /* temperature adjusted transition point in */
                        /* the curve matching (Fc * Vj ) */
    double DIOtSatCur;  /* temperature adjusted saturation current */
    double DIOtVcrit;   /* temperature adjusted V crit */
    double DIOtF1;      /* temperature adjusted f1 */
    double DIOtBrkdwnV; /* temperature adjusted breakdown voltage */
    double DIOvte;
    double DIOcd;       /* diode current */
    double DIOgd;       /* diode conductance */
    double DIOcap;      /* diode capacitance */
    double DIOvd;       /* temporary diode voltage */

/*
 * naming convention:
 * x = vdiode
 */

/* the following are relevant to s.s. sinusoidal distortion analysis */

#define DIONDCOEFFS    6

#ifndef NODISTO
    double DIOdCoeffs[DIONDCOEFFS];
#else /* NODISTO */
    double *DIOdCoeffs;
#endif /* NODISTO */

#ifndef CONFIG

#define    id_x2        DIOdCoeffs[0]
#define    id_x3        DIOdCoeffs[1]
#define    cdif_x2      DIOdCoeffs[2]
#define    cdif_x3      DIOdCoeffs[3]
#define    cjnc_x2      DIOdCoeffs[4]
#define    cjnc_x3      DIOdCoeffs[5]

#endif

/* indices to array of diode noise  sources */

#define DIORSNOIZ  0
#define DIOIDNOIZ  1
#define DIOFLNOIZ  2
#define DIOTOTNOIZ 3

#define DIONSRCS   4

#ifndef NONOISE
     double DIOnVar[NSTATVARS][DIONSRCS];
#else /* NONOISE */
    double **DIOnVar;
#endif /* NONOISE */

} DIOinstance ;


#define DIOvoltage    DIOstate
#define DIOcapCharge  DIOstate+1
#define DIOcapCurrent DIOstate+2

#define DIOnumStates 3

/* per model data */

typedef struct sDIOmodel {       /* model structure for a diode */
    int DIOmodType; /* type index of this device type */
    struct sDIOmodel *DIOnextModel; /* pointer to next possible model in 
                                     * linked list */
    DIOinstance * DIOinstances; /* pointer to list of instances 
                                 * that have this model */
    IFuid DIOmodName; /* pointer to character string naming this model */

    unsigned DIOsatCurGiven : 1;
    unsigned DIOresistGiven : 1;
    unsigned DIOemissionCoeffGiven : 1;
    unsigned DIOtransitTimeGiven : 1;
    unsigned DIOjunctionCapGiven : 1;
    unsigned DIOjunctionPotGiven : 1;
    unsigned DIOgradingCoeffGiven : 1;
    unsigned DIOactivationEnergyGiven : 1;
    unsigned DIOsaturationCurrentExpGiven : 1;
    unsigned DIOdepletionCapCoeffGiven : 1;
    unsigned DIObreakdownVoltageGiven : 1;
    unsigned DIObreakdownCurrentGiven : 1;
    unsigned DIOnomTempGiven : 1;
    unsigned DIOfNcoefGiven : 1;
    unsigned DIOfNexpGiven : 1;

    double DIOsatCur;       /* saturation current */
    double DIOresist;       /* ohmic series resistance */
    double DIOconductance;  /* conductance corresponding to ohmic R */
    double DIOemissionCoeff;/* emission coefficient (N) */
    double DIOtransitTime;  /* transit time (TT) */
    double DIOjunctionCap;  /* Junction Capacitance (Cj0) */
    double DIOjunctionPot;  /* Junction Potential (Vj) or (PB) */
    double DIOgradingCoeff; /* grading coefficient (m) */
    double DIOactivationEnergy; /* activation energy (EG) */
    double DIOsaturationCurrentExp; /* Saturation current exponential (XTI) */
    double DIOdepletionCapCoeff;/* Depletion Cap fraction coefficient (FC)*/
    double DIObreakdownVoltage; /* Voltage at reverse breakdown */
    double DIObreakdownCurrent; /* Current at above voltage */
    double DIOf2;   /* coefficient for capacitance equation precomputation */
    double DIOf3;   /* coefficient for capacitance equation precomputation */
    double DIOnomTemp; /* nominal temperature (temp at which parms measured */
    double DIOfNcoef;
    double DIOfNexp;


} DIOmodel;


/* device parameters */
#define DIO_AREA            1 
#define DIO_IC              2
#define DIO_OFF             3
#define DIO_CURRENT         4
#define DIO_VOLTAGE         5
#define DIO_CHARGE          6
#define DIO_CAPCUR          7
#define DIO_CONDUCT         8
#define DIO_POWER           9
#define DIO_TEMP            10
#define DIO_CAP             11


/* model parameters */
#define DIO_MOD_IS          101
#define DIO_MOD_RS          102
#define DIO_MOD_N           103
#define DIO_MOD_TT          104
#define DIO_MOD_CJO         105
#define DIO_MOD_VJ          106
#define DIO_MOD_M           107
#define DIO_MOD_EG          108
#define DIO_MOD_XTI         109
#define DIO_MOD_FC          110
#define DIO_MOD_BV          111
#define DIO_MOD_IBV         112
#define DIO_MOD_D           113
#define DIO_MOD_COND        114
#define DIO_MOD_TNOM        115
#define DIO_MOD_KF          116
#define DIO_MOD_AF          117


#ifdef __STDC__

extern int  DIOacLoad(GENmodel*,CKTcircuit*);
extern int  DIOask(CKTcircuit*,GENinstance*,int,IFvalue*,IFvalue*);
extern int  DIOconvTest(GENmodel *,CKTcircuit*);
extern int  DIOdisto(int,GENmodel*,CKTcircuit*);
extern int  DIOgetic(GENmodel*,CKTcircuit*);
extern int  DIOload(GENmodel*,CKTcircuit*);
extern int  DIOmAsk(CKTcircuit*,GENmodel*,int,IFvalue*);
extern int  DIOmParam(int,IFvalue*,GENmodel*);
extern int  DIOnoise(int,int,GENmodel*,CKTcircuit*,GENERIC*,double*);
extern int  DIOparam(CKTcircuit*,int,IFvalue*,GENinstance*,IFvalue*);
extern void DIOparse(int,GENERIC*,GENERIC*,GENERIC*);
extern int  DIOpzLoad(GENmodel*,CKTcircuit*,SPcomplex*);
extern int  DIOsetup(SMPmatrix*,GENmodel*,CKTcircuit*,int*);
extern int  DIOtemp(GENmodel*,CKTcircuit*);
extern int  DIOtrunc(GENmodel*,CKTcircuit*,double*);

#else /* stdc */

extern int  DIOacLoad();
extern int  DIOask();
extern int  DIOconvTest();
extern int  DIOdisto();
extern int  DIOgetic();
extern int  DIOload();
extern int  DIOmAsk();
extern int  DIOmParam();
extern int  DIOnoise();
extern int  DIOparam();
extern void DIOparse();
extern int  DIOpzLoad();
extern int  DIOsetup();
extern int  DIOtemp();
extern int  DIOtrunc();

#endif /* stdc */


#endif /*DIO*/
