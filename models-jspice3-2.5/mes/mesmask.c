/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1987 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "mesdefs.h"
#include "sperror.h"


/* ARGSUSED */
int
MESmAsk(ckt,inst,which,value)

CKTcircuit *ckt;
GENmodel *inst;
int which;
IFvalue *value;
{
    MESmodel *here = (MESmodel*)inst;

    switch (which) {

        case MES_MOD_VTO:
            value->rValue = here->MESthreshold;
            break;
        case MES_MOD_ALPHA:
            value->rValue = here->MESalpha;
            break;
        case MES_MOD_BETA:
            value->rValue = here->MESbeta;
            break;
        case MES_MOD_LAMBDA:
            value->rValue = here->MESlModulation;
            break;
        case MES_MOD_B:
            value->rValue = here->MESb;
            break;
        case MES_MOD_RD:
            value->rValue = here->MESdrainResist;
            break;
        case MES_MOD_RS:
            value->rValue = here->MESsourceResist;
            break;
        case MES_MOD_CGS:
            value->rValue = here->MEScapGS;
            break;
        case MES_MOD_CGD:
            value->rValue = here->MEScapGD;
            break;
        case MES_MOD_PB:
            value->rValue = here->MESgatePotential;
            break;
        case MES_MOD_IS:
            value->rValue = here->MESgateSatCurrent;
            break;
        case MES_MOD_FC:
            value->rValue = here->MESdepletionCapCoeff;
            break;
        case MES_MOD_DRAINCONDUCT:
            value->rValue = here->MESdrainConduct;
            break;
        case MES_MOD_SOURCECONDUCT:
            value->rValue = here->MESsourceConduct;
            break;
        case MES_MOD_DEPLETIONCAP:
            value->rValue = here->MESdepletionCap;
            break;
        case MES_MOD_VCRIT:
            value->rValue = here->MESvcrit;
            break;
        /* new in ef2 */
        case MES_MOD_TYPE:
            if (here->MEStype == NMF)
                value->sValue = "nmf";
            else
                value->sValue = "pmf";
        default:
            return (E_BADPARM);
    }
    return (OK);
}
