/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1987 Mathew Lew and Thomas L. Quarles
         1989 Takayasu Sakurai
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include <math.h>
#include "mosdefs.h"
#include "tskdefs.h"
#include "const.h"
#include "sperror.h"
#include "util.h"


/*ARGSUSED*/
int
MOSask(ckt,inst,which,value,select)

CKTcircuit *ckt;
GENinstance *inst;
int which;
IFvalue *value;
IFvalue *select;
{
    MOSinstance *here = (MOSinstance *)inst;
    static char *msg = "Current and power not available for ac analysis";

    switch (which) {

        case MOS_TEMP:
            value->rValue = here->MOStemp-CONSTCtoK;
            break;
        case MOS_L:
            value->rValue = here->MOSl;
            break;
        case MOS_W:
            value->rValue = here->MOSw;
            break;
        case MOS_AD:
            value->rValue = here->MOSdrainArea;
            break;
        case MOS_AS:
            value->rValue = here->MOSsourceArea;
            break;
        case MOS_PD:
            value->rValue = here->MOSdrainPerimeter;
            break;
        case MOS_PS:
            value->rValue = here->MOSsourcePerimeter;
            break;
        case MOS_NRD:
            value->rValue = here->MOSdrainSquares;
            break;
        case MOS_NRS:
            value->rValue = here->MOSsourceSquares;
            break;
        case MOS_OFF:
            value->rValue = here->MOSoff;
            break;
        case MOS_IC_VDS:
            value->rValue = here->MOSicVDS;
            break;
        case MOS_IC_VGS:
            value->rValue = here->MOSicVGS;
            break;
        case MOS_IC_VBS:
            value->rValue = here->MOSicVBS;
            break;
        case MOS_CGD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->MOScapgd);
            break;
        case MOS_CGS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->MOScapgs);
            break;
        case MOS_DNODE:
            value->iValue = here->MOSdNode;
            break;
        case MOS_GNODE:
            value->iValue = here->MOSgNode;
            break;
        case MOS_SNODE:
            value->iValue = here->MOSsNode;
            break;
        case MOS_BNODE:
            value->iValue = here->MOSbNode;
            break;
        case MOS_DNODEPRIME:
            value->iValue = here->MOSdNodePrime;
            break;
        case MOS_SNODEPRIME:
            value->iValue = here->MOSsNodePrime;
            break;
        case MOS_DRAINCONDUCT:
            value->rValue = here->MOSdrainConductance;
            break;
        case MOS_SOURCECONDUCT:
            value->rValue = here->MOSsourceConductance;
            break;
        case MOS_VON:
            value->rValue = here->MOSvon;
            break;
        case MOS_VDSAT:
            value->rValue = here->MOSvdsat;
            break;
        case MOS_DRAINVCRIT:
            value->rValue = here->MOSdrainVcrit;
            break;
        case MOS_SOURCEVCRIT:
            value->rValue = here->MOSsourceVcrit;
            break;
        case MOS_CD:
            value->rValue = here->MOScd;
            break;
        case MOS_CBD:
            value->rValue = here->MOScbd;
            break;
        case MOS_CBS:
            value->rValue = here->MOScbs;
            break;
        case MOS_GMBS:
            value->rValue = here->MOSgmbs;
            break;
        case MOS_GM:
            value->rValue = here->MOSgm;
            break;
        case MOS_GDS:
            value->rValue = here->MOSgds;
            break;
        case MOS_GBD:
            value->rValue = here->MOSgbd;
            break;
        case MOS_GBS:
            value->rValue = here->MOSgbs;
            break;
        case MOS_CAPBD:
            value->rValue = here->MOScapbd;
            break;
        case MOS_CAPBS:
            value->rValue = here->MOScapbs;
            break;
        case MOS_CAPZEROBIASBD:
            value->rValue = here->MOSCbd;
            break;
        case MOS_CAPZEROBIASBDSW:
            value->rValue = here->MOSCbdsw;
            break;
        case MOS_CAPZEROBIASBS:
            value->rValue = here->MOSCbs;
            break;
        case MOS_CAPZEROBIASBSSW:
            value->rValue = here->MOSCbssw;
            break;
        case MOS_VBD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->MOSvbd);
            break;
        case MOS_VBS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->MOSvbs);
            break;
        case MOS_VGS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->MOSvgs);
            break;
        case MOS_VDS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->MOSvds);
            break;
        case MOS_CAPGD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->MOScapgd);
            break;
        case MOS_QGD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->MOSqgd);
            break;
        case MOS_CQGD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->MOScqgd);
            break;
        case MOS_CAPGS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->MOScapgs);
            break;
        case MOS_QGS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->MOSqgs);
            break;
        case MOS_CQGS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->MOScqgs);
            break;
        case MOS_CAPGB:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->MOScapgb);
            break;
        case MOS_QGB:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->MOSqgb);
            break;
        case MOS_CQGB:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->MOScqgb);
            break;
        case MOS_QBD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->MOSqbd);
            break;
        case MOS_CQBD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->MOScqbd);
            break;
        case MOS_QBS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->MOSqbs);
            break;
        case MOS_CQBS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->MOScqbs);
            break;
        case MOS_CG:
            if (ckt->CKTcurrentAnalysis & DOING_AC) {
                errMsg = MALLOC(strlen(msg)+1);
                errRtn = "MOSask.c";
                strcpy(errMsg,msg);
                return (E_ASKCURRENT);
            }
            if (ckt->CKTcurrentAnalysis & DOING_TRAN) {
                if (!(ckt->CKTmode & MODETRANOP)) {
                    if (ckt->CKTstate0)
                        value->rValue =
                            *(ckt->CKTstate0 + here->MOScqgb) +
                            *(ckt->CKTstate0 + here->MOScqgd) +
                            *(ckt->CKTstate0 + here->MOScqgs);
                }
            }
            break;
        case MOS_CS:
            if (ckt->CKTcurrentAnalysis & DOING_AC) {
                errMsg = MALLOC(strlen(msg)+1);
                errRtn = "MOSask.c";
                strcpy(errMsg,msg);
                return (E_ASKCURRENT);
            }
            if (ckt->CKTstate0) {
                value->rValue = -here->MOScd;
                value->rValue -= here->MOScbd + here->MOScbs -
                        *(ckt->CKTstate0 + here->MOScqgb);
                if ((ckt->CKTcurrentAnalysis & DOING_TRAN) && 
                        !(ckt->CKTmode & MODETRANOP)) {
                    value->rValue -= *(ckt->CKTstate0 + here->MOScqgb) + 
                            *(ckt->CKTstate0 + here->MOScqgd) + 
                            *(ckt->CKTstate0 + here->MOScqgs);
                }
            }
            break;
        case MOS_CB:
            if (ckt->CKTcurrentAnalysis & DOING_AC) {
                errMsg = MALLOC(strlen(msg)+1);
                errRtn = "MOSask.c";
                strcpy(errMsg,msg);
                return (E_ASKCURRENT);
            }
            if (ckt->CKTstate0) {
                value->rValue = here->MOScbd + here->MOScbs -
                    *(ckt->CKTstate0 + here->MOScqgb);
            }
            break;
        case MOS_POWER:
            if (ckt->CKTcurrentAnalysis & DOING_AC) {
                errMsg = MALLOC(strlen(msg)+1); 
                errRtn = "MOSask.c";
                strcpy(errMsg,msg);
                return (E_ASKPOWER);
            }
            if (ckt->CKTstate0 && ckt->CKTrhsOld) {
                double temp;

                value->rValue = here->MOScd * 
                        *(ckt->CKTrhsOld + here->MOSdNode);
                value->rValue += (here->MOScbd + here->MOScbs -
                        *(ckt->CKTstate0 + here->MOScqgb)) * 
                        *(ckt->CKTrhsOld + here->MOSbNode);
                if ((ckt->CKTcurrentAnalysis & DOING_TRAN) && 
                        !(ckt->CKTmode & MODETRANOP)) {
                    value->rValue += (*(ckt->CKTstate0 + here->MOScqgb) + 
                            *(ckt->CKTstate0 + here->MOScqgd) + 
                            *(ckt->CKTstate0 + here->MOScqgs)) *
                            *(ckt->CKTrhsOld + here->MOSgNode);
                }
                temp = -here->MOScd;
                temp -= here->MOScbd + here->MOScbs ;
                if ((ckt->CKTcurrentAnalysis & DOING_TRAN) && 
                        !(ckt->CKTmode & MODETRANOP)) {
                    temp -= *(ckt->CKTstate0 + here->MOScqgb) + 
                            *(ckt->CKTstate0 + here->MOScqgd) + 
                            *(ckt->CKTstate0 + here->MOScqgs);
                }
                value->rValue += temp * *(ckt->CKTrhsOld + here->MOSsNode);
            }
            break;
        case MOS_DRAINRESIST:
            if (here->MOSdNodePrime != here->MOSdNode &&
                    here->MOSdrainConductance != 0)
                value->rValue = 1.0 / here->MOSdrainConductance;
            else
                value->rValue = 0.0;
            break;
        case MOS_SOURCERESIST:
            if (here->MOSsNodePrime != here->MOSsNode &&
                    here->MOSsourceConductance != 0)
                value->rValue = 1.0 / here->MOSsourceConductance;
            else
                value->rValue = 0.0;
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}

