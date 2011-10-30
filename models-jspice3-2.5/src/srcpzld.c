/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
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


/*ARGSUSED*/
int
SRCpzLoad(inModel,ckt,s)

/* actually load the current voltage value into the 
 * sparse matrix previously provided 
 */
GENmodel *inModel;
CKTcircuit *ckt;
SPcomplex *s;
{
    SRCmodel *model = (SRCmodel*)inModel;
    SRCinstance *here;
    int i, j;
    double value, deriv;

    /*  loop through all the Arbitrary source models */
    for ( ; model != NULL; model = model->SRCnextModel) {

        /* loop through all the instances of the model */
        for (here = model->SRCinstances; here != NULL;
                here = here->SRCnextInstance) {
            
            if (here->SRCtype == SRC_V) {

                if (here->SRCacGiven) {
                    /* an ac source,
                     * no effective contribution
                     * diagonal element made 1
                     */
                    *(here->SRCposIbrptr)  +=  1.0 ;
                    *(here->SRCnegIbrptr)  += -1.0 ;
                    *(here->SRCibrIbrptr)  +=  1.0 ;
                    continue;
                }
                if (here->SRCposNode) {
                    *(here->SRCposIbrptr) += 1.0;
                    *(here->SRCibrPosptr) += 1.0;
                }
                if (here->SRCnegNode) {
                    *(here->SRCnegIbrptr) -= 1.0;
                    *(here->SRCibrNegptr) -= 1.0;
                }
                if (here->SRCccCoeffGiven) {
                    *(here->SRCibrContBrptr) += here->SRCcoeff;
                }
                else if (here->SRCvcCoeffGiven) {
                    *(here->SRCibrContPosptr) -= here->SRCcoeff;
                    *(here->SRCibrContNegptr) += here->SRCcoeff;
                }
                else if (here->SRCtree) {
                    /* a dc source,
                     * the connecting nodes are shorted
                     * unless it has dependence
                     */

                    for (i = 0; i < here->SRCtree->numVars; i++) {
                        here->SRCvalues[i] =
                            *(ckt->CKTrhsOld + here->SRCeqns[i]);
                    }

                    if ((*(here->SRCtree->IFeval))
                        (here->SRCtree, ckt->CKTgmin,&value,here->SRCvalues,
                            here->SRCderivs,(double*)NULL) == OK) {
                        /* !! last arg bogus if ft_sim->specSigs changed !! */

                        for (i = 0; i < here->SRCtree->numVars; i++) {
                            *(here->SRCposptr[i]) -= here->SRCderivs[i];
                        }
                    }
                    else {
                        return (E_BADPARM);
                    }
                    
                }
            }
            else {
                if (here->SRCacGiven) {
                    continue;
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
                    for (i = 0; i < here->SRCtree->numVars; i++) {
                        here->SRCvalues[i] =
                            *(ckt->CKTrhsOld + here->SRCeqns[i]);
                    }

                    if ((*(here->SRCtree->IFeval))
                        (here->SRCtree, ckt->CKTgmin,&value,
                        here->SRCvalues,here->SRCderivs,(double*)NULL) == OK) {
                        /* !! last arg bogus if ft_sim->specSigs changed !! */

                        for (j = 0,i = 0; i < here->SRCtree->numVars; i++) {
                            deriv = here->SRCderivs[i];
                            *(here->SRCposptr[j++]) += deriv;
                            *(here->SRCposptr[j++]) -= deriv;
                        }
                    }
                    else {
                        return (E_BADPARM);
                    }
                }
            }
        }
    }
    return (OK);
}
