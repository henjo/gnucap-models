/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

/*
 * This routine gives access to the internal model parameters
 * of Uniform distributed RC lines
 */

#include "spice.h"
#include <stdio.h>
#include "urcdefs.h"
#include "sperror.h"


/* ARGSUSED */
int
URCmAsk(ckt,inst,which,value)

CKTcircuit *ckt;
GENmodel *inst;
int which;
IFvalue *value;
{
    URCmodel *here = (URCmodel *)inst;

    switch (which) {
        case URC_MOD_K:
            value->rValue = here->URCk;
            break;
        case URC_MOD_FMAX:
            value->rValue = here->URCfmax;
            break;
        case URC_MOD_RPERL:
            value->rValue = here->URCrPerL;
            break;
        case URC_MOD_CPERL:
            value->rValue = here->URCcPerL;
            break;
        case URC_MOD_ISPERL:
            value->rValue = here->URCisPerL;
            break;
        case URC_MOD_RSPERL:
            value->rValue = here->URCrsPerL;
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}
