/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 S. Hwang
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "mesdefs.h"
#include "sperror.h"
#include "cktext.h"


int
MEStrunc(inModel,ckt,timeStep)
    GENmodel *inModel;
    register CKTcircuit *ckt;
    double *timeStep;
{
    register MESmodel *model = (MESmodel*)inModel;
    register MESinstance *here;

    for( ; model != NULL; model = model->MESnextModel) {
        for(here=model->MESinstances;here!=NULL;here = here->MESnextInstance){
            CKTterr(here->MESqgs,ckt,timeStep);
            CKTterr(here->MESqgd,ckt,timeStep);
        }
    }
    return(OK);
}
