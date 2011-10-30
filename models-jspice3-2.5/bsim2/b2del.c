/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Hong June Park, Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "bsim2def.h"
#include "sperror.h"
#include "util.h"


int
B2delete(inModel,name,inInst)
    GENmodel *inModel;
    IFuid name;
    GENinstance **inInst;

{

    B2instance **fast = (B2instance**)inInst;
    B2model *model = (B2model*)inModel;
    B2instance **prev = NULL;
    B2instance *here;

    for( ; model ; model = model->B2nextModel) {
        prev = &(model->B2instances);
        for(here = *prev; here ; here = *prev) {
            if(here->B2name == name || (fast && here==*fast) ) {
                *prev= here->B2nextInstance;
                FREE(here);
                return(OK);
            }
            prev = &(here->B2nextInstance);
        }
    }
    return(E_NODEV);
}
