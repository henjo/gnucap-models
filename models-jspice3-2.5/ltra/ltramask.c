/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "ltradefs.h"
#include "const.h"
#include "sperror.h"


/* ARGSUSED */
int 
LTRAmAsk(ckt,inModel,which,value)

CKTcircuit *ckt;
GENmodel *inModel;
int which;
IFvalue *value;
{
    LTRAmodel *model = (LTRAmodel *)inModel;

    switch (which) {
        case LTRA_MOD_R:
            value->rValue = model->LTRAresist;
            break;
        case LTRA_MOD_L:
            value->rValue = model->LTRAinduct;
            break;
        case LTRA_MOD_G:
            value->rValue = model->LTRAconduct;
            break;
        case LTRA_MOD_C:
            value->rValue = model->LTRAcapac;
            break;
        case LTRA_MOD_LEN:
            value->rValue = model->LTRAlength;
            break;
        case LTRA_MOD_RELTOL:
            value->rValue = model->LTRAreltol;
            break;
        case LTRA_MOD_ABSTOL:
            value->rValue = model->LTRAabstol;
            break;
        case LTRA_MOD_NOCONTROL:
            value->iValue = (model->LTRAlteConType == LTRA_MOD_NOCONTROL);
            break;
        case LTRA_MOD_HALFCONTROL:
            value->iValue = (model->LTRAlteConType == LTRA_MOD_HALFCONTROL);
            break;
        case LTRA_MOD_FULLCONTROL:
            value->iValue = (model->LTRAlteConType == LTRA_MOD_FULLCONTROL);
            break;
        case LTRA_MOD_STEPLIMIT:
            value->iValue = (model->LTRAstepLimit == LTRA_MOD_STEPLIMIT);
            break;
        case LTRA_MOD_NOSTEPLIMIT:
            value->iValue = (model->LTRAstepLimit == LTRA_MOD_NOSTEPLIMIT);
            break;
        case LTRA_MOD_LININTERP:
            value->iValue = (model->LTRAhowToInterp == LTRA_MOD_LININTERP);
            break;
        case LTRA_MOD_QUADINTERP:
            value->iValue = (model->LTRAhowToInterp == LTRA_MOD_QUADINTERP);
            break;
        case LTRA_MOD_MIXEDINTERP:
            value->iValue = (model->LTRAhowToInterp == LTRA_MOD_MIXEDINTERP);
            break;
        case LTRA_MOD_TRUNCNR:
            value->iValue = model->LTRAtruncNR;
            break;
        case LTRA_MOD_TRUNCDONTCUT:
            value->iValue = model->LTRAtruncDontCut;
            break;
        case LTRA_MOD_STLINEREL:
            value->rValue = model->LTRAstLineReltol;
            break;
        case LTRA_MOD_STLINEABS:
            value->rValue = model->LTRAstLineAbstol;
            break;
        case LTRA_MOD_Z0:
            value->rValue = model->LTRAimped;
            break;
        case LTRA_MOD_TD:
            value->rValue = model->LTRAtd;
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}


