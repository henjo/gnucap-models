/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include <math.h>
#include "inddefs.h"
#include "tskdefs.h"
#include "sperror.h"
#include "util.h"


/*ARGSUSED*/
int
INDask(ckt,inst,which,value,select)

CKTcircuit *ckt;
GENinstance *inst;
int which;
IFvalue *value;
IFvalue *select;
{
    INDinstance *here = (INDinstance*)inst;
    static char *msg = "Current and power not available for ac analysis";

    switch (which) {

        case IND_FLUX:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0+here->INDflux);
            break;
        case IND_VOLT:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0+here->INDvolt);
            break;
        case IND_IND:
            value->rValue = here->INDinduct;
            break;
        case IND_IC:    
            value->rValue = here->INDinitCond;
            break;
        case IND_CURRENT :
            if (ckt->CKTcurrentAnalysis & DOING_AC) {
                errMsg = MALLOC(strlen(msg)+1);
                errRtn = "INDask";
                strcpy(errMsg,msg);
                return (E_ASKCURRENT);
            }
            if (ckt->CKTrhsOld)
                value->rValue = *(ckt->CKTrhsOld + here->INDbrEq);
            break;
        case IND_POWER :
            if (ckt->CKTcurrentAnalysis & DOING_AC) {
                errMsg = MALLOC(strlen(msg)+1);
                errRtn = "INDask";
                strcpy(errMsg,msg);
                return (E_ASKPOWER);
            }
            if (ckt->CKTstate0 && ckt->CKTrhsOld)
                value->rValue = *(ckt->CKTrhsOld + here->INDbrEq) *
                    *(ckt->CKTstate0+here->INDvolt);
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}
