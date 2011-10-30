/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1987 Mathew Lew and Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/
/********** new in 3f2
Sydney University mods Copyright(c) 1989 Anthony E. Parker, David J. Skellern
    Laboratory for Communication Science Engineering
    Sydney University Department of Electrical Engineering, Australia
**********/

#include "spice.h"
#include <stdio.h>
#include "jfetdefs.h"
#include "sperror.h"
#include "const.h"


/*ARGSUSED*/
int
JFETmAsk(ckt,inModel,which,value)

CKTcircuit *ckt;
GENmodel *inModel;
int which;
IFvalue *value;
{
    JFETmodel *model = (JFETmodel*)inModel;

    switch (which) {

        case JFET_MOD_VTO:
            value->rValue = model->JFETthreshold;
            break;
        case JFET_MOD_BETA:
            value->rValue = model->JFETbeta;
            break;
        case JFET_MOD_LAMBDA:
            value->rValue = model->JFETlModulation;
            break;
        case JFET_MOD_RD:
            value->rValue = model->JFETdrainResist;
            break;
        case JFET_MOD_RS:
            value->rValue = model->JFETsourceResist;
            break;
        case JFET_MOD_CGS:
            value->rValue = model->JFETcapGS;
            break;
        case JFET_MOD_CGD:
            value->rValue = model->JFETcapGD;
            break;
        case JFET_MOD_PB:
            value->rValue = model->JFETgatePotential;
            break;
        case JFET_MOD_IS:
            value->rValue = model->JFETgateSatCurrent;
            break;
        case JFET_MOD_FC:
            value->rValue = model->JFETdepletionCapCoeff;
            break;
        case JFET_MOD_B:
            value->rValue = model->JFETb;
            break;
        case JFET_MOD_DRAINCONDUCT:
            value->rValue = model->JFETdrainConduct;
            break;
        case JFET_MOD_SOURCECONDUCT:
            value->rValue = model->JFETsourceConduct;
            break;
        case JFET_MOD_TYPE:
            if (model->JFETtype == NJF)
                value->sValue = "njf";
            else
                value->sValue = "pjf";
            break;
        case JFET_MOD_TNOM:
            value->rValue = model->JFETtnom-CONSTCtoK;
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}

