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
#include "util.h"


int
CAPmParam(param,value,inModel)

int param;
IFvalue *value;
GENmodel *inModel;
{
    CAPmodel *mod = (CAPmodel*)inModel;

    switch (param) {

        case CAP_MOD_CJ :
            mod->CAPcj = value->rValue;
            mod->CAPcjGiven = TRUE;
            break;
        case CAP_MOD_CJSW :
            mod->CAPcjsw = value->rValue;
            mod->CAPcjswGiven = TRUE;
            break;
        case CAP_MOD_DEFWIDTH:
            mod->CAPdefWidth = value->rValue;
            mod->CAPdefWidthGiven = TRUE;
            break;
        case CAP_MOD_NARROW:
            mod->CAPnarrow = value->rValue;
            mod->CAPnarrowGiven = TRUE;
            break;
        case CAP_MOD_C:
            /* just being reassured by the user that we are a capacitor */
            /* no-op */
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}

