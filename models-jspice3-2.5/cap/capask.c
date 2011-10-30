/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include <math.h>
#include "capdefs.h"
#include "tskdefs.h"
#include "sperror.h"
#include "util.h"


/* ARGSUSED */
int
CAPask(ckt,inst, which, value, select)

CKTcircuit *ckt;
GENinstance *inst;
int which;
IFvalue *value;  
IFvalue *select;  
{
    CAPinstance *here = (CAPinstance *)inst;
    static char *msg = "Current and power not available for ac analysis";

    switch (which) {

        case CAP_CAP:
            value->rValue = here->CAPcapac;
            break;
        case CAP_IC:
            value->rValue = here->CAPinitCond;
            break;
        case CAP_WIDTH:
            value->rValue = here->CAPwidth;
            break;
        case CAP_LENGTH:
            value->rValue = here->CAPlength;
            break;
        case CAP_CURRENT:
            if (ckt->CKTcurrentAnalysis & DOING_AC) {
                errMsg = MALLOC(strlen(msg)+1);
                errRtn = "CAPask";
                strcpy(errMsg,msg);
                return (E_ASKCURRENT);
            }
            if (ckt->CKTcurrentAnalysis & DOING_TRAN) {
                if (!(ckt->CKTmode & MODETRANOP)) {
                    if (ckt->CKTstate0)
                        value->rValue = *(ckt->CKTstate0 + here->CAPccap);
                }
            }
            break;
        case CAP_POWER:
            if (ckt->CKTcurrentAnalysis & DOING_AC) {
                errMsg = MALLOC(strlen(msg)+1);
                errRtn = "CAPask";
                strcpy(errMsg,msg);
                return (E_ASKPOWER);
            }
            if (ckt->CKTcurrentAnalysis & DOING_TRAN) {
                if (!(ckt->CKTmode & MODETRANOP)) {
                    if (ckt->CKTstate0 && ckt->CKTrhsOld)
                        value->rValue = *(ckt->CKTstate0 + here->CAPccap) *
                            (*(ckt->CKTrhsOld + here->CAPposNode) - 
                            *(ckt->CKTrhsOld + here->CAPnegNode));
                }
            }
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}

