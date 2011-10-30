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
JJparam(ckt,param,value,inst,select)

CKTcircuit *ckt;
int param;
IFvalue *value;
GENinstance *inst;
IFvalue *select;
{
    JJinstance *here = (JJinstance *)inst;

    switch (param) {
    case JJ_AREA:
        here->JJarea = value->rValue;
        here->JJareaGiven = TRUE;
        break;
    case JJ_IC:
        switch(value->v.numValue) {
        case 2:
            here->JJinitPhase = *(value->v.vec.rVec+1);
            here->JJinitPhaseGiven = TRUE;
        case 1:
            here->JJinitVoltage = *(value->v.vec.rVec);
            here->JJinitVoltGiven = TRUE;
            break;
        default:
            return E_BADPARM;
        }
        break;
    case JJ_ICP:
        here->JJinitPhase = value->rValue;
        here->JJinitPhaseGiven = TRUE;
        break;
    case JJ_ICV:
        here->JJinitVoltage = value->rValue;
        here->JJinitVoltGiven = TRUE;
        break;
    case JJ_CON:
        here->JJcontrol = value->uValue;
        here->JJcontrolGiven = TRUE;
        break;
    default:
        return E_BADPARM;
    }
    return OK;
}
