/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "resdefs.h"
#include "sperror.h"
#include "util.h"


/* ARGSUSED */
int
RESsetup(matrix,inModel,ckt,state)

/* load the resistor structure with those pointers needed later 
 * for fast matrix loading 
 */
SMPmatrix *matrix;
GENmodel *inModel;
CKTcircuit*ckt;
int *state;
{
    RESmodel *model = (RESmodel *)inModel;
    RESinstance *here;

    /*  loop through all the resistor models */
    for ( ; model != NULL; model = model->RESnextModel) {

        /* loop through all the instances of the model */
        for (here = model->RESinstances; here != NULL;
                here = here->RESnextInstance) {
            
            TSTALLOC(RESposPosptr, RESposNode, RESposNode);
            TSTALLOC(RESnegNegptr, RESnegNode, RESnegNode);
            TSTALLOC(RESposNegptr, RESposNode, RESnegNode);
            TSTALLOC(RESnegPosptr, RESnegNode, RESposNode);
        }
    }
    return(OK);
}
