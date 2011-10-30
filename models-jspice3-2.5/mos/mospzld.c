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
MOSpzLoad(inModel,ckt,s)

GENmodel *inModel;
CKTcircuit *ckt;
SPcomplex *s;
{
    MOSmodel *model = (MOSmodel *)inModel;
    MOSinstance *here;
    int xnrm;
    int xrev;
    double xgs;
    double xgd;
    double xgb;
    double xbd;
    double xbs;
    double capgs;
    double capgd;
    double capgb;

    for ( ; model != NULL; model = model->MOSnextModel) {
        for (here = model->MOSinstances; here!= NULL;
                here = here->MOSnextInstance) {
        
            if (here->MOSmode < 0) {
                xnrm=0;
                xrev=1;
            }
            else {
                xnrm=1;
                xrev=0;
            }
            /*
             *     meyer's model parameters
             */
            capgs = ( 2* *(ckt->CKTstate0+here->MOScapgs)+ 
                      here->MOSgateSourceOverlapCap );
            capgd = ( 2* *(ckt->CKTstate0+here->MOScapgd)+ 
                      here->MOSgateDrainOverlapCap );
            capgb = ( 2* *(ckt->CKTstate0+here->MOScapgb)+ 
                      here->MOSgateBulkOverlapCap );
            xgs = capgs;
            xgd = capgd;
            xgb = capgb;
            xbd = here->MOScapbd;
            xbs = here->MOScapbs;
            /* printf("mos: xgs=%g, xgd=%g, xgb=%g, xbd=%g, xbs=%g\n",
                    xgs,xgd,xgb,xbd,xbs); */
            /*
             *    load matrix
             */

            *(here->MOSGgPtr     ) += (xgd+xgs+xgb)*s->real;
            *(here->MOSGgPtr   +1) += (xgd+xgs+xgb)*s->imag;
            *(here->MOSBbPtr     ) += (xgb+xbd+xbs)*s->real;
            *(here->MOSBbPtr   +1) += (xgb+xbd+xbs)*s->imag;
            *(here->MOSDPdpPtr   ) += (xgd+xbd)*s->real;
            *(here->MOSDPdpPtr +1) += (xgd+xbd)*s->imag;
            *(here->MOSSPspPtr   ) += (xgs+xbs)*s->real;
            *(here->MOSSPspPtr +1) += (xgs+xbs)*s->imag;
            *(here->MOSGbPtr     ) -= xgb*s->real;
            *(here->MOSGbPtr   +1) -= xgb*s->imag;
            *(here->MOSGdpPtr    ) -= xgd*s->real;
            *(here->MOSGdpPtr  +1) -= xgd*s->imag;
            *(here->MOSGspPtr    ) -= xgs*s->real;
            *(here->MOSGspPtr  +1) -= xgs*s->imag;
            *(here->MOSBgPtr     ) -= xgb*s->real;
            *(here->MOSBgPtr   +1) -= xgb*s->imag;
            *(here->MOSBdpPtr    ) -= xbd*s->real;
            *(here->MOSBdpPtr  +1) -= xbd*s->imag;
            *(here->MOSBspPtr    ) -= xbs*s->real;
            *(here->MOSBspPtr  +1) -= xbs*s->imag;
            *(here->MOSDPgPtr    ) -= xgd*s->real;
            *(here->MOSDPgPtr  +1) -= xgd*s->imag;
            *(here->MOSDPbPtr    ) -= xbd*s->real;
            *(here->MOSDPbPtr  +1) -= xbd*s->imag;
            *(here->MOSSPgPtr    ) -= xgs*s->real;
            *(here->MOSSPgPtr  +1) -= xgs*s->imag;
            *(here->MOSSPbPtr    ) -= xbs*s->real;
            *(here->MOSSPbPtr  +1) -= xbs*s->imag;

            *(here->MOSDdPtr) += here->MOSdrainConductance;
            *(here->MOSSsPtr) += here->MOSsourceConductance;
            *(here->MOSBbPtr) += here->MOSgbd+here->MOSgbs;

            *(here->MOSDPdpPtr) += here->MOSdrainConductance+
                    here->MOSgds+here->MOSgbd+
                    xrev*(here->MOSgm+here->MOSgmbs);

            *(here->MOSSPspPtr) += here->MOSsourceConductance+
                    here->MOSgds+here->MOSgbs+
                    xnrm*(here->MOSgm+here->MOSgmbs);

            *(here->MOSDdpPtr) -= here->MOSdrainConductance;
            *(here->MOSSspPtr) -= here->MOSsourceConductance;
            *(here->MOSBdpPtr) -= here->MOSgbd;
            *(here->MOSBspPtr) -= here->MOSgbs;
            *(here->MOSDPdPtr) -= here->MOSdrainConductance;
            *(here->MOSDPgPtr) += (xnrm-xrev)*here->MOSgm;
            *(here->MOSDPbPtr) += -here->MOSgbd+(xnrm-xrev)*here->MOSgmbs;
            *(here->MOSDPspPtr) -= here->MOSgds+
                    xnrm*(here->MOSgm+here->MOSgmbs);

            *(here->MOSSPgPtr) -= (xnrm-xrev)*here->MOSgm;
            *(here->MOSSPsPtr) -= here->MOSsourceConductance;
            *(here->MOSSPbPtr) -= here->MOSgbs+(xnrm-xrev)*here->MOSgmbs;
            *(here->MOSSPdpPtr) -= here->MOSgds+
                    xrev*(here->MOSgm+here->MOSgmbs);
        }
    }
    return (OK);
}
