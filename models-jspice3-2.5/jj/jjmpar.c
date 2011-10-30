/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Author: 1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "jjdefs.h"
#include "const.h"
#include "sperror.h"
#include "util.h"


int
JJmParam(param,value,inModel)

int param;
IFvalue *value;
GENmodel *inModel;
{
    JJmodel *model = (JJmodel *)inModel; 

    switch (param) {
    case JJ_MOD_RT:
        model->JJrtype = value->iValue;
        model->JJrtypeGiven = TRUE;
        break;
    case JJ_MOD_IC:
        model->JJictype = value->iValue;
        model->JJictypeGiven = TRUE;
        break;
    case JJ_MOD_VG:
        model->JJvg = value->rValue;
        model->JJvgGiven = TRUE;
        break;
    case JJ_MOD_DV:
        model->JJdelv = value->rValue;
        model->JJdelvGiven = TRUE;
        break;
    case JJ_MOD_CRT:
        model->JJcriti = value->rValue;
        model->JJcritiGiven = TRUE;
        break;
    case JJ_MOD_CAP:
        model->JJcap = value->rValue;
        model->JJcapGiven = TRUE;
        break;
    case JJ_MOD_R0:
        model->JJr0 = value->rValue;
        model->JJr0Given = TRUE;
        break;
    case JJ_MOD_RN:
        model->JJrn = value->rValue;
        model->JJrnGiven = TRUE;
        break;
    case JJ_MOD_CCS:
        model->JJccsens = value->rValue;
        model->JJccsensGiven = TRUE;
        break;
    case JJ_MOD_JJ:
        break;
    default:
        return E_BADPARM;
    }
    return OK;
}
