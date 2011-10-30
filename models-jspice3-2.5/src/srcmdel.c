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
#include "sperror.h"


int
SRCmDelete(modList,modname,killModel)

GENmodel **modList;
IFuid modname;
GENmodel *killModel;
{

    SRCmodel **model = (SRCmodel**)modList;
    SRCmodel *modfast = (SRCmodel*)killModel;
    SRCinstance *here, *next;
    SRCmodel **oldmod;
    oldmod = model;

    for ( ; *model; model = &((*model)->SRCnextModel)) {
        if ( (*model)->SRCmodName == modname || 
                (modfast && *model == modfast) ) break;
        oldmod = model;
    }
    if (!*model) return (E_NOMOD);

    /* cut deleted device out of list */
    *oldmod = (*model)->SRCnextModel;

    for (here = (*model)->SRCinstances; here; here = next) {
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
    FREE(*model);
    return (OK);

}
