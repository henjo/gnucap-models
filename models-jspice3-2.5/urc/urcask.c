/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

/*
 * This routine gives access to the internal device parameters
 * of Uniform distributed RC lines
 */

#include "spice.h"
#include <stdio.h>
#include "urcdefs.h"
#include "sperror.h"


/* ARGSUSED */
int
URCask(ckt,inst,which,value,select)

CKTcircuit *ckt;
GENinstance *inst;
int which;
IFvalue *value;
IFvalue *select;
{
    URCinstance *here = (URCinstance *)inst;

    switch (which) {
        case URC_POS_NODE:
            value->iValue = here->URCposNode;
            break;
        case URC_NEG_NODE:
            value->iValue = here->URCnegNode;
            break;
        case URC_GND_NODE:
            value->iValue = here->URCgndNode;
            break;
        case URC_LEN:
            value->rValue = here->URClength;
            break;
        case URC_LUMPS:
            value->iValue = here->URClumps;
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}
