/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1990 Jaijeet S. Roychowdhury
         1993 Stephen R. Whiteley
****************************************************************************/

#ifndef LTRA
#define LTRA 
#undef LTRALTEINFO
#undef LTRADEBUG

#include "devdefs.h"

/* structures used to describe lossy transmission liness */

/* information used to describe a single instance */

typedef struct sLTRAinstance {
    struct sLTRAmodel *LTRAmodPtr;    /* backpointer to model */
    struct sLTRAinstance *LTRAnextInstance;
                          /* pointer to next instance of current model */
    IFuid LTRAname;       /* pointer to char string naming this instance */
    int LTRAstate;        /* pointer to start of state vector for line */
    int LTRAposNode1;     /* number of positive node of end 1 of t. line */
    int LTRAnegNode1;     /* number of negative node of end 1 of t. line */
    int LTRAposNode2;     /* number of positive node of end 2 of t. line */
    int LTRAnegNode2;     /* number of negative node of end 2 of t. line */
    int LTRAbrEq1;        /* number of branch equation for end 1 of t. line */
    int LTRAbrEq2;        /* number of branch equation for end 2 of t. line */
    double LTRAinput1;    /* accumulated excitation for port 1 */
    double LTRAinput2;    /* accumulated excitation for port 2 */
    double LTRAinput1Old; /* previous accumulated excitation for port 1 */
    double LTRAinput2Old; /* previous accumulated excitation for port 2 */
    double LTRAinitVolt1; /* initial condition:  voltage on port 1 */
    double LTRAinitCur1;  /* initial condition:  current at port 1 */
    double LTRAinitVolt2; /* initial condition:  voltage on port 2 */
    double LTRAinitCur2;  /* initial condition:  current at port 2 */
    double *LTRAv1;       /* past values of v1 */
    double *LTRAi1;       /* past values of i1 */
    double *LTRAv2;       /* past values of v2 */
    double *LTRAi2;       /* past values of i2 */
    int LTRAinstListSize; /* size of above lists */

    double *LTRAibr1Ibr1Ptr;   /* pointer to sparse matrix */
    double *LTRAibr1Ibr2Ptr;   /* pointer to sparse matrix */
    double *LTRAibr1Pos1Ptr;   /* pointer to sparse matrix */
    double *LTRAibr1Neg1Ptr;   /* pointer to sparse matrix */
    double *LTRAibr1Pos2Ptr;   /* pointer to sparse matrix */
    double *LTRAibr1Neg2Ptr;   /* pointer to sparse matrix */
    double *LTRAibr2Ibr1Ptr;   /* pointer to sparse matrix */
    double *LTRAibr2Ibr2Ptr;   /* pointer to sparse matrix */
    double *LTRAibr2Pos1Ptr;   /* pointer to sparse matrix */
    double *LTRAibr2Neg1Ptr;   /* pointer to sparse matrix */
    double *LTRAibr2Pos2Ptr;   /* pointer to sparse matrix */
    double *LTRAibr2Neg2Ptr;   /* pointer to sparse matrix */
    double *LTRAneg1Ibr1Ptr;   /* pointer to sparse matrix */
    double *LTRAneg2Ibr2Ptr;   /* pointer to sparse matrix */
    double *LTRApos1Ibr1Ptr;   /* pointer to sparse matrix */
    double *LTRApos2Ibr2Ptr;   /* pointer to sparse matrix */
    double *LTRApos1Pos1Ptr;   /* pointer to sparse matrix */
    double *LTRAneg1Neg1Ptr;   /* pointer to sparse matrix */
    double *LTRApos2Pos2Ptr;   /* pointer to sparse matrix */
    double *LTRAneg2Neg2Ptr;   /* pointer to sparse matrix */

    unsigned LTRAicV1Given:1;  /* flag, init. voltage at port 1 given */
    unsigned LTRAicC1Given:1;  /* flag, init. current at port 1 given */
    unsigned LTRAicV2Given:1;  /* flag, init. voltage at port 2 given */
    unsigned LTRAicC2Given:1;  /* flag, init. current at port 2 given */
} LTRAinstance ;

#define LTRAv1Tot  LTRAstate
#define LTRAv2Tot  LTRAstate+1


/* per model data */
/* model structure for transmission lines */

typedef struct sLTRAmodel {

    int LTRAmodType;   /* type index of this device type */
    struct sLTRAmodel *LTRAnextModel;
                       /* pointer to next possible model in linked list */
    LTRAinstance *LTRAinstances;
                       /* pointer to list of instances that have this model */
    IFuid LTRAmodName; /* pointer to character string naming this model */

    double LTRAh1dashFirstVal;
                       /* first needed value of h1dash at current timepoint */
    double LTRAh2FirstVal;
                       /* first needed value of h2 at current timepoint */
    double LTRAh3dashFirstVal;
                       /* first needed value of h3dash at current timepoint */
    double LTRAh1dashFirstCoeff;
                 /* first needed coeff of h1dash for the current timepoint */
    double LTRAh2FirstCoeff;
                 /* first needed coeff of h2 for the current timepoint */
    double LTRAh3dashFirstCoeff;
                 /* first needed coeff of h3dash for the current timepoint */

    double *LTRAh1dashCoeffs; /* list of other coefficients for h1dash */
    double *LTRAh2Coeffs;     /* list of other coefficients for h2 */
    double *LTRAh3dashCoeffs; /* list of other coefficients for h3dash */
    int LTRAmodelListSize;    /* size of above lists */

    double LTRAconduct;       /* conductance G  - input */
    double LTRAresist;        /* resistance R  - input */
    double LTRAinduct;        /* inductance L - input */
    double LTRAcapac;         /* capacitance C - input */
    double LTRAlength;        /* length l - input */
    double LTRAtd;            /* propagation delay T - calculated */
    double LTRAimped;         /* impedance Z - calculated */
    double LTRAadmit;         /* admittance Y - calculated */
    double LTRAalpha;         /* alpha - calculated */
    double LTRAbeta;          /* beta - calculated */
    double LTRAattenuation;   /* e^(-beta T) - calculated */
    double LTRAcByR;          /* C/R - for the RC line - calculated */
    double LTRArclsqr;        /* RCl^2 - for the RC line - calculated */
    double LTRAintH1dash;     /* int 0-inf h'1(tau) d tau - calculated */
    double LTRAintH2;         /* int 0-inf h 2(tau) d tau - calculated */
    double LTRAintH3dash;     /* int 0-inf h'3(tau) d tau - calculated */

    double LTRAcoshlrootGR;   /* cosh(l*sqrt(G*R)), used for DC anal */
    double LTRArRsLrGRorG;    /* sqrt(R)*sinh(l*sqrt(G*R))/sqrt(G) */
    double LTRArGsLrGRorR;    /* sqrt(G)*sinh(l*sqrt(G*R))/sqrt(R) */

    int LTRAauxIndex;         /* auxiliary index for h2 and h3dash */
    double LTRAstLineReltol;  /* reltol for checking st. lines */
    double LTRAchopReltol;    /* reltol for truncation of impulse responses */
    double LTRAstLineAbstol;  /* abstol for checking st.  lines */
    double LTRAchopAbstol;    /* abstol for truncation of impulse responses */

    unsigned LTRAreltolGiven:1; /* relative deriv. tol. given */
    unsigned LTRAabstolGiven:1; /* absolute deriv. tol. given */
    unsigned LTRAtruncNR;
             /* use N-R iterations for calculating step in LTRAtrunc */
    unsigned LTRAtruncDontCut;
             /* don't bother about errors in impulse response calculations
                due to large steps */
    double LTRAmaxSafeStep;
             /* maximum safe step for impulse response calculations */
    unsigned LTRAresistGiven:1; /* R was specified */
    unsigned LTRAconductGiven:1;/* G was specified */
    unsigned LTRAinductGiven:1; /* L was specified */
    unsigned LTRAcapacGiven:1;  /* C was specified */
    unsigned LTRAlengthGiven:1; /* length was specified */
    int LTRAlteConType;
             /* indicates whether full control, half control or no control */
    int LTRAhowToInterp;
             /* indicates how to interpolate for delayed timepoint */
    int LTRAstepLimit;
             /* flag to indicate that the timestep should always be
                limited to 0.8*LTRAtd */
    unsigned LTRAfGiven:1;  /* flag to indicate freq was specified */
    double LTRAabstol;      /* absolute deriv. tol. for breakpoint setting */
    double LTRAreltol;      /* relative deriv. tol. for breakpoint setting */

    int LTRAspecialCase;    /* what kind of model (RC, RLC, RL, ...) */

/* special cases */
#define LTRA_MOD_RLC         1
#define LTRA_MOD_RC          2
#define LTRA_MOD_RG          3
#define LTRA_MOD_LC          4
#define LTRA_MOD_RL          5

} LTRAmodel;


/* device parameters */
#define LTRA_V1              1
#define LTRA_I1              2
#define LTRA_V2              3
#define LTRA_I2              4
#define LTRA_IC              5
#define LTRA_POS_NODE1       6
#define LTRA_NEG_NODE1       7
#define LTRA_POS_NODE2       8
#define LTRA_NEG_NODE2       9
#define LTRA_BR_EQ1          10
#define LTRA_BR_EQ2          11

/* model parameters */
#define LTRA_MOD_LTRA        100
#define LTRA_MOD_R           101
#define LTRA_MOD_L           102
#define LTRA_MOD_G           103
#define LTRA_MOD_C           104
#define LTRA_MOD_LEN         105
#define LTRA_MOD_RELTOL      106
#define LTRA_MOD_ABSTOL      107
#define LTRA_MOD_NOCONTROL   108
#define LTRA_MOD_HALFCONTROL 109
#define LTRA_MOD_FULLCONTROL 110
#define LTRA_MOD_STEPLIMIT   111
#define LTRA_MOD_NOSTEPLIMIT 112
#define LTRA_MOD_LININTERP   113
#define LTRA_MOD_QUADINTERP  114
#define LTRA_MOD_MIXEDINTERP 115
#define LTRA_MOD_TRUNCNR     116
#define LTRA_MOD_TRUNCDONTCUT 117
#define LTRA_MOD_STLINEREL   118
#define LTRA_MOD_STLINEABS   119

/* model questions */
#define LTRA_MOD_Z0          200
#define LTRA_MOD_TD          201


#ifdef __STDC__

extern int  LTRAaccept(CKTcircuit*,GENmodel*);
extern int  LTRAask(CKTcircuit*,GENinstance*,int,IFvalue*,IFvalue*);
extern int  LTRAacLoad(GENmodel*,CKTcircuit*);
extern int  LTRAdelete(GENmodel*,IFuid,GENinstance**);
extern void LTRAdestroy(GENmodel**);
extern int  LTRAload(GENmodel*,CKTcircuit*);
extern int  LTRAmAsk(CKTcircuit*,GENmodel*,int,IFvalue*);
extern int  LTRAmDelete(GENmodel**,IFuid,GENmodel*);
extern int  LTRAmParam(int,IFvalue*,GENmodel*);
extern int  LTRAparam(CKTcircuit*,int,IFvalue*,GENinstance*,IFvalue*);
extern void LTRAparse(int,GENERIC*,GENERIC*,GENERIC*);
extern int  LTRAsetup(SMPmatrix*,GENmodel*,CKTcircuit*,int*);
extern int  LTRAtrunc(GENmodel*,CKTcircuit*,double*);

extern void LTRArcCoeffsSetup(CKTcircuit*,GENmodel*);
extern void LTRArlcCoeffsSetup(CKTcircuit*,GENmodel*);
extern double LTRArlcH2Func(double,GENmodel*);
extern double LTRArlcH3dashFunc(double,double,double,double);
extern int  LTRAstraightLineCheck(double,double,double,double,double,double,double,double);
extern double LTRAlteCalculate(CKTcircuit*,GENmodel*,GENinstance*,double);

#else /* stdc */

extern int  LTRAaccept();
extern int  LTRAask();
extern int  LTRAacLoad();
extern int  LTRAdelete();
extern void LTRAdestroy();
extern int  LTRAload();
extern int  LTRAmAsk();
extern int  LTRAmDelete();
extern int  LTRAmParam();
extern int  LTRAparam();
extern void LTRAparse();
extern int  LTRAsetup();
extern int  LTRAtrunc();

extern void LTRArcCoeffsSetup();
extern void LTRArlcCoeffsSetup();
extern double LTRArlcH2Func();
extern double LTRArlcH3dashFunc();
extern int  LTRAstraightLineCheck();
extern double LTRAlteCalculate();

#endif /* stdc */


#endif /*LTRA*/

