/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include <math.h>
#include "inddefs.h"
#include "sperror.h"


#ifdef MUTUAL

/*ARGSUSED*/
int
MUTask(ckt,inst,which,value,select)

CKTcircuit *ckt;
GENinstance *inst;
int which;
IFvalue *value;
IFvalue *select;
{
    MUTinstance *here = (MUTinstance*)inst;

    switch (which) {
        case MUT_COEFF:
            value->rValue = here->MUTfactor;
            break;
        case MUT_IND1:
            value->uValue = here->MUTindName1;
            break;
        case MUT_IND2:
            value->uValue = here->MUTindName2;
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}
#endif /* MUTUAL */
