/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1987 Kanwar Jit Singh
         1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include <math.h>
#include "srcdefs.h"
#include "sperror.h"
#include "util.h"


int
SRCload(inModel,ckt)

GENmodel *inModel;
CKTcircuit *ckt;
{
    SRCmodel *model = (SRCmodel *)inModel;
    SRCinstance *here;

    /*  loop through all the voltage source models */
    for ( ; model != NULL; model = model->SRCnextModel) {

        if (ckt->CKTmode & (MODEDCOP | MODEDCTRANCURVE)) {

            /* loop through all the instances of the model */
            for (here = model->SRCinstances; here != NULL;
                    here = here->SRCnextInstance) {

                (*here->SRCdcFunc)(ckt,here);

            }
        }
        else {
            /* loop through all the instances of the model */
            for (here = model->SRCinstances; here != NULL;
                    here = here->SRCnextInstance) {

                (*here->SRCtranFunc)(ckt,here);

            }
        }
    }
    return (OK);
}
