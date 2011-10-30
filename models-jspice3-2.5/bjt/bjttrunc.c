/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

/*
 * This routine performs truncation error calculations for
 * BJTs in the circuit.
 */

#include "spice.h"
#include <stdio.h>
#include "bjtdefs.h"
#include "sperror.h"
#include "cktext.h"


int
BJTtrunc(inModel,ckt,timeStep)

GENmodel *inModel;
CKTcircuit *ckt;
double *timeStep;
{
    BJTmodel *model = (BJTmodel*)inModel;
    BJTinstance *here;

    for ( ; model != NULL; model = model->BJTnextModel) {
        for (here = model->BJTinstances; here != NULL;
                here = here->BJTnextInstance) {
            CKTterr(here->BJTqbe,ckt,timeStep);
            CKTterr(here->BJTqbc,ckt,timeStep);
            CKTterr(here->BJTqcs,ckt,timeStep);
        }
    }
    return (OK);
}
