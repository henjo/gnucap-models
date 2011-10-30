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


int
CAPacLoad(inModel,ckt)

GENmodel *inModel;
CKTcircuit *ckt;
{
    CAPmodel *model = (CAPmodel*)inModel;
    double val;
    CAPinstance *here;

    for ( ; model != NULL; model = model->CAPnextModel) {
        for (here = model->CAPinstances; here != NULL; 
                here = here->CAPnextInstance) {
    
            val = ckt->CKTomega * here->CAPcapac;
            *(here->CAPposPosptr +1) += val;
            *(here->CAPnegNegptr +1) += val;
            *(here->CAPposNegptr +1) -= val;
            *(here->CAPnegPosptr +1) -= val;
        }
    }
    return (OK);
}

