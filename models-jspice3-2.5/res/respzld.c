/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "resdefs.h"
#include "sperror.h"


/*ARGSUSED*/
int
RESpzLoad(inModel,ckt,s)

GENmodel *inModel;
CKTcircuit *ckt;
SPcomplex *s;
/* actually load the current resistance value into the 
 * sparse matrix previously provided 
 */
{
    RESmodel *model = (RESmodel *)inModel;
    RESinstance *here;

    /* loop through all the resistor models */
    for ( ; model != NULL; model = model->RESnextModel) {

        /* loop through all the instances of the model */
        for (here = model->RESinstances; here != NULL ;
                here = here->RESnextInstance) {
            
            *(here->RESposPosptr) += here->RESconduct;
            *(here->RESnegNegptr) += here->RESconduct;
            *(here->RESposNegptr) -= here->RESconduct;
            *(here->RESnegPosptr) -= here->RESconduct;
        }
    }
    return (OK);
}
