/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Gordon M. Jacobs
         1992 Stephen R. Whiteley
****************************************************************************/

/*
 * This routine gives access to the internal model parameter
 * of voltage controlled SWitch
 */

#include "spice.h"
#include <stdio.h>
#include "swdefs.h"
#include "sperror.h"


/* ARGSUSED */
int
SWmAsk(ckt,inModel,which,value)

CKTcircuit *ckt;
GENmodel *inModel;
int which;
IFvalue *value;
{
    SWmodel *model = (SWmodel *)inModel;

    switch (which) {

        case SW_MOD_RON:
            value->rValue = model->SWonResistance;
            break;
        case SW_MOD_ROFF:
            value->rValue = model->SWoffResistance;
            break;
        case SW_MOD_VTH:
            value->rValue = model->SWvThreshold;
            break;
        case SW_MOD_VHYS:
            value->rValue = model->SWvHysteresis;
            break;
        case SW_MOD_ITH:
            value->rValue = model->SWiThreshold;
            break;
        case SW_MOD_IHYS:
            value->rValue = model->SWiHysteresis;
            break;
        case SW_MOD_GON:
            value->rValue = model->SWonConduct;
            break;
        case SW_MOD_GOFF:
            value->rValue = model->SWoffConduct;
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}
