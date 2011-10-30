/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include <math.h>
#include "resdefs.h"
#include "tskdefs.h"
#include "const.h"
#include "sperror.h"
#include "util.h"


/*ARGSUSED*/
int
RESask(ckt,inst,which,value,select)

CKTcircuit *ckt;
GENinstance *inst;
int which;
IFvalue *value;
IFvalue *select;
{
    RESinstance *fast = (RESinstance *)inst;
    static char *msg = "Current and power not available for ac analysis";

    switch (which) {
        case RES_TEMP:
            value->rValue = fast->REStemp-CONSTCtoK;
            break;
        case RES_CONDUCT:
            value->rValue = fast->RESconduct;
            break;
        case RES_RESIST:
            value->rValue = fast->RESresist;
            break;
        case RES_LENGTH:
            value->rValue = fast->RESlength;
            break;
        case RES_WIDTH :
            value->rValue = fast->RESwidth;
            break;
        case RES_CURRENT:
            if (ckt->CKTcurrentAnalysis & DOING_AC) {
                errMsg = MALLOC(strlen(msg)+1);
                errRtn = "RESask";
                strcpy(errMsg,msg);
                return (E_ASKCURRENT);
            }
            if (ckt->CKTrhsOld) {
                value->rValue = (*(ckt->CKTrhsOld + fast->RESposNode) -  
                    *(ckt->CKTrhsOld + fast->RESnegNode))
                    *fast->RESconduct;    
            }
            break;
        case RES_POWER:
            if (ckt->CKTcurrentAnalysis & DOING_AC) {
                errMsg = MALLOC(strlen(msg)+1);
                errRtn = "RESask";
                strcpy(errMsg,msg);
                return (E_ASKPOWER);
            }
            if (ckt->CKTrhsOld) {
                value->rValue = (*(ckt->CKTrhsOld + fast->RESposNode) -  
                    *(ckt->CKTrhsOld + fast->RESnegNode)) * 
                    fast->RESconduct *  
                    (*(ckt->CKTrhsOld + fast->RESposNode) - 
                    *(ckt->CKTrhsOld + fast->RESnegNode));
            }
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}
