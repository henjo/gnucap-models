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


int
SWmParam(param,value,inModel)

int param;
IFvalue *value;
GENmodel *inModel;
{
    SWmodel *model = (SWmodel *)inModel;

    switch (param) {

        case SW_MOD_SW:
            /* just says that this is a switch */
            break;
        case SW_MOD_RON:
            model->SWonResistance = value->rValue;
            model->SWonConduct = 1.0/(value->rValue);
            model->SWonGiven = TRUE;
            break;
        case SW_MOD_ROFF:
            model->SWoffResistance = value->rValue;
            model->SWoffConduct = 1.0/(value->rValue);
            model->SWoffGiven = TRUE;
            break;
        case SW_MOD_VTH:
            model->SWvThreshold = value->rValue;
            model->SWvThreshGiven = TRUE;
            break;
        case SW_MOD_VHYS:
            /* take absolute value of hysteresis voltage */
            model->SWvHysteresis = FABS(value->rValue);
            model->SWvHystGiven = TRUE;
            break;
        case SW_MOD_ITH:
            model->SWiThreshold = value->rValue;
            model->SWiThreshGiven = TRUE;
            break;
        case SW_MOD_IHYS:
            /* take absolute value of hysteresis current */
            model->SWiHysteresis = FABS(value->rValue);
            model->SWiHystGiven = TRUE;
            break;
        default:
            return(E_BADPARM);
    }
    return(OK);
}
