/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "capdefs.h"
#include "sperror.h"
#include "cktext.h"


int
CAPtrunc(inModel,ckt,timeStep)

GENmodel *inModel;
CKTcircuit *ckt;
double *timeStep;
{
    CAPmodel *model = (CAPmodel*)inModel;
    CAPinstance *here;

    for ( ; model!= NULL; model = model->CAPnextModel) {
        for (here = model->CAPinstances; here != NULL;
                here = here->CAPnextInstance) {

            CKTterr(here->CAPqcap,ckt,timeStep);
        }
    }
    return (OK);
}
