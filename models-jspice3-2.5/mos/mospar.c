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
#include "const.h"
#include "sperror.h"
#include "util.h"


/* ARGSUSED */
int
MOSparam(ckt,param,value,inst,select)

CKTcircuit *ckt;
int param;
IFvalue *value;
GENinstance *inst;
IFvalue *select;
{
    MOSinstance *here = (MOSinstance *)inst;

    switch (param) {

        case MOS_TEMP:
            here->MOStemp = value->rValue+CONSTCtoK;
            here->MOStempGiven = TRUE;
            break;
        case MOS_L:
            here->MOSl = value->rValue;
            here->MOSlGiven = TRUE;
            break;
        case MOS_W:
            here->MOSw = value->rValue;
            here->MOSwGiven = TRUE;
            break;
        case MOS_AD:
            here->MOSdrainArea = value->rValue;
            here->MOSdrainAreaGiven = TRUE;
            break;
        case MOS_AS:
            here->MOSsourceArea = value->rValue;
            here->MOSsourceAreaGiven = TRUE;
            break;
        case MOS_PD:
            here->MOSdrainPerimeter = value->rValue;
            here->MOSdrainPerimeterGiven = TRUE;
            break;
        case MOS_PS:
            here->MOSsourcePerimeter = value->rValue;
            here->MOSsourcePerimeterGiven = TRUE;
            break;
        case MOS_NRD:
            here->MOSdrainSquares = value->rValue;
            here->MOSdrainSquaresGiven = TRUE;
            break;
        case MOS_NRS:
            here->MOSsourceSquares = value->rValue;
            here->MOSsourceSquaresGiven = TRUE;
            break;
        case MOS_OFF:
            here->MOSoff = value->iValue;
            break;
        case MOS_IC_VDS:
            here->MOSicVDS = value->rValue;
            here->MOSicVDSGiven = TRUE;
            break;
        case MOS_IC_VGS:
            here->MOSicVGS = value->rValue;
            here->MOSicVGSGiven = TRUE;
            break;
        case MOS_IC_VBS:
            here->MOSicVBS = value->rValue;
            here->MOSicVBSGiven = TRUE;
            break;
        case MOS_IC:
            switch (value->v.numValue) {
                case 3:
                    here->MOSicVBS = *(value->v.vec.rVec+2);
                    here->MOSicVBSGiven = TRUE;
                case 2:
                    here->MOSicVGS = *(value->v.vec.rVec+1);
                    here->MOSicVGSGiven = TRUE;
                case 1:
                    here->MOSicVDS = *(value->v.vec.rVec);
                    here->MOSicVDSGiven = TRUE;
                    break;
                default:
                    return (E_BADPARM);
            }
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}
