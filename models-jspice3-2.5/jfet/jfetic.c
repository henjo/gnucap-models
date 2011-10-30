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


int
JFETgetic(inModel,ckt)
    GENmodel *inModel;
    CKTcircuit *ckt;
{
    JFETmodel *model = (JFETmodel*)inModel;
    JFETinstance *here;
    /*
     * grab initial conditions out of rhs array.   User specified, so use
     * external nodes to get values
     */

    for( ; model ; model = model->JFETnextModel) {
        for(here = model->JFETinstances; here ; here = here->JFETnextInstance) {
            if(!here->JFETicVDSGiven) {
                here->JFETicVDS = 
                        *(ckt->CKTrhs + here->JFETdrainNode) - 
                        *(ckt->CKTrhs + here->JFETsourceNode);
            }
            if(!here->JFETicVGSGiven) {
                here->JFETicVGS = 
                        *(ckt->CKTrhs + here->JFETgateNode) - 
                        *(ckt->CKTrhs + here->JFETsourceNode);
            }
        }
    }
    return(OK);
}
