/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1989 Takayasu Sakurai
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include <math.h>
#include "mosdefs.h"
#include "sperror.h"
#include "util.h"


int
MOSconvTest(inModel,ckt)

GENmodel *inModel;
CKTcircuit *ckt;
{
    MOSmodel *model = (MOSmodel *)inModel;
    MOSinstance *here;
    double delvbs, delvbd, delvgs, delvds, delvgd;
    double cbhat, cdhat;
    double vbs, vbd, vgs, vds, vgd;
    double tol, A1, A2, A3;

    if (ckt->CKTnoncon > 0)
        /* something else didn't converge, bypass */
        return (OK);

    for ( ; model != NULL; model = model->MOSnextModel) {
        for (here = model->MOSinstances; here!= NULL;
                here = here->MOSnextInstance) {
        
            if (model->MOStype > 0) {
                vbs = *(ckt->CKTrhs + here->MOSbNode) -
                    *(ckt->CKTrhs + here->MOSsNodePrime);
                vgs = *(ckt->CKTrhs + here->MOSgNode) -
                    *(ckt->CKTrhs + here->MOSsNodePrime);
                vds = *(ckt->CKTrhs + here->MOSdNodePrime) -
                    *(ckt->CKTrhs + here->MOSsNodePrime);
            }
            else {
                vbs = *(ckt->CKTrhs + here->MOSsNodePrime) -
                    *(ckt->CKTrhs + here->MOSbNode);
                vgs = *(ckt->CKTrhs + here->MOSsNodePrime) -
                    *(ckt->CKTrhs + here->MOSgNode);
                vds = *(ckt->CKTrhs + here->MOSsNodePrime) -
                    *(ckt->CKTrhs + here->MOSdNodePrime);
            }
            vbd = vbs - vds;
            vgd = vgs - vds;

            delvbs = vbs - *(ckt->CKTstate0 + here->MOSvbs);
            delvbd = vbd - *(ckt->CKTstate0 + here->MOSvbd);
            delvgs = vgs - *(ckt->CKTstate0 + here->MOSvgs);
            delvds = vds - *(ckt->CKTstate0 + here->MOSvds);
            delvgd = vgd - (*(ckt->CKTstate0 + here->MOSvgs) -
                            *(ckt->CKTstate0 + here->MOSvds));

            if (here->MOSmode >= 0) {
                cdhat = here->MOScd -
                    here->MOSgbd*delvbd + here->MOSgmbs*delvbs +
                    here->MOSgm*delvgs + here->MOSgds*delvds;
            }
            else {
                cdhat = here->MOScd -
                    (here->MOSgbd - here->MOSgmbs)*delvbd -
                    here->MOSgm*delvgd + here->MOSgds*delvds;
            }

            /*
             *  check convergence
             */
            A1 = FABS(cdhat);
            A2 = FABS(here->MOScd);
            A3 = cdhat - here->MOScd;
            A3 = FABS(A3);
            tol = ckt->CKTreltol*MAX(A1,A2) + ckt->CKTabstol;
            if (A3 >= tol) { 
                ckt->CKTnoncon++;
                ckt->CKTtroubleElt = (GENinstance *) here;
                /* no reason to continue, we haven't converged */
                return (OK);
            }

            cbhat = here->MOScbs + here->MOScbd +
                here->MOSgbd*delvbd + here->MOSgbs*delvbs;

            A1 = FABS(cbhat);
            A2 = here->MOScbs + here->MOScbd;
            A2 = FABS(A2);
            A3 = cbhat - (here->MOScbs + here->MOScbd);
            A3 = FABS(A3);
            tol = ckt->CKTreltol*MAX(A1,A2) + ckt->CKTabstol;
            if (A3 > tol) {
                ckt->CKTnoncon++;
                ckt->CKTtroubleElt = (GENinstance *) here;
                /* no reason to continue, we haven't converged*/
                return (OK);
            }
        }
    }
    return (OK);
}
