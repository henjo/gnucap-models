/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "inddefs.h"
#include "sperror.h"
#include "cktext.h"


int
INDtrunc(inModel,ckt,timeStep)

GENmodel *inModel;
CKTcircuit *ckt;
double *timeStep;
{
    INDmodel *model = (INDmodel*)inModel;
    INDinstance *here;

    for ( ; model != NULL; model = model->INDnextModel) {
        for (here = model->INDinstances; here != NULL;
                here = here->INDnextInstance) {
            CKTterr(here->INDflux,ckt,timeStep);
        }
    }
    return (OK);
}
