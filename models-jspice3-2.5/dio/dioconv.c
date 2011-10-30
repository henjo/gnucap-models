/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

#include <stdio.h>
#include "spice.h"
#include "diodefs.h"
#include "const.h"
#include "util.h"
#include "sperror.h"

int
DIOconvTest(inModel,ckt)

/* Check the devices for convergence
 */
GENmodel *inModel;
CKTcircuit *ckt;
{
    DIOmodel *model = (DIOmodel*)inModel;
    DIOinstance *here;
    double delvd, vd, cdhat, cd;
    double tol, A1, A2;

    /* loop through all the diode models */
    for ( ; model != NULL; model = model->DIOnextModel) {

        /* loop through all the instances of the model */
        for (here = model->DIOinstances; here != NULL;
                here = here->DIOnextInstance) {
                
            /*  
             *   initialization 
             */

            vd = *(ckt->CKTrhsOld + here->DIOposPrimeNode) -
                    *(ckt->CKTrhsOld + here->DIOnegNode);

            delvd = vd - *(ckt->CKTstate0 + here->DIOvoltage);
            cd    = here->DIOcd;
            cdhat = cd + here->DIOgd*delvd;

            /*
             *   check convergence
             */
            A1  = FABS(cdhat);
            A2  = FABS(cd);
            tol = ckt->CKTreltol*MAX(A1,A2) + ckt->CKTabstol;
            A1 = cdhat - cd;
            if (FABS(A1) > tol) {
                ckt->CKTnoncon++;
                ckt->CKTtroubleElt = (GENinstance *) here;
                return (OK);
            }
        }
    }
    return (OK);
}
