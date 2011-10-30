/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include <math.h>
#include "diodefs.h"
#include "tskdefs.h"
#include "sperror.h"
#include "const.h"
#include "util.h"


/* ARGSUSED */
int
DIOask (ckt,inst,which,value,select)

CKTcircuit *ckt;
GENinstance *inst;
int which;
IFvalue *value;
IFvalue *select;
{
    DIOinstance *here = (DIOinstance*)inst;
    static char *msg = "Current and power not available for ac analysis";

    switch (which) {
        case DIO_AREA:
            value->rValue = here->DIOarea;
            break;
        case DIO_IC:
            value->rValue = here->DIOinitCond;
            break;
        case DIO_OFF:
            value->iValue = here->DIOoff;
            break;
        case DIO_CURRENT:
            if (ckt->CKTstate0)
                value->rValue = here->DIOcd;
            break;
        case DIO_VOLTAGE:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->DIOvoltage);
            break;
        case DIO_CHARGE: 
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->DIOcapCharge);
            break;
        case DIO_CAPCUR:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->DIOcapCurrent);
            break;
        case DIO_CONDUCT:
            if (ckt->CKTstate0)
                value->rValue = here->DIOgd;
            break;
        case DIO_POWER :
            if (ckt->CKTcurrentAnalysis & DOING_AC) {
                errMsg = MALLOC(strlen(msg)+1);
                errRtn = "DIOask";
                strcpy(errMsg,msg);
                return (E_ASKPOWER);
            }
            if (ckt->CKTstate0) {
                value->rValue = here->DIOcd *
                        *(ckt->CKTstate0 + here->DIOvoltage);
            }
            break;
        case DIO_TEMP:
            value->rValue = here->DIOtemp - CONSTCtoK;
            break;
        /* new in 3f2 */
        case DIO_CAP: 
            value->rValue = here->DIOcap;
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}  

