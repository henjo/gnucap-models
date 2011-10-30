/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

#ifndef URC
#define URC


#include "devdefs.h"

    /*
     *  structures used to describe uniform RC lines
     */

/* information needed for each instance */

typedef struct sURCinstance {
    struct sURCmodel *URCmodPtr;    /* backpointer to model */
    struct sURCinstance *URCnextInstance;   /* pointer to next instance of 
                                             * current model*/
    IFuid URCname;    /* pointer to character string naming this instance */
    int UNCstate;     /* not used, placeholder for GENinstance */
    int URCposNode;   /* number of positive node of URC */
    int URCnegNode;   /* number of negative node of URC */
    int URCgndNode;   /* number of the "ground" node of the URC */

    double URClength; /* length of line */
    int URClumps;     /* number of lumps in line */
    unsigned URClenGiven : 1;   /* flag to indicate length was specified */
    unsigned URClumpsGiven : 1; /* flag to indicate lumps was specified */

} URCinstance;

/* per model data */

typedef struct sURCmodel {     /* model structure for a resistor */
    int URCmodType;            /* type index of this device type */
    struct sURCmodel *URCnextModel; /* pointer to next possible model 
                                     * in linked list */
    URCinstance *URCinstances; /* pointer to list of instances that have this
                                * model */
    IFuid URCmodName;   /* pointer to character string naming this model */

    double URCk;        /* propagation constant for URC */
    double URCfmax;     /* max frequence of interest */
    double URCrPerL;    /* resistance per unit length */
    double URCcPerL;    /* capacitance per unit length */
    double URCisPerL;   /* diode saturation current per unit length */
    double URCrsPerL;   /* diode resistance per unit length */
    unsigned URCkGiven : 1;      /* flag to indicate k was specified */
    unsigned URCfmaxGiven : 1;   /* flag to indicate fmax was specified */
    unsigned URCrPerLGiven : 1;  /* flag to indicate rPerL was specified */
    unsigned URCcPerLGiven : 1;  /* flag to indicate cPerL was specified */
    unsigned URCisPerLGiven : 1; /* flag to indicate isPerL was specified */
    unsigned URCrsPerLGiven : 1; /* flag to indicate rsPerL was specified */

} URCmodel;

/* device parameters */
#define URC_LEN      1
#define URC_LUMPS    2
#define URC_POS_NODE 3
#define URC_NEG_NODE 4
#define URC_GND_NODE 5

/* model parameters */
#define URC_MOD_K       101
#define URC_MOD_FMAX    102
#define URC_MOD_RPERL   103
#define URC_MOD_CPERL   104
#define URC_MOD_ISPERL  105
#define URC_MOD_RSPERL  106
#define URC_MOD_URC     107


#ifdef __STDC__

extern int  URCask(CKTcircuit*,GENinstance*,int,IFvalue*,IFvalue*);
extern int  URCmAsk(CKTcircuit*,GENmodel*,int,IFvalue*);
extern int  URCmParam(int,IFvalue*,GENmodel*);
extern int  URCparam(CKTcircuit*,int,IFvalue*,GENinstance*,IFvalue*);
extern void URCparse(int,GENERIC*,GENERIC*,GENERIC*);
extern int  URCsetup(SMPmatrix*,GENmodel*,CKTcircuit*,int*);

#else /* stdc */

extern int  URCask();
extern int  URCmAsk();
extern int  URCmParam();
extern int  URCparam();
extern void URCparse();
extern int  URCsetup();

#endif /* stdc */

#endif /*URC*/
