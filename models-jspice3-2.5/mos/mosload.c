/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1989 Takayasu Sakurai
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include <math.h>
#include "mosdefs.h"
#include "const.h"
#include "sperror.h"
#include "util.h"
#include "niext.h"

/* some constants to avoid slow divides */
#define P66 .66666666666667
#define P33 .33333333333333


#ifdef __STDC__
static int  mos_limiting(CKTcircuit*,MOSinstance*,struct mosstuff*);
static void mos_limvds(double*,double);
static int  mos_pnjlim(double*,double,double,double);
static void mos_fetlim(double*,double,double);
static int  mos_bypass(CKTcircuit*,MOSmodel*,MOSinstance*,struct mosstuff*);
static void mos_iv(CKTcircuit*,MOSmodel*,MOSinstance*,struct mosstuff*);
static void mos_cap(CKTcircuit*,MOSmodel*,MOSinstance*,struct mosstuff*);
static int  mos_integ(CKTcircuit*,MOSinstance*,struct mosstuff*);
static void mos_load(CKTcircuit*,MOSmodel*,MOSinstance*,struct mosstuff*);
static void mos_load_dc(CKTcircuit*,MOSmodel*,MOSinstance*,struct mosstuff*);
#else
static int  mos_limiting();
static void mos_limvds();
static int  mos_pnjlim();
static void mos_fetlim();
static int  mos_bypass();
static void mos_iv();
static void mos_cap();
static int  mos_integ();
static void mos_load();
static void mos_load_dc();
#endif


int
MOSload(inModel,ckt)

GENmodel *inModel;
CKTcircuit *ckt;
/*
 * actually load the current value into the 
 * sparse matrix previously provided 
 */
{
    MOSmodel *model = (MOSmodel *)inModel;
    MOSinstance *here;
    struct mosstuff ms;
    int error;

    if (ckt->CKTmode & MODEINITFLOAT) {

        /* loop through all the MOS device models */
        for ( ; model != NULL; model = model->MOSnextModel) {

            /* loop through all the instances of the model */
            for (here = model->MOSinstances; here != NULL;
                    here = here->MOSnextInstance) {

                ms.ms_vt = CONSTKoverQ * here->MOStemp;

                if (model->MOStype > 0) {
                    ms.ms_vbs = *(ckt->CKTrhsOld + here->MOSbNode) -
                            *(ckt->CKTrhsOld + here->MOSsNodePrime);
                    ms.ms_vgs = *(ckt->CKTrhsOld + here->MOSgNode) -
                            *(ckt->CKTrhsOld + here->MOSsNodePrime);
                    ms.ms_vds = *(ckt->CKTrhsOld + here->MOSdNodePrime) -
                            *(ckt->CKTrhsOld + here->MOSsNodePrime);
                    ms.ms_von = here->MOSvon;
                }
                else {
                    ms.ms_vbs = *(ckt->CKTrhsOld + here->MOSsNodePrime) -
                            *(ckt->CKTrhsOld + here->MOSbNode);
                    ms.ms_vgs = *(ckt->CKTrhsOld + here->MOSsNodePrime) -
                            *(ckt->CKTrhsOld + here->MOSgNode);
                    ms.ms_vds = *(ckt->CKTrhsOld + here->MOSsNodePrime) -
                            *(ckt->CKTrhsOld + here->MOSdNodePrime);
                    ms.ms_von = -here->MOSvon;
                }
                ms.ms_vbd = ms.ms_vbs - ms.ms_vds;
                ms.ms_vgd = ms.ms_vgs - ms.ms_vds;
                ms.ms_vgb = ms.ms_vgs - ms.ms_vbs;

#ifndef NOBYPASS
                if (ckt->CKTbypass && mos_bypass(ckt,model,here,&ms)) {
                    continue;
                }
#endif
                (void)mos_limiting(ckt,here,&ms);
                mos_iv(ckt,model,here,&ms);
                if (ckt->CKTmode & MODETRAN) {
                    mos_cap(ckt,model,here,&ms);
                    error = mos_integ(ckt,here,&ms);
                    if (error) return(error);
                    mos_load(ckt,model,here,&ms);
                }
                else
                    mos_load_dc(ckt,model,here,&ms);
            }
        }
        return (OK);
    }

    if (ckt->CKTmode & MODEINITPRED) {
        double xf1;
        double xf2;

        xf2 = -ckt->CKTdelta/ckt->CKTdeltaOld[1];
        xf1 = 1 - xf2;

        /* loop through all the MOS device models */
        for ( ; model != NULL; model = model->MOSnextModel) {

            /* loop through all the instances of the model */
            for (here = model->MOSinstances; here != NULL;
                    here = here->MOSnextInstance) {

                ms.ms_vt = CONSTKoverQ * here->MOStemp;

                /* predictor step */
                ms.ms_vbs = xf1* *(ckt->CKTstate1 + here->MOSvbs)
                        + xf2* *(ckt->CKTstate2 + here->MOSvbs);

                ms.ms_vgs = xf1* *(ckt->CKTstate1 + here->MOSvgs)
                        + xf2* *(ckt->CKTstate2 + here->MOSvgs);

                ms.ms_vds = xf1* *(ckt->CKTstate1 + here->MOSvds)
                        + xf2* *(ckt->CKTstate2 + here->MOSvds);

                ms.ms_vbd = ms.ms_vbs - ms.ms_vds;
                ms.ms_vgd = ms.ms_vgs - ms.ms_vds;
                ms.ms_vgb = ms.ms_vgs - ms.ms_vbs;
                if (model->MOStype > 0)
                    ms.ms_von = here->MOSvon;
                else
                    ms.ms_von = -here->MOSvon;

                *(ckt->CKTstate0 + here->MOSvbs) = 
                        *(ckt->CKTstate1 + here->MOSvbs);
                *(ckt->CKTstate0 + here->MOSvgs) = 
                        *(ckt->CKTstate1 + here->MOSvgs);
                *(ckt->CKTstate0 + here->MOSvds) = 
                        *(ckt->CKTstate1 + here->MOSvds);
                *(ckt->CKTstate0 + here->MOSvbd) = 
                        *(ckt->CKTstate0 + here->MOSvbs)-
                        *(ckt->CKTstate0 + here->MOSvds);

                (void)mos_limiting(ckt,here,&ms);

                mos_iv(ckt,model,here,&ms);
                if (ckt->CKTmode & MODETRAN) {
                    mos_cap(ckt,model,here,&ms);
                    error = mos_integ(ckt,here,&ms);
                    if (error) return(error);
                    mos_load(ckt,model,here,&ms);
                }
                else
                    mos_load_dc(ckt,model,here,&ms);

            }
        }
        return (OK);
    }

    if (ckt->CKTmode & MODEINITFIX) {

        /* loop through all the MOS device models */
        for ( ; model != NULL; model = model->MOSnextModel) {

            /* loop through all the instances of the model */
            for (here = model->MOSinstances; here != NULL;
                    here = here->MOSnextInstance) {

                ms.ms_vt  = CONSTKoverQ * here->MOStemp;

                if (here->MOSoff) {
                    ms.ms_vbs = 0;
                    ms.ms_vgs = 0;
                    ms.ms_vds = 0;
                    ms.ms_vbd = 0;
                    ms.ms_vgd = 0;
                    ms.ms_vgb = 0;
                }
                else {
                    if (model->MOStype > 0) {
                        ms.ms_vbs = *(ckt->CKTrhsOld + here->MOSbNode) -
                                *(ckt->CKTrhsOld + here->MOSsNodePrime);
                        ms.ms_vgs = *(ckt->CKTrhsOld + here->MOSgNode) -
                                *(ckt->CKTrhsOld + here->MOSsNodePrime);
                        ms.ms_vds = *(ckt->CKTrhsOld + here->MOSdNodePrime) -
                                *(ckt->CKTrhsOld + here->MOSsNodePrime);
                    }
                    else {
                        ms.ms_vbs = *(ckt->CKTrhsOld + here->MOSsNodePrime) -
                                *(ckt->CKTrhsOld + here->MOSbNode);
                        ms.ms_vgs = *(ckt->CKTrhsOld + here->MOSsNodePrime) -
                                *(ckt->CKTrhsOld + here->MOSgNode);
                        ms.ms_vds = *(ckt->CKTrhsOld + here->MOSsNodePrime) -
                                *(ckt->CKTrhsOld + here->MOSdNodePrime);
                    }
                    ms.ms_vbd = ms.ms_vbs - ms.ms_vds;
                    ms.ms_vgd = ms.ms_vgs - ms.ms_vds;
                    ms.ms_vgb = ms.ms_vgs - ms.ms_vbs;
                }

                if (model->MOStype > 0)
                    ms.ms_von = here->MOSvon;
                else
                    ms.ms_von = -here->MOSvon;

#ifndef NOBYPASS
                if (!here->MOSoff)
                if (ckt->CKTbypass && mos_bypass(ckt,model,here,&ms)) {
                    continue;
                }
#endif
                if (!here->MOSoff && mos_limiting(ckt,here,&ms))
                    ckt->CKTnoncon++;

                mos_iv(ckt,model,here,&ms);
                mos_load_dc(ckt,model,here,&ms);
            }
        }
        return (OK);
    }

    if (ckt->CKTmode & MODEINITTRAN) {

        /* loop through all the MOS device models */
        for ( ; model != NULL; model = model->MOSnextModel) {

            /* loop through all the instances of the model */
            for (here = model->MOSinstances; here != NULL;
                    here = here->MOSnextInstance) {

                ms.ms_vt = CONSTKoverQ * here->MOStemp;

                ms.ms_vbs = *(ckt->CKTstate1 + here->MOSvbs);
                ms.ms_vgs = *(ckt->CKTstate1 + here->MOSvgs);
                ms.ms_vds = *(ckt->CKTstate1 + here->MOSvds);
                ms.ms_vbd = ms.ms_vbs - ms.ms_vds;
                ms.ms_vgd = ms.ms_vgs - ms.ms_vds;
                ms.ms_vgb = ms.ms_vgs - ms.ms_vbs;

                mos_iv(ckt,model,here,&ms);
                mos_cap(ckt,model,here,&ms);
                error = mos_integ(ckt,here,&ms);
                if (error) return(error);
                mos_load(ckt,model,here,&ms);
            }
        }
        return (OK);
    }

    if (ckt->CKTmode & MODEINITSMSIG) {

        /* loop through all the MOS device models */
        for ( ; model != NULL; model = model->MOSnextModel) {

            /* loop through all the instances of the model */
            for (here = model->MOSinstances; here != NULL;
                    here = here->MOSnextInstance) {

                ms.ms_vt = CONSTKoverQ * here->MOStemp;
                ms.ms_vbs = *(ckt->CKTstate0 + here->MOSvbs);
                ms.ms_vgs = *(ckt->CKTstate0 + here->MOSvgs);
                ms.ms_vds = *(ckt->CKTstate0 + here->MOSvds);
                ms.ms_vbd = ms.ms_vbs - ms.ms_vds;
                ms.ms_vgd = ms.ms_vgs - ms.ms_vds;
                ms.ms_vgb = ms.ms_vgs - ms.ms_vbs;

                mos_iv(ckt,model,here,&ms);
                mos_cap(ckt,model,here,&ms);
            }
        }
        return (OK);
    }

    if ((ckt->CKTmode & MODEINITJCT) && 
            (ckt->CKTmode & MODETRANOP) && (ckt->CKTmode & MODEUIC) ) {

        /* loop through all the MOS device models */
        for ( ; model != NULL; model = model->MOSnextModel) {

            /* loop through all the instances of the model */
            for (here = model->MOSinstances; here != NULL;
                    here = here->MOSnextInstance) {

                ms.ms_vt  = CONSTKoverQ * here->MOStemp;
                if (model->MOStype > 0) {
                    ms.ms_vds = here->MOSicVDS;
                    ms.ms_vgs = here->MOSicVGS;
                    ms.ms_vbs = here->MOSicVBS;
                }
                else {
                    ms.ms_vds = -here->MOSicVDS;
                    ms.ms_vgs = -here->MOSicVGS;
                    ms.ms_vbs = -here->MOSicVBS;
                }
                *(ckt->CKTstate0 + here->MOSvds) = ms.ms_vds;
                *(ckt->CKTstate0 + here->MOSvgs) = ms.ms_vgs;
                *(ckt->CKTstate0 + here->MOSvbs) = ms.ms_vbs;
            }
        }
        return (OK);
    }

    if (ckt->CKTmode & MODEINITJCT) {

        /* loop through all the MOS device models */
        for ( ; model != NULL; model = model->MOSnextModel) {

            /* loop through all the instances of the model */
            for (here = model->MOSinstances; here != NULL;
                    here = here->MOSnextInstance) {

                ms.ms_vt = CONSTKoverQ * here->MOStemp;

                if (here->MOSoff) {
                    ms.ms_vbs = 0;
                    ms.ms_vgs = 0;
                    ms.ms_vds = 0;
                    ms.ms_vbd = 0;
                    ms.ms_vgd = 0;
                    ms.ms_vgb = 0;
                }
                else {
                    if ((here->MOSicVDS == 0) &&
                        (here->MOSicVGS == 0) &&
                            (here->MOSicVBS == 0)) {
                        ms.ms_vbs = -1;
                        if (model->MOStype > 0)
                            ms.ms_vgs = here->MOStVto;
                        else
                            ms.ms_vgs = -here->MOStVto;
                        ms.ms_vds = 0;
                    }
                    else {
                        if (model->MOStype > 0) {
                            ms.ms_vds = here->MOSicVDS;
                            ms.ms_vgs = here->MOSicVGS;
                            ms.ms_vbs = here->MOSicVBS;
                        }
                        else {
                            ms.ms_vds = -here->MOSicVDS;
                            ms.ms_vgs = -here->MOSicVGS;
                            ms.ms_vbs = -here->MOSicVBS;
                        }
                    }
                    ms.ms_vbd = ms.ms_vbs - ms.ms_vds;
                    ms.ms_vgd = ms.ms_vgs - ms.ms_vds;
                    ms.ms_vgb = ms.ms_vgs - ms.ms_vbs;
                }
                mos_iv(ckt,model,here,&ms);
                mos_load_dc(ckt,model,here,&ms);
            }
        }
    }
    return (OK);
}


static int
mos_limiting(ckt,here,ms)

CKTcircuit *ckt;
MOSinstance *here;
struct mosstuff *ms;
{
    int check;
    double vgdo;
    /*
     * limiting
     * We want to keep device voltages from changing
     * so fast that the exponentials churn out overflows 
     * and similar rudeness
     */

    if (*(ckt->CKTstate0 + here->MOSvds) >= 0) {
        mos_fetlim(&ms->ms_vgs,*(ckt->CKTstate0 + here->MOSvgs),ms->ms_von);
        ms->ms_vds = ms->ms_vgs - ms->ms_vgd;
        mos_limvds(&ms->ms_vds,*(ckt->CKTstate0 + here->MOSvds));
        ms->ms_vgd = ms->ms_vgs - ms->ms_vds;
    }
    else {
        vgdo = *(ckt->CKTstate0 + here->MOSvgs) - 
            *(ckt->CKTstate0 + here->MOSvds);
        mos_fetlim(&ms->ms_vgd,vgdo,ms->ms_von);
        ms->ms_vds = ms->ms_vgs - ms->ms_vgd;
        if (!ckt->CKTfixLimit) {
            ms->ms_vds = -ms->ms_vds;
            mos_limvds(&ms->ms_vds,-(*(ckt->CKTstate0 + here->MOSvds)));
            ms->ms_vds = -ms->ms_vds;
        }
        ms->ms_vgs = ms->ms_vgd + ms->ms_vds;
    }
    if (ms->ms_vds >= 0) {
        check = mos_pnjlim(&ms->ms_vbs,*(ckt->CKTstate0 + here->MOSvbs),
                ms->ms_vt,here->MOSsourceVcrit);
        ms->ms_vbd = ms->ms_vbs - ms->ms_vds;
    }
    else {
        check = mos_pnjlim(&ms->ms_vbd,*(ckt->CKTstate0 + here->MOSvbd),
                ms->ms_vt,here->MOSdrainVcrit);
        ms->ms_vbs = ms->ms_vbd + ms->ms_vds;
    }
    ms->ms_vgb = ms->ms_vgs - ms->ms_vbs;
    return (check);
}


static void
mos_limvds(vnew,vold)

/*
 * limit the per-iteration change of VDS
 */
double *vnew;
double vold;
{
    double vtmp;
    double vn;

    vn = *vnew;
    if (vold >= 3.5) {
        if (vn > vold) {
            vtmp = 3*vold + 2;
            if (vn > vtmp)
                *vnew = vtmp;
        }
        else {
            if (vn < 2)
                *vnew = 2;
        }
    }
    else {
        if (vn > vold) {
            if (vn > 4)
                *vnew = 4;
        }
        else {
            if (vn < -.5)
                *vnew = -.5;
        }
    }
}


static int
mos_pnjlim(vnew,vold,vt,vcrit)

/*
 * limit the per-iteration change of PN junction  voltages 
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
mos_fetlim(vnew,vold,vto)

/*
 * limit the per-iteration change of FET voltages 
 */
double *vnew;
double vold;
double vto;
{
    double vtsthi;
    double vtstlo;
    double vtox;
    double delv;
    double vtemp;
    double vn;

    vn = *vnew;
    vtsthi = FABS(2*(vold - vto)) + 2;
    vtstlo = .5*vtsthi + 2;
    vtox = vto + 3.5;
    delv = vn - vold;

    if (vold >= vto) {
        if (vold >= vtox) {
            if (delv <= 0) {
                /* going off */
                if (vn >= vtox) {
                    if (-delv > vtstlo) {
                        *vnew = vold - vtstlo;
                    }
                }
                else {
                    vtemp = vto + 2;
                    if (vn < vtemp)
                        *vnew = vtemp;
                }
            }
            else {
                /* staying on */
                if (delv >= vtsthi) {
                    *vnew = vold + vtsthi;
                }
            }
        }
        else {
            /* middle region */
            if (delv <= 0) {
                /* decreasing */
                vtemp = vto - .5;
                if (vn < vtemp)
                    *vnew = vtemp;
            }
            else {
                /* increasing */
                vtemp = vto + 4;
                if (vn > vtemp)
                    *vnew = vtemp;
            }
        }
    }
    else {
        /* off */
        if (delv <= 0) {
            if (-delv > vtsthi) {
                *vnew = vold - vtsthi;
            } 
        }
        else {
            vtemp = vto + .5;
            if (vn <= vtemp) {
                if (delv > vtstlo) {
                    *vnew = vold + vtstlo;
                }
            }
            else {
                *vnew = vtemp;
            }
        }
    }
}


#ifndef NOBYPASS

static int
mos_bypass(ckt,model,here,ms)

CKTcircuit* ckt;
MOSmodel *model;
MOSinstance *here;
struct mosstuff *ms;
{
    double delvbs;
    double delvbd;
    double delvgs;
    double delvds;
    double delvgd;
    double cdhat;
    double cbhat;
    double A1, A2, A3;
    int error;

    delvbs = ms->ms_vbs - *(ckt->CKTstate0 + here->MOSvbs);
    delvgs = ms->ms_vgs - *(ckt->CKTstate0 + here->MOSvgs);
    delvds = ms->ms_vds - *(ckt->CKTstate0 + here->MOSvds);
    delvbd = ms->ms_vbd - *(ckt->CKTstate0 + here->MOSvbd);
    delvgd = ms->ms_vgd - (*(ckt->CKTstate0 + here->MOSvgs) - 
                            *(ckt->CKTstate0 + here->MOSvds));

    cbhat = here->MOScbs + here->MOScbd +
        here->MOSgbd*delvbd + here->MOSgbs*delvbs;

    /* now lets see if we can bypass (ugh) */

    A1 = FABS(cbhat);
    A2 = here->MOScbs + here->MOScbd;
    A2 = FABS(A2);
    A3 = MAX(A1,A2) + ckt->CKTabstol;
    A1 = cbhat - (here->MOScbs + here->MOScbd);
    if (FABS(A1) >= ckt->CKTreltol*A3)
        return (0);

    A1 = FABS(ms->ms_vbs);
    A2 = FABS(*(ckt->CKTstate0 + here->MOSvbs));
    if (FABS(delvbs) >= (ckt->CKTreltol*MAX(A1,A2) + ckt->CKTvoltTol))
        return (0);

    A1 = FABS(ms->ms_vbd);
    A2 = FABS(*(ckt->CKTstate0 + here->MOSvbd));
    if (FABS(delvbd) >= (ckt->CKTreltol*MAX(A1,A2) + ckt->CKTvoltTol))
        return (0);

    A1 = FABS(ms->ms_vgs);
    A2 = FABS(*(ckt->CKTstate0 + here->MOSvgs));
    if (FABS(delvgs) >= (ckt->CKTreltol*MAX(A1,A2) + ckt->CKTvoltTol))
        return (0);

    A1 = FABS(ms->ms_vds);
    A2 = FABS(*(ckt->CKTstate0 + here->MOSvds));
    if (FABS(delvds) >= (ckt->CKTreltol*MAX(A1,A2) + ckt->CKTvoltTol))
        return (0);

    if (here->MOSmode >= 0) {
        cdhat = here->MOScd - 
            here->MOSgbd*delvbd + here->MOSgmbs*delvbs +
                here->MOSgm*delvgs + here->MOSgds*delvds;
    }
    else {
        cdhat = here->MOScd +
            (here->MOSgmbs - here->MOSgbd)*delvbd -
                here->MOSgm*delvgd + here->MOSgds*delvds;
    }

    A1 = cdhat - here->MOScd;
    A2 = FABS(cdhat);
    A3 = FABS(here->MOScd);
    if ((FABS(A1) >= ckt->CKTreltol*MAX(A2,A3) + ckt->CKTabstol))
        return (0);

    /* bypass code */
    /* nothing interesting has changed since last 
     * iteration on this device, so we just
     * copy all the values computed last iteration 
     * out and keep going
     */
    ms->ms_vbs = *(ckt->CKTstate0 + here->MOSvbs);
    ms->ms_vgs = *(ckt->CKTstate0 + here->MOSvgs);
    ms->ms_vds = *(ckt->CKTstate0 + here->MOSvds);
    ms->ms_vbd = *(ckt->CKTstate0 + here->MOSvbd);
    ms->ms_vgd = ms->ms_vgs - ms->ms_vds;
    ms->ms_vgb = ms->ms_vgs - ms->ms_vbs;

    ms->ms_cdrain = here->MOSmode * (here->MOScd + here->MOScbd);

    if (ckt->CKTmode & MODETRAN) {

        ms->ms_capgs = *(ckt->CKTstate0 + here->MOScapgs) +
                    *(ckt->CKTstate1 + here->MOScapgs) +
                        here->MOSgateSourceOverlapCap;

        ms->ms_capgd = *(ckt->CKTstate0 + here->MOScapgd) +
                    *(ckt->CKTstate1 + here->MOScapgd) +
                        here->MOSgateDrainOverlapCap;

        ms->ms_capgb = *(ckt->CKTstate0 + here->MOScapgb) +
                    *(ckt->CKTstate1 + here->MOScapgb) +
                        here->MOSgateBulkOverlapCap;

        /*
         *    calculate equivalent conductances and currents for
         *    Meyer's capacitors
         */
        NI_INTEG(ckt,ms->ms_gcgs,ms->ms_ceqgs,ms->ms_capgs,here->MOSqgs);

        NI_INTEG(ckt,ms->ms_gcgd,ms->ms_ceqgd,ms->ms_capgd,here->MOSqgd);

        NI_INTEG(ckt,ms->ms_gcgb,ms->ms_ceqgb,ms->ms_capgb,here->MOSqgb);

        ms->ms_ceqgs +=
                ckt->CKTag[0]* *(ckt->CKTstate0 + here->MOSqgs)
                - ms->ms_gcgs*ms->ms_vgs;
        ms->ms_ceqgd +=
                ckt->CKTag[0]* *(ckt->CKTstate0 + here->MOSqgd)
                - ms->ms_gcgd*ms->ms_vgd;
        ms->ms_ceqgb +=
                ckt->CKTag[0]* *(ckt->CKTstate0 + here->MOSqgb)
                - ms->ms_gcgb*ms->ms_vgb;

        mos_load(ckt,model,here,ms);
    }
    else
        mos_load_dc(ckt,model,here,ms);

    return (1);
}
#endif /*NOBYPASS*/


static void
mos_iv(ckt,model,here,ms)

CKTcircuit* ckt;
MOSmodel *model;
MOSinstance *here;
struct mosstuff *ms;
{
    double evb;

    /* initialize Meyer parameters */
    ms->ms_gcgs  = 0;
    ms->ms_ceqgs = 0;
    ms->ms_gcgd  = 0;
    ms->ms_ceqgd = 0;
    ms->ms_gcgb  = 0;
    ms->ms_ceqgb = 0;
    
    /* bulk-source and bulk-drain doides
     * here we just evaluate the ideal diode current and the
     * correspoinding derivative (conductance).
     */

    if (ms->ms_vbs <= 0) {
        here->MOSgbs = here->MOStSourceSatCur/ms->ms_vt;
        here->MOScbs = here->MOSgbs*ms->ms_vbs;
        here->MOSgbs += ckt->CKTgmin;
    }
    else {
        evb = exp(ms->ms_vbs/ms->ms_vt);
        here->MOSgbs =
            here->MOStSourceSatCur*evb/ms->ms_vt + ckt->CKTgmin;
        here->MOScbs = here->MOStSourceSatCur * (evb-1);
    }
    if (ms->ms_vbd <= 0) {
        here->MOSgbd = here->MOStDrainSatCur/ms->ms_vt;
        here->MOScbd = here->MOSgbd *ms->ms_vbd;
        here->MOSgbd += ckt->CKTgmin;
    }
    else {
        evb = exp(ms->ms_vbd/ms->ms_vt);
        here->MOSgbd =
            here->MOStDrainSatCur*evb/ms->ms_vt + ckt->CKTgmin;
        here->MOScbd = here->MOStDrainSatCur *(evb-1);
    }


    /* level 8, CryoMOS by Luong Huynh 1/19/95
     * cryo-MOSFETs have zero diode currents and diode conductances
     * at normal operating voltage
     */
    if (model->MOSlevel == 8) {
        here->MOScbs = 0;
	here->MOSgbs = 0;
	here->MOScbd = 0;
	here->MOSgbd = 0;
    }
    /*****/


    /* now to determine whether the user was able to correctly
     * identify the source and drain of his device
     */
    if (ms->ms_vds >= 0) {
        /* normal mode */
        here->MOSmode = 1;
    }
    else {
        /* inverse mode */
        here->MOSmode = -1;
    }

    if (model->MOSlevel == 1)
        ms->ms_cdrain = MOSeq1(model,here,ms);
    else if (model->MOSlevel == 2)
        ms->ms_cdrain = MOSeq2(model,here,ms);
    else if (model->MOSlevel == 3)
        ms->ms_cdrain = MOSeq3(model,here,ms);

    /* level 8, CryoMOS by Luong Huynh 1/19/95 */
    else if (model->MOSlevel == 8)
        ms->ms_cdrain = cryoMOSeq(model,here,ms);
    /**/

    else
        ms->ms_cdrain = MOSeq6(model,here,ms);

    /* now deal with n vs p polarity */

    if (model->MOStype > 0) {
        here->MOSvon = ms->ms_von;
        here->MOSvdsat = ms->ms_vdsat;
    }
    else {
        here->MOSvon = -ms->ms_von;
        here->MOSvdsat = -ms->ms_vdsat;
    }
    if (here->MOSmode > 0)
        here->MOScd = ms->ms_cdrain - here->MOScbd;
    else
        here->MOScd = -ms->ms_cdrain - here->MOScbd;
}


static void
mos_cap(ckt,model,here,ms)

CKTcircuit* ckt;
MOSmodel *model;
MOSinstance *here;
struct mosstuff *ms;
{
    double arg;
    double sarg;
    double sargsw;
    double vgx;
    double vgy;
    double vxy;
    double vgxt;
    double vddif1;
    double vddif2;
    double *px;
    double *py;

    
    /* 
     * now we do the hard part of the bulk-drain and bulk-source
     * diode - we evaluate the non-linear capacitance and
     * charge
     *
     * the basic equations are not hard, but the implementation
     * is somewhat long in an attempt to avoid log/exponential
     * evaluations
     *
     *  charge storage elements
     *
     * bulk-drain and bulk-source depletion capacitances
     */

    if (here->MOSCbs != 0 || here->MOSCbssw != 0) {
        if (ms->ms_vbs < here->MOStDepCap) {
            arg = 1 - ms->ms_vbs/here->MOStBulkPot;

            SARGS(arg,model->MOSbulkJctBotGradingCoeff,
                model->MOSbulkJctSideGradingCoeff,sarg,sargsw);

            *(ckt->CKTstate0 + here->MOSqbs) = here->MOStBulkPot *
                ( here->MOSCbs *
                (1 - arg*sarg)/(1 - model->MOSbulkJctBotGradingCoeff)
                + here->MOSCbssw *
                (1 - arg*sargsw)/(1 - model->MOSbulkJctSideGradingCoeff) );

            here->MOScapbs = here->MOSCbs*sarg + here->MOSCbssw*sargsw;
        }
        else {
            *(ckt->CKTstate0 + here->MOSqbs) = here->MOSf4s +
                ms->ms_vbs*(here->MOSf2s + .5*ms->ms_vbs*here->MOSf3s);
            here->MOScapbs = here->MOSf2s + here->MOSf3s*ms->ms_vbs;
        }
    }
    else {
        *(ckt->CKTstate0 + here->MOSqbs) = 0;
        here->MOScapbs = 0;
    }

    if (here->MOSCbd != 0 || here->MOSCbdsw != 0) {
        if (ms->ms_vbd < here->MOStDepCap) {
            arg = 1 - ms->ms_vbd/here->MOStBulkPot;

            SARGS(arg,model->MOSbulkJctBotGradingCoeff,
                model->MOSbulkJctSideGradingCoeff,sarg,sargsw);

            *(ckt->CKTstate0 + here->MOSqbd) = here->MOStBulkPot *
                ( here->MOSCbd *
                (1 - arg*sarg)/(1 - model->MOSbulkJctBotGradingCoeff)
                + here->MOSCbdsw *
                (1 - arg*sargsw)/(1 - model->MOSbulkJctSideGradingCoeff) );

            here->MOScapbd  = here->MOSCbd*sarg + here->MOSCbdsw*sargsw;
        }
        else {
            *(ckt->CKTstate0 + here->MOSqbd) = here->MOSf4d +
                ms->ms_vbd*(here->MOSf2d + .5*ms->ms_vbd*here->MOSf3d);
            here->MOScapbd = here->MOSf2d + ms->ms_vbd*here->MOSf3d;
        }
    }
    else {
        *(ckt->CKTstate0 + here->MOSqbd) = 0;
        here->MOScapbd = 0;
    }
    
    /*
     *     calculate meyer's capacitors
     *
     * new cmeyer - this just evaluates at the current time,
     * expects you to remember values from previous time
     * returns 1/2 of non-constant portion of capacitance
     * you must add in the other half from previous time
     * and the constant part
     */

    if (here->MOSmode > 0) {
        vgx = ms->ms_vgs;
        vgy = ms->ms_vgd;
        px  = (ckt->CKTstate0 + here->MOScapgs);
        py  = (ckt->CKTstate0 + here->MOScapgd);
    }
    else {
        vgx = ms->ms_vgd;
        vgy = ms->ms_vgs;
        px  = (ckt->CKTstate0 + here->MOScapgd);
        py  = (ckt->CKTstate0 + here->MOScapgs);
    }
/*
    DEVqmeyer (vgx,vgy,ms->ms_von,ms->ms_vdsat,px,py,
            (ckt->CKTstate0 + here->MOScapgb),
            here->MOStPhi,here->MOSoxideCap);
*/

    vgxt = vgx - ms->ms_von;
    if (vgxt <= -here->MOStPhi) {
        *(ckt->CKTstate0 + here->MOScapgb) = .5*here->MOSoxideCap;
        *px = 0;
        *py = 0;
    }
    else if (vgxt <= -.5*here->MOStPhi) {
        *(ckt->CKTstate0 + here->MOScapgb) =
                -.5*vgxt*here->MOSoxideCap/here->MOStPhi;
        *px = 0;
        *py = 0;
    }
    else if (vgxt <= 0) {
        *(ckt->CKTstate0 + here->MOScapgb) =
                -.5*vgxt*here->MOSoxideCap/here->MOStPhi;
        *px = P66*vgxt*here->MOSoxideCap/here->MOStPhi +
                P33*here->MOSoxideCap;
        *py = 0;
    }
    else  {
        vxy = vgx - vgy;
        if (ms->ms_vdsat <= vxy) {
            *px = P33*here->MOSoxideCap;
            *py = 0;
        }
        else {
            vddif1 = ms->ms_vdsat - vxy;
            vddif2 = vddif1 + ms->ms_vdsat;
            vddif2 = 1.0/(vddif2*vddif2);
            *py = here->MOSoxideCap*
                    P33*(1.0 - ms->ms_vdsat*ms->ms_vdsat*vddif2);
            *px = here->MOSoxideCap*
                    P33*(1.0 - vddif1*vddif1*vddif2);
        }
        *(ckt->CKTstate0 + here->MOScapgb) = 0;
    }

    /* level 8, CryoMOS by Luong Huynh 1/19/95
     * bulk caps are zero due to freeze out
     */
    if (model->MOSlevel == 8) {
        *(ckt->CKTstate0 + here->MOScapgb) = 0;
	here->MOScapbs = 0;
	here->MOScapbd = 0;
    }
    /*****/

    if (ckt->CKTmode & (MODEINITTRAN|MODEINITSMSIG)) {

        *(ckt->CKTstate1 + here->MOScapgs) =
            *(ckt->CKTstate0 + here->MOScapgs);
        *(ckt->CKTstate1 + here->MOScapgd) =
            *(ckt->CKTstate0 + here->MOScapgd);
        *(ckt->CKTstate1 + here->MOScapgb) =
            *(ckt->CKTstate0 + here->MOScapgb);

        ms->ms_capgs = *(ckt->CKTstate0 + here->MOScapgs) +
                *(ckt->CKTstate0 + here->MOScapgs) +
                here->MOSgateSourceOverlapCap;
        ms->ms_capgd = *(ckt->CKTstate0 + here->MOScapgd) +
                *(ckt->CKTstate0 + here->MOScapgd) +
                here->MOSgateDrainOverlapCap;
        ms->ms_capgb = *(ckt->CKTstate0 + here->MOScapgb) +
                *(ckt->CKTstate0 + here->MOScapgb) +
                here->MOSgateBulkOverlapCap;

        *(ckt->CKTstate0 + here->MOSqgs) = ms->ms_capgs*ms->ms_vgs;
        *(ckt->CKTstate0 + here->MOSqgd) = ms->ms_capgd*ms->ms_vgd;
        *(ckt->CKTstate0 + here->MOSqgb) = ms->ms_capgb*ms->ms_vgb;

        *(ckt->CKTstate1 + here->MOSqbd) =
            *(ckt->CKTstate0 + here->MOSqbd);
        *(ckt->CKTstate1 + here->MOSqbs) =
            *(ckt->CKTstate0 + here->MOSqbs);
        *(ckt->CKTstate1 + here->MOSqgs) =
            *(ckt->CKTstate0 + here->MOSqgs);
        *(ckt->CKTstate1 + here->MOSqgd) =
            *(ckt->CKTstate0 + here->MOSqgd);
        *(ckt->CKTstate1 + here->MOSqgb) =
            *(ckt->CKTstate0 + here->MOSqgb);
    }
    else {

        ms->ms_capgs = *(ckt->CKTstate0 + here->MOScapgs) +
                *(ckt->CKTstate1 + here->MOScapgs) +
                here->MOSgateSourceOverlapCap;
        ms->ms_capgd = *(ckt->CKTstate0 + here->MOScapgd) +
                *(ckt->CKTstate1 + here->MOScapgd) +
                here->MOSgateDrainOverlapCap;
        ms->ms_capgb = *(ckt->CKTstate0 + here->MOScapgb) +
                *(ckt->CKTstate1 + here->MOScapgb) +
                here->MOSgateBulkOverlapCap;

        vgx = *(ckt->CKTstate1 + here->MOSvgs);
        vgy = vgx - *(ckt->CKTstate1 + here->MOSvds);
        vxy = vgx - *(ckt->CKTstate1 + here->MOSvbs);

        *(ckt->CKTstate0 + here->MOSqgs) =
                (ms->ms_vgs - vgx)*ms->ms_capgs +
                *(ckt->CKTstate1 + here->MOSqgs);
        *(ckt->CKTstate0 + here->MOSqgd) =
                (ms->ms_vgd - vgy)*ms->ms_capgd +
                *(ckt->CKTstate1 + here->MOSqgd);
        *(ckt->CKTstate0 + here->MOSqgb) =
                (ms->ms_vgb - vxy)*ms->ms_capgb +
                *(ckt->CKTstate1 + here->MOSqgb);
    }
}


static int
mos_integ(ckt,here,ms)

CKTcircuit *ckt;
MOSinstance *here;
struct mosstuff *ms;
{
    double geq;
    double ceq;
    int error;

    /*
     *    calculate equivalent conductances and currents for
     *    depletion capacitors
     */
    NI_INTEG(ckt,geq,ceq,here->MOScapbd,here->MOSqbd);

    here->MOSgbd += geq;
    here->MOScbd += *(ckt->CKTstate0 + here->MOScqbd);
    here->MOScd  -= *(ckt->CKTstate0 + here->MOScqbd);

    NI_INTEG(ckt,geq,ceq,here->MOScapbs,here->MOSqbs);

    here->MOSgbs += geq;
    here->MOScbs += *(ckt->CKTstate0 + here->MOScqbs);
    
    if (ms->ms_capgs == 0)
            *(ckt->CKTstate0 + here->MOScqgs) = 0;
    if (ms->ms_capgd == 0)
            *(ckt->CKTstate0 + here->MOScqgd) = 0;
    if (ms->ms_capgb == 0)
            *(ckt->CKTstate0 + here->MOScqgb) = 0;

    /*
     *    calculate equivalent conductances and currents for
     *    Meyer's capacitors
     */
    NI_INTEG(ckt,ms->ms_gcgs,ms->ms_ceqgs,ms->ms_capgs,here->MOSqgs);

    NI_INTEG(ckt,ms->ms_gcgd,ms->ms_ceqgd,ms->ms_capgd,here->MOSqgd);

    NI_INTEG(ckt,ms->ms_gcgb,ms->ms_ceqgb,ms->ms_capgb,here->MOSqgb);

    ms->ms_ceqgs +=
            ckt->CKTag[0]* *(ckt->CKTstate0 + here->MOSqgs)
            - ms->ms_gcgs*ms->ms_vgs;
    ms->ms_ceqgd +=
            ckt->CKTag[0]* *(ckt->CKTstate0 + here->MOSqgd)
            - ms->ms_gcgd*ms->ms_vgd;
    ms->ms_ceqgb +=
            ckt->CKTag[0]* *(ckt->CKTstate0 + here->MOSqgb)
            - ms->ms_gcgb*ms->ms_vgb;
    
    return (OK);
}


static void
mos_load(ckt,model,here,ms)

CKTcircuit* ckt;
MOSmodel *model;
MOSinstance *here;
struct mosstuff *ms;
{
    double ceqbs;
    double ceqbd;
    double cdreq;
    double dcon;
    double scon;

     /* save things away for next time */
     *(ckt->CKTstate0 + here->MOSvbs) = ms->ms_vbs;
     *(ckt->CKTstate0 + here->MOSvbd) = ms->ms_vbd;
     *(ckt->CKTstate0 + here->MOSvgs) = ms->ms_vgs;
     *(ckt->CKTstate0 + here->MOSvds) = ms->ms_vds;

    /*
     *  load current vector
     */

    ceqbs = here->MOScbs - (here->MOSgbs - ckt->CKTgmin)*ms->ms_vbs;
    ceqbd = here->MOScbd - (here->MOSgbd - ckt->CKTgmin)*ms->ms_vbd;

    if (here->MOSmode > 0) {
        cdreq =   ms->ms_cdrain - here->MOSgds*ms->ms_vds -
                    here->MOSgm*ms->ms_vgs - here->MOSgmbs*ms->ms_vbs;
    }
    else {
        cdreq = -(ms->ms_cdrain + here->MOSgds*ms->ms_vds -
                    here->MOSgm*ms->ms_vgd - here->MOSgmbs*ms->ms_vbd);
    }

    if (model->MOStype > 0) {

        *(ckt->CKTrhs + here->MOSgNode) -= 
                ms->ms_ceqgs + ms->ms_ceqgb + ms->ms_ceqgd;
        *(ckt->CKTrhs + here->MOSbNode)      -= ceqbs + ceqbd - ms->ms_ceqgb;
        *(ckt->CKTrhs + here->MOSdNodePrime) += ceqbd - cdreq + ms->ms_ceqgd;
        *(ckt->CKTrhs + here->MOSsNodePrime) += cdreq + ceqbs + ms->ms_ceqgs;
    }
    else {

        *(ckt->CKTrhs + here->MOSgNode) += 
                ms->ms_ceqgs + ms->ms_ceqgb + ms->ms_ceqgd;
        *(ckt->CKTrhs + here->MOSbNode)      += ceqbs + ceqbd - ms->ms_ceqgb;
        *(ckt->CKTrhs + here->MOSdNodePrime) -= ceqbd - cdreq + ms->ms_ceqgd;
        *(ckt->CKTrhs + here->MOSsNodePrime) -= cdreq + ceqbs + ms->ms_ceqgs;
    }

    dcon = here->MOSdrainConductance;
    scon = here->MOSsourceConductance;


    /* level 8, CryoMOS by Luong Huynh 1/19/95
     * rd and rs parameters in cryo-MOS model are drain
     * and source resistances multiplied by the channel width
     * rsh should not be used
     */
    if (model->MOSlevel == 8) {
        dcon *= here->MOSw;
	scon *= here->MOSw;
    }
    /*****/


    /*
     *  load y matrix
     */
    *(here->MOSDdPtr)  += dcon;
    *(here->MOSGgPtr)  += ms->ms_gcgd + ms->ms_gcgs + ms->ms_gcgb;
    *(here->MOSSsPtr)  += scon;
    *(here->MOSBbPtr)  += here->MOSgbd + here->MOSgbs + ms->ms_gcgb;
    *(here->MOSDdpPtr) -= dcon;
    *(here->MOSGbPtr)  -= ms->ms_gcgb;
    *(here->MOSGdpPtr) -= ms->ms_gcgd;
    *(here->MOSGspPtr) -= ms->ms_gcgs;
    *(here->MOSSspPtr) -= scon;
    *(here->MOSBgPtr)  -= ms->ms_gcgb;
    *(here->MOSBdpPtr) -= here->MOSgbd;
    *(here->MOSBspPtr) -= here->MOSgbs;
    *(here->MOSDPdPtr) -= dcon;
    *(here->MOSSPsPtr) -= scon;

    if (here->MOSmode > 0) {
        *(here->MOSDPdpPtr)+= dcon + here->MOSgds +
                here->MOSgbd + ms->ms_gcgd;

        *(here->MOSSPspPtr)+= scon + here->MOSgds +
                here->MOSgbs + here->MOSgm + here->MOSgmbs + ms->ms_gcgs;

        *(here->MOSDPgPtr) += here->MOSgm  - ms->ms_gcgd;
        *(here->MOSDPbPtr) += -here->MOSgbd+ here->MOSgmbs;
        *(here->MOSDPspPtr)-= here->MOSgds + here->MOSgm + here->MOSgmbs;
        *(here->MOSSPgPtr) -= here->MOSgm  + ms->ms_gcgs;
        *(here->MOSSPbPtr) -= here->MOSgbs + here->MOSgmbs;
        *(here->MOSSPdpPtr)-= here->MOSgds;
    }
    else {
        *(here->MOSDPdpPtr)+= dcon + here->MOSgds +
                here->MOSgbd + here->MOSgm + here->MOSgmbs + ms->ms_gcgd;

        *(here->MOSSPspPtr)+= scon + here->MOSgds +
                here->MOSgbs + ms->ms_gcgs;

        *(here->MOSDPgPtr) -= here->MOSgm  + ms->ms_gcgd;
        *(here->MOSDPbPtr) -= here->MOSgbd + here->MOSgmbs;
        *(here->MOSDPspPtr)-= here->MOSgds;
        *(here->MOSSPgPtr) -= -here->MOSgm + ms->ms_gcgs;
        *(here->MOSSPbPtr) -= here->MOSgbs - here->MOSgmbs;
        *(here->MOSSPdpPtr)-= here->MOSgds + here->MOSgm + here->MOSgmbs;
    }
}


static void
mos_load_dc(ckt,model,here,ms)

CKTcircuit* ckt;
MOSmodel *model;
MOSinstance *here;
struct mosstuff *ms;
{
    /*
     * Same as above, but avoids processing 0's from Meyer parameters
     */

    double ceqbs;
    double ceqbd;
    double cdreq;
    double dcon;
    double scon;

     /* save things away for next time */
     *(ckt->CKTstate0 + here->MOSvbs) = ms->ms_vbs;
     *(ckt->CKTstate0 + here->MOSvbd) = ms->ms_vbd;
     *(ckt->CKTstate0 + here->MOSvgs) = ms->ms_vgs;
     *(ckt->CKTstate0 + here->MOSvds) = ms->ms_vds;

    /*
     *  load current vector
     */

    ceqbs = here->MOScbs - (here->MOSgbs - ckt->CKTgmin)*ms->ms_vbs;
    ceqbd = here->MOScbd - (here->MOSgbd - ckt->CKTgmin)*ms->ms_vbd;

    if (here->MOSmode > 0) {
        cdreq =   ms->ms_cdrain - here->MOSgds*ms->ms_vds -
                    here->MOSgm*ms->ms_vgs - here->MOSgmbs*ms->ms_vbs;
    }
    else {
        cdreq = -(ms->ms_cdrain + here->MOSgds*ms->ms_vds -
                    here->MOSgm*ms->ms_vgd - here->MOSgmbs*ms->ms_vbd);
    }

    if (model->MOStype > 0) {
        *(ckt->CKTrhs + here->MOSbNode)      -= ceqbs + ceqbd;
        *(ckt->CKTrhs + here->MOSdNodePrime) += ceqbd - cdreq;
        *(ckt->CKTrhs + here->MOSsNodePrime) += cdreq + ceqbs;
    }
    else {
        *(ckt->CKTrhs + here->MOSbNode)      += ceqbs + ceqbd;
        *(ckt->CKTrhs + here->MOSdNodePrime) -= ceqbd - cdreq;
        *(ckt->CKTrhs + here->MOSsNodePrime) -= cdreq + ceqbs;
    }

    dcon = here->MOSdrainConductance;
    scon = here->MOSsourceConductance;



    /* level 8, CryoMOS by Luong Huynh 1/19/95
     * rd and rs parameters in COLD MOSFET model are drain
     * and source resistances multiplied by the channel width
     * rsh should not be used
     */
    if (model->MOSlevel == 8) {
        dcon *= here->MOSw;
	scon *= here->MOSw;
    }
    /**/



    /*
     *  load y matrix
     */
    *(here->MOSDdPtr)  += dcon;
    *(here->MOSSsPtr)  += scon;
    *(here->MOSBbPtr)  += here->MOSgbd + here->MOSgbs;
    *(here->MOSDdpPtr) -= dcon;
    *(here->MOSSspPtr) -= scon;
    *(here->MOSBdpPtr) -= here->MOSgbd;
    *(here->MOSBspPtr) -= here->MOSgbs;
    *(here->MOSDPdPtr) -= dcon;
    *(here->MOSSPsPtr) -= scon;

    if (here->MOSmode > 0) {
        *(here->MOSDPdpPtr)+= dcon + here->MOSgds + here->MOSgbd;

        *(here->MOSSPspPtr)+= scon + here->MOSgds +
                here->MOSgbs + here->MOSgm + here->MOSgmbs;

        *(here->MOSDPgPtr) += here->MOSgm;
        *(here->MOSDPbPtr) += -here->MOSgbd+ here->MOSgmbs;
        *(here->MOSDPspPtr)-= here->MOSgds + here->MOSgm + here->MOSgmbs;
        *(here->MOSSPgPtr) -= here->MOSgm;
        *(here->MOSSPbPtr) -= here->MOSgbs + here->MOSgmbs;
        *(here->MOSSPdpPtr)-= here->MOSgds;
    }
    else {
        *(here->MOSDPdpPtr)+= dcon + here->MOSgds +
                here->MOSgbd + here->MOSgm + here->MOSgmbs;

        *(here->MOSSPspPtr)+= scon + here->MOSgds + here->MOSgbs;

        *(here->MOSDPgPtr) -= here->MOSgm;
        *(here->MOSDPbPtr) -= here->MOSgbd + here->MOSgmbs;
        *(here->MOSDPspPtr)-= here->MOSgds;
        *(here->MOSSPgPtr) -= -here->MOSgm;
        *(here->MOSSPbPtr) -= here->MOSgbs - here->MOSgmbs;
        *(here->MOSSPdpPtr)-= here->MOSgds + here->MOSgm + here->MOSgmbs;
    }
}
