/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

/* actually load the current inductance value into the 
 * sparse matrix previously provided 
 */

#include "spice.h"
#include <stdio.h>
#include <math.h>
#include "inddefs.h"
#include "util.h"
#include "sperror.h"
#include "niext.h"


int
INDload(inModel,ckt)

GENmodel *inModel;
CKTcircuit *ckt;
{
    INDmodel *model = (INDmodel*)inModel;
    INDinstance *here;
    double factor;
    int error;
#ifdef MUTUAL
    MUTinstance *muthere;
    MUTmodel *mutmodel;
#endif

    if (ckt->CKTmode & MODEDC)
        return (OK);

    if (ckt->CKTmode & MODEINITFLOAT) {

#ifdef MUTUAL
        mutmodel = (MUTmodel *)model->INDmutModels;

        /* loop through all the mutual inductor models */
        for ( ; mutmodel != NULL; mutmodel = mutmodel->MUTnextModel) {

            /* loop through all the instances of the model */
            for (muthere = mutmodel->MUTinstances; muthere != NULL;
                    muthere = muthere->MUTnextInstance) {

                factor = muthere->MUTfactor;
                *(ckt->CKTstate0 + muthere->MUTind1->INDflux) +=
                    factor * *(ckt->CKTrhsOld +
                        muthere->MUTind2->INDbrEq);

                *(ckt->CKTstate0 + muthere->MUTind2->INDflux) +=
                    factor * *(ckt->CKTrhsOld +
                        muthere->MUTind1->INDbrEq);

                factor *= ckt->CKTag[0];
                *(muthere->MUTbr1br2) -= factor;
                *(muthere->MUTbr2br1) -= factor;
            }
        }
#endif
        /*  loop through all the inductor models */
        for ( ; model != NULL; model = model->INDnextModel) {

            /* loop through all the instances of the model */
            for (here = model->INDinstances; here != NULL;
                    here = here->INDnextInstance) {

                *(ckt->CKTstate0 + here->INDflux) +=
                    here->INDinduct * *(ckt->CKTrhsOld + here->INDbrEq) -
                    here->INDprevFlux;

                here->INDprevFlux = *(ckt->CKTstate0 + here->INDflux);

                *(ckt->CKTstate0 + here->INDvolt) =
                    ckt->CKTag[0] * *(ckt->CKTstate0 + here->INDflux) +
                    here->INDveq;

                *(ckt->CKTrhs + here->INDbrEq) += here->INDveq;
                *(here->INDibrIbrptr) -= here->INDreq;
            }
        }
        return (OK);
    }

    if (ckt->CKTmode & MODEINITPRED) {

#ifdef MUTUAL
        mutmodel = (MUTmodel *)model->INDmutModels;

        /*  loop through all the mutual inductor models */
        for ( ; mutmodel != NULL; mutmodel = mutmodel->MUTnextModel) {

            /* loop through all the instances of the model */
            for (muthere = mutmodel->MUTinstances; muthere != NULL;
                    muthere = muthere->MUTnextInstance) {

                factor = muthere->MUTfactor*ckt->CKTag[0];
                *(muthere->MUTbr1br2) -= factor;
                *(muthere->MUTbr2br1) -= factor;
            }
        }
#endif /* MUTUAL */

        /*  loop through all the inductor models */
        for ( ; model != NULL; model = model->INDnextModel) {

            /* loop through all the instances of the model */
            for (here = model->INDinstances; here != NULL;
                    here = here->INDnextInstance) {

                here->INDreq = ckt->CKTag[0] * here->INDinduct;

                NI_SUM(ckt,here->INDveq,here->INDflux);
                
                *(ckt->CKTrhs + here->INDbrEq) += here->INDveq;
                *(here->INDibrIbrptr) -= here->INDreq;

                here->INDprevFlux = 0;
                *(ckt->CKTstate0 + here->INDflux) = 0;
            }
        }
        return (OK);
    }

    if (ckt->CKTmode & MODEINITTRAN) {

#ifdef MUTUAL
        mutmodel = (MUTmodel *)model->INDmutModels;

        /* loop through all the mutual inductor models */
        for ( ; mutmodel != NULL; mutmodel = mutmodel->MUTnextModel) {

            /* loop through all the instances of the model */
            for (muthere = mutmodel->MUTinstances; muthere != NULL;
                    muthere = muthere->MUTnextInstance) {

                factor = muthere->MUTfactor;
                if (ckt->CKTmode & MODEUIC) {

                    *(ckt->CKTstate1 + muthere->MUTind1->INDflux) +=
                        factor * muthere->MUTind2->INDinitCond;

                    *(ckt->CKTstate1 + muthere->MUTind2->INDflux) +=
                        factor * muthere->MUTind1->INDinitCond;
                }
                else {
                    *(ckt->CKTstate1 + muthere->MUTind1->INDflux) +=
                        factor * *(ckt->CKTrhsOld +
                        muthere->MUTind2->INDbrEq);

                    *(ckt->CKTstate1 + muthere->MUTind2->INDflux) +=
                        factor * *(ckt->CKTrhsOld +
                        muthere->MUTind1->INDbrEq);
                }
                factor *= ckt->CKTag[0];
                *(muthere->MUTbr1br2) -= factor;
                *(muthere->MUTbr2br1) -= factor;
            }
        }
#endif /* MUTUAL */

        /*  loop through all the inductor models */
        for ( ; model != NULL; model = model->INDnextModel) {

            /* loop through all the instances of the model */
            for (here = model->INDinstances; here != NULL;
                    here = here->INDnextInstance) {

                if (ckt->CKTmode & MODEUIC) {
                    *(ckt->CKTstate1 + here->INDflux) += here->INDinduct *
                            here->INDinitCond;
                }
                else {
                    *(ckt->CKTstate1 + here->INDflux) += here->INDinduct *
                            *(ckt->CKTrhsOld + here->INDbrEq);
                }
                here->INDprevFlux = 0;
                *(ckt->CKTstate0 + here->INDflux) = 0;

                here->INDreq = ckt->CKTag[0] * here->INDinduct;

                NI_SUM(ckt,here->INDveq,here->INDflux);
                
                *(ckt->CKTrhs + here->INDbrEq) += here->INDveq;
                *(here->INDibrIbrptr) -= here->INDreq;
            }
        }
    }
    return (OK);
}
