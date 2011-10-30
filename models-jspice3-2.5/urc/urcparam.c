/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "urcdefs.h"
#include "sperror.h"
#include "util.h"


/* ARGSUSED */
int
URCparam(ckt,param,value,inst,select)

CKTcircuit *ckt;
int param;
IFvalue *value;
GENinstance *inst;
IFvalue *select;
{
    URCinstance *here = (URCinstance *)inst;
    switch(param) {
        case URC_LEN:
            here->URClength = value->rValue;
            here->URClenGiven = TRUE;
            break;
        case URC_LUMPS:
            here->URClumps = value->rValue;
            here->URClumpsGiven = TRUE;
            break;
        default:
            return(E_BADPARM);
    }
    return(OK);
}
