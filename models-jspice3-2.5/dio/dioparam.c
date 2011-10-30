/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "diodefs.h"
#include "const.h"
#include "util.h"
#include "sperror.h"


/* ARGSUSED */
int
DIOparam(ckt,param,value,inst,select)

CKTcircuit *ckt;
int param;
IFvalue *value;
GENinstance *inst;
IFvalue *select;
{
    DIOinstance *here = (DIOinstance*)inst;

    switch (param) {
        case DIO_AREA:
            here->DIOarea = value->rValue;
            here->DIOareaGiven = TRUE;
            break;
        case DIO_IC:
            here->DIOinitCond = value->rValue;
            break;
        case DIO_OFF:
            here->DIOoff = value->iValue;
            break;
        case DIO_TEMP:
            here->DIOtemp = value->rValue+CONSTCtoK;
            here->DIOtempGiven = TRUE;
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}
