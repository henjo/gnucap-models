/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 THomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

#ifndef DEV
#define DEV

#include "cktdefs.h"

/* devsup.c */
#ifdef __STDC__

extern double DEVlimvds(double,double);
extern double DEVpnjlim(double,double,double,double,int*);
extern double DEVfetlim(double,double,double);
extern void   DEVqmeyer(double,double,double,double,double*,double*,
                    double*,double,double);
extern double DEVpred(CKTcircuit*,int);
extern double *DEVpredNew(CKTcircuit*);
extern int    GENdelete(GENmodel*,IFuid,GENinstance**);
extern void   GENdestroy(GENmodel**);
extern int    GENmDelete(GENmodel**,IFuid,GENmodel*);
extern void   GENmosParse(int,GENERIC*,GENERIC*,GENERIC*);

#else /* stdc */

extern double DEVlimvds();
extern double DEVpnjlim();
extern double DEVfetlim();
extern void   DEVqmeyer();
extern double DEVpred();
extern double *DEVpredNew();
extern int    GENdelete();
extern void   GENdestroy();
extern int    GENmDelete();
extern void   GENmosParse();

#endif /* stdc */

typedef struct SPICEdev {
    IFdevice DEVpublic;

#ifdef __STDC__
    int (*DEVparam)(CKTcircuit*,int,IFvalue*,GENinstance*,IFvalue *);  
        /* routine to input a parameter to a device instance */
    int (*DEVmodParam)(int,IFvalue*,GENmodel*);   
        /* routine to input a paramater to a model */
    int (*DEVload)(GENmodel*,CKTcircuit*);   
        /* routine to load the device into the matrix */
    int (*DEVsetup)(SMPmatrix*,GENmodel*,CKTcircuit*,int*);  
        /* setup routine to preprocess devices once before soloution begins */
    int (*DEVpzSetup)(SMPmatrix*,GENmodel*,CKTcircuit*,int*);  
        /* setup routine to process devices specially for pz analysis */
    int (*DEVtemperature)(GENmodel*,CKTcircuit*);    
        /* subroutine to do temperature dependent setup processing */
    int (*DEVtrunc)(GENmodel*,CKTcircuit*,double*);  
        /* subroutine to perform truncation error calc. */
    int (*DEVfindBranch)(CKTcircuit*,GENmodel*,IFuid); 
        /* subroutine to search for device branch eq.s */
    int (*DEVacLoad)(GENmodel*,CKTcircuit*); 
        /* ac analysis loading function */
    int (*DEVaccept)(CKTcircuit*,GENmodel*); 
        /* subroutine to call on acceptance of a timepoint */
    void (*DEVdestroy)(GENmodel**);   
        /* subroutine to destroy all models and instances */
    int (*DEVmodDelete)(GENmodel**,IFuid,GENmodel*);  
        /* subroutine to delete a model and all instances */
    int (*DEVdelete)(GENmodel*,IFuid,GENinstance**); 
        /* subroutine to delete an instance */
    int (*DEVsetic)(GENmodel*,CKTcircuit*);  
        /* routine to pick up device init conds from rhs */
    int (*DEVask)(CKTcircuit*,GENinstance*,int,IFvalue*,IFvalue*);    
        /* routine to ask about device details*/
    int (*DEVmodAsk)(CKTcircuit*,GENmodel*,int,IFvalue*); 
        /* routine to ask about model details*/
    int (*DEVpzLoad)(GENmodel*,CKTcircuit*,SPcomplex*); 
        /* routine to load for pole-zero analysis */
    int (*DEVconvTest)(GENmodel*,CKTcircuit*);  
        /* convergence test function */
    int (*DEVdisto)(int,GENmodel*,CKTcircuit*);  
        /* procedure to do distortion operations */
    int (*DEVnoise)(int, int, GENmodel*,CKTcircuit*, GENERIC *, double *);
        /* noise routine */

#else /* stdc */

    int (*DEVparam)();      /* input a parameter to a device instance */
    int (*DEVmodParam)();   /* input a paramater to a model */
    int (*DEVload)();       /* load the device into the matrix */
    int (*DEVsetup)();      /* preprocess devices before soloution begins */
    int (*DEVpzSetup)();    /* process devices specially for pz analysis */
    int (*DEVtemperature)();/* temperature dependent setup processing */
    int (*DEVtrunc)();      /* perform truncation error calc. */
    int (*DEVfindBranch)(); /* search for device branch eq.s */
    int (*DEVacLoad)();     /* ac analysis loading function */
    int (*DEVaccept)();     /* call on acceptance of a timepoint */
    void (*DEVdestroy)();   /* destroy all models and instances */
    int (*DEVmodDelete)();  /* delete a model and all instances */
    int (*DEVdelete)();     /* delete an instance */
    int (*DEVsetic)();      /* pick up device init conds from rhs */
    int (*DEVask)();        /* ask about device details*/
    int (*DEVmodAsk)();     /* ask about model details*/
    int (*DEVpzLoad)();     /* load for pole-zero analysis */
    int (*DEVconvTest)();   /* convergence test function */
    int (*DEVdisto)();      /* distortion routine */
    int (*DEVnoise)();      /* noise routine */

#endif /* stdc */

    int *DEVinstSize;    /* size of an instance */
    int *DEVmodSize;     /* size of a model */

} SPICEdev;  /* instance of structure for each possible type of device */

#define IP(a,b,c,d)  { a , b , c|IF_SET , d }
#define OP(a,b,c,d)  { a , b , c|IF_ASK , d }
#define IOP(a,b,c,d) { a , b , c|IF_SET|IF_ASK , d }
#define P(a,b,c,d)   { a , b , c , d }

/* macro to make elements with built in test for out of memory */
#define TSTALLOC(ptr,first,second) \
if((here->ptr = spGetElement(matrix,here->first,here->second))==(double *)NULL){\
    return(E_NOMEM);\
}

/* macros to do trapezoidal integration inline */
#define NI_SUM(ckt,ceq,qcap){ \
  if(ckt->CKTintegrateMethod==TRAPEZOIDAL){ \
    if(ckt->CKTorder==1) \
      ceq=ckt->CKTag[1]* *(ckt->CKTstate1+qcap); \
    else \
      ceq= - *(ckt->CKTstate1+qcap+1)*ckt->CKTag[1]- \
        ckt->CKTag[0]* *(ckt->CKTstate1+qcap); \
  } \
  else {error=NIsum(ckt,&ceq,qcap); if(error) return(error);}\
}

#define NI_INTEG(ckt,geq,ceq,cap,qcap){ \
  if (ckt->CKTintegrateMethod==TRAPEZOIDAL){ \
    if (ckt->CKTorder==1) \
      ceq=ckt->CKTag[1]* *(ckt->CKTstate1+qcap); \
    else \
      ceq= - *(ckt->CKTstate1+qcap+1)*ckt->CKTag[1]- \
        ckt->CKTag[0]* *(ckt->CKTstate1+qcap); \
    *(ckt->CKTstate0+qcap+1)= \
      ckt->CKTag[0]* *(ckt->CKTstate0+qcap)+ceq; \
    geq=ckt->CKTag[0]*cap; \
  } \
  else {error=NIintegrate(ckt,&geq,&ceq,cap,qcap); if(error) return(error);} \
}

#endif /*DEV*/
