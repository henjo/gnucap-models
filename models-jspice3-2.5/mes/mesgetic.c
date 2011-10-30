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
#include "util.h"


int
MESgetic(inModel,ckt)
    GENmodel *inModel;
    CKTcircuit *ckt;
{
    MESmodel *model = (MESmodel*)inModel;
    MESinstance *here;
    /*
     * grab initial conditions out of rhs array.   User specified, so use
     * external nodes to get values
     */

    for( ; model ; model = model->MESnextModel) {
        for(here = model->MESinstances; here ; here = here->MESnextInstance) {
            if(!here->MESicVDSGiven) {
                here->MESicVDS = 
                        *(ckt->CKTrhs + here->MESdrainNode) - 
                        *(ckt->CKTrhs + here->MESsourceNode);
            }
            if(!here->MESicVGSGiven) {
                here->MESicVGS = 
                        *(ckt->CKTrhs + here->MESgateNode) - 
                        *(ckt->CKTrhs + here->MESsourceNode);
            }
        }
    }
    return(OK);
}
