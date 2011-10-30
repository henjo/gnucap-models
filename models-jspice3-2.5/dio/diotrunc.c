/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "diodefs.h"
#include "sperror.h"
#include "cktext.h"


int
DIOtrunc(inModel,ckt,timeStep)

GENmodel *inModel;
CKTcircuit *ckt;
double *timeStep;
{
    DIOmodel *model = (DIOmodel*)inModel;
    DIOinstance *here;

    for ( ; model != NULL; model = model->DIOnextModel) {
        for (here = model->DIOinstances; here != NULL;
                here = here->DIOnextInstance) {

            CKTterr(here->DIOcapCharge,ckt,timeStep);
        }
    }
    return (OK);
}
