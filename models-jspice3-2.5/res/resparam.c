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


/* ARGSUSED */
int
RESparam(ckt,param,value,inst,select)

CKTcircuit *ckt;
int param;
IFvalue *value;
GENinstance *inst;
IFvalue *select;
{
    RESinstance *here = (RESinstance *)inst;

    switch(param) {
        case RES_TEMP:
            here->REStemp = value->rValue+CONSTCtoK;
            here->REStempGiven = TRUE;
            break;
        case RES_RESIST:
            here->RESresist = value->rValue;
            here->RESresGiven = TRUE;
            break;
        case RES_WIDTH:
            here->RESwidth = value->rValue;
            here->RESwidthGiven = TRUE;
            break;
        case RES_LENGTH:
            here->RESlength = value->rValue;
            here->RESlengthGiven = TRUE;
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}
