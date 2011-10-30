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


int
LTRAtrunc(inModel,ckt,timeStep)

GENmodel *inModel;
CKTcircuit *ckt;
double *timeStep;
{
    LTRAmodel* model = (LTRAmodel *)inModel;
    LTRAinstance *here;
    double i1,i2,i3;
    double d1,d2;
    double tmp;
    double tolerance;
    double current_lte;
    double x, y, change, deriv, deriv_delta;
    double A1, A2, A3;
    double time_step;
    int i;
    int maxiter = 2, iterations = 0;

    /* in jspice3, the timeStep passed to the trunc functions is
     * not normalized to integration order, which saves having to
     * call an expensive math operation at each CKTterr() call.
     * We have to undo this here.
     */
    if (ckt->CKTorder == 1)
        time_step = *timeStep;
    else if (ckt->CKTorder == 2)
        time_step = sqrt(*timeStep);
    else
        time_step = exp(log(*timeStep)/ckt->CKTorder);
        
        
    /* loop through all the transmission line models */
    for ( ; model != NULL; model = model->LTRAnextModel) {

        /* loop through all the instances of the model */
        for (here = model->LTRAinstances; here != NULL;
                here = here->LTRAnextInstance) {

            switch (model->LTRAspecialCase) {

            case LTRA_MOD_LC:
            case LTRA_MOD_RLC:

                if (model->LTRAstepLimit == LTRA_MOD_STEPLIMIT) {
                    tmp = model->LTRAtd;
                    time_step = MIN(time_step,tmp);
                }
                else {
                    i1 = ((*(ckt->CKTrhsOld + here->LTRAposNode2)
                        - *(ckt->CKTrhsOld + here->LTRAnegNode2))*
                        model->LTRAadmit +
                        *(ckt->CKTrhsOld + here->LTRAbrEq2))*
                        model->LTRAattenuation;
                    i2 = (*(here->LTRAv2+ ckt->CKTtimeIndex)*
                        model->LTRAadmit +
                        *(here->LTRAi2 + ckt->CKTtimeIndex))*
                        model->LTRAattenuation;
                    i3 = (*(here->LTRAv2 + ckt->CKTtimeIndex - 1)*
                        model->LTRAadmit + 
                        *(here->LTRAi2 + ckt->CKTtimeIndex - 1))*
                        model->LTRAattenuation;
                    d1 = (i1-i2)/(ckt->CKTtime - *(ckt->CKTtimePoints +
                        ckt->CKTtimeIndex));
                    d2 = (i2-i3)/(*(ckt->CKTtimePoints + ckt->CKTtimeIndex)
                        - *(ckt->CKTtimePoints + ckt->CKTtimeIndex - 1));

                    A1 = d1 - d2;
                    A2 = FABS(d1);
                    A3 = FABS(d2);

                    if (FABS(A1) >= model->LTRAreltol*MAX(A2,A3) +
                        model->LTRAabstol) {
                        /* derivitive changing - need to schedule after delay */
                        /* the PREVIOUS point was the breakpoint*/
                        /* the previous timepoint plus the delay */

                        tmp = model->LTRAtd;
                        time_step = MIN(time_step,tmp);
                        break;
                    }

                    i1 = ((*(ckt->CKTrhsOld + here->LTRAposNode1) - 
                        *(ckt->CKTrhsOld + here->LTRAnegNode1))*
                        model->LTRAadmit +
                        *(ckt->CKTrhsOld + here->LTRAbrEq1))*
                        model->LTRAattenuation;
                    i2 = (*(here->LTRAv1+ ckt->CKTtimeIndex)*
                        model->LTRAadmit + 
                        *(here->LTRAi1 + ckt->CKTtimeIndex))*
                        model->LTRAattenuation;
                    i3 = (*(here->LTRAv1 + ckt->CKTtimeIndex - 1)*
                        model->LTRAadmit + 
                        *(here->LTRAi1 + ckt->CKTtimeIndex - 1))*
                        model->LTRAattenuation;
                    d1 = (i1-i2)/(ckt->CKTtime - *(ckt->CKTtimePoints +
                        ckt->CKTtimeIndex));
                    d2 = (i2-i3)/(*(ckt->CKTtimePoints + ckt->CKTtimeIndex)
                        - *(ckt->CKTtimePoints + ckt->CKTtimeIndex - 1));

                    A1 = d1 - d2;
                    A2 = FABS(d1);
                    A3 = FABS(d2);

                    if (FABS(A1) >= model->LTRAreltol*MAX(A2,A3) +
                        model->LTRAabstol) {
                        /* derivitive changing - need to schedule after delay */
                        /* the PREVIOUS point was the breakpoint*/
                        /* the previous timepoint plus the delay */

                        tmp = model->LTRAtd;
                        time_step = MIN(time_step,tmp);
                    }
                }
                break;

            case LTRA_MOD_RC:
            case LTRA_MOD_RG:
                break;

            default:
                return(E_BADPARM);
            }

            /*
             * the above was for the parts of the equations that
             * resemble the lossless equations. Now we need to 
             * estimate the local truncation error in each of the
             * three convolution equations, and if possible adjust
             * the timestep so that all of them remain within some
             * bound. Unfortunately, the expression for the LTE in a
             * convolution operation is complicated and costly to
             * evaluate; in addition, no explicit inverse exists.
             *
             * So what we do here (for the moment) is check to see
             * the current error is acceptable. If so, the timestep
             * is not changed. If not, then an estimate is made for the 
             * new timestep using a few iterations of the
             * newton-raphson method.
             *
             * modification: we change the timestep to half its
             * previous value
             */

            if ((model->LTRAspecialCase == LTRA_MOD_RLC) && 
                (!model->LTRAtruncDontCut)) {
                time_step = MIN(time_step, model->LTRAmaxSafeStep);
            }

            if (model->LTRAlteConType != LTRA_MOD_NOCONTROL) {
                switch(model->LTRAspecialCase) {

                case LTRA_MOD_RLC:
                case LTRA_MOD_RC:
                    tolerance = ckt->CKTtrtol*(ckt->CKTreltol*(
                        FABS(here->LTRAinput1) + FABS(here->LTRAinput2)) 
                        + ckt->CKTabstol);

                    current_lte = LTRAlteCalculate(ckt,(GENmodel *) model,
                        (GENinstance *) here, ckt->CKTtime);

                    if (current_lte >= tolerance) {
                        if (model->LTRAtruncNR) {

                            x = ckt->CKTtime;
                            y = current_lte;
                            for (;;) {
                                deriv_delta =
                                    0.01 * (x - *(ckt->CKTtimePoints +
                                    ckt->CKTtimeIndex));

#ifdef LTRADEBUG
                                if (deriv_delta <= 0.0)
                                    fprintf(stdout,
                    "LTRAtrunc: error: timestep is now less than zero\n");
#endif
                                deriv = LTRAlteCalculate(ckt,
                                    (GENmodel *) model,
                                    (GENinstance *) here,
                                    x + deriv_delta) - y;
                                deriv /= deriv_delta;
                                change = (tolerance - y)/deriv;
                                x += change;
                                if (maxiter == 0) {
                                    if (FABS(change) <= FABS(deriv_delta))
                                        break;
                                }
                                else {
                                    iterations++;
                                    if (iterations >= maxiter) break;
                                }
                                y = LTRAlteCalculate(ckt, (GENmodel *) model,
                                    (GENinstance *) here,x);
                            }

                            tmp = x - *(ckt->CKTtimePoints +
                                ckt->CKTtimeIndex);
                            time_step = MIN(time_step,tmp);
                        }
                        else 
                            time_step *= 0.5;
                    }
                    break;

                case LTRA_MOD_RG:
                case LTRA_MOD_LC:
                    break;

                default:
                    return(E_BADPARM);
                }
            }

        }
#ifdef LTRADEBUG
        if (time_step >= model->LTRAtd) {
            fprintf(stdout,
"LTRAtrunc: Warning: Timestep bigger than delay of line %s\n",
model->LTRAmodName);
            fflush(stdout);
        }
#endif
    }

    /* see note above regarding renormalization */
    tmp = time_step;
    for (i = ckt->CKTorder - 1; i > 0; i--)
        tmp *= time_step;
    *timeStep = tmp;

    return (OK);
}
