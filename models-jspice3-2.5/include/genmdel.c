/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

/*
 * This routine deletes a typed model and its instances from the circuit
 * and frees the storage used.
 */

#include "spice.h"
#include <stdio.h>
#include "gendefs.h"
#include "util.h"
#include "sperror.h"


int
GENmDelete(model,modname,modfast)

GENmodel **model;
IFuid modname;
GENmodel *modfast;
{
    GENinstance *here, *next;
    GENmodel **oldmod;

    oldmod = model;
    for ( ; *model; model = &((*model)->GENnextModel)) {
        if ((*model)->GENmodName == modname || 
                (modfast && *model == modfast)) break;
        oldmod = model;
    }
    if (!*model) return(E_NOMOD);

    /* cut deleted device out of list */
    *oldmod = (*model)->GENnextModel;

    for (here = (*model)->GENinstances; here; here = next) {
        next = here->GENnextInstance;
        FREE(here);
    }
    FREE(*model);
    return (OK);
}
