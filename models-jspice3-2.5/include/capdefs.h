/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

#ifndef CAP
#define CAP


#include "devdefs.h"

    /* structures used to describe capacitors */


/* information to describe each instance */

typedef struct sCAPinstance {
    struct sCAPmodel *CAPmodPtr;    /* backpointer to model */
    struct sCAPinstance *CAPnextInstance;  /* pointer to next instance of 
                                            * current model*/
    IFuid CAPname;      /* pointer to character string naming this instance */
    int CAPstate;       /* pointer to start of capacitor state vector */
    int CAPposNode;     /* number of positive node of capacitor */
    int CAPnegNode;     /* number of negative node of capacitor */
    double CAPcapac;    /* capacitance */
    double CAPinitCond; /* initial capacitor voltage if specified */
    double CAPceq;      /* storage for ceq, rhs entry */
    double CAPgeq;      /* stroage for geq, matrix entry */
    double CAPwidth;    /* width of the capacitor */
    double CAPlength;   /* length of the capacitor */

    double *CAPposPosptr;    /* pointer to sparse matrix diagonal at 
                              * (positive,positive) */
    double *CAPnegNegptr;    /* pointer to sparse matrix diagonal at 
                              * (negative,negative) */
    double *CAPposNegptr;    /* pointer to sparse matrix offdiagonal at 
                              * (positive,negative) */
    double *CAPnegPosptr;    /* pointer to sparse matrix offdiagonal at 
                              * (negative,positive) */

    /* flags to indicate... */
    unsigned CAPcapGiven    : 1; /* capacitance was specified */
    unsigned CAPicGiven     : 1; /* init. condition was specified */
    unsigned CAPwidthGiven  : 1; /* capacitor width given */
    unsigned CAPlengthGiven : 1; /* capacitor length given */

} CAPinstance;

/* data per model */

typedef struct sCAPmodel {      /* model structure for a capacitor */
    int CAPmodType; /* type index of this device type */
    struct sCAPmodel *CAPnextModel; /* pointer to next possible model in 
                                     * linked list */
    CAPinstance * CAPinstances; /* pointer to list of instances that have this
                                 * model */
    IFuid CAPmodName;   /* pointer to character string naming this model */
    double CAPcj;       /* Unit Area Capacitance ( F/ M**2 ) */
    double CAPcjsw;     /* Unit Length Sidewall Capacitance ( F / M ) */
    double CAPdefWidth; /* the default width of a capacitor */
    double CAPnarrow;   /* amount by which length/width are less than drawn */
    unsigned CAPcjGiven : 1;       /* Unit Area Capacitance ( F/ M**2 ) */
    unsigned CAPcjswGiven : 1;     /* Unit Length Sidewall Capacitance(F/M) */
    unsigned CAPdefWidthGiven : 1; /* flag indicates default width given*/
    unsigned CAPnarrowGiven : 1;   /* flag indicates narrowing factor given */

} CAPmodel;


#define CAPqcap CAPstate     /* charge on the capacitor */
#define CAPccap CAPstate+1   /* current through the capacitor */
                            
/* device parameters */
#define CAP_CAP      1
#define CAP_IC       2
#define CAP_WIDTH    3
#define CAP_LENGTH   4
#define CAP_CURRENT  5
#define CAP_POWER    6

/* model parameters */
#define CAP_MOD_CJ          101
#define CAP_MOD_CJSW        102
#define CAP_MOD_DEFWIDTH    103
#define CAP_MOD_C           104
#define CAP_MOD_NARROW      105


#ifdef __STDC__

extern int  CAPacLoad(GENmodel*,CKTcircuit*);
extern int  CAPask(CKTcircuit*,GENinstance*,int,IFvalue*,IFvalue*);
extern int  CAPgetic(GENmodel*,CKTcircuit*);
extern int  CAPload(GENmodel*,CKTcircuit*);
extern int  CAPmAsk(CKTcircuit*,GENmodel*,int,IFvalue*);
extern int  CAPmParam(int,IFvalue*,GENmodel*);
extern int  CAPparam(CKTcircuit*,int,IFvalue*,GENinstance*,IFvalue*);
extern void CAPparse(int,GENERIC*,GENERIC*,GENERIC*);
extern int  CAPpzLoad(GENmodel*,CKTcircuit*,SPcomplex*);
extern int  CAPsetup(SMPmatrix*,GENmodel*,CKTcircuit*,int*);
extern int  CAPtemp(GENmodel*,CKTcircuit*);
extern int  CAPtrunc(GENmodel*,CKTcircuit*,double*);

#else /* stdc */

extern int  CAPacLoad();
extern int  CAPask();
extern int  CAPgetic();
extern int  CAPload();
extern int  CAPmAsk();
extern int  CAPmParam();
extern int  CAPparam();
extern void CAPparse();
extern int  CAPpzLoad();
extern int  CAPsetup();
extern int  CAPtemp();
extern int  CAPtrunc();

#endif /* stdc */


#endif /*CAP*/
