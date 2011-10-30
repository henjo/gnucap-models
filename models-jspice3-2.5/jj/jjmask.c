/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Author: 1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "jjdefs.h"
#include "const.h"
#include "sperror.h"


int
JJmAsk(ckt,inModel,which,value)

CKTcircuit *ckt;
GENmodel *inModel;
int which;
IFvalue *value;
{
    JJmodel *model = (JJmodel *)inModel;

    switch (which) {
        case JJ_MOD_RT:
            value->iValue = model->JJrtype;
            break;
        case JJ_MOD_IC:
            value->iValue = model->JJictype;
            break;
        case JJ_MOD_VG:
            value->rValue = model->JJvg;
            break;
        case JJ_MOD_DV:
            value->rValue = model->JJdelv;
            break;
        case JJ_MOD_CRT:
            value->rValue = model->JJcriti;
            break;
        case JJ_MOD_CAP:
            value->rValue = model->JJcap;
            break;
        case JJ_MOD_R0:
            value->rValue = model->JJr0;
            break;
        case JJ_MOD_RN:
            value->rValue = model->JJrn;
            break;
        case JJ_MOD_CCS:
            value->rValue = model->JJccsens;
            break;
        case JJ_MQUEST_VL:
            value->rValue = model->JJvless;
            break;
        case JJ_MQUEST_VM:
            value->rValue = model->JJvmore;
            break;
        case JJ_MQUEST_VDP:
            value->rValue = model->JJvdpbak;
            break;
        case JJ_MOD_JJ:
            value->iValue = 1;
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}



