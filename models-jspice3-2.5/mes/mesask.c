/***************************************************************************
JSPICE3 adaptation of Spice322 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1987 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "mesdefs.h"
#include "tskdefs.h"
#include "sperror.h"
#include "util.h"


/* ARGSUSED */
int
MESask(ckt,inst,which,value,select)

CKTcircuit *ckt;
GENinstance *inst;
int which;
IFvalue *value;
IFvalue *select;
{
    MESinstance *here = (MESinstance*)inst;
    static char *msg = "Current and power not available in ac analysis";

    switch (which) {

        case MES_AREA:
            value->rValue = here->MESarea;
            break;
        case MES_IC_VDS:
            value->rValue = here->MESicVDS;
            break;
        case MES_IC_VGS:
            value->rValue = here->MESicVGS;
            break;
        case MES_OFF:
            value->iValue = here->MESoff;
            break;
        case MES_CS :
            if (ckt->CKTcurrentAnalysis & DOING_AC) {
                errMsg = MALLOC(strlen(msg)+1);
                errRtn = "MESask";
                strcpy(errMsg,msg);
                return (E_ASKCURRENT);
            }
            if (ckt->CKTstate0) {
                 value->rValue = -*(ckt->CKTstate0 + here->MEScd);
                 value->rValue -= *(ckt->CKTstate0 + here->MEScg);
            }
            break;
        case MES_POWER :
            if (ckt->CKTcurrentAnalysis & DOING_AC) {
                errMsg = MALLOC(strlen(msg)+1);
                errRtn = "MESask";
                strcpy(errMsg,msg);
                return (E_ASKPOWER);
            }
            if (ckt->CKTstate0 && ckt->CKTrhsOld) {
                value->rValue = *(ckt->CKTstate0 + here->MEScd) *
                        *(ckt->CKTrhsOld + here->MESdrainNode);
                value->rValue += *(ckt->CKTstate0 + here->MEScg) *
                        *(ckt->CKTrhsOld + here->MESgateNode);
                value->rValue -= (*(ckt->CKTstate0+here->MEScd) +
                        *(ckt->CKTstate0 + here->MEScg)) *
                        *(ckt->CKTrhsOld + here->MESsourceNode);
            }
            break;
        case MES_DRAINNODE:
            value->iValue = here->MESdrainNode;
            break;
        case MES_GATENODE:
            value->iValue = here->MESgateNode;
            break;
        case MES_SOURCENODE:
            value->iValue = here->MESsourceNode;
            break;
        case MES_DRAINPRIMENODE:
            value->iValue = here->MESdrainPrimeNode;
            break;
        case MES_VGS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->MESvgs);
            break;
        case MES_VGD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->MESvgd);
            break;
        case MES_CG:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->MEScg);
            break;
        case MES_CD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->MEScd);
            break;
        case MES_CGD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->MEScgd);
            break;
        case MES_GM:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->MESgm);
            break;
        case MES_GDS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->MESgds);
            break;
        case MES_GGS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->MESggs);
            break;
        case MES_GGD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->MESggd);
            break;
        case MES_QGS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->MESqgs);
            break;
        case MES_CQGS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->MEScqgs);
            break;
        case MES_QGD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->MESqgd);
            break;
        case MES_CQGD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->MEScqgd);
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}
