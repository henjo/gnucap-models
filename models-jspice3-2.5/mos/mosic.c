/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1989 Takayasu Sakurai
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "mosdefs.h"
#include "sperror.h"


int
MOSgetic(inModel,ckt)

GENmodel *inModel;
CKTcircuit *ckt;
{
    MOSmodel *model = (MOSmodel *)inModel;
    MOSinstance *here;
    /*
     * grab initial conditions out of rhs array.   User specified, so use
     * external nodes to get values
     */

    for ( ; model; model = model->MOSnextModel) {
        for (here = model->MOSinstances; here;
                here = here->MOSnextInstance) {

            if (!here->MOSicVBSGiven) {
                here->MOSicVBS = 
                        *(ckt->CKTrhs + here->MOSbNode) - 
                        *(ckt->CKTrhs + here->MOSsNode);
            }
            if (!here->MOSicVDSGiven) {
                here->MOSicVDS = 
                        *(ckt->CKTrhs + here->MOSdNode) - 
                        *(ckt->CKTrhs + here->MOSsNode);
            }
            if (!here->MOSicVGSGiven) {
                here->MOSicVGS = 
                        *(ckt->CKTrhs + here->MOSgNode) - 
                        *(ckt->CKTrhs + here->MOSsNode);
            }
        }
    }
    return (OK);
}
