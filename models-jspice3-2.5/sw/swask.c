/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1987 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

/*
 * This routine gives access to the internal device parameters
 * of voltage controlled SWitch
 */

#include "spice.h"
#include <stdio.h>
#include "swdefs.h"
#include "tskdefs.h"
#include "sperror.h"
#include "util.h"


/* ARGSUSED */
int
SWask(ckt,inst,which,value,select)

CKTcircuit *ckt;
GENinstance *inst;
int which;
IFvalue *value;
IFvalue *select;
{
    SWinstance *here = (SWinstance *)inst;
    static char *msg = "Current and power not available in ac analysis";

    switch (which) {

        case SW_POS_NODE:
            value->iValue = here->SWposNode;
            break;
        case SW_NEG_NODE:
            value->iValue = here->SWnegNode;
            break;
        case SW_CONTROL:
            value->uValue = here->SWcontName;
            break;
        case SW_POS_CONT_NODE:
            value->iValue = here->SWposCntrlNode;
            break;
        case SW_NEG_CONT_NODE:
            value->iValue = here->SWnegCntrlNode;
            break;
        case SW_CURRENT:
            if (ckt->CKTcurrentAnalysis & DOING_AC) {
                errMsg = MALLOC(strlen(msg)+1);
                errRtn = "SWask";
                strcpy(errMsg,msg);
                return (E_ASKCURRENT);
            }
            if (ckt->CKTrhsOld) {
                value->rValue = (*(ckt->CKTrhsOld + here->SWposNode)
                        - *(ckt->CKTrhsOld + here->SWnegNode)) *
                        here->SWcond;
            }
            break;
        case SW_POWER:
            if (ckt->CKTcurrentAnalysis & DOING_AC) {
                errMsg = MALLOC(strlen(msg)+1);
                errRtn = "SWask";
                strcpy(errMsg,msg);
                return (E_ASKPOWER);
            }
            if (ckt->CKTrhsOld) {
                value->rValue = (*(ckt->CKTrhsOld + here->SWposNode)
                        - *(ckt->CKTrhsOld + here->SWnegNode)) *
                        (*(ckt->CKTrhsOld + here->SWposNode)
                        - *(ckt->CKTrhsOld + here->SWnegNode)) *
                        here->SWcond;
            }
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}
