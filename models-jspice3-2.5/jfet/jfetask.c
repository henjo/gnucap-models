/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1987 Mathew Lew and Thomas L. Quarles
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
#include "tskdefs.h"
#include "sperror.h"
#include "const.h"
#include "util.h"


/*ARGSUSED*/
int
JFETask(ckt,inst,which,value,select)

CKTcircuit *ckt;
GENinstance *inst;
int which;
IFvalue *value;
IFvalue *select;
{
    JFETinstance *here = (JFETinstance*)inst;
    static char *msg = "Current and power not available for ac analysis";

    switch (which) {

        case JFET_AREA:
            value->rValue = here->JFETarea;
            break;
        case JFET_IC_VDS:
            value->rValue = here->JFETicVDS;
            break;
        case JFET_IC_VGS:
            value->rValue = here->JFETicVGS;
            break;
        case JFET_OFF:
            value->iValue = here->JFEToff;
            break;
        case JFET_TEMP:
            value->rValue = here->JFETtemp-CONSTCtoK;
            break;
        case JFET_DRAINNODE:
            value->iValue = here->JFETdrainNode;
            break;
        case JFET_GATENODE:
            value->iValue = here->JFETgateNode;
            break;
        case JFET_SOURCENODE:
            value->iValue = here->JFETsourceNode;
            break;
        case JFET_DRAINPRIMENODE:
            value->iValue = here->JFETdrainPrimeNode;
            break;
        case JFET_SOURCEPRIMENODE:
            value->iValue = here->JFETsourcePrimeNode;
            break;
        case JFET_VGS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->JFETvgs);
            break;
        case JFET_VGD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->JFETvgd);
            break;
        case JFET_CG:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->JFETcg);
            break;
        case JFET_CD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->JFETcd);
            break;
        case JFET_CGD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->JFETcgd);
            break;
        case JFET_GM:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->JFETgm);
            break;
        case JFET_GDS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->JFETgds);
            break;
        case JFET_GGS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->JFETggs);
            break;
        case JFET_GGD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->JFETggd);
            break;
        case JFET_QGS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->JFETqgs);
            break;
        case JFET_CQGS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->JFETcqgs);
            break;
        case JFET_QGD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->JFETqgd);
            break;
        case JFET_CQGD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->JFETcqgd);
            break;
        case JFET_CS :
            if (ckt->CKTcurrentAnalysis & DOING_AC) {
                errMsg = MALLOC(strlen(msg)+1);
                errRtn = "JFETask";
                strcpy(errMsg,msg);
                return (E_ASKCURRENT);
            }
            if (ckt->CKTstate0) {
                value->rValue = -*(ckt->CKTstate0 + here->JFETcd);
                value->rValue -= *(ckt->CKTstate0 + here->JFETcg);
            }
            break;
        case JFET_POWER :
            if (ckt->CKTcurrentAnalysis & DOING_AC) {
                errMsg = MALLOC(strlen(msg)+1);
                errRtn = "JFETask";
                strcpy(errMsg,msg);
                return (E_ASKPOWER);
            }
            if (ckt->CKTstate0 && ckt->CKTrhsOld) {
                value->rValue = *(ckt->CKTstate0 + here->JFETcd) *
                        *(ckt->CKTrhsOld + here->JFETdrainNode);
                value->rValue += *(ckt->CKTstate0 + here->JFETcg) * 
                        *(ckt->CKTrhsOld + here->JFETgateNode);
                value->rValue -= (*(ckt->CKTstate0 + here->JFETcd) +
                        *(ckt->CKTstate0 + here->JFETcg)) *
                *(ckt->CKTrhsOld + here->JFETsourceNode);
            }
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}

