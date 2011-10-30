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


/* ARGSUSED */
int
CAPpzLoad(inModel,ckt,s)

GENmodel *inModel;
CKTcircuit *ckt;
SPcomplex *s;
{
    CAPmodel *model = (CAPmodel*)inModel;
    double val;
    CAPinstance *here;

    for ( ; model != NULL; model = model->CAPnextModel) {
        for (here = model->CAPinstances; here != NULL; 
                here = here->CAPnextInstance) {
    
            val = here->CAPcapac;
            *(here->CAPposPosptr   ) += val * s->real;
            *(here->CAPposPosptr +1) += val * s->imag;
            *(here->CAPnegNegptr   ) += val * s->real;
            *(here->CAPnegNegptr +1) += val * s->imag;
            *(here->CAPposNegptr   ) -= val * s->real;
            *(here->CAPposNegptr +1) -= val * s->imag;
            *(here->CAPnegPosptr   ) -= val * s->real;
            *(here->CAPnegPosptr +1) -= val * s->imag;
        }
    }
    return (OK);
}
