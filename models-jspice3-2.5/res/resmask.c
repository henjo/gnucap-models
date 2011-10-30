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


/* ARGSUSED */
int 
RESmAsk(ckt,inModel,which,value)

CKTcircuit *ckt;
GENmodel *inModel;
int which;
IFvalue *value;
{
    RESmodel *model = (RESmodel *)inModel;

    switch (which) {
        case RES_MOD_TNOM:
            value->rValue = model->REStnom-CONSTCtoK;
            break;
        case RES_MOD_TC1:
            value->rValue = model->REStempCoeff1;
            break;
        case RES_MOD_TC2:
            value->rValue = model->REStempCoeff2;
            break;
        case RES_MOD_RSH:
            value->rValue = model->RESsheetRes;
            break;
        case RES_MOD_DEFWIDTH:
            value->rValue = model->RESdefWidth;
            break;
        case RES_MOD_NARROW: 
            value->rValue = model->RESnarrow;
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}

