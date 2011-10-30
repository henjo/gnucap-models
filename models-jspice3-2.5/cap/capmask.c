/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "capdefs.h"
#include "sperror.h"


/* ARGSUSED */
int
CAPmAsk(ckt,inst,which,value)

CKTcircuit *ckt;
GENmodel *inst;
int which;
IFvalue *value;
{
    CAPmodel *here = (CAPmodel*)inst;

    switch(which) {
        case CAP_MOD_CJ:
            value->rValue = here->CAPcj;
            break;
        case CAP_MOD_CJSW:
            value->rValue = here->CAPcjsw;
            break;
        case CAP_MOD_DEFWIDTH:
            value->rValue = here->CAPdefWidth;
            break;
        case CAP_MOD_NARROW:
            value->rValue = here->CAPnarrow;
            break;
        default:  
            return (E_BADPARM);
    }
    return (OK);
}
