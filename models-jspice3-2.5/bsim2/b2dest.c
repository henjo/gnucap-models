/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Hong June Park, Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "bsim2def.h"
#include "util.h"


void
B2destroy(inModel)
    GENmodel **inModel;

{

    B2model **model = (B2model**)inModel;
    B2instance *here;
    B2instance *prev = NULL;
    B2model *mod = *model;
    B2model *oldmod = NULL;

    for( ; mod ; mod = mod->B2nextModel) {
        if(oldmod) FREE(oldmod);
        oldmod = mod;
        prev = (B2instance *)NULL;
        for(here = mod->B2instances ; here ; here = here->B2nextInstance) {
            if(prev) FREE(prev);
            prev = here;
        }
        if(prev) FREE(prev);
    }
    if(oldmod) FREE(oldmod);
    *model = NULL;
}

