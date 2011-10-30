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
CAPtemp(inModel,ckt)

GENmodel *inModel;
CKTcircuit *ckt;
{
    CAPmodel *model = (CAPmodel*)inModel;
    CAPinstance *here;

    /*  loop through all the capacitor models */
    for ( ; model != NULL; model = model->CAPnextModel) {

        /* loop through all the instances of the model */
        for (here = model->CAPinstances; here != NULL;
                here = here->CAPnextInstance) {

            /* Default Value Processing for Capacitor Instance */
            if (!here->CAPwidthGiven) {
                here->CAPwidth = model->CAPdefWidth;
            }
            if (!here->CAPcapGiven) {
                here->CAPcapac = 
                    model->CAPcj * 
                        (here->CAPwidth - model->CAPnarrow) * 
                        (here->CAPlength - model->CAPnarrow) + 
                    model->CAPcjsw * 2 * (
                        (here->CAPlength - model->CAPnarrow) +
                        (here->CAPwidth - model->CAPnarrow) );
            }
        }
    }
    return (OK);
}

