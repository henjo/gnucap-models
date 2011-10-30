/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1990 Jaijeet S. Roychowdhury
         1993 Stephen R. Whiteley
****************************************************************************/

/*
 * This routine gives access to the internal device parameter
 * of LTRA lines
 */

#include "spice.h"
#include <stdio.h>
#include <math.h>
#include "ltradefs.h"
#include "sperror.h"
#include "util.h"


/* ARGSUSED */
int
LTRAask(ckt,inst,which,value,select)

CKTcircuit *ckt;
GENinstance *inst;
int which;
IFvalue *value;
IFvalue *select;
{
    LTRAinstance *here = (LTRAinstance *)inst;

    switch (which) {

        case LTRA_V1:
            value->rValue = here->LTRAinitVolt1;
            break;
        case LTRA_I1:
            value->rValue = here->LTRAinitCur1;
            break;
        case LTRA_V2:
            value->rValue = here->LTRAinitVolt2;
            break;
        case LTRA_I2:
            value->rValue = here->LTRAinitCur2;
            break;
        case LTRA_POS_NODE1:
            value->iValue = here->LTRAposNode1;
            break;
        case LTRA_NEG_NODE1:
            value->iValue = here->LTRAnegNode1;
            break;
        case LTRA_POS_NODE2:
            value->iValue = here->LTRAposNode2;
            break;
        case LTRA_NEG_NODE2:
            value->iValue = here->LTRAnegNode2;
            break;
        case LTRA_BR_EQ1:
            value->rValue = here->LTRAbrEq1;
            break;
        case LTRA_BR_EQ2:
            value->rValue = here->LTRAbrEq2;
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}
