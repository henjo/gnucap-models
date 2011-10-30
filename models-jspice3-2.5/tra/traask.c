/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1987 Thomas L. Quarles
**********/

/*
 * This routine gives access to the internal device parameter
 * of TRAnsmission lines
 */

#include "spice.h"
#include <stdio.h>
#include "tradefs.h"
#include "sperror.h"
#include "util.h"


/* ARGSUSED */
int
TRAask(ckt,inst,which,value,select)

CKTcircuit *ckt;
GENinstance *inst;
int which;
IFvalue *value;
IFvalue *select;
{
    TRAinstance *here = (TRAinstance *)inst;
    int temp;

    switch(which) {
        case TRA_POS_NODE1:
            value->iValue = here->TRAposNode1;
            break;
        case TRA_NEG_NODE1:
            value->iValue = here->TRAnegNode1;
            break;
        case TRA_POS_NODE2:
            value->iValue = here->TRAposNode2;
            break;
        case TRA_NEG_NODE2:
            value->iValue = here->TRAnegNode2;
            break;
        case TRA_INT_NODE1:
            value->iValue = here->TRAintNode1;
            break;
        case TRA_INT_NODE2:
            value->iValue = here->TRAintNode2;
            break;
        case TRA_Z0:
            value->rValue = here->TRAimped;
            break;
        case TRA_TD:
            value->rValue = here->TRAtd;
            break;
        case TRA_NL:
            value->rValue = here->TRAnl;
            break;
        case TRA_FREQ:
            value->rValue = here->TRAf;
            break;
        case TRA_V1:
            value->rValue = here->TRAinitVolt1;
            break;
        case TRA_I1:
            value->rValue = here->TRAinitCur1;
            break;
        case TRA_V2:
            value->rValue = here->TRAinitVolt2;
            break;
        case TRA_I2:
            value->rValue = here->TRAinitCur2;
            break;
        case TRA_RELTOL:
            value->rValue = here->TRAreltol;
            break;
        case TRA_ABSTOL:
            value->rValue = here->TRAabstol;
            break;
        case TRA_BR_EQ1:
            value->rValue = here->TRAbrEq1;
            break;
        case TRA_BR_EQ2:
            value->rValue = here->TRAbrEq2;
            break;
        case TRA_DELAY:
            value->v.vec.rVec = (double *) MALLOC(here->TRAsizeDelay);
            value->v.numValue = temp = here->TRAsizeDelay;
            while (temp--) {
                *value->v.vec.rVec++ = *here->TRAdelays++;
            }
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}
