/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "resdefs.h"
#include "const.h"
#include "sperror.h"
#include "util.h"


int
RESmParam(param,value,inModel)

int param;
IFvalue *value;
GENmodel *inModel;
{
    RESmodel *model = (RESmodel *)inModel;

    switch (param) {
        case RES_MOD_TNOM:
            model->REStnom = value->rValue+CONSTCtoK;
            model->REStnomGiven = TRUE;
            break;
        case RES_MOD_TC1:
            model->REStempCoeff1 = value->rValue;
            model->REStc1Given = TRUE;
            break;
        case RES_MOD_TC2:
            model->REStempCoeff2 = value->rValue;
            model->REStc2Given = TRUE;
            break;
        case RES_MOD_RSH:
            model->RESsheetRes = value->rValue;
            model->RESsheetResGiven = TRUE;
            break;
        case RES_MOD_DEFWIDTH:
            model->RESdefWidth = value->rValue;
            model->RESdefWidthGiven = TRUE;
            break;
        case RES_MOD_NARROW:
            model->RESnarrow = value->rValue;
            model->RESnarrowGiven = TRUE;
            break;
        case RES_MOD_R:
            /* just being reassured by user that this is a resistor model */
            /* no-op */
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}
