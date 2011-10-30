/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "inddefs.h"
#include "util.h"
#include "sperror.h"


/* ARGSUSED */
int
INDparam(ckt,param,value,inst,select)

CKTcircuit *ckt;
int param;
IFvalue *value;
GENinstance *inst;
IFvalue *select;
{
    INDinstance *here = (INDinstance*)inst;

    switch (param) {
        case IND_IND:
            here->INDinduct = value->rValue;
            here->INDindGiven = TRUE;
            break;
        case IND_IC:
            here->INDinitCond = value->rValue;
            here->INDicGiven = TRUE;
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}
