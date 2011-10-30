/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Gordon M. Jacobs
         1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "swdefs.h"
#include "sperror.h"
#include "util.h"


/*ARGSUSED*/
int
SWparam(ckt,param,value,inst,select)

CKTcircuit *ckt;
int param;
IFvalue *value;
GENinstance *inst;
IFvalue *select;
{
    SWinstance *here = (SWinstance *)inst;

    switch (param) {

        case SW_IC_ON:
            if (value->iValue) {
                here->SWzero_stateGiven = TRUE;
            }
            break;
        case SW_IC_OFF:
            if (value->iValue) {
                here->SWzero_stateGiven = FALSE;
            }
            break;
        case  SW_CONTROL:
            here->SWcontName = value->uValue;
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}
