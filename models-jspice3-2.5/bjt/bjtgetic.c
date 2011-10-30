/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

/*
 * This routine gets the device initial conditions for the BJTs
 * from the RHS vector
 */

#include "spice.h"
#include <stdio.h>
#include "bjtdefs.h"
#include "sperror.h"


int
BJTgetic(inModel,ckt)

GENmodel *inModel;
CKTcircuit *ckt;
{

    BJTmodel *model = (BJTmodel*)inModel;
    BJTinstance *here;
    /*
     * grab initial conditions out of rhs array.   User specified, so use
     * external nodes to get values
     */

    for ( ; model; model = model->BJTnextModel) {
        for (here = model->BJTinstances; here;
                here = here->BJTnextInstance) {
            if (!here->BJTicVBEGiven) {
                here->BJTicVBE = 
                        *(ckt->CKTrhs + here->BJTbaseNode) - 
                        *(ckt->CKTrhs + here->BJTemitNode);
            }
            if (!here->BJTicVCEGiven) {
                here->BJTicVCE = 
                        *(ckt->CKTrhs + here->BJTcolNode) - 
                        *(ckt->CKTrhs + here->BJTemitNode);
            }
        }
    }
    return (OK);
}
