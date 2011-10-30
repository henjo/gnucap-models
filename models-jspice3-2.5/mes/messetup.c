/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 S. Hwang
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "mesdefs.h"
#include "const.h"
#include "sperror.h"
#include "util.h"
#include "cktext.h"


int
MESsetup(matrix,inModel,ckt,states)

/* load the diode structure with those pointers needed later 
 * for fast matrix loading 
 */
SMPmatrix *matrix;
GENmodel *inModel;
CKTcircuit *ckt;
int *states;
{
    MESmodel *model = (MESmodel*)inModel;
    MESinstance *here;
    int error;
    CKTnode *tmp;

    /*  loop through all the diode models */
    for ( ; model != NULL; model = model->MESnextModel) {

        if ( (model->MEStype != NMF) && (model->MEStype != PMF) ) {
            model->MEStype = NMF;
        }
        if (!model->MESthresholdGiven) {
            model->MESthreshold = -2;
        }
        if (!model->MESbetaGiven) {
            model->MESbeta = 2.5e-3;
        }
        if (!model->MESbGiven) {
            model->MESb = 0.3;
        }
        if (!model->MESalphaGiven) {
            model->MESalpha = 2;
        }
        if (!model->MESlModulationGiven) {
            model->MESlModulation = 0;
        }
        if (!model->MESdrainResistGiven) {
            model->MESdrainResist = 0;
        }
        if (!model->MESsourceResistGiven) {
            model->MESsourceResist = 0;
        }
        if (!model->MEScapGSGiven) {
            model->MEScapGS = 0;
        }
        if (!model->MEScapGDGiven) {
            model->MEScapGD = 0;
        }
        if (!model->MESgatePotentialGiven) {
            model->MESgatePotential = 1;
        }
        if (!model->MESgateSatCurrentGiven) {
            model->MESgateSatCurrent = 1e-14;
        }
        if (!model->MESdepletionCapCoeffGiven) {
            model->MESdepletionCapCoeff = .5;
        }
        if (!model->MESfNcoefGiven) {
            model->MESfNcoef = 0;
        }
        if (!model->MESfNexpGiven) {
            model->MESfNexp = 1;
        }

        /* loop through all the instances of the model */
        for (here = model->MESinstances; here != NULL;
                here = here->MESnextInstance) {
            
            if (!here->MESareaGiven) {
                here->MESarea = 1;
            }
            here->MESstate = *states;
            *states += MESnumStates;

            if (model->MESsourceResist != 0 &&
                    here->MESsourcePrimeNode == 0) {
                error = CKTmkVolt(ckt,&tmp,here->MESname,"source");
                if (error) return (error);
                here->MESsourcePrimeNode = tmp->number;
            }
            else {
                here->MESsourcePrimeNode = here->MESsourceNode;
            }
            if (model->MESdrainResist != 0 &&
                    here->MESdrainPrimeNode == 0) {
                error = CKTmkVolt(ckt,&tmp,here->MESname,"drain");
                if (error) return (error);
                here->MESdrainPrimeNode = tmp->number;
            }
            else {
                here->MESdrainPrimeNode = here->MESdrainNode;
            }

            TSTALLOC(MESdrainDrainPrimePtr,MESdrainNode,MESdrainPrimeNode)
            TSTALLOC(MESgateDrainPrimePtr,MESgateNode,MESdrainPrimeNode)
            TSTALLOC(MESgateSourcePrimePtr,MESgateNode,MESsourcePrimeNode)
            TSTALLOC(MESsourceSourcePrimePtr,MESsourceNode,
                    MESsourcePrimeNode)
            TSTALLOC(MESdrainPrimeDrainPtr,MESdrainPrimeNode,MESdrainNode)
            TSTALLOC(MESdrainPrimeGatePtr,MESdrainPrimeNode,MESgateNode)
            TSTALLOC(MESdrainPrimeSourcePrimePtr,MESdrainPrimeNode,
                    MESsourcePrimeNode)
            TSTALLOC(MESsourcePrimeGatePtr,MESsourcePrimeNode,MESgateNode)
            TSTALLOC(MESsourcePrimeSourcePtr,MESsourcePrimeNode,
                    MESsourceNode)
            TSTALLOC(MESsourcePrimeDrainPrimePtr,MESsourcePrimeNode,
                    MESdrainPrimeNode)
            TSTALLOC(MESdrainDrainPtr,MESdrainNode,MESdrainNode)
            TSTALLOC(MESgateGatePtr,MESgateNode,MESgateNode)
            TSTALLOC(MESsourceSourcePtr,MESsourceNode,MESsourceNode)
            TSTALLOC(MESdrainPrimeDrainPrimePtr,MESdrainPrimeNode,
                    MESdrainPrimeNode)
            TSTALLOC(MESsourcePrimeSourcePrimePtr,MESsourcePrimeNode,
                    MESsourcePrimeNode)
        }
    }
    return (OK);
}
