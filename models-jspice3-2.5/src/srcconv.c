/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1987 Kanwar Jit Singh
         1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "misc.h"
#include "srcdefs.h"
#include "sperror.h"
#include "util.h"


int
SRCconvTest(inModel, ckt)

GENmodel *inModel;
CKTcircuit *ckt;
{
    SRCmodel *model = (SRCmodel *)inModel;
    SRCinstance *here;
    int i;
    double diff;
    double prev;
    double tol;
    double rhs;

    for ( ; model != NULL; model = model->SRCnextModel) {
        for (here = model->SRCinstances; here != NULL;
                here = here->SRCnextInstance) {

            if (!here->SRCtree || !here->SRCtree->numVars) continue;

            for (i = 0; i < here->SRCtree->numVars; i++) {
                here->SRCvalues[i] = *(ckt->CKTrhsOld + here->SRCeqns[i]);
            }

            if ((*(here->SRCtree->IFeval))
                (here->SRCtree, ckt->CKTgmin,&rhs,here->SRCvalues,
                here->SRCderivs,&ckt->CKTtime) == OK) {
                /* !! last arg bogus if ft_sim->specSigs changed !! */

                prev = here->SRCprev;
                diff = FABS(prev - rhs);
                if (here->SRCtype == SRC_V) {
                    tol = ckt->CKTreltol * 
                            MAX(FABS(rhs),FABS(prev)) + ckt->CKTvoltTol;
                }
                else {
                    tol = ckt->CKTreltol * 
                            MAX(FABS(rhs),FABS(prev)) + ckt->CKTabstol;
                }

                if (diff > tol) {
                    ckt->CKTnoncon++;
                    ckt->CKTtroubleElt = (GENinstance *) here;
                    return (OK);
                }
            }
            else {
                return (E_BADPARM);
            }
        }
    }
    return (OK);
}
