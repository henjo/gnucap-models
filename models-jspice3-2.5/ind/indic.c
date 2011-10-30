/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors:  1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "inddefs.h"
#include "util.h"
#include "sperror.h"


int
INDgetic(inModel,ckt)

GENmodel *inModel;
CKTcircuit *ckt;
{

    INDmodel *model = (INDmodel*)inModel;
    INDinstance *here;
    /*
     * Assign initial conditions to rhs array.   User specified, so use
     * external nodes to get values
     */

    for ( ; model; model = model->INDnextModel) {
        for (here = model->INDinstances; here;
                here = here->INDnextInstance) {

            if (here->INDicGiven) {
                *(ckt->CKTrhs + here->INDbrEq) = here->INDinitCond;
            }
        }
    }
    return (OK);
}


