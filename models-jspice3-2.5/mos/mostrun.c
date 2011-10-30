/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "mosdefs.h"
#include "sperror.h"
#include "cktext.h"


int
MOStrunc(inModel,ckt,timeStep)

GENmodel *inModel;
CKTcircuit *ckt;
double *timeStep;
{
    MOSmodel *model = (MOSmodel *)inModel;
    MOSinstance *here;

    for ( ; model != NULL; model = model->MOSnextModel) {
        for (here = model->MOSinstances; here!=NULL;
                here = here->MOSnextInstance) {

            CKTterr(here->MOSqgs,ckt,timeStep);
            CKTterr(here->MOSqgd,ckt,timeStep);
            CKTterr(here->MOSqgb,ckt,timeStep);
        }
    }
    return (OK);
}
