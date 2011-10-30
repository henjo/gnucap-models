/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

    /* load the diode structure with those pointers needed later 
     * for fast matrix loading 
     */

#include "spice.h"
#include <stdio.h>
#include "diodefs.h"
#include "util.h"
#include "sperror.h"
#include "cktext.h"


int
DIOsetup(matrix,inModel,ckt,states)

SMPmatrix *matrix;
GENmodel *inModel;
CKTcircuit *ckt;
int *states;
{
    DIOmodel *model = (DIOmodel*)inModel;
    DIOinstance *here;
    int error;
    CKTnode *tmp;

    /* loop through all the diode models */
    for ( ; model != NULL; model = model->DIOnextModel) {

        if (!model->DIOemissionCoeffGiven) {
            model->DIOemissionCoeff = 1;
        }
        if (!model->DIOsatCurGiven) {
            model->DIOsatCur = 1e-14;
        }
        if (!model->DIObreakdownCurrentGiven) {
            model->DIObreakdownCurrent = 1e-3;
        }
        if (!model->DIOjunctionPotGiven){
            model->DIOjunctionPot = 1;
        }
        if (!model->DIOgradingCoeffGiven) {
            model->DIOgradingCoeff = .5;
        } 
        if (!model->DIOdepletionCapCoeffGiven) {
            model->DIOdepletionCapCoeff = .5;
        } 
        if (!model->DIOtransitTimeGiven) {
            model->DIOtransitTime = 0;
        }
        if (!model->DIOjunctionCapGiven) {
            model->DIOjunctionCap = 0;
        }
        if (!model->DIOactivationEnergyGiven) {
            model->DIOactivationEnergy = 1.11;
        } 
        if (!model->DIOsaturationCurrentExpGiven) {
            model->DIOsaturationCurrentExp = 3;
        }
        if (!model->DIOfNcoefGiven) {
            model->DIOfNcoef = 0.0;
        }
        if (!model->DIOfNexpGiven) {
            model->DIOfNexp = 1.0;
        }

        /* loop through all the instances of the model */
        for (here = model->DIOinstances; here != NULL;
                here = here->DIOnextInstance) {
            
            if (!here->DIOareaGiven) {
                here->DIOarea = 1;
            }
            if (model->DIOresist == 0) {
                here->DIOposPrimeNode = here->DIOposNode;
            }
            else if (here->DIOposPrimeNode == 0) {
                error = CKTmkVolt(ckt,&tmp,here->DIOname,"internal");
                if (error) return (error);
                here->DIOposPrimeNode = tmp->number;
            }

            here->DIOstate = *states;
            *states += DIOnumStates;

            TSTALLOC(DIOposPosPrimePtr,DIOposNode,DIOposPrimeNode)
            TSTALLOC(DIOnegPosPrimePtr,DIOnegNode,DIOposPrimeNode)
            TSTALLOC(DIOposPrimePosPtr,DIOposPrimeNode,DIOposNode)
            TSTALLOC(DIOposPrimeNegPtr,DIOposPrimeNode,DIOnegNode)
            TSTALLOC(DIOposPosPtr,DIOposNode,DIOposNode)
            TSTALLOC(DIOnegNegPtr,DIOnegNode,DIOnegNode)
            TSTALLOC(DIOposPrimePosPrimePtr,DIOposPrimeNode,DIOposPrimeNode)
        }
    }
    return (OK);
}
