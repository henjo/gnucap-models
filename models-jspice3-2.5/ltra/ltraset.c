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


int
LTRAsetup(matrix,inModel,ckt,state)

SMPmatrix *matrix;
GENmodel *inModel;
CKTcircuit *ckt;
int *state;
/* load the transmission line structure with those pointers needed later
 * for fast matrix loading 
 */
{
    LTRAmodel *model = (LTRAmodel *)inModel;
    LTRAinstance *here;
    int error;
    double dummy1, dummy2;
    CKTnode *tmp;

    /* loop through all the transmission line models */
    for ( ; model != NULL; model = model->LTRAnextModel) {


        if (!model->LTRAreltolGiven) {
            model->LTRAreltol = 1;
        }
        if (!model->LTRAabstolGiven) {
            model->LTRAabstol = 1;
        }
        if (!model->LTRAresistGiven) {
            (*(SPfrontEnd->IFerror))(ERR_WARNING,
              "%s: lossy line series resistance not given, assumed zero",
              &(model->LTRAmodName));
            model->LTRAresist = 0.0;
        }

        if (model->LTRAstLineReltol == 0.0) 
            model->LTRAstLineReltol = ckt->CKTreltol;
        if (model->LTRAstLineAbstol == 0.0) 
            model->LTRAstLineAbstol = ckt->CKTabstol;
        /* LTRAchopReltol and LTRAchopAbstol default zero */

        if ((model->LTRAhowToInterp != LTRA_MOD_LININTERP) &&
            (model->LTRAhowToInterp != LTRA_MOD_QUADINTERP) &&
            (model->LTRAhowToInterp != LTRA_MOD_MIXEDINTERP)) {

            if (ckt->CKTtryToCompact) {
                model->LTRAhowToInterp = LTRA_MOD_LININTERP;
                (*(SPfrontEnd->IFerror))(ERR_WARNING,
    "%s: using linear interpolation because trytocompact option specified",
                    &(model->LTRAmodName));
            }
            else {
                model->LTRAhowToInterp = LTRA_MOD_QUADINTERP;
            }
        }

        if ((model->LTRAstepLimit != LTRA_MOD_NOSTEPLIMIT))
            model->LTRAstepLimit = LTRA_MOD_STEPLIMIT;

        if ((model->LTRAlteConType != LTRA_MOD_FULLCONTROL) &&
            (model->LTRAlteConType != LTRA_MOD_HALFCONTROL))
            model->LTRAlteConType = LTRA_MOD_NOCONTROL;

        if (!model->LTRAconductGiven) {
            model->LTRAconduct = 0.0;
        }
        if (!model->LTRAinductGiven) {
            (*(SPfrontEnd->IFerror))(ERR_WARNING,
              "%s: lossy line series inductance not given, assumed zero",
                &(model->LTRAmodName));
            model->LTRAinduct = 0.0;
        }
        if (!model->LTRAcapacGiven) {
            (*(SPfrontEnd->IFerror))(ERR_FATAL,
            "%s: lossy line parallel capacitance not given, assumed zero",
                &(model->LTRAmodName));
            model->LTRAcapac = 0.0;
        }
        if (!model->LTRAlengthGiven) {
            (*(SPfrontEnd->IFerror))(ERR_FATAL,
                "%s: lossy line length must be given",
                &(model->LTRAmodName));
            return(E_BADPARM);
        }

        if ((model->LTRAresist == 0) && (model->LTRAconduct == 0) &&
            (model->LTRAcapac != 0) && (model->LTRAinduct != 0)) {
            model->LTRAspecialCase = LTRA_MOD_LC;

#ifdef LTRADEBUG
            (*(SPfrontEnd->IFerror))(ERR_INFO,
                "%s: lossless line",
                &(model->LTRAmodName));
#endif
        }

        if ((model->LTRAresist != 0) && (model->LTRAconduct == 0) &&
            (model->LTRAcapac != 0) && (model->LTRAinduct != 0)) {
            model->LTRAspecialCase = LTRA_MOD_RLC;
#ifdef LTRADEBUG
            (*(SPfrontEnd->IFerror))(ERR_INFO,
                "%s: RLC line",
                &(model->LTRAmodName));
#endif
        }

        if ((model->LTRAresist != 0) && (model->LTRAconduct == 0) &&
            (model->LTRAcapac != 0) && (model->LTRAinduct == 0)) {
            model->LTRAspecialCase = LTRA_MOD_RC;
#ifdef LTRADEBUG
            (*(SPfrontEnd->IFerror))(ERR_INFO,
                "%s: RC line",
                &(model->LTRAmodName));
#endif
        }

        if ((model->LTRAresist != 0) && (model->LTRAconduct == 0) &&
            (model->LTRAcapac == 0) && (model->LTRAinduct != 0)) {
            model->LTRAspecialCase = LTRA_MOD_RL;
            (*(SPfrontEnd->IFerror))(ERR_FATAL,
                "%s: RL line not supported yet",
                &(model->LTRAmodName));
            return(E_BADPARM);
        }

        if ((model->LTRAresist != 0) && (model->LTRAconduct != 0) &&
            (model->LTRAcapac == 0) && (model->LTRAinduct == 0)) {
            model->LTRAspecialCase = LTRA_MOD_RG;
#ifdef LTRADEBUG
            (*(SPfrontEnd->IFerror))(ERR_INFO,
                "%s: RG line",
                &(model->LTRAmodName));
#endif
        }

        if ((model->LTRAconduct != 0) && ( (model->LTRAcapac != 0) || 
            (model->LTRAinduct != 0))) {
            model->LTRAspecialCase = LTRA_MOD_LTRA;
            (*(SPfrontEnd->IFerror))(ERR_FATAL,
                "%s: Nonzero G (except RG) line not supported yet",
                &(model->LTRAmodName));
            return(E_BADPARM);
        }

        if ((model->LTRAresist  == 0.0 ? 0 : 1) +
            (model->LTRAconduct == 0.0 ? 0 : 1) +
            (model->LTRAinduct  == 0.0 ? 0 : 1) + 
            (model->LTRAcapac   == 0.0 ? 0 : 1) <= 1) {

            (*(SPfrontEnd->IFerror))(ERR_FATAL,
            "%s: At least two of R,L,G,C must be specified and nonzero",
                &(model->LTRAmodName));
            return(E_BADPARM);
        }

        switch (model->LTRAspecialCase) {

            case LTRA_MOD_LC:

                model->LTRAimped =
                    sqrt(model->LTRAinduct/model->LTRAcapac);
                model->LTRAadmit = 1/model->LTRAimped;
                model->LTRAtd =
                    sqrt(model->LTRAinduct*model->LTRAcapac)*
                    model->LTRAlength;
                model->LTRAattenuation = 1.0;
                break;

            case LTRA_MOD_RLC:
                model->LTRAimped =
                    sqrt(model->LTRAinduct/model->LTRAcapac);
                model->LTRAadmit = 1/model->LTRAimped;
                model->LTRAtd =
                    sqrt(model->LTRAinduct*model->LTRAcapac)*
                    model->LTRAlength;
                model->LTRAalpha =
                    0.5*(model->LTRAresist/model->LTRAinduct);
                model->LTRAbeta = model->LTRAalpha;
                model->LTRAattenuation = exp(- model->LTRAbeta *
                    model->LTRAtd);
                if (model->LTRAalpha > 0.0) {
                    model->LTRAintH1dash = -1.0;
                    model->LTRAintH2 = 1.0 - model->LTRAattenuation;
                    model->LTRAintH3dash = - model->LTRAattenuation;
                }
                else if (model->LTRAalpha == 0.0) {
                    model->LTRAintH1dash = model->LTRAintH2 = 
                        model->LTRAintH3dash = 0.0;
                }
                else {
#ifdef LTRADEBUG
                    fprintf(stdout,"LTRAtemp: error: alpha < 0.0\n");
#endif
                }

                if (!model->LTRAtruncDontCut) {

                    /*
                    There was a bug<?) in spice3 here - the args to
                    the second line check were identical to the first
                    call, so all of the y2 terms were superfluous. 
                    Replacing with the y2 values in the original
                    algorithm cut the resulting step size too much, so
                    here we do a binary search to find a more accurate
                    value.  This is worthwhile since maxSafeStep has a
                    profound effect on default simulation speed.
                    */

                    double xbig, xsmall, xmid, y1big, y1small, y1mid;
                    double xdel, y2big, y2small, y2mid;
                    int done=0, done0=0, maxiter=50, iters=0;

                    xbig = model->LTRAtd + 9*model->LTRAtd;
                    /* hack! ckt is not yet initialised... */
                    xsmall = model->LTRAtd;
                    xmid = 0.5*(xbig+xsmall);
                    y1small = LTRArlcH2Func(xsmall,(GENmodel*)model);
                    y2small = LTRArlcH3dashFunc(xsmall,model->LTRAtd,
                        model->LTRAbeta,model->LTRAbeta);
                    iters = 0;
                    for (;;) {
                        iters++;
                        y1big = LTRArlcH2Func(xbig,(GENmodel*)model);
                        y1mid = LTRArlcH2Func(xmid,(GENmodel*)model);
                        y2big = LTRArlcH3dashFunc(xbig,model->LTRAtd,
                            model->LTRAbeta,model->LTRAbeta);
                        y2mid = LTRArlcH3dashFunc(xmid,model->LTRAtd,
                            model->LTRAbeta,model->LTRAbeta);
                        done =
                            LTRAstraightLineCheck(xbig,y1big,xmid,y1mid,
                            xsmall,y1small,model->LTRAstLineReltol,
                            model->LTRAstLineAbstol) + 
                            LTRAstraightLineCheck(xbig,y2big,xmid,y2mid,
                            xsmall,y2small,model->LTRAstLineReltol,
                            model->LTRAstLineAbstol);

                        if (iters > maxiter)
                            break;
                        if (done == 2) {
                            if (!done0) {
                                maxiter = iters + 8;
                                done0 = 1;
                                xdel = xbig - xmid;
                            }
                            xbig += xdel;
                            xdel *= 0.5;
                        }
                        else {
                            if (done0) {
                                xbig -= xdel;
                                xdel *= 0.5;
                            }
                            else
                                xbig = xmid;
                        }
                        xmid = 0.5*(xbig + xsmall);
                    }
                    model->LTRAmaxSafeStep = xbig - model->LTRAtd;
                }
                break;

            case LTRA_MOD_RC:
                model->LTRAcByR = model->LTRAcapac/model->LTRAresist;
                model->LTRArclsqr = model->LTRAresist*model->LTRAcapac
                    *model->LTRAlength*model->LTRAlength;
                model->LTRAintH1dash = 0.0;
                model->LTRAintH2 = 1.0;
                model->LTRAintH3dash = 0.0;

                model->LTRAh1dashCoeffs = (double *) NULL;
                model->LTRAh2Coeffs = (double *) NULL;
                model->LTRAh3dashCoeffs = (double *) NULL;

                break;

            case LTRA_MOD_RG:
                dummy1 = model->LTRAlength*
                    sqrt(model->LTRAresist*model->LTRAconduct);
                dummy2 = exp(-dummy1);
                /* warning: may overflow! */
                dummy1 = exp(dummy1);
                model->LTRAcoshlrootGR = 0.5*(dummy1 + dummy2);

                if (model->LTRAconduct <= 1.0e-10) { /* hack! */
                    model->LTRArRsLrGRorG =
                        model->LTRAlength*model->LTRAresist;
                }
                else {
                    model->LTRArRsLrGRorG = 0.5*(dummy1 - dummy2)*
                        sqrt(model->LTRAresist/model->LTRAconduct);
                }

                if (model->LTRAresist <= 1.0e-10) { /* hack! */
                    model->LTRArGsLrGRorR =
                        model->LTRAlength*model->LTRAconduct;
                }
                else {
                    model->LTRArGsLrGRorR = 0.5*(dummy1 - dummy2)*
                        sqrt(model->LTRAconduct/model->LTRAresist);
                }
                break;

            default:
                return (E_BADPARM);
        }

        FREE(model->LTRAh1dashCoeffs);
        FREE(model->LTRAh2Coeffs);
        FREE(model->LTRAh3dashCoeffs);

        /* loop through all the instances of the model */
        for (here = model->LTRAinstances; here != NULL ;
                here = here->LTRAnextInstance) {
        
            FREE(here->LTRAv1);
            FREE(here->LTRAi1);
            FREE(here->LTRAv2);
            FREE(here->LTRAi2);

            if (here->LTRAbrEq1 == 0) {
                error = CKTmkVolt(ckt,&tmp,here->LTRAname,"i1");
                if (error) return (error);
                here->LTRAbrEq1 = tmp->number;
            }

            if (here->LTRAbrEq2==0) {
                error = CKTmkVolt(ckt,&tmp,here->LTRAname,"i2");
                if (error) return   (error);
                here->LTRAbrEq2 = tmp->number;
            }

            TSTALLOC(LTRAibr1Pos1Ptr, LTRAbrEq1, LTRAposNode1)
            TSTALLOC(LTRAibr1Neg1Ptr, LTRAbrEq1, LTRAnegNode1)
            TSTALLOC(LTRAibr1Pos2Ptr, LTRAbrEq1, LTRAposNode2)
            TSTALLOC(LTRAibr1Neg2Ptr, LTRAbrEq1, LTRAnegNode2)
            TSTALLOC(LTRAibr1Ibr1Ptr, LTRAbrEq1, LTRAbrEq1)
            TSTALLOC(LTRAibr1Ibr2Ptr, LTRAbrEq1, LTRAbrEq2)
            TSTALLOC(LTRAibr2Pos1Ptr, LTRAbrEq2, LTRAposNode1)
            TSTALLOC(LTRAibr2Neg1Ptr, LTRAbrEq2, LTRAnegNode1)
            TSTALLOC(LTRAibr2Pos2Ptr, LTRAbrEq2, LTRAposNode2)
            TSTALLOC(LTRAibr2Neg2Ptr, LTRAbrEq2, LTRAnegNode2)
            TSTALLOC(LTRAibr2Ibr1Ptr, LTRAbrEq2, LTRAbrEq1)
            TSTALLOC(LTRAibr2Ibr2Ptr, LTRAbrEq2, LTRAbrEq2)
            TSTALLOC(LTRApos1Ibr1Ptr, LTRAposNode1, LTRAbrEq1)
            TSTALLOC(LTRAneg1Ibr1Ptr, LTRAnegNode1, LTRAbrEq1)
            TSTALLOC(LTRApos2Ibr2Ptr, LTRAposNode2, LTRAbrEq2)
            TSTALLOC(LTRAneg2Ibr2Ptr, LTRAnegNode2, LTRAbrEq2)
            /* the following are done so that SMPpreOrder does not
             * screw up on occasion - for example, when one end
             * of the lossy line is hanging
             */
            TSTALLOC(LTRApos1Pos1Ptr, LTRAposNode1, LTRAposNode1)
            TSTALLOC(LTRAneg1Neg1Ptr, LTRAnegNode1, LTRAnegNode1)
            TSTALLOC(LTRApos2Pos2Ptr, LTRAposNode2, LTRAposNode2)
            TSTALLOC(LTRAneg2Neg2Ptr, LTRAnegNode2, LTRAnegNode2)

            here->LTRAstate = *state;
            *state += 2;
        }
    }
    return(OK);
}
