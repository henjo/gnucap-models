/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "capdefs.h"
#include "util.h"
#include "sperror.h"
#include "niext.h"


int
CAPload(inModel,ckt)

/* actually load the current capacitance value into the 
 * sparse matrix previously provided 
 */
GENmodel *inModel;
CKTcircuit *ckt;
{
    CAPmodel *model = (CAPmodel*)inModel;
    CAPinstance *here;
    int error;

    if (ckt->CKTmode & MODEDC)
        return (OK);

    if (ckt->CKTmode & MODEINITFLOAT) {
        double ag0 = ckt->CKTag[0];
        double qcap;

        /* loop through all the capacitor models */
        for ( ; model != NULL; model = model->CAPnextModel) {

            /* loop through all the instances of the model */
            for (here = model->CAPinstances; here != NULL;
                    here = here->CAPnextInstance) {

                qcap = here->CAPcapac * 
                    (*(ckt->CKTrhsOld + here->CAPposNode) - 
                    *(ckt->CKTrhsOld + here->CAPnegNode));

                *(ckt->CKTstate0 + here->CAPqcap) = qcap;

                *(ckt->CKTstate0 + here->CAPccap) =
                    ag0*qcap + here->CAPceq;

                if (here->CAPposNode == 0) {
                    *(ckt->CKTrhs + here->CAPnegNode) += here->CAPceq;
                    *(here->CAPnegNegptr) += here->CAPgeq;
                    continue;
                }
                if (here->CAPnegNode == 0) {
                    *(ckt->CKTrhs + here->CAPposNode) -= here->CAPceq;
                    *(here->CAPposPosptr) += here->CAPgeq;
                    continue;
                }
                *(here->CAPposPosptr) += here->CAPgeq;
                *(here->CAPnegNegptr) += here->CAPgeq;
                *(here->CAPposNegptr) -= here->CAPgeq;
                *(here->CAPnegPosptr) -= here->CAPgeq;
                *(ckt->CKTrhs + here->CAPposNode) -= here->CAPceq;
                *(ckt->CKTrhs + here->CAPnegNode) += here->CAPceq;
            }
        }
        return (OK);
    }

    if (ckt->CKTmode & MODEINITPRED) {

        /* loop through all the capacitor models */
        for ( ; model != NULL; model = model->CAPnextModel) {

            /* loop through all the instances of the model */
            for (here = model->CAPinstances; here != NULL;
                    here = here->CAPnextInstance) {

                here->CAPgeq = ckt->CKTag[0] * here->CAPcapac;

                NI_SUM(ckt,here->CAPceq,here->CAPqcap);

                if (here->CAPposNode == 0) {
                    *(ckt->CKTrhs + here->CAPnegNode) += here->CAPceq;
                    *(here->CAPnegNegptr) += here->CAPgeq;
                    continue;
                }
                if (here->CAPnegNode == 0) {
                    *(ckt->CKTrhs + here->CAPposNode) -= here->CAPceq;
                    *(here->CAPposPosptr) += here->CAPgeq;
                    continue;
                }
                *(here->CAPposPosptr) += here->CAPgeq;
                *(here->CAPnegNegptr) += here->CAPgeq;
                *(here->CAPposNegptr) -= here->CAPgeq;
                *(here->CAPnegPosptr) -= here->CAPgeq;
                *(ckt->CKTrhs + here->CAPposNode) -= here->CAPceq;
                *(ckt->CKTrhs + here->CAPnegNode) += here->CAPceq;
            }
        }
        return (OK);
    }

    if (ckt->CKTmode & MODEINITTRAN) {

        /* loop through all the capacitor models */
        for ( ; model != NULL; model = model->CAPnextModel) {

            /* loop through all the instances of the model */
            for (here = model->CAPinstances; here != NULL;
                    here = here->CAPnextInstance) {

                if (ckt->CKTmode & MODEUIC) {
                    *(ckt->CKTstate1 + here->CAPqcap) =
                        here->CAPcapac * here->CAPinitCond;
                }
                else {
                    *(ckt->CKTstate1 + here->CAPqcap) =
                        here->CAPcapac * 
                        (*(ckt->CKTrhsOld + here->CAPposNode) - 
                        *(ckt->CKTrhsOld + here->CAPnegNode));   
                }

                here->CAPgeq = ckt->CKTag[0] * here->CAPcapac;

                NI_SUM(ckt,here->CAPceq,here->CAPqcap);

                *(here->CAPposPosptr) += here->CAPgeq;
                *(here->CAPnegNegptr) += here->CAPgeq;
                *(here->CAPposNegptr) -= here->CAPgeq;
                *(here->CAPnegPosptr) -= here->CAPgeq;
                *(ckt->CKTrhs + here->CAPposNode) -= here->CAPceq;
                *(ckt->CKTrhs + here->CAPnegNode) += here->CAPceq;
            }
        }
    }
    return (OK);
}
