/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "capdefs.h"
#include "util.h"
#include "sperror.h"


/* ARGSUSED */
int
CAPparam(ckt,param,value,inst,select)

CKTcircuit *ckt;
int param;
IFvalue *value;
GENinstance *inst;
IFvalue *select;
{
    CAPinstance *here = (CAPinstance*)inst;

    switch (param) {

        case CAP_CAP:
            here->CAPcapac = value->rValue;
            here->CAPcapGiven = TRUE;
            break;
        case CAP_IC:
            here->CAPinitCond = value->rValue;
            here->CAPicGiven = TRUE;
            break;
        case CAP_WIDTH:
            here->CAPwidth = value->rValue;
            here->CAPwidthGiven = TRUE;
            break;
        case CAP_LENGTH:
            here->CAPlength = value->rValue;
            here->CAPlengthGiven = TRUE;
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}
