/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "mosdefs.h"
#include "sperror.h"


int
MOSacLoad(inModel,ckt)

GENmodel *inModel;
CKTcircuit *ckt;
{
    MOSmodel *model = (MOSmodel *)inModel;
    MOSinstance *here;
    double xgs;
    double xgd;
    double xgb;
    double xbd;
    double xbs;
    double dcon;
    double scon;
    double omega = ckt->CKTomega;

    for ( ; model != NULL; model = model->MOSnextModel) {
        for (here = model->MOSinstances; here!= NULL;
                here = here->MOSnextInstance) {
        
            /*
             *     meyer's model parameters
             */
            xgs = *(ckt->CKTstate0 + here->MOScapgs) + 
                    *(ckt->CKTstate0 + here->MOScapgs) +
                    here->MOSgateSourceOverlapCap;
            xgd = *(ckt->CKTstate0 + here->MOScapgd) + 
                    *(ckt->CKTstate0 + here->MOScapgd) +
                    here->MOSgateDrainOverlapCap;
            xgb = *(ckt->CKTstate0 + here->MOScapgb) + 
                    *(ckt->CKTstate0 + here->MOScapgb) +
                    here->MOSgateBulkOverlapCap;

            xgs *= omega;
            xgd *= omega;
            xgb *= omega;
            xbd = here->MOScapbd * omega;
            xbs = here->MOScapbs * omega;

            dcon = here->MOSdrainConductance;
            scon = here->MOSsourceConductance;
            /*
             *    load matrix
             */
            *(here->MOSGgPtr  +1) += xgd + xgs + xgb;
            *(here->MOSBbPtr  +1) += xgb + xbd + xbs;
            *(here->MOSDPdpPtr+1) += xgd + xbd;
            *(here->MOSSPspPtr+1) += xgs + xbs;
            *(here->MOSGbPtr  +1) -= xgb;
            *(here->MOSGdpPtr +1) -= xgd;
            *(here->MOSGspPtr +1) -= xgs;
            *(here->MOSBgPtr  +1) -= xgb;
            *(here->MOSBdpPtr +1) -= xbd;
            *(here->MOSBspPtr +1) -= xbs;
            *(here->MOSDPgPtr +1) -= xgd;
            *(here->MOSDPbPtr +1) -= xbd;
            *(here->MOSSPgPtr +1) -= xgs;
            *(here->MOSSPbPtr +1) -= xbs;
            *(here->MOSDdPtr) += dcon;
            *(here->MOSSsPtr) += scon;
            *(here->MOSBbPtr) += here->MOSgbd + here->MOSgbs;
            *(here->MOSDdpPtr) -= dcon;
            *(here->MOSSspPtr) -= scon;
            *(here->MOSBdpPtr) -= here->MOSgbd;
            *(here->MOSBspPtr) -= here->MOSgbs;
            *(here->MOSDPdPtr) -= dcon;
            *(here->MOSSPsPtr) -= scon;

            if (here->MOSmode > 0) {

                *(here->MOSDPdpPtr) +=
                    dcon + here->MOSgds + here->MOSgbd;
                *(here->MOSSPspPtr) +=
                    scon + here->MOSgds + here->MOSgbs +
                        here->MOSgm + here->MOSgmbs;

                *(here->MOSDPgPtr)  += here->MOSgm;
                *(here->MOSDPbPtr)  += -here->MOSgbd + here->MOSgmbs;
                *(here->MOSDPspPtr) -=
                    here->MOSgds + here->MOSgm + here->MOSgmbs;

                *(here->MOSSPgPtr)  -= here->MOSgm;
                *(here->MOSSPbPtr)  -= here->MOSgbs + here->MOSgmbs;
                *(here->MOSSPdpPtr) -= here->MOSgds;
            }
            else {
                *(here->MOSDPdpPtr) +=
                    dcon + here->MOSgds + here->MOSgbd +
                        here->MOSgm + here->MOSgmbs;
                *(here->MOSSPspPtr) +=
                    scon + here->MOSgds + here->MOSgbs;

                *(here->MOSDPgPtr)  -= here->MOSgm;
                *(here->MOSDPbPtr)  -= here->MOSgbd + here->MOSgmbs;
                *(here->MOSDPspPtr) -= here->MOSgds;

                *(here->MOSSPgPtr)  += here->MOSgm;
                *(here->MOSSPbPtr)  -= here->MOSgbs - here->MOSgmbs;
                *(here->MOSSPdpPtr) -=
                    here->MOSgds + here->MOSgm + here->MOSgmbs;
            }
        }
    }
    return (OK);
}
