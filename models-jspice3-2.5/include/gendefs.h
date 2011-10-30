/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

/* IMPORTANT:  The the first four items in the data structures defined
 * in this file must match the heads of the model and instance structures
 * defined for the devices.
 */


#ifndef GEN
#define GEN


#include "ifsim.h"

        /* definitions used to describe generic devices */

/* information used to describe a single instance */

typedef struct sGENinstance {
    struct sGENmodel *GENmodPtr;    /* backpointer to model */
    struct sGENinstance *GENnextInstance;   /* pointer to next instance of 
                                             * current model*/
    IFuid GENname;  /* pointer to character string naming this instance */
    int GENstate;   /* state index number */
    int GENnode1;   /* appropriate node numbers */
    int GENnode2;   /* appropriate node numbers */
    int GENnode3;   /* appropriate node numbers */
    int GENnode4;   /* appropriate node numbers */
    int GENnode5;   /* appropriate node numbers */
} GENinstance;

/* per model data */

typedef struct sGENmodel {     /* model structure for a resistor */
    int GENmodType;            /* type index of this device type */
    struct sGENmodel *GENnextModel; /* pointer to next possible model in 
                                     * linked list */
    GENinstance *GENinstances; /* pointer to list of instances that have this
                                * model */
    IFuid GENmodName;     /* pointer to character string naming this model */
} GENmodel;

#endif /*GEN*/
