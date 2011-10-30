/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/
/********** new in 3f2
Sydney University mods Copyright(c) 1989 Anthony E. Parker, David J. Skellern
    Laboratory for Communication Science Engineering
    Sydney University Department of Electrical Engineering, Australia
**********/

#include "spice.h"
#include <stdio.h>
#include "jfetdefs.h"
#include "sperror.h"
#include "cktext.h"


int
JFETtrunc(inModel,ckt,timeStep)
    GENmodel *inModel;
    register CKTcircuit *ckt;
    double *timeStep;
{
    register JFETmodel *model = (JFETmodel*)inModel;
    register JFETinstance *here;

    for( ; model != NULL; model = model->JFETnextModel) {
        for(here=model->JFETinstances;here!=NULL;here = here->JFETnextInstance){
            CKTterr(here->JFETqgs,ckt,timeStep);
            CKTterr(here->JFETqgd,ckt,timeStep);
        }
    }
    return(OK);
}
