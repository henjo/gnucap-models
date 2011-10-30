/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

/*
 * This routine performs the device convergence test for
 * BJTs in the circuit.
 */

#include "spice.h"
#include <stdio.h>
#include "bjtdefs.h"
#include "sperror.h"
#include "util.h"


int
BJTconvTest(inModel,ckt)

GENmodel *inModel;
CKTcircuit *ckt;
{
    BJTinstance *here;
    BJTmodel *model = (BJTmodel *) inModel;
    double tol;
    double c0;
    double chat;
    double vbe;
    double vbc;
    double delvbe;
    double delvbc;
    double A1, A2;

    for ( ; model != NULL; model = model->BJTnextModel) {
        for (here = model->BJTinstances; here != NULL;
                here = here->BJTnextInstance) {

            if (model->BJTtype > 0) {
                vbe = *(ckt->CKTrhsOld + here->BJTbasePrimeNode) -
                    *(ckt->CKTrhsOld + here->BJTemitPrimeNode);
                vbc = *(ckt->CKTrhsOld + here->BJTbasePrimeNode) -
                    *(ckt->CKTrhsOld + here->BJTcolPrimeNode);
            }
            else {
                vbe = *(ckt->CKTrhsOld + here->BJTemitPrimeNode) -
                    *(ckt->CKTrhsOld + here->BJTbasePrimeNode);
                vbc = *(ckt->CKTrhsOld + here->BJTcolPrimeNode) -
                    *(ckt->CKTrhsOld + here->BJTbasePrimeNode);
            }
            delvbe = vbe - *(ckt->CKTstate0 + here->BJTvbe);
            delvbc = vbc - *(ckt->CKTstate0 + here->BJTvbc);

            /*
             *   check convergence, collector current
             */
            c0 = here->BJTcc;
            chat = c0 +
                (here->BJTgm + here->BJTgo)*delvbe -
                (here->BJTgo + here->BJTgmu)*delvbc;

            A1 = FABS(chat);
            A2 = FABS(c0);
            tol = ckt->CKTreltol*MAX(A1,A2) + ckt->CKTabstol;

            A1 = chat - c0;
            if (FABS(A1) > tol) {
                ckt->CKTnoncon++;
                ckt->CKTtroubleElt = (GENinstance *) here;
                return (OK); /* no reason to continue - we've failed... */
            }

            /*
             *   check convergence, base current
             */
            c0 = here->BJTcb;
            chat = c0 +
                here->BJTgpi*delvbe +
                here->BJTgmu*delvbc;

            A1 = FABS(chat);
            A2 = FABS(c0);
            tol = ckt->CKTreltol*MAX(A1,A2) + ckt->CKTabstol;

            A1 = chat - c0;
            if (FABS(A1) > tol) {
                ckt->CKTnoncon++;
                ckt->CKTtroubleElt = (GENinstance *) here;
                return (OK); /* no reason to continue - we've failed... */
            }
        }
    }
    return (OK);
}
