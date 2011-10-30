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


#ifdef MUTUAL

/* ARGSUSED */
int
MUTparam(ckt,param,value,inst,select)

CKTcircuit *ckt;
int param;
IFvalue *value;
GENinstance *inst;
IFvalue *select;
{
    MUTinstance *here = (MUTinstance*)inst;

    switch (param) {
        case MUT_COEFF:
            here->MUTcoupling = value->rValue;
            here->MUTindGiven = TRUE;
            break;
        case MUT_IND1:
            here->MUTindName1 = value->uValue;
            break;
        case MUT_IND2:
            here->MUTindName2 = value->uValue;
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}

#endif /* MUTUAL */
