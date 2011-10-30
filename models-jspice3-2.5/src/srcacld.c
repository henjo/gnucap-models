/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1987 Kanwar Jit Singh
         1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "srcdefs.h"
#include "sperror.h"
#include "util.h"


int
SRCacLoad(inModel,ckt)

GENmodel *inModel;
CKTcircuit *ckt;
{

    /*
     * Actually load the current voltage value into the 
     * sparse matrix previously provided. The values have
     * been precomputed and stored with the instance model.
     */

    SRCmodel *model = (SRCmodel*)inModel;
    SRCinstance *here;
    int i, j;
    double *derivs;
    double rhs;

    /*  loop through all the Arbitrary source models */
    for ( ; model != NULL; model = model->SRCnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->SRCinstances; here != NULL;
                here = here->SRCnextInstance) {
            
            /* If SRCtree:
             * Get the function and its derivatives from the
             * field in the instance structure. The field is 
             * an array of doubles holding the rhs, and the
             * entries of the jacobian.
             */

            if (here->SRCtype == SRC_V) {

                if (here->SRCposNode) {
                    *(here->SRCposIbrptr) += 1.0;
                    *(here->SRCibrPosptr) += 1.0;
                }
                if (here->SRCnegNode) {
                    *(here->SRCnegIbrptr) -= 1.0;
                    *(here->SRCibrNegptr) -= 1.0;
                }

                if (here->SRCacGiven) {
                    *(ckt->CKTrhs  + (here->SRCbranch)) += here->SRCacReal;
                    *(ckt->CKTirhs + (here->SRCbranch)) += here->SRCacImag;
                }
                else if (here->SRCccCoeffGiven) {
                    *(here->SRCibrContBrptr) += here->SRCcoeff;
                }
                if (here->SRCvcCoeffGiven) {
                    *(here->SRCibrContPosptr) -= here->SRCcoeff;
                    *(here->SRCibrContNegptr) += here->SRCcoeff;
                }
                else if (here->SRCtree) {
                    derivs = here->SRCacValues;
                    rhs = (here->SRCacValues)[here->SRCtree->numVars];
                    for (i = 0; i < here->SRCtree->numVars; i++) {
                        *(here->SRCposptr[i]) -= derivs[i];
                    }
                    *(ckt->CKTrhs+(here->SRCbranch)) += rhs;
                }
            }
            else {

                if (here->SRCacGiven) {
                    *(ckt->CKTrhs + (here->SRCposNode)) += 
                        here->SRCacReal;
                    *(ckt->CKTrhs + (here->SRCnegNode)) -= 
                        here->SRCacReal;
                    *(ckt->CKTirhs + (here->SRCposNode)) += 
                        here->SRCacImag;
                    *(ckt->CKTirhs + (here->SRCnegNode)) -= 
                        here->SRCacImag;
                }
                else if (here->SRCccCoeffGiven) {
                    *(here->SRCposContBrptr) += here->SRCcoeff;
                    *(here->SRCnegContBrptr) -= here->SRCcoeff;
                }
                else if (here->SRCvcCoeffGiven) {
                    *(here->SRCposContPosptr) += here->SRCcoeff;
                    *(here->SRCposContNegptr) -= here->SRCcoeff;
                    *(here->SRCnegContPosptr) -= here->SRCcoeff;
                    *(here->SRCnegContNegptr) += here->SRCcoeff;
                }
                else if (here->SRCtree) {
                    derivs = here->SRCacValues;
                    rhs = (here->SRCacValues)[here->SRCtree->numVars];
                    for (j = 0,i = 0; i < here->SRCtree->numVars; i++) {
                        *(here->SRCposptr[j++]) += derivs[i];
                        *(here->SRCposptr[j++]) -= derivs[i];
                    }
                    *(ckt->CKTrhs+(here->SRCposNode)) -= rhs;
                    *(ckt->CKTrhs+(here->SRCnegNode)) += rhs;
                }
            }
        }
    }
    return (OK);
}
