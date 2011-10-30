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
SRCdelete(model,name,fast)

GENmodel *model;
IFuid name;
GENinstance **fast;
{
    SRCinstance **instPtr = (SRCinstance**)fast;
    SRCmodel *modPtr = (SRCmodel*)model;
    SRCinstance **prev = NULL;
    SRCinstance *here;

    for ( ; modPtr; modPtr = modPtr->SRCnextModel) {
        prev = &(modPtr->SRCinstances);
        for (here = *prev; here; here = *prev) {
            if (here->SRCname == name || (instPtr && here == *instPtr) ) {
                *prev = here->SRCnextInstance;
                FREE(here->SRCvalues);
                FREE(here->SRCderivs);
                FREE(here->SRCeqns);
                FREE(here->SRCacValues);
                FREE(here->SRCposptr);
                if (here->SRCtree)
                    (*here->SRCtree->IFfree)(here->SRCtree);
                FREE(here);
                return (OK);
            }
            prev = &(here->SRCnextInstance);
        }
    }
    return (E_NODEV);
}
