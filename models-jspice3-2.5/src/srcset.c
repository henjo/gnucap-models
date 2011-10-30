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
#include "cktext.h"

#ifdef __STDC__
static void src_ccvs(CKTcircuit*,SRCinstance*);
static void src_vcvs(CKTcircuit*,SRCinstance*);
static void src_cccs(CKTcircuit*,SRCinstance*);
static void src_vccs(CKTcircuit*,SRCinstance*);
static void src_dc(CKTcircuit*,SRCinstance*);
static void src_func0(CKTcircuit*,SRCinstance*);
static void src_func1(CKTcircuit*,SRCinstance*);
#else
static void src_ccvs();
static void src_vcvs();
static void src_cccs();
static void src_vccs();
static void src_dc();
static void src_func0();
static void src_func1();
#endif

#define MY_TSTALLOC(ptr,first,second) \
if((here->ptr = spGetElement(matrix,here->first,((CKTnode*)(second))->number))\
        ==(double *)NULL){\
    return(E_NOMEM);\
}


/*ARGSUSED*/
int
SRCsetup(matrix,inModel,ckt,states)

/* load the voltage source structure with those 
 * pointers needed later for fast matrix loading 
 */
SMPmatrix *matrix;
GENmodel *inModel;
CKTcircuit *ckt;
int *states;
{
    SRCinstance *here;
    SRCmodel *model = (SRCmodel*)inModel;
    int error, i, j;
    CKTnode *tmp;
    double radians;

    /*  loop through all the user models*/
    for ( ; model != NULL; model = model->SRCnextModel) {

        /* loop through all the instances of the model */
        for (here = model->SRCinstances; here != NULL;
                here = here->SRCnextInstance) {
            
            if (here->SRCtype == SRC_V) {
                if (here->SRCbranch == 0) {
                    error = CKTmkCur(ckt,&tmp,here->SRCname,"branch");
                    if (error)
                        return (error);
                    here->SRCbranch = tmp->number;
                }
            }

            here->SRCvalue = here->SRCdcValue;

            if (here->SRCacGiven && !here->SRCacMGiven)
                here->SRCacMag = 1;
            if (here->SRCacGiven && !here->SRCacPGiven)
                here->SRCacPhase = 0;
            radians = here->SRCacPhase * M_PI / 180.0;
            here->SRCacReal = here->SRCacMag * cos(radians);
            here->SRCacImag = here->SRCacMag * sin(radians);

#ifdef notdef
            if (!here->SRCdcGiven) {
                /* no DC value - either have a transient value, or none */
                if (here->SRCfuncTGiven) {
                    (*(SPfrontEnd->IFerror))(ERR_WARNING,
                            "%s: no DC value, transient time 0 value used",
                            &(here->SRCname));
                }
                else {
                    (*(SPfrontEnd->IFerror))(ERR_WARNING,
                            "%s: has no value, DC 0 assumed",
                            &(here->SRCname));
                }
            }
#endif

            here->SRCstate = *states;

            if (here->SRCtree) {
                if (here->SRCtree->numVars) {
                    *states += here->SRCtree->numVars;
                    here->SRCvalues = (double *)
                        MALLOC((here->SRCtree->numVars)*sizeof(double));
                    if (here->SRCvalues == NULL)
                        return (E_NOMEM);
                    here->SRCderivs = (double *)
                        MALLOC((here->SRCtree->numVars)*sizeof(double));
                    if (here->SRCderivs == NULL)
                        return (E_NOMEM);
                    here->SRCeqns = (int *)
                        MALLOC((here->SRCtree->numVars)*sizeof(int));
                    if (here->SRCeqns == NULL)
                        return (E_NOMEM);
                }
                here->SRCacValues = (double *)
                    MALLOC((here->SRCtree->numVars + 1)*sizeof(double));
                if (here->SRCacValues == NULL)
                    return (E_NOMEM);
            }
            else if (here->SRCccCoeffGiven) {
                here->SRCcontBranch = CKTfndBranch(ckt,here->SRCcontName);
                if (here->SRCcontBranch == 0) {
                    IFuid namarray[2];
                    namarray[0] =  here->SRCname;
                    namarray[1] = here->SRCcontName;
                    (*(SPfrontEnd->IFerror))(ERR_FATAL,
                            "%s: unknown controlling source %s",namarray);
                    return (E_BADPARM);
                }
            }

            if (here->SRCtype == SRC_V) {
                TSTALLOC(SRCposIbrptr, SRCposNode, SRCbranch)
                TSTALLOC(SRCnegIbrptr, SRCnegNode, SRCbranch)
                TSTALLOC(SRCibrNegptr, SRCbranch,  SRCnegNode)
                TSTALLOC(SRCibrPosptr, SRCbranch,  SRCposNode)

                /* for pz analysis, an ac source is open, ac has precedence
                 * over FUNC
                 */
                if (here->SRCacGiven)
                    TSTALLOC(SRCibrIbrptr, SRCbranch,  SRCbranch)

                if (ckt->CKTpreload) {
                    /* preload constants */

                    if (here->SRCposNode) {
                        *(here->SRCposIbrptr) += 1.0;
                        *(here->SRCibrPosptr) += 1.0;
                    }
                    if (here->SRCnegNode) {
                        *(here->SRCnegIbrptr) -= 1.0;
                        *(here->SRCibrNegptr) -= 1.0;
                    }
                }

                if (here->SRCccCoeffGiven) {
                    TSTALLOC(SRCibrContBrptr, SRCbranch, SRCcontBranch)
                    here->SRCtranFunc = src_ccvs;
                    here->SRCdcFunc = src_ccvs;
                    continue;
                }
                else if (here->SRCvcCoeffGiven) {
                    TSTALLOC(SRCibrContPosptr, SRCbranch, SRCcontPosNode)
                    TSTALLOC(SRCibrContNegptr, SRCbranch, SRCcontNegNode)
                    here->SRCtranFunc = src_vcvs;
                    here->SRCdcFunc = src_vcvs;
                    continue;
                }
            }
            else {

                if (here->SRCccCoeffGiven) {
                    TSTALLOC(SRCposContBrptr,SRCposNode,SRCcontBranch)
                    TSTALLOC(SRCnegContBrptr,SRCnegNode,SRCcontBranch)
                    here->SRCtranFunc = src_cccs;
                    here->SRCdcFunc = src_cccs;
                    continue;
                }
                else if (here->SRCvcCoeffGiven) {
                    TSTALLOC(SRCposContPosptr, SRCposNode, SRCcontPosNode)
                    TSTALLOC(SRCposContNegptr, SRCposNode, SRCcontNegNode)
                    TSTALLOC(SRCnegContPosptr, SRCnegNode, SRCcontPosNode)
                    TSTALLOC(SRCnegContNegptr, SRCnegNode, SRCcontNegNode)
                    here->SRCtranFunc = src_vccs;
                    here->SRCdcFunc = src_vccs;
                    continue;
                }
            }

            if (here->SRCtree == NULL) {
                here->SRCtranFunc = src_dc;
                here->SRCdcFunc = src_dc;
                continue;
            }
            if (here->SRCtree->numVars == 0) {
                here->SRCtranFunc = src_func0;
                if (here->SRCdcGiven)
                    here->SRCdcFunc = src_dc;
                else
                    here->SRCdcFunc = src_func0;
                continue;
            }
            here->SRCtranFunc = src_func1;
            if (here->SRCdcGiven)
                here->SRCdcFunc = src_dc;
            else
                here->SRCdcFunc = src_func1;

            /* For each controlling variable set the entries
             * in the vector of the positions of the SMP
             */

            j = here->SRCtree->numVars;
            if (here->SRCtype == SRC_I) j *= 2;

            here->SRCposptr = (double **)MALLOC(j * sizeof(double *));

            for (j = 0,i = 0; i < here->SRCtree->numVars; i++) {
                switch (here->SRCtree->varTypes[i]) {

                case IF_INSTANCE:
                    here->SRCcontBranch = CKTfndBranch(ckt,
                            here->SRCtree->vars[i].uValue);
                    if (here->SRCcontBranch == 0) {
                        IFuid namarray[2];
                        namarray[0] =  here->SRCname;
                        namarray[1] = here->SRCtree->vars[i].uValue;
                        (*(SPfrontEnd->IFerror))(ERR_FATAL,
                                "%s: unknown controlling source %s",namarray);
                        return (E_BADPARM);
                    }

                    if (here->SRCtype == SRC_V) {
                        TSTALLOC(SRCposptr[j++],SRCbranch,SRCcontBranch);
                    }
                    else if (here->SRCtype == SRC_I) {
                        TSTALLOC(SRCposptr[j++],SRCposNode,SRCcontBranch);
                        TSTALLOC(SRCposptr[j++],SRCnegNode,SRCcontBranch);
                    }
                    else {
                        return (E_BADPARM);
                    }
                    here->SRCeqns[i] = here->SRCcontBranch;
                    break;

                case IF_NODE:
                    if (here->SRCtype == SRC_V) {
                        MY_TSTALLOC(SRCposptr[j++],SRCbranch,
                            here->SRCtree->vars[i].nValue);
                    }
                    else if(here->SRCtype == SRC_I) {
                        MY_TSTALLOC(SRCposptr[j++],SRCposNode,
                            here->SRCtree->vars[i].nValue);
                        MY_TSTALLOC(SRCposptr[j++],SRCnegNode,
                            here->SRCtree->vars[i].nValue);
                    }
                    else{
                        return (E_BADPARM);
                    }
                    here->SRCeqns[i] = ((CKTnode *)(here->SRCtree->vars[i].
                        nValue))->number;
                    break;

                default:
                    break;
                }
            }
        }
    }
    return (OK);
}

/* evaluation functions */

static void
src_ccvs(ckt,here)

CKTcircuit* ckt;
SRCinstance *here;
{
    *(here->SRCibrContBrptr) += here->SRCcoeff;
}


static void
src_vcvs(ckt,here)

CKTcircuit* ckt;
SRCinstance *here;
{
    *(here->SRCibrContPosptr) -= here->SRCcoeff;
    *(here->SRCibrContNegptr) += here->SRCcoeff;
}


static void
src_cccs(ckt,here)

CKTcircuit* ckt;
SRCinstance *here;
{
    *(here->SRCposContBrptr) += here->SRCcoeff;
    *(here->SRCnegContBrptr) -= here->SRCcoeff;
}


static void
src_vccs(ckt,here)

CKTcircuit* ckt;
SRCinstance *here;
{
    *(here->SRCposContPosptr) += here->SRCcoeff;
    *(here->SRCposContNegptr) -= here->SRCcoeff;
    *(here->SRCnegContPosptr) -= here->SRCcoeff;
    *(here->SRCnegContNegptr) += here->SRCcoeff;
}


static void
src_dc(ckt,here)

CKTcircuit *ckt;
SRCinstance *here;
{
    double rhs = ckt->CKTsrcFact * here->SRCdcValue;

    if (here->SRCtype == SRC_V) {
        *(ckt->CKTrhs + here->SRCbranch) = rhs;
    }
    else {
        *(ckt->CKTrhs + (here->SRCposNode)) -= rhs;
        *(ckt->CKTrhs + (here->SRCnegNode)) += rhs;
    }
}


static void
src_func0(ckt,here)

CKTcircuit *ckt;
SRCinstance *here;
{
    double rhs;

    if (ckt->CKTmode & (MODEINITTRAN|MODEINITPRED|MODEDC)) {
        rhs = 0;

        if (ckt->CKTmode & MODEDCTRANCURVE)
            rhs = ckt->CKTsrcFact * here->SRCdcValue;
        else if ((*(here->SRCtree->IFeval))
            (here->SRCtree,ckt->CKTgmin,&rhs,NULL,
                NULL,&ckt->CKTtime) == OK) {
            /* !! last arg bogus if ft_sim->specSigs changed !! */

            here->SRCvalue = rhs;

            /* Store the rhs for small signal analysis */
            if (ckt->CKTmode & MODEINITSMSIG) {
                here->SRCacValues[0] = rhs; 
            }
        }
    }
    else {
        rhs = here->SRCvalue;
    }

    if (here->SRCtype == SRC_V) {
        *(ckt->CKTrhs + here->SRCbranch) = rhs;
    }
    else {
        *(ckt->CKTrhs + (here->SRCposNode)) -= rhs;
        *(ckt->CKTrhs + (here->SRCnegNode)) += rhs;
    }
}


static void
src_func1(ckt,here)

CKTcircuit *ckt;
SRCinstance *here;
{
    int i, j;
    double rhs, deriv;

    if (ckt->CKTmode & (MODEINITFLOAT|MODEINITFIX)) {
        for (i = 0; i < here->SRCtree->numVars; i++) {
            here->SRCvalues[i] =
                *(ckt->CKTrhsOld + here->SRCeqns[i]);
        }
    }
    else if (ckt->CKTmode & MODEINITPRED) {
        double *d = ckt->CKTpred;
        for (i = 0; i < here->SRCtree->numVars; i++) {
            here->SRCvalues[i] =
                *(ckt->CKTstate1 + here->SRCstate + i)*d[0] +
                *(ckt->CKTstate2 + here->SRCstate + i)*d[1] +
                *(ckt->CKTstate3 + here->SRCstate + i)*d[2] +
                *(ckt->CKTstate0 + here->SRCstate + i)*d[3];
        }
    }
    else if (ckt->CKTmode & MODEINITJCT) {
        for (i = 0; i < here->SRCtree->numVars; i++) {
            here->SRCvalues[i] = 0;
        }
    }
    else {
        for (i = 0; i < here->SRCtree->numVars; i++) {
            here->SRCvalues[i] =
                *(ckt->CKTstate1 + here->SRCstate + i);
        }
    }

    for (i = 0; i < here->SRCtree->numVars; i++) {
        *(ckt->CKTstate0 + here->SRCstate + i) =
            here->SRCvalues[i];
    }

    rhs = 0;
    if ((*(here->SRCtree->IFeval))
        (here->SRCtree,ckt->CKTgmin,&rhs,here->SRCvalues,
            here->SRCderivs,&ckt->CKTtime) == OK) {
        /* !! last arg bogus if ft_sim->specSigs changed !! */

        here->SRCprev = rhs;

        if (here->SRCtype == SRC_V) {
            for (i = 0; i < here->SRCtree->numVars; i++) {
                deriv = here->SRCderivs[i];
                rhs -= (here->SRCvalues[i] * deriv);
                *(here->SRCposptr[i]) -= deriv;
            }
            *(ckt->CKTrhs + here->SRCbranch) = rhs;
        }
        else {
            for (j = 0,i = 0; i < here->SRCtree->numVars; i++) {
                deriv = here->SRCderivs[i];
                rhs -= (here->SRCvalues[i] * deriv);
                *(here->SRCposptr[j++]) += deriv;
                *(here->SRCposptr[j++]) -= deriv;
            }
            *(ckt->CKTrhs + (here->SRCposNode)) -= rhs;
            *(ckt->CKTrhs + (here->SRCnegNode)) += rhs;
        }

        /* Store the rhs for small signal analysis */
        if (ckt->CKTmode & MODEINITSMSIG) {
            for (i = 0; i < here->SRCtree->numVars; i++) {
                here->SRCacValues[i] = here->SRCderivs[i]; 
            }
            here->SRCacValues[here->SRCtree->numVars] = rhs; 
        }
    }
}
