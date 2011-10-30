/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Author: 1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include <math.h>
#include "jjdefs.h"
#include "sperror.h"
#include "const.h"
#include "util.h"


/* ARGSUSED */
int
JJask(ckt,inst,which,value,select)

CKTcircuit *ckt;
GENinstance *inst;
int which;
IFvalue *value;
IFvalue *select;
{
    JJinstance *here = (JJinstance*)inst;

    switch (which) {
        case JJ_AREA:
            value->rValue = here->JJarea;
            break;
        case JJ_IC:
            value->v.vec.rVec = here->JJinitCnd;
            value->v.numValue = 2;
            break;
        case JJ_ICP:
            value->rValue = here->JJinitPhase;
            break;
        case JJ_ICV:
            value->rValue = here->JJinitVoltage;
            break;
        case JJ_CON:
            value->uValue = here->JJcontrol;
            break;
        case JJ_QUEST_CRT:
            value->rValue = here->JJcriti;
            break;
        case JJ_QUEST_CAP:
            value->rValue = here->JJcap;
            break;
        case JJ_QUEST_G0:
            value->rValue = here->JJg0;
            break;
        case JJ_QUEST_GN:
            value->rValue = here->JJgn;
            break;
        case JJ_QUEST_GS:
            value->rValue = here->JJgs;
            break;
        case JJ_QUEST_G1:
            value->rValue = here->JJg1;
            break;
        case JJ_QUEST_G2:
            value->rValue = here->JJg2;
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}


