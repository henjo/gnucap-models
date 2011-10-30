/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

/*
 * This routine deletes all typed models and instances from the circuit
 * and frees the storage they were using.
 */

#include "spice.h"
#include <stdio.h>
#include "gendefs.h"
#include "util.h"


void
GENdestroy(model)

GENmodel **model;
{

    GENinstance *here, *next;
    GENmodel *mod, *nextmod;

    for (mod = *model; mod; mod = nextmod) {
        nextmod = mod->GENnextModel;
        for (here = mod->GENinstances; here; here = next) {
            next = here->GENnextInstance;
            FREE(here);
        }
        FREE(mod);
    }
    *model = NULL;
}
