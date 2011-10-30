/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Gordon M. Jacobs
         1992 Stephen R. Whiteley
****************************************************************************/

#ifndef SW
#define SW

#include "devdefs.h"

    /* structures used to describe switches */


/* information to describe each instance */

typedef struct sSWinstance {
    struct sSWmodel *SWmodPtr;  /* backpointer to model */
    struct sSWinstance *SWnextInstance;   /* pointer to next instance of 
                                           * current model*/
    IFuid SWname;  /* pointer to character string naming this instance */
    int SWstate;   /* pointer to start of switch's section of state vector */

    int SWposNode; /* number of positive node of switch */
    int SWnegNode; /* number of negative node of switch */
    int SWposCntrlNode; /* number of positive controlling node of switch */
    int SWnegCntrlNode; /* number of negative controlling node of switch */
    int SWcontBranch;   /* number of branch of controlling current */
    IFuid SWcontName;   /* name of controlling source */

    double *SWposPosptr;  /* pointer to sparse matrix diagonal at
                                (positive,positive) for switch conductance */
    double *SWnegPosptr;  /* pointer to sparse matrix offdiagonal at
                                (neagtive,positive) for switch conductance */
    double *SWposNegptr;  /* pointer to sparse matrix offdiagonal at
                                (positive,neagtive) for switch conductance */
    double *SWnegNegptr;  /* pointer to sparse matrix diagonal at
                                (neagtive,neagtive) for switch conductance */

    double SWcond; /* conductance of the switch now */

    unsigned SWzero_stateGiven : 1;  /* flag to indicate initial state */
#ifndef NONOISE
    double SWnVar[NSTATVARS];
#else /* NONOISE */
    double *SWnVar;
#endif /* NONOISE */

} SWinstance;

/* data per model */

#define SW_ON_CONDUCTANCE 1.0   /* default on conductance = 1 mho */
#define SW_OFF_CONDUCTANCE ckt->CKTgmin   /* default off conductance */
#define SW_NUM_STATES 1   

typedef struct sSWmodel {      /* model structure for a switch */
    int SWmodType;             /* type index of this device type */
    struct sSWmodel *SWnextModel; /* pointer to next possible model in 
                                   * linked list */
    SWinstance *SWinstances;   /* pointer to list of instances that have this
                                * model */
    IFuid SWmodName;     /* pointer to character string naming this model */

    double SWonResistance;  /* switch "on" resistance */
    double SWoffResistance; /* switch "off" resistance */
    double SWvThreshold;    /* switching threshold voltage */
    double SWvHysteresis;   /* switching hysteresis voltage */
    double SWiThreshold;    /* switching threshold current */
    double SWiHysteresis;   /* switching hysteresis current */
    double SWonConduct;     /* switch "on" conductance  */
    double SWoffConduct;    /* switch "off" conductance  */

    /* flags to indicate... */
    unsigned SWonGiven : 1;      /* on-resistance was given */
    unsigned SWoffGiven : 1;     /* off-resistance was given */
    unsigned SWvThreshGiven : 1; /* threshold volt was given */
    unsigned SWvHystGiven : 1;   /* hysteresis volt was given */
    unsigned SWiThreshGiven : 1; /* threshold current was given */
    unsigned SWiHystGiven : 1;   /* hysteresis current was given */

} SWmodel;

/* device parameters */
#define SW_IC_ON         1
#define SW_IC_OFF        2
#define SW_POS_NODE      3
#define SW_NEG_NODE      4
#define SW_POS_CONT_NODE 5
#define SW_NEG_CONT_NODE 6
#define SW_CONTROL       7
#define SW_CURRENT       8
#define SW_POWER         9

/* model parameters */
#define SW_MOD_SW        101
#define SW_MOD_RON       102
#define SW_MOD_ROFF      103
#define SW_MOD_VTH       104
#define SW_MOD_VHYS      105
#define SW_MOD_ITH       106
#define SW_MOD_IHYS      107
#define SW_MOD_GON       108
#define SW_MOD_GOFF      109


#ifdef __STDC__

extern int  SWacLoad(GENmodel*,CKTcircuit*);
extern int  SWask(CKTcircuit*,GENinstance*,int,IFvalue*,IFvalue*);
extern int  SWload(GENmodel*,CKTcircuit*);
extern int  SWmAsk(CKTcircuit*,GENmodel*,int,IFvalue*);
extern int  SWmParam(int,IFvalue*,GENmodel*);
extern int  SWnoise(int,int,GENmodel*,CKTcircuit*,GENERIC*,double*);
extern int  SWparam(CKTcircuit*,int,IFvalue*,GENinstance*,IFvalue*);
extern void SWparse(int,GENERIC*,GENERIC*,GENERIC*);
extern int  SWpzLoad(GENmodel*,CKTcircuit*,SPcomplex*);
extern int  SWsetup(SMPmatrix*,GENmodel*,CKTcircuit*,int*);

#else /* stdc */

extern int  SWacLoad();
extern int  SWask();
extern int  SWload();
extern int  SWmAsk();
extern int  SWmParam();
extern int  SWnoise();
extern int  SWparam();
extern void SWparse();
extern int  SWpzLoad();
extern int  SWsetup();

#endif /* stdc */


#endif /*SW*/
