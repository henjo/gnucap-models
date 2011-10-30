/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

/*
 * This routine deletes an instance from the circuit and frees
 * the storage it was using.
 */

#include "spice.h"
#include <stdio.h>
#include "ltradefs.h"
#include "util.h"
#include "sperror.h"


int
LTRAdelete(model,name,fast)

GENmodel *model;
IFuid name;
GENinstance **fast;
{
    GENinstance **prev = NULL;
    GENinstance *here;

    for ( ; model; model = model->GENnextModel) {
        prev = &(model->GENinstances);
        for (here = *prev; here; here = *prev) {
            if (here->GENname == name || (fast && here == *fast)) {
                *prev = here->GENnextInstance;
                FREE(((LTRAinstance*)here)->LTRAv1);
                FREE(((LTRAinstance*)here)->LTRAi1);
                FREE(((LTRAinstance*)here)->LTRAv2);
                FREE(((LTRAinstance*)here)->LTRAi2);
                FREE(here);
                return (OK);
            }
            prev = &(here->GENnextInstance);
        }
    }
    return(E_NODEV);
}
