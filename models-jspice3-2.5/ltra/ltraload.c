/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1990 Jaijeet S. Roychowdhury
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include <math.h>
#include "util.h"
#include "ltradefs.h"
#include "sperror.h"

struct ltrastuff {
    double ls_t1, ls_t2, ls_t3;
    double ls_qf1, ls_qf2, ls_qf3;
    double ls_lf2, ls_lf3;
    int ls_saved,ls_over;
    int ls_qinterp;
};


#ifdef __STDC__
static int ltra_pred(CKTcircuit*,LTRAmodel*,LTRAinstance*,struct ltrastuff*);
static double ltra_interp(LTRAinstance*,struct ltrastuff*,double*);
static int ltra_lin_interp(double,struct ltrastuff*);
static int ltra_quad_interp(double,struct ltrastuff*);
#else
static int ltra_pred();
static double ltra_interp();
static int ltra_lin_interp();
static int ltra_quad_interp();
#endif

int
LTRAload(inModel,ckt)

GENmodel *inModel;
CKTcircuit *ckt;
/* load the appropriate values for the current timepoint into the 
 * sparse matrix and the right-hand-side vector
 */
{
    LTRAmodel *model = (LTRAmodel *)inModel;
    LTRAinstance *here;
    struct ltrastuff ls;
    double dummy1, dummy2;
    int i;

    /* loop through all the transmission line models */
    for ( ; model != NULL; model = model->LTRAnextModel) {

        if (ckt->CKTmode & (MODEINITTRAN | MODEINITPRED)) {

            switch (model->LTRAspecialCase) {

                case LTRA_MOD_RLC:
                    /*
                     * set up lists of values of the functions at the
                     * necessary timepoints. 
                     */
                    LTRArlcCoeffsSetup(ckt,(GENmodel*)model);

                case LTRA_MOD_LC:

                    /* setting up the coefficients for interpolation */
                    if (ckt->CKTtime > model->LTRAtd) {
                        ls.ls_over = 1;

                        /* serious hack - fix! */
                        dummy1 = ckt->CKTtime - model->LTRAtd; 
                        for (i = ckt->CKTtimeIndex; i >= 0; i--) {
                            if (*(ckt->CKTtimePoints + i) < dummy1) {
                                break;
                            }
                        }

                        if (i == ckt->CKTtimeIndex) i--;
                        ls.ls_saved = i;

                        ls.ls_t2 = *(ckt->CKTtimePoints + i);
                        ls.ls_t3 = *(ckt->CKTtimePoints + i + 1);
                        /* linear interpolation */
                        (void)ltra_lin_interp(dummy1,&ls);

                        ls.ls_qinterp =
                            ((model->LTRAhowToInterp ==
                                LTRA_MOD_QUADINTERP) ||
                            (model->LTRAhowToInterp ==
                                LTRA_MOD_MIXEDINTERP));

                        if ((i != 0) && ls.ls_qinterp) {
                            /* quadratic interpolation */

                            ls.ls_t1 = *(ckt->CKTtimePoints + i - 1);
                            (void)ltra_quad_interp(dummy1,&ls);
                        }
                        /* interpolation coefficients set-up */
                    }
                    else {
                        ls.ls_over = 0;
                    }
                    break;

                case LTRA_MOD_RC:
                    /*
                     * set up lists of values of the coefficients at the
                     * necessary timepoints. 
                     */
                    LTRArcCoeffsSetup(ckt,(GENmodel*)model);
                    break;

                case LTRA_MOD_RG:
                    break;

                default:
                    return (E_BADPARM);
            }                            
        }


        /* loop through all the instances of the model */
        for (here = model->LTRAinstances; here != NULL;
                here = here->LTRAnextInstance) {

            if ((ckt->CKTmode & MODEDC) || 
                (model->LTRAspecialCase == LTRA_MOD_RG)) {

                switch (model->LTRAspecialCase) {

                    case LTRA_MOD_RG:

                        *(here->LTRAibr1Pos1Ptr) += 1.0;
                        *(here->LTRAibr1Neg1Ptr) -= 1.0;
                        *(here->LTRAibr1Pos2Ptr) -= model->LTRAcoshlrootGR;
                        *(here->LTRAibr1Neg2Ptr) += model->LTRAcoshlrootGR;
                        *(here->LTRAibr1Ibr2Ptr) +=
                            (1+ckt->CKTgmin)*model->LTRArRsLrGRorG;

                        *(here->LTRAibr2Ibr2Ptr) += model->LTRAcoshlrootGR;
                        *(here->LTRAibr2Pos2Ptr) -=
                            (1+ckt->CKTgmin)*model->LTRArGsLrGRorR;
                        *(here->LTRAibr2Neg2Ptr) +=
                            (1+ckt->CKTgmin)*model->LTRArGsLrGRorR;
                        *(here->LTRAibr2Ibr1Ptr) += 1.0;

                        *(here->LTRApos1Ibr1Ptr) += 1.0;
                        *(here->LTRAneg1Ibr1Ptr) -= 1.0;
                        *(here->LTRApos2Ibr2Ptr) += 1.0;
                        *(here->LTRAneg2Ibr2Ptr) -= 1.0;

                        /* Somewhere else, we have fixed the matrix with zero
                         * entries so that SMPpreOrder doesn't have fits
                         */
                        break;

                    case LTRA_MOD_LC:
                    case LTRA_MOD_RLC:
                    case LTRA_MOD_RC:

                        /* load a simple resistor */

                        *(here->LTRApos1Ibr1Ptr) += 1.0;
                        *(here->LTRAneg1Ibr1Ptr) -= 1.0;
                        *(here->LTRApos2Ibr2Ptr) += 1.0;
                        *(here->LTRAneg2Ibr2Ptr) -= 1.0;

                        *(here->LTRAibr1Ibr1Ptr) += 1.0;
                        *(here->LTRAibr1Ibr2Ptr) += 1.0;
                        *(here->LTRAibr2Pos1Ptr) += 1.0;
                        *(here->LTRAibr2Pos2Ptr) -= 1.0;
                        *(here->LTRAibr2Ibr1Ptr) -=
                            model->LTRAresist*model->LTRAlength;
                        break;

                    default:
                        return(E_BADPARM);
                }
                continue;
            }

            /*
             * all cases other than DC or the RG case
             */

            /* first timepoint after zero */
            if (ckt->CKTmode & MODEINITTRAN) {
                if (!(ckt->CKTmode & MODEUIC)) {

                    here->LTRAinitVolt1 = 
                        ( *(ckt->CKTrhsOld + here->LTRAposNode1)
                        - *(ckt->CKTrhsOld + here->LTRAnegNode1) );
                    here->LTRAinitVolt2 = 
                        ( *(ckt->CKTrhsOld + here->LTRAposNode2)
                        - *(ckt->CKTrhsOld + here->LTRAnegNode2) );
                    here->LTRAinitCur1 = *(ckt->CKTrhsOld + here->LTRAbrEq1);
                    here->LTRAinitCur2 = *(ckt->CKTrhsOld + here->LTRAbrEq2);
                }
            }

            /* matrix loading  */
            switch (model->LTRAspecialCase) {

                case LTRA_MOD_RLC:

                    /* loading for convolution parts' first terms */

                    dummy1 = model->LTRAadmit*model->LTRAh1dashFirstCoeff;
                    *(here->LTRAibr1Pos1Ptr) += dummy1;
                    *(here->LTRAibr1Neg1Ptr) -= dummy1;
                    *(here->LTRAibr2Pos2Ptr) += dummy1;
                    *(here->LTRAibr2Neg2Ptr) -= dummy1;

                case LTRA_MOD_LC:

                    /* this section loads for the parts of the equations
                     * that resemble the lossless equations
                     */

                    *(here->LTRAibr1Pos1Ptr) += model->LTRAadmit;
                    *(here->LTRAibr1Neg1Ptr) -= model->LTRAadmit;
                    *(here->LTRAibr1Ibr1Ptr) -= 1.0;
                    *(here->LTRApos1Ibr1Ptr) += 1.0;
                    *(here->LTRAneg1Ibr1Ptr) -= 1.0;

                    *(here->LTRAibr2Pos2Ptr) += model->LTRAadmit;
                    *(here->LTRAibr2Neg2Ptr) -= model->LTRAadmit;
                    *(here->LTRAibr2Ibr2Ptr) -= 1.0;
                    *(here->LTRApos2Ibr2Ptr) += 1.0;
                    *(here->LTRAneg2Ibr2Ptr) -= 1.0;
                    break;

                case LTRA_MOD_RC:
            
                    /* this section loads for the parts of the equations
                     * that have no convolution
                     */

                    *(here->LTRAibr1Ibr1Ptr) -= 1.0;
                    *(here->LTRApos1Ibr1Ptr) += 1.0;
                    *(here->LTRAneg1Ibr1Ptr) -= 1.0;

                    *(here->LTRAibr2Ibr2Ptr) -= 1.0;
                    *(here->LTRApos2Ibr2Ptr) += 1.0;
                    *(here->LTRAneg2Ibr2Ptr) -= 1.0;

                    /* loading for convolution parts' first terms */

                    dummy1 = model->LTRAh1dashFirstCoeff;
                    *(here->LTRAibr1Pos1Ptr) += dummy1;
                    *(here->LTRAibr1Neg1Ptr) -= dummy1;
                    *(here->LTRAibr2Pos2Ptr) += dummy1;
                    *(here->LTRAibr2Neg2Ptr) -= dummy1;

                    dummy1 = model->LTRAh2FirstCoeff;
                    *(here->LTRAibr1Ibr2Ptr) -= dummy1;
                    *(here->LTRAibr2Ibr1Ptr) -= dummy1;

                    dummy1 = model->LTRAh3dashFirstCoeff;
                    *(here->LTRAibr1Pos2Ptr) -= dummy1;
                    *(here->LTRAibr1Neg2Ptr) += dummy1;
                    *(here->LTRAibr2Pos1Ptr) -= dummy1;
                    *(here->LTRAibr2Neg1Ptr) += dummy1;
                    break;

                default:
                    return(E_BADPARM);
            }

            /* set up LTRAinputs - to go into the RHS of the circuit
             * equations
             */

            if (ckt->CKTmode & (MODEINITPRED | MODEINITTRAN)) {
                double old1 = here->LTRAinput1Old;
                double old2 = here->LTRAinput2Old;
                double tol, A1, A2 , rt, at;

                /* first iteration of each timepoint */
                /* set LTRAinput1,2 */
                i = ltra_pred(ckt,model,here,&ls);
                if (i) return (i);
                /*
                if (ckt->CKTmode && MODEINITPRED && ckt->CKTtimeIndex >= 2) {
                    rt = .5;
                    at = 1e-2;

                    A1 = FABS(old1);
                    A2 = FABS(here->LTRAinput1);
                    tol = rt*MAX(A1,A2) + at;
                    A1 = old1 - here->LTRAinput1;
                    if (FABS(A1) > tol) {
            printf("%g %g\n",old1,here->LTRAinput1);
                        return (E_ITERLIM);
                    }
                    A1 = FABS(old2);
                    A2 = FABS(here->LTRAinput2);
                    tol = rt*MAX(A1,A2) + at;
                    A1 = old2 - here->LTRAinput2;
                    if (FABS(A1) > tol) {
            printf("%g %g\n",old2,here->LTRAinput2);
                        return (E_ITERLIM);
                    }
                }
                */
                /*
                here->LTRAinput1 = 0;
                here->LTRAinput2 = 0;
                */
            }

            /* load the RHS */
            *(ckt->CKTrhs + here->LTRAbrEq1) += here->LTRAinput1;
            *(ckt->CKTrhs + here->LTRAbrEq2) += here->LTRAinput2;
        }
    }
    return (OK);
}


static int
ltra_pred(ckt,model,here,ls)

CKTcircuit* ckt;
LTRAmodel *model;
LTRAinstance *here;
struct ltrastuff *ls;
{
    double v1d, v2d, i1d, i2d;
    double dummy1, dummy2;
    int i;

    here->LTRAinput1 = here->LTRAinput2 = 0.0;

    if (ls->ls_over) {
        /* have to interpolate values */

        switch (model->LTRAspecialCase) {

            case LTRA_MOD_LC:
            case LTRA_MOD_RLC:

                v1d = ltra_interp(here,ls,here->LTRAv1);
                i1d = ltra_interp(here,ls,here->LTRAi1);
                v2d = ltra_interp(here,ls,here->LTRAv2);
                i2d = ltra_interp(here,ls,here->LTRAi2);
                /* interpolation done */
                break;

            case LTRA_MOD_RC:
                break;

            default:
                return(E_BADPARM);
        }
    }

    switch (model->LTRAspecialCase) {
        case LTRA_MOD_RLC:

            /* begin convolution parts
             *
             * the matrix has already been loaded above
             */

            /* convolution of h1dash with v1 and v2 */
            dummy1 = 0.0;
            dummy2 = 0.0;
            for (i = ckt->CKTtimeIndex; i > 0; i--) {
                if (*(model->LTRAh1dashCoeffs + i) != 0.0) {
                    dummy1 += *(model->LTRAh1dashCoeffs + i)*
                        (*(here->LTRAv1 + i) - here->LTRAinitVolt1);
                    dummy2 += *(model->LTRAh1dashCoeffs + i)*
                        (*(here->LTRAv2 + i) - here->LTRAinitVolt2);
                }
            }

            /* the initial-condition terms */
            dummy1 += here->LTRAinitVolt1 * model->LTRAintH1dash;
            dummy2 += here->LTRAinitVolt2 * model->LTRAintH1dash;
            dummy1 -= here->LTRAinitVolt1 * model->LTRAh1dashFirstCoeff;
            dummy2 -= here->LTRAinitVolt2 * model->LTRAh1dashFirstCoeff;

            here->LTRAinput1 -= dummy1*model->LTRAadmit;
            here->LTRAinput2 -= dummy2*model->LTRAadmit;
            /* end convolution of h1dash with v1 and v2 */

            /* convolution of h2 with i2 and i1 */
            dummy1 = 0.0;
            dummy2 = 0.0;
            if (ls->ls_over) {

                dummy1 = (i2d - here->LTRAinitCur2)*
                    model->LTRAh2FirstCoeff;
                dummy2 = (i1d - here->LTRAinitCur1)*
                    model->LTRAh2FirstCoeff;
    
                /* the rest of the convolution */
                for (i = model->LTRAauxIndex; i > 0; i--) {

                    if (*(model->LTRAh2Coeffs + i) != 0.0) {
                        dummy1 += *(model->LTRAh2Coeffs + i)*
                            (*(here->LTRAi2 + i) - here->LTRAinitCur2);
                        dummy2 += *(model->LTRAh2Coeffs + i)*
                            (*(here->LTRAi1 + i) - here->LTRAinitCur1);
                    }
                }
            }

            /* the initial-condition terms */
            dummy1 += here->LTRAinitCur2 * model->LTRAintH2;
            dummy2 += here->LTRAinitCur1 * model->LTRAintH2;
    
            here->LTRAinput1 += dummy1;
            here->LTRAinput2 += dummy2;
            /* end convolution of h2 with i2 and i1 */

            /* convolution of h3dash with v2 and v1 */
            dummy1 = 0.0;
            dummy2 = 0.0;
            if (ls->ls_over) {

                dummy1 = (v2d - here->LTRAinitVolt2)*
                    model->LTRAh3dashFirstCoeff;
                dummy2 = (v1d - here->LTRAinitVolt1)*
                    model->LTRAh3dashFirstCoeff;
    
                /* the rest of the convolution */
                for (i = model->LTRAauxIndex; i > 0; i--) {
                    if (*(model->LTRAh3dashCoeffs + i) != 0.0) {
                        dummy1 += *(model->LTRAh3dashCoeffs + i)*
                            (*(here->LTRAv2 + i) - here->LTRAinitVolt2);
                        dummy2 += *(model->LTRAh3dashCoeffs + i)*
                            (*(here->LTRAv1 + i) - here->LTRAinitVolt1);
                    }
                }
            }

            /* the initial-condition terms */
            dummy1 += here->LTRAinitVolt2 * model->LTRAintH3dash;
            dummy2 += here->LTRAinitVolt1 * model->LTRAintH3dash;
    
            here->LTRAinput1 += model->LTRAadmit*dummy1;
            here->LTRAinput2 += model->LTRAadmit*dummy2;
            /* end convolution of h3dash with v2 and v1 */

        case LTRA_MOD_LC:
            /* begin lossless-like parts */

            if (!ls->ls_over) {

                here->LTRAinput1 += model->LTRAattenuation*
                    (here->LTRAinitVolt2*model->LTRAadmit +
                    here->LTRAinitCur2);
                here->LTRAinput2 += model->LTRAattenuation*
                    (here->LTRAinitVolt1*model->LTRAadmit +
                    here->LTRAinitCur1);
            }
            else {

                here->LTRAinput1 += model->LTRAattenuation*
                    (v2d*model->LTRAadmit + i2d);
                here->LTRAinput2 += model->LTRAattenuation*
                    (v1d*model->LTRAadmit + i1d);

            }

            /* end lossless-like parts */
            break;
        
        case LTRA_MOD_RC:

            /* begin convolution parts
             *
             * the matrix has already been loaded above
             */

            /* convolution of h1dash with v1 and v2 */
            dummy1 = 0.0;
            dummy2 = 0.0;
            for (i = ckt->CKTtimeIndex; i > 0; i--) {
                if (*(model->LTRAh1dashCoeffs + i) != 0.0) {
                    dummy1 += *(model->LTRAh1dashCoeffs + i)*
                        (*(here->LTRAv1 + i) - here->LTRAinitVolt1);
                    dummy2 += *(model->LTRAh1dashCoeffs + i)*
                        (*(here->LTRAv2 + i) - here->LTRAinitVolt2);
                }
            }

            /* the initial condition terms */
            dummy1 += here->LTRAinitVolt1 * model->LTRAintH1dash;
            dummy2 += here->LTRAinitVolt2 * model->LTRAintH1dash;
            dummy1 -= here->LTRAinitVolt1 * model->LTRAh1dashFirstCoeff;
            dummy2 -= here->LTRAinitVolt2 * model->LTRAh1dashFirstCoeff;

            here->LTRAinput1 -= dummy1;
            here->LTRAinput2 -= dummy2;
            /* end convolution of h1dash with v1 and v2 */

            /* convolution of h2 with i2 and i1 */
            dummy1 = 0.0;
            dummy2 = 0.0;
            for (i = ckt->CKTtimeIndex; i > 0; i--) {
                if (*(model->LTRAh2Coeffs+ i) != 0.0) {
                    dummy1 += *(model->LTRAh2Coeffs + i)*
                        (*(here->LTRAi2 + i) - here->LTRAinitCur2);
                    dummy2 += *(model->LTRAh2Coeffs + i)*
                        (*(here->LTRAi1 + i) - here->LTRAinitCur1);
                }
            }

            /* the initial-condition terms */
            dummy1 += here->LTRAinitCur2 * model->LTRAintH2;
            dummy2 += here->LTRAinitCur1 * model->LTRAintH2;
            dummy1 -= here->LTRAinitCur2 * model->LTRAh2FirstCoeff;
            dummy2 -= here->LTRAinitCur1 * model->LTRAh2FirstCoeff;

            here->LTRAinput1 += dummy1;
            here->LTRAinput2 += dummy2;
            /* end convolution of h2 with i2 and i1 */

            /* convolution of h3dash with v2 and v1 */
            dummy1 = 0.0;
            dummy2 = 0.0;
            for (i = ckt->CKTtimeIndex; i > 0; i--) {
                if (*(model->LTRAh3dashCoeffs+ i) != 0.0) {
                    dummy1 += *(model->LTRAh3dashCoeffs + i)*
                        (*(here->LTRAv2 + i) - here->LTRAinitVolt2);
                    dummy2 += *(model->LTRAh3dashCoeffs + i)*
                        (*(here->LTRAv1 + i) - here->LTRAinitVolt1);
                }
            }

            /* the initial-condition terms */
            dummy1 += here->LTRAinitVolt2 * model->LTRAintH3dash;
            dummy2 += here->LTRAinitVolt1 * model->LTRAintH3dash;
            dummy1 -= here->LTRAinitVolt2 * model->LTRAh3dashFirstCoeff;
            dummy2 -= here->LTRAinitVolt1 * model->LTRAh3dashFirstCoeff;

            here->LTRAinput1 += dummy1;
            here->LTRAinput2 += dummy2;
            /* end convolution of h3dash with v2 and v1 */

            break;

        default:
            return (E_BADPARM);
    }
    return (OK);
}


static double
ltra_interp(here,ls,ptr)

LTRAinstance *here;
struct ltrastuff *ls;
double *ptr;
{
    double ret;
    double max, min;

    ptr += ls->ls_saved;
    if ((ls->ls_saved != 0) && ls->ls_qinterp) {

        ret = *(ptr-1)* ls->ls_qf1
            + *(ptr  )* ls->ls_qf2
            + *(ptr+1)* ls->ls_qf3;

        max = MAX(*(ptr-1),*(ptr));
        max = MAX(max,*(ptr+1));
        min = MIN(*(ptr-1),*(ptr));
        min = MIN(min,*(ptr+1));

        if ((ret <= max) && (ret >= min))
            return (ret);
    }
    ret = *(ptr)  * ls->ls_lf2
        + *(ptr+1)* ls->ls_lf3;

    return (ret);
}


/* linear interpolation */

static int
ltra_lin_interp(t,ls)

double t;
struct ltrastuff *ls;
{
    if (ls->ls_t2 == ls->ls_t3) return (1);

    if (t == ls->ls_t2) {
        ls->ls_lf2 = 1.0;
        ls->ls_lf3 = 0.0;
        return (0);
    }

    if (t == ls->ls_t3) {
        ls->ls_lf2 = 0.0;
        ls->ls_lf3 = 1.0;
        return (0);
    }

    ls->ls_lf3 = (t - ls->ls_t2)/(ls->ls_t3 - ls->ls_t2);
    ls->ls_lf2 = 1 - ls->ls_lf3;
    return (0);
}


/* quadratic interpolation */

static int
ltra_quad_interp(t,ls)

double t;
struct ltrastuff *ls;
{
    if (t == ls->ls_t1) {
        ls->ls_qf1 = 1.0;
        ls->ls_qf2 = 0.0;
        ls->ls_qf3 = 0.0;
        return (0);
    }
    if (t == ls->ls_t2) {
        ls->ls_qf1 = 0.0;
        ls->ls_qf2 = 1.0;
        ls->ls_qf3 = 0.0;
        return (0);
    }
    if (t == ls->ls_t3) {
        ls->ls_qf1 = 0.0;
        ls->ls_qf2 = 0.0;
        ls->ls_qf3 = 1.0;
        return (0);
    }
    if ((ls->ls_t2 - ls->ls_t1) == 0 ||
        (ls->ls_t3 - ls->ls_t2) == 0 ||
        (ls->ls_t1 - ls->ls_t3) == 0) return(1);

    ls->ls_qf1 = (t - ls->ls_t2)*(t - ls->ls_t3)/
        ((ls->ls_t1 - ls->ls_t2)*(ls->ls_t1 - ls->ls_t3));
    ls->ls_qf2 = (t - ls->ls_t1)*(t - ls->ls_t3)/
        ((ls->ls_t2 - ls->ls_t1)*(ls->ls_t2 - ls->ls_t3));
    ls->ls_qf3 = (t - ls->ls_t1)*(t - ls->ls_t2)/
        ((ls->ls_t2 - ls->ls_t3)*(ls->ls_t1 - ls->ls_t3));
    return (0);
}
