/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1987 Kanwar Jit Singh
         1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "srcdefs.h"
#include "util.h"


void
SRCdestroy(inModel)

GENmodel **inModel;
{
    SRCmodel **model = (SRCmodel **)inModel;
    SRCinstance *here, *next;
    SRCmodel *mod, *nextmod;

    for (mod = *model; mod; mod = nextmod) {
        nextmod = mod->SRCnextModel;
        for (here = mod->SRCinstances; here; here = next) {
            next = here->SRCnextInstance;
            FREE(here->SRCvalues);
            FREE(here->SRCderivs);
            FREE(here->SRCeqns);
            FREE(here->SRCacValues);
            FREE(here->SRCposptr);
            if (here->SRCtree)
                (*here->SRCtree->IFfree)(here->SRCtree);
            FREE(here);
        }
        FREE(mod);
    }
    *model = NULL;
}
