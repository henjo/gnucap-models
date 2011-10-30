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
#include "util.h"


/*ARGSUSED*/
int
CAPsetup(matrix,inModel,ckt,states)

/* load the capacitor structure with those pointers needed later 
 * for fast matrix loading 
 */
SMPmatrix *matrix;
GENmodel *inModel;
CKTcircuit *ckt;
int *states;
{
    CAPmodel *model = (CAPmodel*)inModel;
    CAPinstance *here;

    /*  loop through all the capacitor models */
    for ( ; model != NULL; model = model->CAPnextModel) {

        /*Default Value Processing for Model Parameters */
        if (!model->CAPcjGiven) {
            model->CAPcj = 0;
        }
        if (!model->CAPcjswGiven){
             model->CAPcjsw = 0;
        }
        if (!model->CAPdefWidthGiven) {
            model->CAPdefWidth = 10.e-6;
        }
        if (!model->CAPnarrowGiven) {
            model->CAPnarrow = 0;
        }

        /* loop through all the instances of the model */
        for (here = model->CAPinstances; here != NULL;
                here = here->CAPnextInstance) {

            /* Default Value Processing for Capacitor Instance */
            if (!here->CAPlengthGiven) {
                here->CAPlength = 0;
            }
            TSTALLOC(CAPposPosptr,CAPposNode,CAPposNode)
            TSTALLOC(CAPnegNegptr,CAPnegNode,CAPnegNode)
            TSTALLOC(CAPposNegptr,CAPposNode,CAPnegNode)
            TSTALLOC(CAPnegPosptr,CAPnegNode,CAPposNode)
            here->CAPqcap = *states;
            *states += 2;
        }
    }
    return (OK);
}

