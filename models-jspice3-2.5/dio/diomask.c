/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "diodefs.h"
#include "sperror.h"
#include "const.h"


/* ARGSUSED */
int
DIOmAsk (ckt,inModel,which, value)

CKTcircuit *ckt;
int which;
IFvalue *value;
GENmodel *inModel;
{
    DIOmodel *model = (DIOmodel*)inModel;

    switch (which) {
        case DIO_MOD_IS:
            value->rValue = model->DIOsatCur;
            break;
        case DIO_MOD_RS:
            value->rValue = model->DIOresist;
            break;
        case DIO_MOD_N:
            value->rValue = model->DIOemissionCoeff;
            break;
        case DIO_MOD_TT:
            value->rValue = model->DIOtransitTime;
            break;
        case DIO_MOD_CJO:
            value->rValue = model->DIOjunctionCap;
            break;
        case DIO_MOD_VJ:
            value->rValue = model->DIOjunctionPot;
            break;
        case DIO_MOD_M:
            value->rValue = model->DIOgradingCoeff;
            break;
        case DIO_MOD_EG:
            value->rValue = model->DIOactivationEnergy;
            break;
        case DIO_MOD_XTI:
            value->rValue = model->DIOsaturationCurrentExp;
            break;
        case DIO_MOD_FC:
            value->rValue = model->DIOdepletionCapCoeff;
            break;
        case DIO_MOD_BV:
            value->rValue = model->DIObreakdownVoltage;
            break;
        case DIO_MOD_IBV:
            value->rValue = model->DIObreakdownCurrent;
            break;
        case DIO_MOD_COND:
            value->rValue = model->DIOconductance;
            break;
        case DIO_MOD_TNOM:
            value->rValue = model->DIOnomTemp-CONSTCtoK;
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}

