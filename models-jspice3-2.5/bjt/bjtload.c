/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

/*
 * This is the function called each iteration to evaluate the
 * BJTs in the circuit and load them into the matrix as appropriate
 */

#include "spice.h"
#include <stdio.h>
#include <math.h>
#include "util.h"
#include "bjtdefs.h"
#include "const.h"
#include "sperror.h"
#include "niext.h"

struct bjtstuff {
    double bj_vt;
    double bj_vbe;
    double bj_vbc;
    double bj_vcs;
    double bj_vbx;
    double bj_cbe;
    double bj_cbc;
    double bj_gbe;
    double bj_gbc;
};

#ifdef __STDC__
static int  bjt_bypass(CKTcircuit*,BJTmodel*,BJTinstance*,struct bjtstuff*);
static int  bjt_limiting(CKTcircuit*,BJTmodel*,BJTinstance*,struct bjtstuff*);
static int  bjt_pnjlim(double*,double,double,double);
static void bjt_iv(CKTcircuit*,BJTmodel*,BJTinstance*,struct bjtstuff*);
static void bjt_cap(CKTcircuit*,BJTmodel*,BJTinstance*,struct bjtstuff*);
static int  bjt_integ(CKTcircuit*,BJTmodel*,BJTinstance*,struct bjtstuff*);
static void bjt_load(CKTcircuit*,BJTmodel*,BJTinstance*,struct bjtstuff*);
#else
static int  bjt_bypass();
static int  bjt_limiting();
static int  bjt_pnjlim();
static void bjt_iv();
static void bjt_cap();
static int  bjt_integ();
static void bjt_load();
#endif


int
BJTload(inModel,ckt)

/* actually load the current resistance value into the 
 * sparse matrix previously provided 
 */
GENmodel *inModel;
CKTcircuit *ckt;
{
    BJTmodel *model = (BJTmodel*)inModel;
    BJTinstance *here;
    struct bjtstuff bj;
    int error;

    if (ckt->CKTmode & MODEINITFLOAT) {

        /* loop through all the models */
        for ( ; model != NULL; model = model->BJTnextModel) {

            /* loop through all the instances of the model */
            for (here = model->BJTinstances; here != NULL;
                    here = here->BJTnextInstance) {

                bj.bj_vt = here->BJTtemp * CONSTKoverQ;

                if (model->BJTtype > 0) {
                    bj.bj_vbe =
                        *(ckt->CKTrhsOld + here->BJTbasePrimeNode) -
                        *(ckt->CKTrhsOld + here->BJTemitPrimeNode);
                    bj.bj_vbc =
                        *(ckt->CKTrhsOld + here->BJTbasePrimeNode) -
                        *(ckt->CKTrhsOld + here->BJTcolPrimeNode);
                    bj.bj_vbx =
                        *(ckt->CKTrhsOld + here->BJTbaseNode) -
                        *(ckt->CKTrhsOld + here->BJTcolPrimeNode);
                    bj.bj_vcs =
                        *(ckt->CKTrhsOld + here->BJTsubstNode) -
                        *(ckt->CKTrhsOld + here->BJTcolPrimeNode);
                }
                else {
                    bj.bj_vbe =
                        *(ckt->CKTrhsOld + here->BJTemitPrimeNode) -
                        *(ckt->CKTrhsOld + here->BJTbasePrimeNode);
                    bj.bj_vbc =
                        *(ckt->CKTrhsOld + here->BJTcolPrimeNode) -
                        *(ckt->CKTrhsOld + here->BJTbasePrimeNode);
                    bj.bj_vbx =
                        *(ckt->CKTrhsOld + here->BJTcolPrimeNode) -
                        *(ckt->CKTrhsOld + here->BJTbaseNode);
                    bj.bj_vcs =
                        *(ckt->CKTrhsOld + here->BJTcolPrimeNode) -
                        *(ckt->CKTrhsOld + here->BJTsubstNode);
                }
                if (ckt->CKTbypass && bjt_bypass(ckt,model,here,&bj))
                    continue;
                (void)bjt_limiting(ckt,model,here,&bj);
                bjt_iv(ckt,model,here,&bj);
                if (ckt->CKTmode & MODETRAN) {
                    bjt_cap(ckt,model,here,&bj);
                    (void)bjt_integ(ckt,model,here,&bj);
                }
                bjt_load(ckt,model,here,&bj);
            }
        }
        return (OK);
    }

    if (ckt->CKTmode & MODEINITPRED) {
        double xf1;
        double xf2;

        xf2 = -ckt->CKTdelta/ckt->CKTdeltaOld[1];
        xf1 = 1 - xf2;

        /* loop through all the models */
        for ( ; model != NULL; model = model->BJTnextModel) {

            /* loop through all the instances of the model */
            for (here = model->BJTinstances; here != NULL;
                    here = here->BJTnextInstance) {

                bj.bj_vt = here->BJTtemp * CONSTKoverQ;

                bj.bj_vbe = xf1* *(ckt->CKTstate1 + here->BJTvbe) +
                        xf2* *(ckt->CKTstate2 + here->BJTvbe);
                bj.bj_vbc = xf1* *(ckt->CKTstate1 + here->BJTvbc) +
                        xf2* *(ckt->CKTstate2 + here->BJTvbc);
                bj.bj_vbx = xf1* *(ckt->CKTstate1 + here->BJTvbx) +
                        xf2* *(ckt->CKTstate2 + here->BJTvbx);
                bj.bj_vcs = xf1* *(ckt->CKTstate1 + here->BJTvcs) +
                        xf2* *(ckt->CKTstate2 + here->BJTvcs);
                *(ckt->CKTstate0 + here->BJTvbe) = 
                        *(ckt->CKTstate1 + here->BJTvbe);
                *(ckt->CKTstate0 + here->BJTvbc) = 
                        *(ckt->CKTstate1 + here->BJTvbc);

                (void)bjt_limiting(ckt,model,here,&bj);
                bjt_iv(ckt,model,here,&bj);
                if (ckt->CKTmode & MODETRAN) {
                    bjt_cap(ckt,model,here,&bj);
                    (void)bjt_integ(ckt,model,here,&bj);
                }
                bjt_load(ckt,model,here,&bj);
            }
        }
        return (OK);
    }

    if (ckt->CKTmode & MODEINITFIX) {

        /* loop through all the models */
        for ( ; model != NULL; model = model->BJTnextModel) {

            /* loop through all the instances of the model */
            for (here = model->BJTinstances; here != NULL;
                    here = here->BJTnextInstance) {

                bj.bj_vt = here->BJTtemp * CONSTKoverQ;

                if (here->BJToff) {
                    bj.bj_vbe = 0;
                    bj.bj_vbc = 0;
                    bj.bj_vcs = 0;
                    bj.bj_vbx = 0;
                }
                else {
                    if (model->BJTtype > 0) {
                        bj.bj_vbe =
                            *(ckt->CKTrhsOld + here->BJTbasePrimeNode) -
                            *(ckt->CKTrhsOld + here->BJTemitPrimeNode);
                        bj.bj_vbc =
                            *(ckt->CKTrhsOld + here->BJTbasePrimeNode) -
                            *(ckt->CKTrhsOld + here->BJTcolPrimeNode);
                        bj.bj_vbx =
                            *(ckt->CKTrhsOld + here->BJTbaseNode) -
                            *(ckt->CKTrhsOld + here->BJTcolPrimeNode);
                        bj.bj_vcs =
                            *(ckt->CKTrhsOld + here->BJTsubstNode) -
                            *(ckt->CKTrhsOld + here->BJTcolPrimeNode);
                    }
                    else {
                        bj.bj_vbe =
                            *(ckt->CKTrhsOld + here->BJTemitPrimeNode) -
                            *(ckt->CKTrhsOld + here->BJTbasePrimeNode);
                        bj.bj_vbc =
                            *(ckt->CKTrhsOld + here->BJTcolPrimeNode) -
                            *(ckt->CKTrhsOld + here->BJTbasePrimeNode);
                        bj.bj_vbx =
                            *(ckt->CKTrhsOld + here->BJTcolPrimeNode) -
                            *(ckt->CKTrhsOld + here->BJTbaseNode);
                        bj.bj_vcs =
                            *(ckt->CKTrhsOld + here->BJTcolPrimeNode) -
                            *(ckt->CKTrhsOld + here->BJTsubstNode);
                    }
                    if (bjt_limiting(ckt,model,here,&bj))
                        ckt->CKTnoncon++;
                }
                bjt_iv(ckt,model,here,&bj);
                bjt_load(ckt,model,here,&bj);
            }
        }
        return (OK);
    }

    if (ckt->CKTmode & MODEINITTRAN) {

        /* loop through all the models */
        for ( ; model != NULL; model = model->BJTnextModel) {

            /* loop through all the instances of the model */
            for (here = model->BJTinstances; here != NULL;
                    here = here->BJTnextInstance) {

                bj.bj_vt = here->BJTtemp * CONSTKoverQ;

                bj.bj_vbe = *(ckt->CKTstate1 + here->BJTvbe);
                bj.bj_vbc = *(ckt->CKTstate1 + here->BJTvbc);
                bj.bj_vbx = *(ckt->CKTstate1 + here->BJTvbx);
                bj.bj_vcs = *(ckt->CKTstate1 + here->BJTvcs);

                bjt_iv(ckt,model,here,&bj);
                bjt_cap(ckt,model,here,&bj);

                *(ckt->CKTstate1 + here->BJTqbe) =
                    *(ckt->CKTstate0 + here->BJTqbe);
                *(ckt->CKTstate1 + here->BJTqbc) =
                    *(ckt->CKTstate0 + here->BJTqbc);
                *(ckt->CKTstate1 + here->BJTqbx) =
                    *(ckt->CKTstate0 + here->BJTqbx);
                *(ckt->CKTstate1 + here->BJTqcs) =
                    *(ckt->CKTstate0 + here->BJTqcs);

                (void)bjt_integ(ckt,model,here,&bj);
                bjt_load(ckt,model,here,&bj);
            }
        }
        return (OK);
    }

    if (ckt->CKTmode & MODEINITSMSIG) {

        /* loop through all the models */
        for ( ; model != NULL; model = model->BJTnextModel) {

            /* loop through all the instances of the model */
            for (here = model->BJTinstances; here != NULL;
                    here = here->BJTnextInstance) {

                bj.bj_vt = here->BJTtemp * CONSTKoverQ;

                bj.bj_vbe = *(ckt->CKTstate0 + here->BJTvbe);
                bj.bj_vbc = *(ckt->CKTstate0 + here->BJTvbc);
                bj.bj_vbx = *(ckt->CKTstate0 + here->BJTvbx);
                bj.bj_vcs = *(ckt->CKTstate0 + here->BJTvcs);

                bjt_iv(ckt,model,here,&bj);
                bjt_cap(ckt,model,here,&bj);
            }
        }
        return (OK);
    }

    if ((ckt->CKTmode & MODEINITJCT) && 
            (ckt->CKTmode & MODETRANOP) && (ckt->CKTmode & MODEUIC)) {

        /* loop through all the models */
        for ( ; model != NULL; model = model->BJTnextModel) {

            /* loop through all the instances of the model */
            for (here = model->BJTinstances; here != NULL;
                    here = here->BJTnextInstance) {

                if (model->BJTtype > 0) {
                    *(ckt->CKTstate0 + here->BJTvbe) = here->BJTicVBE;
                    *(ckt->CKTstate0 + here->BJTvbc) = 
                        (here->BJTicVBE - here->BJTicVCE);
                    *(ckt->CKTstate0 + here->BJTvbx) = 
                        (here->BJTicVBE - here->BJTicVCE);
                }
                else {
                    *(ckt->CKTstate0 + here->BJTvbe) = -here->BJTicVBE;
                    *(ckt->CKTstate0 + here->BJTvbc) = 
                        -(here->BJTicVBE - here->BJTicVCE);
                    *(ckt->CKTstate0 + here->BJTvbx) = 
                        -(here->BJTicVBE - here->BJTicVCE);
                }
                *(ckt->CKTstate0 + here->BJTvcs) = 0;
            }
        }
        return (OK);
    }

    if (ckt->CKTmode & MODEINITJCT) {

        /* loop through all the models */
        for ( ; model != NULL; model = model->BJTnextModel) {

            /* loop through all the instances of the model */
            for (here = model->BJTinstances; here != NULL;
                    here = here->BJTnextInstance) {

                bj.bj_vt = here->BJTtemp * CONSTKoverQ;

                if (here->BJToff)
                    bj.bj_vbe = 0;
                else
                    bj.bj_vbe = here->BJTtVcrit;
                bj.bj_vbc = 0;
                bj.bj_vcs = 0;
                bj.bj_vbx = 0;

                bjt_iv(ckt,model,here,&bj);
                bjt_load(ckt,model,here,&bj);
            }
        }
    }
    return (OK);
}


static int
bjt_bypass(ckt,model,here,bj)

CKTcircuit *ckt;
BJTmodel *model;
BJTinstance *here;
struct bjtstuff *bj;
{
    double delvbe;
    double delvbc;
    double cchat;
    double cbhat;
    double A1, A2, A3;

    /*
     *   bypass if solution has not changed
     */
    delvbe = bj->bj_vbe - *(ckt->CKTstate0 + here->BJTvbe);
    A1 = FABS(bj->bj_vbe);
    A2 = FABS(*(ckt->CKTstate0 + here->BJTvbe));
    if (FABS(delvbe) >= ckt->CKTreltol*MAX(A1,A2) + ckt->CKTvoltTol)
        return (0);

    delvbc = bj->bj_vbc - *(ckt->CKTstate0 + here->BJTvbc);
    A1 = FABS(bj->bj_vbc);
    A2 = FABS(*(ckt->CKTstate0 + here->BJTvbc));
    if (FABS(delvbc) >= ckt->CKTreltol*MAX(A1,A2) + ckt->CKTvoltTol)
        return (0);

    cchat = here->BJTcc +
            (here->BJTgm + here->BJTgo)*delvbe -
            (here->BJTgo + here->BJTgmu)*delvbc;

    A1 = cchat - here->BJTcc;
    A2 = FABS(cchat);
    A3 = FABS(here->BJTcc);
    if (FABS(A1) >= ckt->CKTreltol*MAX(A2,A3) + ckt->CKTabstol)
        return (0);

    cbhat = here->BJTcb +
            here->BJTgpi*delvbe +
            here->BJTgmu*delvbc;

    A1 = cbhat - here->BJTcb;
    A2 = FABS(cbhat);
    A3 = FABS(here->BJTcb);
    if (FABS(A1) >= ckt->CKTreltol*MAX(A2,A3) + ckt->CKTabstol)
        return (0);

    /*
     * bypassing....
     */
    bj->bj_vbe = *(ckt->CKTstate0 + here->BJTvbe);
    bj->bj_vbc = *(ckt->CKTstate0 + here->BJTvbc);
    bj->bj_vbx = *(ckt->CKTstate0 + here->BJTvbx);
    bj->bj_vcs = *(ckt->CKTstate0 + here->BJTvcs);

    bjt_load(ckt,model,here,bj);
    return (1);
}


static int
bjt_limiting(ckt,model,here,bj)

/*
 *   limit nonlinear branch voltages
 */
CKTcircuit *ckt;
BJTmodel *model;
BJTinstance *here;
struct bjtstuff *bj;
{
    int i = 0;

    i += bjt_pnjlim(&bj->bj_vbe,*(ckt->CKTstate0 + here->BJTvbe),bj->bj_vt,
            here->BJTtVcrit);
    i += bjt_pnjlim(&bj->bj_vbc,*(ckt->CKTstate0 + here->BJTvbc),bj->bj_vt,
            here->BJTtVcrit);
    return (i);
}


static int
bjt_pnjlim(vnew,vold,vt,vcrit)

/*
 * limit the per-iteration change of PN junction voltages 
 */
double *vnew;
double vold;
double vt;
double vcrit;
{
    double arg;
    double vn;

    vn = *vnew;
    if ((vn > vcrit) && (FABS(vn - vold) > (vt + vt))) {
        if (vold > 0) {
            arg = 1 + (vn - vold)/vt;
            if (arg > 0) {
                *vnew = vold + vt*log(arg);
            }
            else {
                *vnew = vcrit;
            }
        }
        else {
            *vnew = vt*log(vn/vt);
        }
        return (1);
    }
    return (0);
}


static void
bjt_iv(ckt,model,here,bj)

CKTcircuit *ckt;
BJTmodel *model;
BJTinstance *here;
struct bjtstuff *bj;
{
    double td;
    double vte;
    double vtc;
    double csat;
    double c2;
    double c4;
    double rbpr;
    double rbpi;
    double xjrb;
    double vtn;
    double cbe;
    double gbe;
    double cben;
    double gben;
    double cbc;
    double gbc;
    double cbcn;
    double gbcn;
    double dqbdve;
    double dqbdvc;
    double qb;
    double vbe;
    double vbc;
    double temp, temp1;
    double arg1, arg2, arg3;

    here->BJTgeqbx = 0;
    here->BJTgccs  = 0;
    here->BJTgeqcb = 0;

    vte  = model->BJTleakBEemissionCoeff*bj->bj_vt;
    vtc  = model->BJTleakBCemissionCoeff*bj->bj_vt;
    vbe  = bj->bj_vbe;
    vbc  = bj->bj_vbc;

    csat = here->BJTtSatCur;
    c2   = here->BJTtBEleakCur;
    c4   = here->BJTtBCleakCur;
    rbpr = model->BJTminBaseResist;
    rbpi = model->BJTbaseResist;
    xjrb = model->BJTbaseCurrentHalfResist;

    temp = here->BJTarea;
    if (temp != 1.0) {
        csat *= temp;
        c2   *= temp;
        c4   *= temp;
        rbpr /= temp;
        rbpi /= temp;
        xjrb *= temp;
    }
    rbpi -= rbpr;

    /*
     *   determine dc current and derivitives
     */
    vtn = bj->bj_vt*model->BJTemissionCoeffF;
    if (vbe > -5*vtn) {
        temp = exp(vbe/vtn);
        cbe = csat*(temp - 1) + ckt->CKTgmin*vbe;
        gbe = csat*temp/vtn + ckt->CKTgmin;
        if (c2 == 0) {
            cben = 0;
            gben = 0;
        }
        else {
            temp = exp(vbe/vte);
            cben = c2*(temp - 1);
            gben = c2*temp/vte;
        }
    }
    else {
        gbe  = -csat/vbe + ckt->CKTgmin;
        cbe  = gbe*vbe;
        gben = -c2/vbe;
        cben = gben*vbe;
    }

    vtn = bj->bj_vt*model->BJTemissionCoeffR;
    if (vbc > -5*vtn) {
        temp = exp(vbc/vtn);
        cbc = csat*(temp - 1) + ckt->CKTgmin*vbc;
        gbc = csat*temp/vtn + ckt->CKTgmin;
        if (c4 == 0) {
            cbcn = 0;
            gbcn = 0;
        }
        else {
            temp = exp(vbc/vtc);
            cbcn = c4*(temp - 1);
            gbcn = c4*temp/vtc;
        }
    }
    else {
        gbc = -csat/vbc + ckt->CKTgmin;
        cbc = gbc*vbc;
        gbcn = -c4/vbc;
        cbcn = gbcn*vbc;
    }

    /*
     *   determine base charge terms
     */
    temp = 1/(1 - model->BJTinvEarlyVoltF*vbc -
        model->BJTinvEarlyVoltR*vbe);

    if (model->BJTinvRollOffF == 0 && model->BJTinvRollOffR == 0) {
        qb = temp;
        temp *= temp;
        dqbdve = temp*model->BJTinvEarlyVoltR;
        dqbdvc = temp*model->BJTinvEarlyVoltF;
    }
    else {
        double arg, sqarg, oik, oikr;

        oik  = model->BJTinvRollOffF/here->BJTarea;
        oikr = model->BJTinvRollOffR/here->BJTarea;

        arg = 1 + 4*(oik*cbe + oikr*cbc);
        if (arg < 0) arg = 0;
        sqarg = 1;
        if (arg != 0) sqarg = sqrt(arg);
        qb = .5*temp*(1 + sqarg);
        dqbdve = temp*(qb*model->BJTinvEarlyVoltR + oik*gbe/sqarg);
        dqbdvc = temp*(qb*model->BJTinvEarlyVoltF + oikr*gbc/sqarg);
    }

    qb = 1/qb;

    here->BJTcb = cbe/here->BJTtBetaF + cben + cbc/here->BJTtBetaR + cbcn;

    /*
     *   determine dc incremental conductances
     */
    if (xjrb != 0) {
        temp = here->BJTcb/xjrb;
        arg1 = MAX(temp,1e-9);
        arg2 = (-1 + sqrt(1 + 14.59025*arg1))/(2.4317*sqrt(arg1));
        arg1 = tan(arg2);
        temp = rbpr + 3*rbpi*(arg1 - arg2)/(arg2*arg1*arg1);
    }
    else
        temp = rbpr + rbpi*qb;

    if (temp != 0)
        here->BJTgx = 1/temp;
    else
        here->BJTgx = 0;

    here->BJTgpi = gbe/here->BJTtBetaF + gben;
    here->BJTgmu = gbc/here->BJTtBetaR + gbcn;

    /*
     *   weil's approx. for excess phase applied with backward-
     *   euler integration
     */
    td  = model->BJTexcessPhaseFactor;
    if (ckt->CKTmode & (MODETRAN | MODEAC) && td != 0) {
        double denom, xf1, xf2, cex, gex;

        arg1  = ckt->CKTdelta/td;
        arg2  = 3*arg1;
        arg1  = arg2*arg1;
        denom = 1 + arg1 + arg2;
        arg3  = arg1/denom;
        if (ckt->CKTmode & MODEINITTRAN) {
            *(ckt->CKTstate1 + here->BJTcexbc) = cbe*qb;
            *(ckt->CKTstate2 + here->BJTcexbc) =
                    *(ckt->CKTstate1 + here->BJTcexbc);
        }
        xf2 = -ckt->CKTdelta/ckt->CKTdeltaOld[1];
        xf1 = 1 - xf2;
        here->BJTcc = (*(ckt->CKTstate1 + here->BJTcexbc)*(xf1 + arg2) +
                *(ckt->CKTstate2 + here->BJTcexbc)*xf2)/denom;
        cex = cbe*arg3;
        gex = gbe*arg3;
        *(ckt->CKTstate0 + here->BJTcexbc) = here->BJTcc + cex*qb;
        temp = (cex - cbc)*qb;
        here->BJTcc += temp - cbc/here->BJTtBetaR - cbcn;
        here->BJTgo  = (gbc + temp*dqbdvc)*qb;
        here->BJTgm  = (gex - temp*dqbdve)*qb - here->BJTgo;
    }
    else {
        temp = (cbe - cbc)*qb;
        here->BJTcc  = temp - cbc/here->BJTtBetaR - cbcn;
        here->BJTgo  = (gbc + temp*dqbdvc)*qb;
        here->BJTgm  = (gbe - temp*dqbdve)*qb - here->BJTgo;
    }

    if (ckt->CKTmode & (MODETRAN | MODEAC)) {
        double tf   = model->BJTtransitTimeF;
        double xtf  = model->BJTtransitTimeBiasCoeffF;
        double ovtf = model->BJTtransitTimeVBCFactor;
        double xjtf = model->BJTtransitTimeHighCurrentF;

        if (tf != 0 && vbe > 0) {

            arg1  = 0;
            arg2  = 0;
            arg3  = 0;
            if (xtf != 0) {
                arg1 = xtf;
                if (ovtf != 0) {
                    arg1 *= exp(bj->bj_vbc*ovtf);
                }
                arg2 = arg1;
                if (xjtf != 0) {
                    temp  = cbe/(cbe + xjtf*here->BJTarea);
                    arg1 *= temp*temp;
                    arg2  = arg1*(3 - temp - temp);
                }
                arg3 = cbe*arg1*ovtf;
            }
            cbe *= (1 + arg1)*qb;
            gbe  = (gbe*(1 + arg2) - cbe*dqbdve)*qb;
            here->BJTgeqcb = tf*(arg3 - cbe*dqbdvc)*qb;
        }
    }
    bj->bj_cbe = cbe;
    bj->bj_gbe = gbe;
    bj->bj_cbc = cbc;
    bj->bj_gbc = gbc;
}


static void
bjt_cap(ckt,model,here,bj)

CKTcircuit *ckt;
BJTmodel *model;
BJTinstance *here;
struct bjtstuff *bj;
{
    /*
     *   charge storage elements
     */
    double tf;
    double tr;
    double arg;
    double sarg;
    double xm;
    double pt;
    double fcp;
    double cap;
    double vx;
    double f1, f2, f3;
    double czf2;

    tf  = model->BJTtransitTimeF;
    tr  = model->BJTtransitTimeR;

    xm  = model->BJTjunctionExpBE;
    pt  = here->BJTtBEpot;
    fcp = here->BJTtDepCap;
    cap = here->BJTtBEcap*here->BJTarea;
    vx  = bj->bj_vbe;

    if (vx < fcp) {
        arg  = 1 - vx/pt;
        sarg = exp(-xm*log(arg));
        *(ckt->CKTstate0 + here->BJTqbe) =
            tf*bj->bj_cbe + pt*cap*(1 - arg*sarg)/(1 - xm);
        here->BJTcapbe = tf*bj->bj_gbe + cap*sarg;
    }
    else {
        f1 = here->BJTtf1;
        f2 = model->BJTf2;
        f3 = model->BJTf3;
        czf2 = cap/f2;
        *(ckt->CKTstate0 + here->BJTqbe) = tf*bj->bj_cbe + cap*f1 +
            czf2*(f3*(vx - fcp) + (xm/(pt + pt))*(vx*vx - fcp*fcp));
        here->BJTcapbe = tf*bj->bj_gbe + czf2*(f3 + xm*vx/pt);
    }

    xm  = model->BJTjunctionExpBC;
    pt  = here->BJTtBCpot;
    fcp = here->BJTtf4;
    cap = here->BJTtBCcap * model->BJTbaseFractionBCcap * here->BJTarea;
    vx  = bj->bj_vbc;

    if (vx < fcp) {
        arg  = 1 - vx/pt;
        sarg = exp(-xm*log(arg));
        *(ckt->CKTstate0 + here->BJTqbc) = tr*bj->bj_cbc +
            pt*cap*(1 - arg*sarg)/(1 - xm);
        here->BJTcapbc = tr*bj->bj_gbc + cap*sarg;
    }
    else {
        f1 = here->BJTtf5;
        f2 = model->BJTf6;
        f3 = model->BJTf7;
        czf2 = cap/f2;
        *(ckt->CKTstate0 + here->BJTqbc) = tr*bj->bj_cbc + cap*f1 +
            czf2*(f3*(vx - fcp) + (xm/(pt + pt))*(vx*vx - fcp*fcp));
        here->BJTcapbc = tr*bj->bj_gbc + czf2*(f3 + xm*vx/pt);
    }

    cap = here->BJTtBCcap * here->BJTarea - cap;
    vx  = bj->bj_vbx;

    if (vx < fcp) {
        arg  = 1 - vx/pt;
        sarg = exp(-xm*log(arg));
        *(ckt->CKTstate0 + here->BJTqbx) = pt*cap*(1 - arg*sarg)/(1 - xm);
        here->BJTcapbx = cap*sarg;
    }
    else {
        f1 = here->BJTtf5;
        f2 = model->BJTf6;
        f3 = model->BJTf7;
        czf2 = cap/f2;
        *(ckt->CKTstate0 + here->BJTqbx) = cap*f1 +
            czf2*(f3*(vx - fcp) + (xm/(pt + pt))*(vx*vx - fcp*fcp));
        here->BJTcapbx = czf2*(f3 + xm*vx/pt);
    }

    xm  = model->BJTexponentialSubstrate;
    pt  = model->BJTpotentialSubstrate;
    cap = model->BJTcapCS;
    vx  = bj->bj_vcs;

    if (vx < 0) {
        arg = 1 - vx/pt;
        sarg = exp(-xm*log(arg));
        *(ckt->CKTstate0 + here->BJTqcs) = pt*cap*(1 - arg*sarg)/(1 - xm);
        here->BJTcapcs = cap*sarg;
    }
    else {
        *(ckt->CKTstate0 + here->BJTqcs) = vx*cap*(1 + xm*vx/(pt + pt));
        here->BJTcapcs = cap*(1 + xm*vx/pt);
    }
}


static int
bjt_integ(ckt,model,here,bj)

CKTcircuit *ckt;
BJTmodel *model;
BJTinstance *here;
struct bjtstuff *bj;
{
    double geq, ceq;
    int error; /* in NI_INTEG macro */

    here->BJTgeqcb *= ckt->CKTag[0];

    NI_INTEG(ckt,geq,ceq,here->BJTcapbe,here->BJTqbe);
    here->BJTgpi += geq;
    here->BJTcb  += *(ckt->CKTstate0 + here->BJTcqbe);

    NI_INTEG(ckt,geq,ceq,here->BJTcapbc,here->BJTqbc);
    here->BJTgmu += geq;
    here->BJTcb  += *(ckt->CKTstate0 + here->BJTcqbc);
    here->BJTcc  -= *(ckt->CKTstate0 + here->BJTcqbc);
    /*
     *      charge storage for c-s and b-x junctions
     */
    NI_INTEG(ckt,here->BJTgccs,ceq,here->BJTcapcs,here->BJTqcs);
    NI_INTEG(ckt,here->BJTgeqbx,ceq,here->BJTcapbx,here->BJTqbx);
    return (OK);
}


static void
bjt_load(ckt,model,here,bj)

CKTcircuit *ckt;
BJTmodel *model;
BJTinstance *here;
struct bjtstuff *bj;
{
    double gcpr;
    double gepr;
    double ceqcs;
    double ceqbx;
    double ceqbe;
    double ceqbc;
    double geqbx;
    double gccs;
    double geqcb;
    double cc;
    double cb;
    double gx;
    double gpi;
    double gmu;
    double gm;
    double go;

    cc    = here->BJTcc;
    cb    = here->BJTcb;
    gx    = here->BJTgx;
    gpi   = here->BJTgpi;
    gmu   = here->BJTgmu;
    gm    = here->BJTgm;
    go    = here->BJTgo;
    geqbx = here->BJTgeqbx;
    gccs  = here->BJTgccs;
    geqcb = here->BJTgeqcb;

    gcpr = model->BJTcollectorConduct*here->BJTarea;
    gepr = model->BJTemitterConduct*here->BJTarea;

    *(ckt->CKTstate0 + here->BJTvbe) = bj->bj_vbe;
    *(ckt->CKTstate0 + here->BJTvbc) = bj->bj_vbc;
    *(ckt->CKTstate0 + here->BJTvbx) = bj->bj_vbx;
    *(ckt->CKTstate0 + here->BJTvcs) = bj->bj_vcs;

    /*
     *  load current excitation vector
     */
    if (model->BJTtype > 0) {
        ceqcs = (*(ckt->CKTstate0 + here->BJTcqcs) - bj->bj_vcs*gccs);
        ceqbx = (*(ckt->CKTstate0 + here->BJTcqbx) - bj->bj_vbx*geqbx);
        ceqbe = (cc + cb -
                bj->bj_vbe*(gm + go + gpi) +
                bj->bj_vbc*(go - geqcb));
        ceqbc = (-cc + bj->bj_vbe*(gm + go) -
                bj->bj_vbc*(gmu + go));
    }
    else {
        ceqcs = -(*(ckt->CKTstate0 + here->BJTcqcs) - bj->bj_vcs*gccs);
        ceqbx = -(*(ckt->CKTstate0 + here->BJTcqbx) - bj->bj_vbx*geqbx);
        ceqbe = -(cc + cb -
                bj->bj_vbe*(gm + go + gpi) +
                bj->bj_vbc*(go - geqcb));
        ceqbc = -(-cc + bj->bj_vbe*(gm + go) -
                bj->bj_vbc*(gmu + go));
    }

    *(ckt->CKTrhs + here->BJTbaseNode)      -= ceqbx;
    *(ckt->CKTrhs + here->BJTcolPrimeNode)  += ceqcs + ceqbx + ceqbc;
    *(ckt->CKTrhs + here->BJTbasePrimeNode) -= ceqbe + ceqbc;
    *(ckt->CKTrhs + here->BJTemitPrimeNode) += ceqbe;
    *(ckt->CKTrhs + here->BJTsubstNode)     -= ceqcs;

    /*
     *  load y matrix
     */
    *(here->BJTcolColPtr)             += gcpr;
    *(here->BJTbaseBasePtr)           += gx + geqbx;
    *(here->BJTemitEmitPtr)           += gepr;
    *(here->BJTcolPrimeColPrimePtr)   += gmu + go + gcpr + gccs + geqbx;
    *(here->BJTbasePrimeBasePrimePtr) += gx + gpi + gmu + geqcb;
    *(here->BJTemitPrimeEmitPrimePtr) += gpi + gepr + gm + go;
    *(here->BJTcolColPrimePtr)        -= gcpr;
    *(here->BJTbaseBasePrimePtr)      -= gx;
    *(here->BJTemitEmitPrimePtr)      -= gepr;
    *(here->BJTcolPrimeColPtr)        -= gcpr;
    *(here->BJTcolPrimeBasePrimePtr)  += -gmu + gm;
    *(here->BJTcolPrimeEmitPrimePtr)  -= gm + go;
    *(here->BJTbasePrimeBasePtr)      -= gx;
    *(here->BJTbasePrimeColPrimePtr)  -= gmu + geqcb;
    *(here->BJTbasePrimeEmitPrimePtr) -= gpi;
    *(here->BJTemitPrimeEmitPtr)      -= gepr;
    *(here->BJTemitPrimeColPrimePtr)  += -go + geqcb;
    *(here->BJTemitPrimeBasePrimePtr) -= gpi + gm + geqcb;
    *(here->BJTsubstSubstPtr)         += gccs;
    *(here->BJTcolPrimeSubstPtr)      -= gccs;
    *(here->BJTsubstColPrimePtr)      -= gccs;
    *(here->BJTbaseColPrimePtr)       -= geqbx;
    *(here->BJTcolPrimeBasePtr)       -= geqbx;
}
