/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1990 Jaijeet S. Roychowdhury
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include <math.h>
#include "ltradefs.h"
#include "sperror.h"
#include "util.h"
#include "cktext.h"

#ifdef __STDC__
static void ltra_sbrk(CKTcircuit*,LTRAmodel*,LTRAinstance*);
#else
static void ltra_sbrk();
#endif


int
LTRAaccept(ckt,inModel)

CKTcircuit *ckt;
GENmodel *inModel;
{
    LTRAmodel *model = (LTRAmodel *)inModel;
    LTRAinstance *here;
    int compact = 1;

    if (ckt->CKTmode & MODEINITTRAN) {
        ckt->CKTsizeIncr = 10;
        ckt->CKTtimeIndex = 0;
        ckt->CKTtimeListSize = ckt->CKTfinalTime/ckt->CKTmaxStep + 0.5;
        FREE(ckt->CKTtimePoints);
        ckt->CKTtimePoints =
            (double *)MALLOC(sizeof(double)*ckt->CKTtimeListSize);
    }
    else if (ckt->CKTmode & MODETRAN) {
        ckt->CKTtimeIndex++;
        if (ckt->CKTtimeIndex >= ckt->CKTtimeListSize) {
            /* need more space */

             ckt->CKTsizeIncr = 5 + ckt->CKTtimeIndex*
                (ckt->CKTfinalTime - ckt->CKTtime)/ckt->CKTtime;

            ckt->CKTtimeListSize += ckt->CKTsizeIncr;
            ckt->CKTtimePoints = (double *)REALLOC((char *)
                ckt->CKTtimePoints,sizeof(double)*ckt->CKTtimeListSize);
        }
        *(ckt->CKTtimePoints + ckt->CKTtimeIndex) = ckt->CKTtime;
    }

    /* loop through all the transmission line models */
    for ( ; model != NULL; model = model->LTRAnextModel ) {

        if (ckt->CKTmode & MODEINITTRAN) {
            
            model->LTRAmodelListSize = 10;

            model->LTRAh1dashCoeffs = (double *) 
                MALLOC(sizeof(double)*model->LTRAmodelListSize);
            model->LTRAh2Coeffs = (double *) 
                MALLOC(sizeof(double)*model->LTRAmodelListSize);
            model->LTRAh3dashCoeffs = (double *) 
                MALLOC(sizeof(double)*model->LTRAmodelListSize);
        }
    
        else if (ckt->CKTtimeIndex >= model->LTRAmodelListSize) {
            /* need more space */
            model->LTRAmodelListSize += ckt->CKTsizeIncr;

            model->LTRAh1dashCoeffs = (double *) 
                REALLOC((char *)model->LTRAh1dashCoeffs,
                sizeof(double)*model->LTRAmodelListSize);
            model->LTRAh2Coeffs = (double *) 
                REALLOC((char *)model->LTRAh2Coeffs,
                sizeof(double)*model->LTRAmodelListSize);
            model->LTRAh3dashCoeffs = (double *) 
                REALLOC((char *)model->LTRAh3dashCoeffs,
                sizeof(double)*model->LTRAmodelListSize);
        }

        /* loop through all the instances of the model */
        for (here = model->LTRAinstances; here != NULL;
            here = here->LTRAnextInstance) {

            here->LTRAinput1Old = here->LTRAinput1;
            here->LTRAinput2Old = here->LTRAinput2;

            if (ckt->CKTmode & MODEINITTRAN) {
                here->LTRAinstListSize = 10;

                here->LTRAv1 = (double *)
                    MALLOC(sizeof(double)*here->LTRAinstListSize);
                here->LTRAi1 = (double *)
                    MALLOC(sizeof(double)*here->LTRAinstListSize);
                here->LTRAv2 = (double *)
                    MALLOC(sizeof(double)*here->LTRAinstListSize);
                here->LTRAi2 = (double *)
                    MALLOC(sizeof(double)*here->LTRAinstListSize);
            }

            else if (ckt->CKTtimeIndex >= here->LTRAinstListSize) {
                /* need more space */
                here->LTRAinstListSize += ckt->CKTsizeIncr;

                here->LTRAv1 = (double *) REALLOC((char *)
                    here->LTRAv1,sizeof(double)*(here->LTRAinstListSize));
                here->LTRAi1 = (double *) REALLOC((char *)
                    here->LTRAi1,sizeof(double)*(here->LTRAinstListSize));
                here->LTRAv2 = (double *) REALLOC((char *)
                    here->LTRAv2,sizeof(double)*(here->LTRAinstListSize));
                here->LTRAi2 = (double *) REALLOC((char *)
                    here->LTRAi2,sizeof(double)*(here->LTRAinstListSize));
            }

            *(here->LTRAv1 + ckt->CKTtimeIndex) =
                *(ckt->CKTrhsOld + here->LTRAposNode1) -
                *(ckt->CKTrhsOld + here->LTRAnegNode1) ;
            *(here->LTRAv2 + ckt->CKTtimeIndex) =
                *(ckt->CKTrhsOld + here->LTRAposNode2) -
                *(ckt->CKTrhsOld + here->LTRAnegNode2) ;
            *(here->LTRAi1 + ckt->CKTtimeIndex) =
                *(ckt->CKTrhsOld + here->LTRAbrEq1) ;
            *(here->LTRAi2 + ckt->CKTtimeIndex) =
                *(ckt->CKTrhsOld + here->LTRAbrEq2) ;

            if (ckt->CKTtryToCompact && (ckt->CKTtimeIndex >= 2)) {

                /* figure out if the last 3 points lie on a st. line for
                 * all the terminal variables
                 */
                double t1,t2,t3;

                t1 = *(ckt->CKTtimePoints + ckt->CKTtimeIndex - 2);
                t2 = *(ckt->CKTtimePoints + ckt->CKTtimeIndex - 1);
                t3 = *(ckt->CKTtimePoints + ckt->CKTtimeIndex);

                if (compact) {
                    compact = LTRAstraightLineCheck(
                        t1,*(here->LTRAv1 + ckt->CKTtimeIndex - 2),
                        t2,*(here->LTRAv1 + ckt->CKTtimeIndex - 1),
                        t3,*(here->LTRAv1 + ckt->CKTtimeIndex),
                        model->LTRAstLineReltol,model->LTRAstLineAbstol);
                }
                if (compact) {
                    compact = LTRAstraightLineCheck(
                        t1,*(here->LTRAv2 + ckt->CKTtimeIndex - 2),
                        t2,*(here->LTRAv2 + ckt->CKTtimeIndex - 1),
                        t3,*(here->LTRAv2 + ckt->CKTtimeIndex),
                        model->LTRAstLineReltol,model->LTRAstLineAbstol);
                }
                if (compact) {
                    compact = LTRAstraightLineCheck(
                        t1,*(here->LTRAi1 + ckt->CKTtimeIndex - 2),
                        t2,*(here->LTRAi1 + ckt->CKTtimeIndex - 1),
                        t3,*(here->LTRAi1 + ckt->CKTtimeIndex),
                        model->LTRAstLineReltol,model->LTRAstLineAbstol);
                }
                if (compact) {
                    compact = LTRAstraightLineCheck(
                        t1,*(here->LTRAi2 + ckt->CKTtimeIndex - 2),
                        t2,*(here->LTRAi2 + ckt->CKTtimeIndex - 1),
                        t3,*(here->LTRAi2 + ckt->CKTtimeIndex),
                        model->LTRAstLineReltol,model->LTRAstLineAbstol);
                }
            }

            /*
            *(ckt->CKTstate0 + here->LTRAv1Tot) = 
                (*(here->LTRAv1 + ckt->CKTtimeIndex) +
                *(here->LTRAi1 + ckt->CKTtimeIndex)*model->LTRAimped)*
                model->LTRAattenuation;
            *(ckt->CKTstate0 + here->LTRAv2Tot) = 
                (*(here->LTRAv2 + ckt->CKTtimeIndex) +
                *(here->LTRAi1 + ckt->CKTtimeIndex)*model->LTRAimped)*
                model->LTRAattenuation;

            if (ckt->CKTtimeIndex > 0) {
                ltra_sbrk(ckt,model,here);
            }
            */
        }
    }
    if (ckt->CKTtryToCompact && compact && (ckt->CKTtimeIndex >= 2)) {

        /* last three timepoints have variables lying on a straight
         * line, do a compaction
         */

        model = (LTRAmodel *)inModel;
        for ( ; model != NULL; model = model->LTRAnextModel) {
            for (here = model->LTRAinstances; here != NULL;
                here = here->LTRAnextInstance) {

                *(here->LTRAv1 + ckt->CKTtimeIndex - 1) =
                    *(here->LTRAv1 + ckt->CKTtimeIndex);
                *(here->LTRAv2 + ckt->CKTtimeIndex - 1) =
                    *(here->LTRAv2 + ckt->CKTtimeIndex);
                *(here->LTRAi1 + ckt->CKTtimeIndex - 1) =
                    *(here->LTRAi1 + ckt->CKTtimeIndex);
                *(here->LTRAi2 + ckt->CKTtimeIndex - 1) =
                    *(here->LTRAi2 + ckt->CKTtimeIndex);
            }
        }
    
        *(ckt->CKTtimePoints + ckt->CKTtimeIndex - 1) = 
            *(ckt->CKTtimePoints + ckt->CKTtimeIndex);
        ckt->CKTtimeIndex--;

#ifdef LTRADEBUG
        fprintf(stdout,"compacted at time=%g\n",
            *(ckt->CKTtimePoints+ckt->CKTtimeIndex));
        fflush(stdout);
#endif

    }
    return(OK);
}

#define CHECK(a,b,c) (MAX(MAX(a,b),c)-MIN(MIN(a,b),c) >= \
    FABS(50.0*(.33*ckt->CKTreltol*(a+b+c) + ckt->CKTabstol)))

static void
ltra_sbrk(ckt,model,here)

CKTcircuit *ckt;
LTRAmodel *model;
LTRAinstance *here;
{
    double v1, v2, v3, d1, d2;
    double A1, A2, A3;

    v1 = *(ckt->CKTstate0 + here->LTRAv1Tot);
    v2 = *(ckt->CKTstate1 + here->LTRAv1Tot);
    v3 = ckt->CKTtimeIndex <= 1 ? v2 :
        *(ckt->CKTstate2 + here->LTRAv1Tot);
    d1 = (v1-v2)/ckt->CKTdelta;
    d2 = (v2-v3)/ckt->CKTdeltaOld[1];

    A1 = d1 - d2;
    A2 = FABS(d1);
    A3 = FABS(d2);
    if (FABS(A1) >= model->LTRAreltol*MAX(A2,A3) + model->LTRAabstol
        && CHECK(v1,v2,v3)) {
        (void)CKTsetBreak(ckt,
            *(ckt->CKTtimePoints + ckt->CKTtimeIndex-1) + model->LTRAtd);
        return;
    }

    v1 = *(ckt->CKTstate0 + here->LTRAv2Tot);
    v2 = *(ckt->CKTstate1 + here->LTRAv2Tot);
    v3 = ckt->CKTtimeIndex <= 1 ? v2 :
        *(ckt->CKTstate2 + here->LTRAv2Tot);
    d1 = (v1-v2)/ckt->CKTdelta;
    d2 = (v2-v3)/ckt->CKTdeltaOld[1];

    A1 = d1 - d2;
    A2 = FABS(d1);
    A3 = FABS(d2);
    if (FABS(A1) >= model->LTRAreltol*MAX(A2,A3) + model->LTRAabstol
        && CHECK(v1,v2,v3)) {
        (void)CKTsetBreak(ckt,
            *(ckt->CKTtimePoints + ckt->CKTtimeIndex-1) + model->LTRAtd);
        return;
    }
}
