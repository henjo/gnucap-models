/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 S. Hwang
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "mesdefs.h"
#include "sperror.h"
#include "util.h"


/* ARGSUSED */
int
MESparam(ckt,param,value,inst,select)

CKTcircuit *ckt;
int param;
IFvalue *value;
GENinstance *inst;
IFvalue *select;
{
    MESinstance *here = (MESinstance*)inst;

    switch (param) {

        case MES_AREA:
            here->MESarea = value->rValue;
            here->MESareaGiven = TRUE;
            break;
        case MES_IC_VDS:
            here->MESicVDS = value->rValue;
            here->MESicVDSGiven = TRUE;
            break;
        case MES_IC_VGS:
            here->MESicVGS = value->rValue;
            here->MESicVGSGiven = TRUE;
            break;
        case MES_IC:
            switch(value->v.numValue) {
                case 2:
                    here->MESicVGS = *(value->v.vec.rVec+1);
                    here->MESicVGSGiven = TRUE;
                case 1:
                    here->MESicVDS = *(value->v.vec.rVec);
                    here->MESicVDSGiven = TRUE;
                    break;
                default:
                    return(E_BADPARM);
            }
            break;
        case MES_OFF:
            here->MESoff = value->iValue;
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}
