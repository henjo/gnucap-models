/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 S. Hwang
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "mesdefs.h"
#include "const.h"
#include "sperror.h"
#include "util.h"


/* ARGSUSED */
int
MEStemp(inModel,ckt)
    GENmodel *inModel;
    CKTcircuit *ckt;
        /* load the diode structure with those pointers needed later 
         * for fast matrix loading 
         */
{
    register MESmodel *model = (MESmodel*)inModel;
    double xfc, temp;

    /*  loop through all the diode models */
    for( ; model != NULL; model = model->MESnextModel ) {


        if(model->MESdrainResist != 0) {
            model->MESdrainConduct = 1/model->MESdrainResist;
        } else {
            model->MESdrainConduct = 0;
        }
        if(model->MESsourceResist != 0) {
            model->MESsourceConduct = 1/model->MESsourceResist;
        } else {
            model->MESsourceConduct = 0;
        }

        model->MESdepletionCap = model->MESdepletionCapCoeff *
                model->MESgatePotential;
        xfc = (1 - model->MESdepletionCapCoeff);
        temp = sqrt(xfc);
        model->MESf1 = model->MESgatePotential * (1 - temp)/(1-.5);
        model->MESf2 = temp * temp * temp;
        model->MESf3 = 1 - model->MESdepletionCapCoeff * (1 + .5);
        model->MESvcrit = CONSTvt0 * log(CONSTvt0/
                (CONSTroot2 * model->MESgateSatCurrent));

    }
    return(OK);
}
