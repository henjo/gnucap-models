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

/* assuming silicon - make definition for epsilon of silicon */
#define EPSSIL (11.7 * 8.854214871e-12)

/* some constants to avoid slow divides */
#define P66 .66666666666667
#define P33 .33333333333333
#define P1X3 1.3333333333333

/* (cm**2/m**2) */
#define CM2PM2 1e4

/* (m**2/cm**2) */
#define M2PCM2 1e-4


#ifdef __STDC__
static void mos2_vsat(double*,double*);
#else
static void mos2_vsat();
#endif


double
MOSeq1(model,here,ms)

MOSmodel *model;
MOSinstance *here;
struct mosstuff *ms;
{

    /*
     *     this block of code evaluates the drain current and its 
     *     derivatives using the shichman-hodges model and the 
     *     charges associated with the gate, channel and bulk for 
     *     mosfets
     *
     */
    double arg;
    double betap;
    double sarg;
    double vgst;
    double cdrain;
    double vbx;
    double vgx;
    double vds;

    vbx = ((here->MOSmode == 1) ? ms->ms_vbs : ms->ms_vbd);
    vgx = ((here->MOSmode == 1) ? ms->ms_vgs : ms->ms_vgd);

    if (vbx <= 0) {
        sarg = sqrt(here->MOStPhi - vbx);
    }
    else {
        sarg = sqrt(here->MOStPhi);
        sarg = sarg - vbx/(sarg+sarg);
        sarg = MAX(0,sarg);
    }
    if (model->MOStype > 0)
        ms->ms_von = here->MOStVbi + model->MOSgamma*sarg;
    else
        ms->ms_von = -here->MOStVbi + model->MOSgamma*sarg;

    vgst = vgx - ms->ms_von;
    ms->ms_vdsat = MAX(vgst,0);

    if (vgst <= 0) {
        /* cutoff region */
        cdrain = 0;
        here->MOSgm = 0;
        here->MOSgds = 0;
        here->MOSgmbs = 0;
    }
    else {
        if (here->MOSmode > 0)
            vds = ms->ms_vds;
        else
            vds = -ms->ms_vds;

        if (sarg <= 0)
            arg = 0;
        else
            arg = model->MOSgamma/(sarg+sarg);
        betap = here->MOSbeta * (1 + model->MOSlambda*vds);

        if (vgst <= vds) {
            /* saturation region */

            here->MOSgm   = betap*vgst;
            vgst *= vgst*.5;
            cdrain        = betap*vgst;
            here->MOSgds  = model->MOSlambda*here->MOSbeta*vgst;
            here->MOSgmbs = here->MOSgm*arg;
        }
        else {
            /* linear region */

            here->MOSgm   = betap*vds;
            cdrain        = here->MOSgm*(vgst - .5*vds);
            here->MOSgds  = betap*(vgst - vds) +
                        model->MOSlambda*here->MOSbeta*vds*(vgst - .5*vds);
            here->MOSgmbs = here->MOSgm*arg;
        }
    }
    return (cdrain);
}


double
MOSeq2(model,here,ms)

MOSmodel *model;
MOSinstance *here;
struct mosstuff *ms;
{

    /*
     *     this routine evaluates the drain current, its derivatives and
     *     the charges associated with the gate, channel and bulk
     *     for mosfets (level 2 model)
     *
     */

    double cdrain;
    double arg;
    double sarg;
    double beta1;
    double dsrgdb;
    double d2sdb2;
    double sphi;    /* square root of phi */
    double sphi3;   /* square root of phi cubed */
    double barg;
    double d2bdb2;
    double factor;
    double dbrgdb;
    double eta;
    double vbin;
    double dgddb2;
    double dgddvb;
    double dgdvds;
    double gamasd;
    double gammad;
    double vth;
    double xn;
    double argg;
    double vgst;
    double sarg3;
    double sbiarg;
    double dgdvbs;
    double body;
    double gdbdv;
    double dodvbs;
    double dodvds;
    double dxndvd;
    double dxndvb;
    double dudvgs;
    double dudvds;
    double dudvbs;
    double vgsx;
    double ufact;
    double ueff;
    double dsdvgs;
    double dsdvbs;
    double bsarg;
    double dbsrdb;
    double bodys;
    double gdbdvs;
    double dldvgs;
    double dldvds;
    double dldvbs;
    double dfact;
    double clfact;
    double xwb;
    double vdson;
    double cdson;
    double didvds;
    double gdson;
    double gmw;
    double gbson;
    double xlamda;
    double lvbs;
    double lvds;
    double lvgs;
    double phiMinVbs;
    double oneoverl;
    double oneovere;
    double tmp, tmp1;  /* temporary variable, not used for more than */
                       /* about 10 lines at a time */
    int i;
    int j;

    /* 'local' variables - these switch d & s around appropriately
     * so that we don't have to worry about vds < 0
     */
    if (here->MOSmode > 0) {
        lvbs = ms->ms_vbs;
        lvds = ms->ms_vds;
        lvgs = ms->ms_vgs;
    }
    else {
        lvbs = ms->ms_vbd;
        lvds = -ms->ms_vds;
        lvgs = ms->ms_vgd;
    }
    xlamda = model->MOSlambda;
    phiMinVbs = here->MOStPhi - lvbs;
    oneoverl = 1.0/here->MOSeffectiveLength;

    /*
     *  compute some useful quantities
     */

    if (lvbs <= 0.0) {
        sarg   = sqrt(phiMinVbs);
        dsrgdb = -0.5/sarg;
        d2sdb2 = 0.5*dsrgdb/phiMinVbs;
    }
    else {
        sphi   = sqrt(here->MOStPhi);
        sphi3  = here->MOStPhi*sphi;
        sarg   = sphi/(1.0 + 0.5*lvbs/here->MOStPhi);
        tmp    = sarg/sphi3;
        dsrgdb = -0.5*sarg*tmp;
        d2sdb2 = -dsrgdb*tmp;
    }
    if ((lvds - lvbs) >= 0) {
        barg   = sqrt(phiMinVbs + lvds);
        dbrgdb = -0.5/barg;
        d2bdb2 = 0.5*dbrgdb/(phiMinVbs + lvds);
    }
    else {
        barg   = sphi/(1.0 + 0.5*(lvbs - lvds)/here->MOStPhi);
        tmp    = barg/sphi3;
        dbrgdb = -0.5*barg*tmp;
        d2bdb2 = -dbrgdb*tmp;
    }

    /*
     *  calculate threshold voltage (von)
     *     narrow-channel effect
     */
    factor   = 0.125*model->MOSnarrowFactor*2.0*M_PI*EPSSIL/
                    here->MOSoxideCap*here->MOSeffectiveLength;
    eta      = 1.0 + factor;
    oneovere = 1.0/eta;
    vbin     = here->MOStVbi*model->MOStype + factor*phiMinVbs;

    gamasd   = model->MOSgamma;
    gammad   = model->MOSgamma;
    dgddvb   = 0.0;
    dgdvds   = 0.0;
    dgddb2   = 0.0;

    if ((model->MOSgamma > 0.0) || 
            (model->MOSsubstrateDoping > 0.0)) {

        /*
         *     short-channel effect with vds != 0.0
         */
        dgddvb = -model->MOSgamma;

        if (model->MOSjunctionDepth > 0) {
            double tbxwd, tbxws, targxs, targxd, targs, targd;

            tbxwd  = model->MOSxd*dbrgdb;
            tbxws  = model->MOSxd*dsrgdb;

            tmp    = 2.0/model->MOSjunctionDepth;
            targxs = 1.0 + tmp*model->MOSxd*sarg;
            targxd = 1.0 + tmp*model->MOSxd*barg;
            targs  = sqrt(targxs);
            targd  = sqrt(targxd);
            tmp    = .5*model->MOSjunctionDepth*oneoverl;
            gamasd *= (1.0 - tmp*(targs + targd - 2.0));

            targs  = oneoverl/targs;
            targd  = oneoverl/targd;
            dgddvb *= .5*(tbxws*targs + tbxwd*targd);
            dgdvds = model->MOSgamma*0.5*tbxwd*targd;

            tmp    = -model->MOSxd*(d2sdb2 + dsrgdb*dsrgdb*
                        model->MOSxd/(model->MOSjunctionDepth*targxs))*targs;
            tmp1   = -model->MOSxd*(d2bdb2 + dbrgdb*dbrgdb*
                        model->MOSxd/(model->MOSjunctionDepth*targxd))*targd;
            dgddb2 = -0.5*model->MOSgamma*(tmp + tmp1);
        }
    }

    ms->ms_von   = vbin + gamasd*sarg;
    vth          = ms->ms_von;
    ms->ms_vdsat = 0.0;

    if (model->MOSfastSurfaceStateDensity != 0.0 &&
            here->MOSoxideCap != 0.0) {
        tmp  = CHARGE*model->MOSfastSurfaceStateDensity*CM2PM2;
        tmp1 = -(gamasd*dsrgdb + dgddvb*sarg) + factor;
        xn   = 1.0 +
            tmp/here->MOSoxideCap*here->MOSw*here->MOSeffectiveLength + tmp1;
        tmp  = ms->ms_vt*xn;
        ms->ms_von += tmp;
        argg = 1.0/tmp;
        vgst = lvgs - ms->ms_von;
    }
    else {
        vgst = lvgs - ms->ms_von;
        if (lvgs <= ms->ms_von) {
            /*
             *  cutoff region
             */
            here->MOSgds = 0.0;
            goto bad;
        }
    }

    /*
     *  compute some more useful quantities
     */
    sarg3  = sarg*sarg*sarg;
    sbiarg = sqrt(here->MOStBulkPot);
    gammad = gamasd;
    dgdvbs = dgddvb;
    body   = barg*barg*barg - sarg3;
    gdbdv  = 2.0*gammad*(barg*barg*dbrgdb - sarg*sarg*dsrgdb);
    dodvbs = -factor + dgdvbs*sarg + gammad*dsrgdb;

    if (model->MOSfastSurfaceStateDensity != 0.0 &&
        here->MOSoxideCap != 0.0) {

        dxndvb = 2.0*dgdvbs*dsrgdb + gammad*d2sdb2 + dgddb2*sarg;
        dodvbs = dodvbs + ms->ms_vt*dxndvb;
        dxndvd = dgdvds*dsrgdb;
        dodvds = dgdvds*sarg + ms->ms_vt*dxndvd;
    }

    /*
     *  evaluate effective mobility and its derivatives
     */
    if (here->MOSoxideCap <= 0.0) {
        ufact  = 1.0;
        ueff   = model->MOSsurfaceMobility * M2PCM2;
        dudvgs = 0.0;
        dudvds = 0.0;
        dudvbs = 0.0;
    }
    else {
        tmp = model->MOScritField * 100 /* cm/m */ * EPSSIL/
            model->MOSoxideCapFactor;
        if (vgst <= tmp) {
            ufact  = 1.0;
            ueff   = model->MOSsurfaceMobility * M2PCM2;
            dudvgs = 0.0;
            dudvds = 0.0;
            dudvbs = 0.0;
        }
        else {
            ufact  = exp(model->MOScritFieldExp*log(tmp/vgst));
            ueff   = model->MOSsurfaceMobility * M2PCM2 *ufact;
            dudvgs = -ufact*model->MOScritFieldExp/vgst;
            dudvds = 0.0;
            dudvbs = model->MOScritFieldExp*ufact*dodvbs/vgst;
        }
    }

    /*
     *     evaluate saturation voltage and its derivatives according to
     *     grove-frohman equation
     */
    vgsx   = lvgs;
    gammad = gamasd*oneovere;
    dgdvbs = dgddvb;
    if (model->MOSfastSurfaceStateDensity != 0 && here->MOSoxideCap != 0) {
        vgsx = MAX(lvgs,ms->ms_von);
    }
    if (gammad > 0) {
        tmp = gammad*gammad;
        tmp1 = (vgsx - vbin)*oneovere + phiMinVbs;
        if (tmp1 <= 0.0) {
            ms->ms_vdsat = 0.0;
            dsdvgs = 0.0;
            dsdvbs = 0.0;
        }
        else {
            arg = sqrt(1.0 + 4.0*tmp1/tmp);
            ms->ms_vdsat = (vgsx - vbin)*oneovere + .5*tmp*(1.0 - arg);
            ms->ms_vdsat = MAX(ms->ms_vdsat,0.0);  
            dsdvgs = (1.0 - 1.0/arg)*oneovere;
            dsdvbs = (gammad*(1.0 - arg) + 2.0*tmp1/(gammad*arg))*oneovere
                * dgdvbs + 1.0/arg + factor*dsdvgs;
        }
    }
    else {
        ms->ms_vdsat = (vgsx - vbin)*oneovere;
        ms->ms_vdsat = MAX(ms->ms_vdsat,0.0);
        dsdvgs = 1.0;
        dsdvbs = 0.0;
    }
    if (model->MOSmaxDriftVel > 0) {
        double param[5];

        tmp      = model->MOSmaxDriftVel*here->MOSeffectiveLength/ueff;
        tmp1     = (vgsx - vbin)*oneovere + phiMinVbs;
        param[0] = P1X3*gammad;
        param[1] = -2.0*(tmp1 + tmp);
        param[2] = -2.0*gammad*tmp;
        param[3] = 2.0*tmp1*(phiMinVbs + tmp) - phiMinVbs*phiMinVbs -
                    P1X3*gammad*sarg3;
        param[4] = phiMinVbs;

        mos2_vsat(&ms->ms_vdsat,param);
    }

    /*
     *  evaluate effective channel length and its derivatives
     */
    dldvgs = 0.0;
    dldvds = 0.0;
    dldvbs = 0.0;

    if (lvds != 0.0) {

        gammad = gamasd;
        if ((lvbs - ms->ms_vdsat) <= 0) {
            bsarg = sqrt(ms->ms_vdsat + phiMinVbs);
            dbsrdb = -0.5/bsarg;
        }
        else {
            bsarg = sphi/(1.0 + 0.5*(lvbs - ms->ms_vdsat)/here->MOStPhi);
            dbsrdb = -0.5*bsarg*bsarg/sphi3;
        }
        bodys = bsarg*bsarg*bsarg - sarg3;
        gdbdvs = 2.0*gammad*(bsarg*bsarg*dbsrdb - sarg*sarg*dsrgdb);

        if (model->MOSmaxDriftVel <= 0) {
            if (model->MOSsubstrateDoping != 0.0 && xlamda <= 0.0) {
                tmp    = .25*(lvds - ms->ms_vdsat);
                tmp1   = sqrt(1.0 + tmp*tmp);
                tmp    = sqrt(tmp + tmp1);
                xlamda = model->MOSxd*tmp*oneoverl/lvds;
                tmp    = lvds*xlamda/(8.0*tmp1);
                dldvgs = tmp*dsdvgs;
                dldvds = -xlamda + tmp;
                dldvbs = tmp*dsdvbs;
            }
        }
        else {
            double txdv, txlv, tqdsat;

            txdv   = model->MOSxd/sqrt(model->MOSchannelCharge);
            txlv   = model->MOSmaxDriftVel*txdv/(2.0*ueff);
            tqdsat = -1.0 + gammad*dbsrdb;
            tmp    = model->MOSmaxDriftVel*here->MOSeffectiveLength;
            tmp1   = (vgsx - vbin)*oneovere - ms->ms_vdsat - gammad*bsarg;
            tmp1   = 1.0/(tmp*tqdsat - ueff*tmp1);
            dsdvgs = -(tmp - ueff*ms->ms_vdsat)*oneovere*tmp1;
            dsdvbs = -(-tmp*(1.0 + tqdsat - factor*oneovere) +
                        ueff*(gdbdvs - P66*dgdvbs*bodys)*oneovere)*tmp1;

            if (model->MOSsubstrateDoping != 0.0 && xlamda <= 0.0) {
                tmp    = lvds - ms->ms_vdsat;
                tmp    = MAX(tmp,0.0);
                tmp1   = sqrt(txlv*txlv + tmp);
                tmp    = txdv*oneoverl/(2.0*tmp1);
                xlamda = txdv*(tmp1 - txlv)*oneoverl/lvds;
                dldvgs = tmp*dsdvgs;
                dldvds = -xlamda + tmp;
                dldvbs = tmp*dsdvbs;
            }
        }
    }

    /*
     *     limit channel shortening at punch-through
     */
    xwb    = model->MOSxd*sbiarg;
    clfact = 1.0 - xlamda*lvds;
    dldvds = -xlamda - dldvds;
    tmp1   = here->MOSeffectiveLength*clfact;
    tmp    = xlamda*lvds*here->MOSeffectiveLength;
    if (model->MOSsubstrateDoping == 0.0) xwb = 0.25e-6;
    if (tmp1 < xwb) {
        tmp1 = xwb/(1.0 + (tmp - here->MOSeffectiveLength - xwb)/xwb);
        clfact = tmp1*oneoverl;

        dfact   = tmp1/xwb;
        dfact  *= dfact;
        dldvgs *= dfact;
        dldvds *= dfact;
        dldvbs *= dfact;
    }

    /*
     *  evaluate effective beta (effective kp)
     */
    clfact = 1.0/clfact;
    beta1  = here->MOSbeta*ufact*clfact;
    ufact  = 1.0/ufact;

    /*
     *  test for mode of operation and branch appropriately
     */
    gammad = gamasd;
    dgdvbs = dgddvb;
    if (lvds <= 1.0e-10) {
        if (lvgs <= ms->ms_von) {
            if ((model->MOSfastSurfaceStateDensity == 0.0) ||
                    (here->MOSoxideCap == 0.0)) {
                here->MOSgds = 0.0;
                goto bad;
            }

            here->MOSgds = beta1*(ms->ms_von - vbin - gammad*sarg)*exp(argg*
                (lvgs-ms->ms_von));
            goto bad;
        }

        here->MOSgds = beta1*(lvgs-vbin-gammad*sarg);
        goto bad;
    }

    if (lvgs > ms->ms_von) {

        if (lvds <= ms->ms_vdsat) {
            /*
             *  linear region
             */

            cdrain = beta1*((lvgs - vbin - .5*eta*lvds)*lvds -
                P66*gammad*body);

            tmp = cdrain*(dudvgs*ufact - dldvgs*clfact);
            here->MOSgm = tmp + beta1*lvds;

            tmp = cdrain*(dudvds*ufact - dldvds*clfact);
            here->MOSgds = tmp + beta1*(lvgs - vbin - eta*lvds -
                gammad*barg - P66*dgdvds*body);

            tmp = cdrain*(dudvbs*ufact - dldvbs*clfact);
            here->MOSgmbs = tmp - beta1*(gdbdv + P66*dgdvbs*body -
                factor*lvds);
        }
        else {
            /* 
             *  saturation region
             */
            cdrain = beta1*((lvgs - vbin - eta*
                .5*ms->ms_vdsat)*ms->ms_vdsat - P66*gammad*bodys);

            tmp = cdrain*(dudvgs*ufact - dldvgs*clfact);
            here->MOSgm = tmp + beta1*ms->ms_vdsat + beta1*(lvgs -
                vbin - eta*ms->ms_vdsat - gammad*bsarg)*dsdvgs;
            here->MOSgds = -cdrain*dldvds*clfact - P66*beta1*dgdvds*bodys;
            tmp = cdrain*(dudvbs*ufact - dldvbs*clfact);
            here->MOSgmbs = tmp - beta1*(gdbdvs + P66*dgdvbs*bodys -
                factor*ms->ms_vdsat) + beta1*
                    (lvgs - vbin - eta*ms->ms_vdsat - gammad*bsarg)*dsdvbs;
        }
        return (cdrain);
    }

    /*
     *  subthreshold region
     */
    if (ms->ms_vdsat <= 0) {
        here->MOSgds = 0.0;
        if (lvgs > vth) return (cdrain);
        goto bad;
    } 
    vdson = MIN(ms->ms_vdsat,lvds);
    if (lvds > ms->ms_vdsat) {
        barg = bsarg;
        dbrgdb = dbsrdb;
        body = bodys;
        gdbdv = gdbdvs;
    }
    cdson = beta1*((ms->ms_von - vbin - .5*eta*vdson)*vdson -
        P66*gammad*body);
    didvds = beta1*(ms->ms_von - vbin - eta*vdson - gammad*barg);
    gdson = -cdson*dldvds*clfact - P66*beta1*dgdvds*body;
    if (lvds < ms->ms_vdsat)
        gdson += didvds;
    gbson = -cdson*dldvbs*clfact + beta1*
        (dodvbs*vdson + factor*vdson - P66*dgdvbs*body - gdbdv);
    if (lvds > ms->ms_vdsat)
        gbson += didvds*dsdvbs;
    tmp = exp(argg*(lvgs - ms->ms_von));
    cdrain = cdson*tmp;
    gmw = cdrain*argg;
    here->MOSgm = gmw;
    if (lvds > ms->ms_vdsat) here->MOSgm = gmw + didvds*dsdvgs*tmp;
    tmp1 = gmw*(lvgs - ms->ms_von)/xn;
    here->MOSgds = gdson*tmp - here->MOSgm*dodvds - tmp1*dxndvd;
    here->MOSgmbs = gbson*tmp - here->MOSgm*dodvbs - tmp1*dxndvb;
    return (cdrain);

    /*
     *  finish special cases
     */
bad:
    here->MOSgm = 0.0;
    here->MOSgmbs = 0.0;
    return (0.0);
}


static void
mos2_vsat(vsat,baum)

double *vsat, *baum;
{

    double a00, a01, a02, a03, a04, a05, b00, b01;
    double poly4, az[4], bz[4];
    double a1, a3, b1, b3, c1, d1, p0, p2, p, s2, s, y3;
    double tmp, tmp1, ptmp;
    int i, jknt;

    /* 
     *     evaluate saturation voltage and its derivatives 
     *     according to baum's theory of scattering velocity 
     *     saturation
     */
    a1 = baum[0];
    b1 = baum[1];
    c1 = baum[2];
    d1 = baum[3];

    tmp  = a1*c1 - 4.0*d1;
    tmp1 = -P33*b1*b1 + tmp;
    s    = -2.0*b1*b1*b1/27.0 + P33*b1*tmp;
    tmp  = -d1*(a1*a1 - 4.0*b1) - c1*c1;
    s   += tmp;

    tmp  = P33*tmp1;
    tmp *= tmp*tmp;
    s2   = s*s;
    p    = .25*s2 + tmp;
    p0   = FABS(p);
    p2   = sqrt(p0);

    if (p < 0) {
        tmp  = sqrt(.25*s2 + p0);
        tmp  = exp(P33*log(tmp));
        y3   = 2.0*tmp*cos(P33*atan(-2.0*p2/s)) + P33*b1;
    }
    else {
        tmp  = (-.5*s + p2);
        tmp  = exp(P33*log(FABS(tmp)));
        tmp1 = (-.5*s - p2);
        tmp1 = exp(P33*log(FABS(tmp1)));
        y3  += tmp + tmp1 + P33*b1;
    }

    jknt = 0;
    a3  = sqrt(.25*a1*a1 - b1 + y3);
    b3  = sqrt(.25*y3*y3 - d1);
    b00 = .5*y3 + b3;
    b01 = .5*y3 - b3;
    a00 = .5*a1 + a3;
    a01 = .5*a1 - a3;
    a02 = .25*a00*a00;
    a03 = .25*a01*a01;
    a04 = -.5*a00;
    a05 = -.5*a01;
    az[0] = a02 - b00;
    az[1] = a03 - b00;
    az[2] = a02 - b01;
    az[3] = a03 - b01;
    bz[0] = a04;
    bz[1] = a05;
    bz[2] = a04;
    bz[3] = a05;

    for (i = 0; i < 4; i++) {
        tmp = az[i];
        if (tmp >= 0) {
            tmp = sqrt(tmp);
            ptmp = bz[i] + tmp;
            if (ptmp > 0) {
                poly4 = d1 + ptmp*(c1 + ptmp*(b1 + ptmp*(a1 + ptmp)));
                if (FABS(poly4) <= 1.0e-6) {
                    if (!jknt || ptmp <= tmp1)
                        tmp1 = ptmp;
                    jknt++;
                }
            }
            ptmp = bz[i] - tmp;
            if (ptmp > 0) {
                poly4 = d1 + ptmp*(c1 + ptmp*(b1 + ptmp*(a1 + ptmp)));
                if (FABS(poly4) <= 1.0e-6) {
                    if (!jknt || ptmp <= tmp1)
                        tmp1 = ptmp;
                    jknt++;
                }
            }
        }
    }
    if (jknt > 0)
        *vsat = tmp1*tmp1 - baum[4];
}


double
MOSeq3(model,here,ms)

MOSmodel *model;
MOSinstance *here;
struct mosstuff *ms;
{

    /*
     *     this routine evaluates the drain current, its derivatives and
     *     the charges associated with the gate, channel and bulk
     *     for mosfets based on semi-empirical equations (level 3 model)
     */

    double beta = here->MOSbeta;
    double cdrain;
    double coeff0 = 0.0631353e0;
    double coeff1 = 0.8013292e0;
    double coeff2 = -0.01110777e0;
    double oneoverxl;   /* 1/effective length */
    double eta;         /* eta from model after length factor */
    double phibs;       /* phi - vbs */
    double sqphbs;      /* square root of phibs */
    double dsqdvb;
    double sqphis;      /* square root of phi */
    double sqphs3;      /* square root of phi cubed */
    double wps;
    double oneoverxj;   /* 1/junction depth */
    double xjonxl;      /* junction depth/effective length */
    double djonxj;
    double wponxj;
    double arga;
    double argb;
    double argc;
    double dwpdvb;
    double dadvb;
    double dbdvb;
    double gammas;
    double fbodys;
    double fbody;
    double onfbdy;
    double qbonco;
    double wconxj;
    double dfsdvb;
    double dfbdvb;
    double dqbdvb;
    double vth;
    double dvtdvb;
    double csonco;
    double cdonco;
    double dxndvb;
    double dvodvb;
    double dvodvd;
    double vgsx;
    double dvtdvd;
    double onfg;
    double fgate;
    double us;
    double dfgdvg;
    double dfgdvd;
    double dfgdvb;
    double dvsdvg;
    double dvsdvb;
    double dvsdvd;
    double xn;
    double vdsc;
    double onvdsc;
    double dvsdga;
    double vdsx;
    double dcodvb;
    double cdnorm;
    double cdo;
    double cd1;
    double fdrain;
    double fd2;
    double dfddvg;
    double dfddvb;
    double dfddvd;
    double gdsat;
    double cdsat;
    double gdoncd;
    double gdonfd;
    double gdonfg;
    double dgdvg;
    double dgdvd;
    double dgdvb;
    double emax;
    double emongd;
    double demdvg;
    double demdvd;
    double demdvb;
    double delxl;
    double dldvd;
    double dldem;
    double ddldvg;
    double ddldvd;
    double ddldvb;
    double dlonxl;
    double xlfact;
    double diddl;
    double gds0;
    double emoncd;
    double ondvt;
    double onxn;
    double wfact;
    double gms;
    double gmw;
    double fshort;
    double lvbs;
    double lvds;
    double lvgs;

    /* 'local' variables - these switch d & s around appropriately
     * so that we don't have to worry about vds < 0
     */
    if (here->MOSmode > 0) {
        lvbs = ms->ms_vbs;
        lvds = ms->ms_vds;
        lvgs = ms->ms_vgs;
    }
    else {
        lvbs = ms->ms_vbd;
        lvds = -ms->ms_vds;
        lvgs = ms->ms_vgd;
    }
    /*
     *     bypasses the computation of charges
     */

    /*
     *     reference cdrain equations to source and
     *     charge equations to bulk
     */
    ms->ms_vdsat = 0.0;
    oneoverxl = 1.0/here->MOSeffectiveLength;

    eta = model->MOSeta * 8.15e-22/(model->MOSoxideCapFactor*
            here->MOSeffectiveLength*
            here->MOSeffectiveLength*here->MOSeffectiveLength);
    /*
     *.....square root term
     */
    if (lvbs <=  0.0 ) {
        phibs  =  here->MOStPhi - lvbs;
        sqphbs =  sqrt(phibs);
        dsqdvb =  -0.5/sqphbs;
    }
    else {
        sqphis = sqrt(here->MOStPhi);
        sqphs3 = here->MOStPhi*sqphis;
        sqphbs = sqphis/(1.0 + lvbs/(here->MOStPhi + here->MOStPhi));
        phibs  = sqphbs*sqphbs;
        dsqdvb = -phibs/(sqphs3+sqphs3);
    }
    /*
     *.....short channel effect factor
     */
    if ( (model->MOSjunctionDepth != 0.0) && 
            (model->MOSxd != 0.0) ) {
        wps = model->MOSxd*sqphbs;
        oneoverxj = 1.0/model->MOSjunctionDepth;
        xjonxl = model->MOSjunctionDepth*oneoverxl;
        djonxj = model->MOSlatDiff*oneoverxj;
        wponxj = wps*oneoverxj;
        wconxj = coeff0 + coeff1*wponxj + coeff2*wponxj*wponxj;
        arga   = wconxj + djonxj;
        argc   = wponxj/(1.0 + wponxj);
        argb   = sqrt(1.0 - argc*argc);
        fshort = 1.0 - xjonxl*(arga*argb - djonxj);
        dwpdvb = model->MOSxd*dsqdvb;
        dadvb  = (coeff1 + coeff2*(wponxj+wponxj))*dwpdvb*oneoverxj;
        dbdvb  = -argc*argc*(1.0 - argc)*dwpdvb/(argb*wps);
        dfsdvb = -xjonxl*(dadvb*argb + arga*dbdvb);
    }
    else {
        fshort = 1.0;
        dfsdvb = 0.0;
    }
    /*
     *.....body effect
     */
    gammas = model->MOSgamma*fshort;
    fbodys = 0.5*gammas/(sqphbs+sqphbs);
    fbody  = fbodys+model->MOSnarrowFactor/here->MOSw;
    onfbdy = 1.0/(1.0 + fbody);
    dfbdvb = -fbodys*dsqdvb/sqphbs + fbodys*dfsdvb/fshort;
    qbonco = gammas*sqphbs + model->MOSnarrowFactor*phibs/here->MOSw;
    dqbdvb = gammas*dsqdvb + model->MOSgamma*dfsdvb*sqphbs -
        model->MOSnarrowFactor/here->MOSw;
    /*
     *.....threshold voltage
     */
    vth    = qbonco + here->MOStVbi*model->MOStype - eta*lvds;
    dvtdvd = -eta;
    dvtdvb = dqbdvb;
    /*
     *.....joint weak inversion and strong inversion
     */
    ms->ms_von = vth;
    if (model->MOSfastSurfaceStateDensity != 0.0) {
        csonco = CHARGE*model->MOSfastSurfaceStateDensity * CM2PM2 *
            here->MOSeffectiveLength*here->MOSw/here->MOSoxideCap;
        cdonco = qbonco/(phibs + phibs);
        xn     = 1.0 + csonco + cdonco;
        ms->ms_von = vth + ms->ms_vt*xn;
        dxndvb = dqbdvb/(phibs + phibs) - qbonco*dsqdvb/(phibs*sqphbs);
        dvodvd = dvtdvd;
        dvodvb = dvtdvb + ms->ms_vt*dxndvb;
    }
    else {
        /*
         *.....cutoff region
         */
        if (lvgs <= ms->ms_von ) {
            cdrain = 0.0;
            here->MOSgm = 0.0;
            here->MOSgds = 0.0;
            here->MOSgmbs = 0.0;
            goto done;
        }
    }
    /*
     *.....device is on
     */
    vgsx = MAX(lvgs,ms->ms_von);
    /*
     *.....mobility modulation by gate voltage
     */
    onfg   = 1.0 + model->MOStheta*(vgsx - vth);
    fgate  = 1.0/onfg;
    us     = here->MOStSurfMob * M2PCM2 * fgate;
    dfgdvg = -model->MOStheta*fgate*fgate;
    dfgdvd = -dfgdvg*dvtdvd;
    dfgdvb = -dfgdvg*dvtdvb;
    /*
     *.....saturation voltage
     */
    ms->ms_vdsat = (vgsx - vth)*onfbdy;
    if ( model->MOSmaxDriftVel <= 0.0 ) {
        dvsdvg = onfbdy;
        dvsdvd = -dvsdvg*dvtdvd;
        dvsdvb = -dvsdvg*dvtdvb-ms->ms_vdsat*dfbdvb*onfbdy;
    }
    else {
        vdsc   = here->MOSeffectiveLength*model->MOSmaxDriftVel/us;
        onvdsc = 1.0/vdsc;
        arga   = (vgsx - vth)*onfbdy;
        argb   = sqrt(arga*arga + vdsc*vdsc);
        ms->ms_vdsat = arga+vdsc - argb;
        dvsdga = (1.0 - arga/argb)*onfbdy;
        dvsdvg = dvsdga - (1.0 - vdsc/argb)*vdsc*dfgdvg*onfg;
        dvsdvd = -dvsdvg*dvtdvd;
        dvsdvb = -dvsdvg*dvtdvb - arga*dvsdga*dfbdvb;
    }
    /*
     *.....current factors in linear region
     */
    vdsx = MIN(lvds,ms->ms_vdsat);
    if (vdsx == 0.0) {
        beta *= fgate;
        cdrain = 0.0;
        here->MOSgm = 0.0;
        here->MOSgds = beta*(vgsx-vth);
        here->MOSgmbs = 0.0;
        if ( (model->MOSfastSurfaceStateDensity != 0.0) && 
                (lvgs < ms->ms_von) ) {
            here->MOSgds *= exp( (lvgs - ms->ms_von)/(ms->ms_vt*xn) );
        }
        goto done;
    }

    cdo = vgsx - vth - 0.5*(1.0 + fbody)*vdsx;
    dcodvb = -dvtdvb - 0.5*dfbdvb*vdsx;
    /* 
     *.....normalized drain current
     */
    cdnorm = cdo*vdsx;
    here->MOSgm = vdsx;
    here->MOSgds = vgsx - vth - (1.0 + fbody + dvtdvd)*vdsx;
    here->MOSgmbs = dcodvb*vdsx;
    /* 
     *.....drain current without velocity saturation effect
     */
    cd1 = beta*cdnorm;
    beta *= fgate;
    cdrain = beta*cdnorm;
    here->MOSgm = beta*here->MOSgm+dfgdvg*cd1;
    here->MOSgds = beta*here->MOSgds+dfgdvd*cd1;
    here->MOSgmbs = beta*here->MOSgmbs;
    /*
     *.....velocity saturation factor
     */
    if ( model->MOSmaxDriftVel != 0.0 ) {
        fdrain = 1.0/(1.0+vdsx*onvdsc);
        fd2    = fdrain*fdrain;
        arga   = fd2*vdsx*onvdsc*onfg;
        dfddvg = -dfgdvg*arga;
        dfddvd = -dfgdvd*arga - fd2*onvdsc;
        dfddvb = -dfgdvb*arga;
        /*
         *.....drain current
         */
        here->MOSgm = fdrain*here->MOSgm + dfddvg*cdrain;
        here->MOSgds = fdrain*here->MOSgds + dfddvd*cdrain;
        here->MOSgmbs = fdrain*here->MOSgmbs + dfddvb*cdrain;
        cdrain = fdrain*cdrain;
        beta *= fdrain;
    }
    /*
     *.....channel length modulation
     */
    if (lvds <= ms->ms_vdsat) goto invers;

    if (model->MOSmaxDriftVel == 0.0) {
        delxl =
            sqrt(model->MOSkappa*(lvds - ms->ms_vdsat)*model->MOSalpha);
        dldvd  = 0.5*delxl/(lvds - ms->ms_vdsat);
        ddldvg = 0.0;
        ddldvd = -dldvd;
        ddldvb = 0.0;
    }
    else {

        if (model->MOSalpha == 0.0) goto invers;

        cdsat  = cdrain;
        gdsat  = cdsat*(1.0 - fdrain)*onvdsc;
        gdsat  = MAX(1.0e-12,gdsat);
        gdoncd = gdsat/cdsat;
        gdonfd = gdsat/(1.0 - fdrain);
        gdonfg = gdsat*onfg;
        dgdvg  = gdoncd*here->MOSgm - gdonfd*dfddvg+gdonfg*dfgdvg;
        dgdvd  = gdoncd*here->MOSgds - gdonfd*dfddvd+gdonfg*dfgdvd;
        dgdvb  = gdoncd*here->MOSgmbs - gdonfd*dfddvb+gdonfg*dfgdvb;

        emax = model->MOSkappa * cdsat*oneoverxl/gdsat;

        emoncd = emax/cdsat;
        emongd = emax/gdsat;
        demdvg = emoncd*here->MOSgm - emongd*dgdvg;
        demdvd = emoncd*here->MOSgds - emongd*dgdvd;
        demdvb = emoncd*here->MOSgmbs - emongd*dgdvb;

        arga   = 0.5*emax*model->MOSalpha;
        argc   = model->MOSkappa*model->MOSalpha;
        argb   = sqrt(arga*arga + argc*(lvds - ms->ms_vdsat));
        delxl  = argb - arga;
        dldvd  = argc/(argb+argb);
        dldem  = 0.5*(arga/argb - 1.0)*model->MOSalpha;
        ddldvg = dldem*demdvg;
        ddldvd = dldem*demdvd-dldvd;
        ddldvb = dldem*demdvb;
    }
    /*
     *.....punch through approximation
     */
    if (delxl > (0.5*here->MOSeffectiveLength)) {
        delxl  = here->MOSeffectiveLength -
                (here->MOSeffectiveLength*here->MOSeffectiveLength/
                (4.0*delxl));
        arga   = 4.0*(here->MOSeffectiveLength - delxl)*
                (here->MOSeffectiveLength - delxl)/
                (here->MOSeffectiveLength*here->MOSeffectiveLength);
        ddldvg = ddldvg*arga;
        ddldvd = ddldvd*arga;
        ddldvb = ddldvb*arga;
        dldvd  = dldvd*arga;
    }
    /*
     *.....saturation region
     */
    dlonxl     = delxl*oneoverxl;
    xlfact     = 1.0/(1.0 - dlonxl);
    cdrain     = cdrain*xlfact;
    diddl      = cdrain/(here->MOSeffectiveLength - delxl);
    here->MOSgm= here->MOSgm*xlfact + diddl*ddldvg;
    gds0       = here->MOSgds*xlfact + diddl*ddldvd;
    here->MOSgmbs = here->MOSgmbs*xlfact + diddl*ddldvb;
    here->MOSgm = here->MOSgm + gds0*dvsdvg;
    here->MOSgmbs = here->MOSgmbs + gds0*dvsdvb;
    here->MOSgds = gds0*dvsdvd + diddl*dldvd;

invers:
    /*
     *.....finish strong inversion case
     */
    if (lvgs < ms->ms_von) {
        /*
         *.....weak inversion
         */
        onxn   = 1.0/xn;
        ondvt  = onxn/ms->ms_vt;
        wfact  = exp((lvgs - ms->ms_von)*ondvt);
        cdrain = cdrain*wfact;
        gms    = here->MOSgm*wfact;
        gmw    = cdrain*ondvt;
        here->MOSgm = gmw;
        if (lvds > ms->ms_vdsat) {
            here->MOSgm = here->MOSgm+gds0*dvsdvg*wfact;
        }
        here->MOSgds = here->MOSgds*wfact+(gms-gmw)*dvodvd;
        here->MOSgmbs = here->MOSgmbs*wfact + (gms-gmw)*dvodvb - gmw*
            (lvgs - ms->ms_von)*onxn*dxndvb;
    }

done:
    return (cdrain);
}


double
MOSeq6(model,here,ms)

MOSmodel *model;
MOSinstance *here;
struct mosstuff *ms;
{
    /*
     *     this block of code evaluates the drain current and its 
     *     derivatives using the n-th power MOS model and the 
     *     charges associated with the gate, channel and bulk for 
     *     mosfets
     *
     */
    double arg;
    double sarg;
    double vgon;
    double cdrain;
    double lvds, lvbs, lvgs;
    double idsat, lambda, vonbm;
    double vdst, vdst1, vdst2, ivdst1, vdstg;

    lvbs = (here->MOSmode == 1 ? ms->ms_vbs : ms->ms_vbd);
    lvds = ms->ms_vds * here->MOSmode;
    lvgs = (here->MOSmode == 1 ? ms->ms_vgs : ms->ms_vgd);

    if (lvbs <= 0 ) {
        sarg = sqrt(here->MOStPhi - lvbs);
    }
    else {
        sarg = sqrt(here->MOStPhi);
        sarg = sarg - lvbs/(sarg+sarg);
        sarg = MAX(0,sarg);
    }
    ms->ms_von = (here->MOStVbi*model->MOStype)+model->MOSgamma*sarg
        - model->MOSgamma1 * lvbs;
        - model->MOSsigma  * lvds;
    vgon = lvgs - ms->ms_von;

    if (vgon <= 0) {
        /*
         *     cutoff region
         */
        ms->ms_vdsat   = 0;
        here->MOSgm   = 0;
        here->MOSgds  = 0;
        here->MOSgmbs = 0;
        cdrain         = 0;

    }
    else {
        if (sarg <= 0) {
            arg = 0;
        }
        else {
            if (lvbs <= 0 ) {
                vonbm = model->MOSgamma1
                    + model->MOSgamma/(sarg + sarg);
            }
            else {
                vonbm = model->MOSgamma1
                    + .5*model->MOSgamma/sqrt(here->MOStPhi);
            }
        }
        sarg = log(vgon);
        ms->ms_vdsat = model->MOSkv * exp(sarg * model->MOSnv);
        idsat = here->MOSbeta * exp(sarg * model->MOSnc);
        lambda = model->MOSlamda0 - model->MOSlamda1*lvbs;
        /*
         *     saturation region
         */
        cdrain = idsat*(1 + lambda*lvds);
        here->MOSgm   = cdrain*model->MOSnc/vgon;
        here->MOSgds  = here->MOSgm*model->MOSsigma + idsat*lambda;
        here->MOSgmbs = here->MOSgm*vonbm - idsat*model->MOSlamda1*lvds;

        if (ms->ms_vdsat > lvds) {
            /*
             *     linear region
             */
            vdst   = lvds/ms->ms_vdsat;
            vdst2  = (2 - vdst)*vdst;
            vdstg  = -vdst*model->MOSnv/vgon;
            ivdst1 = cdrain*(2 - vdst - vdst);
            cdrain = cdrain*vdst2;
            here->MOSgm = here->MOSgm*vdst2 + ivdst1*vdstg;
            here->MOSgds = here->MOSgds*vdst2 +
                ivdst1*(1/ms->ms_vdsat + vdstg*model->MOSsigma);
            here->MOSgmbs = here->MOSgmbs*vdst2 +
                ivdst1*vdstg*vonbm;
        }
    }
    return (cdrain);
}


/* level 8, CryoMOS by Luong Huynh 1/19/95*/
double
cryoMOSeq(model,here,ms)

MOSmodel *model;
MOSinstance *here;
struct mosstuff *ms;
{

    /*
     *     this block of code evaluates the drain current and its 
     *     derivatives using the cryo-MOS model and the 
     *     charges associated with the gate, channel and bulk.
     *
     */

    double sarg;
    double vgst;
    double vgst2;
    double cdrain;
    double vbx;
    double vgx;
    double vds;
    double L;
    double delta;
    double lambda;
    double litl;
    double factor;
    double t1,t2,t3,t4,t5;
    double u0;
    double eta;
    double d1u0dvgst;
    double ueff;
    double dueffdvds;
    double dueffdvdsat;
    double dueffdvgst;
    double dueffdvdsatdvgst;
    double vdsat;
    double dvdsatdvgst;
    double idsat;
    double didsatdvdsat;
    double didsatdvgst;
    double va;
    double dvadvds;
    double dvadvgst;
    double vasat;
    double dvasatdvgst;
    double vgst1=1.5;

    L = here->MOSeffectiveLength;
    factor = here->MOSw/L*model->MOSoxideCapFactor;
    litl = sqrt(11.7/3.9*model->MOSoxideThickness*model->MOSjunctionDepth);

    vbx = ((here->MOSmode == 1) ? ms->ms_vbs : ms->ms_vbd);
    vgx = ((here->MOSmode == 1) ? ms->ms_vgs : ms->ms_vgd);

    if (vbx <= 0) {
      sarg = sqrt(model->MOSphi-vbx)-sqrt(model->MOSphi);
      delta = model->MOSgamma/(2.0*sqrt(model->MOSphi-vbx));
    } else if (vbx < model->MOSphi) {
      sarg = -vbx/(2.0*sqrt(model->MOSphi));
      delta = model->MOSgamma/(2.0*sqrt(model->MOSphi));
    } else {
      sarg = -sqrt(model->MOSphi)/2.0;
      delta = 0;
    }

    if (model->MOStype > 0)
        ms->ms_von = model->MOSvt0 + model->MOSgamma*sarg;
    else
        ms->ms_von = -model->MOSvt0 + model->MOSgamma*sarg;
    vgst = vgx - ms->ms_von;
    vgst2 = vgst*vgst;

    u0 = 1.0/(model->MOSk1/vgst + model->MOSk2 
	      + model->MOSk3*vgst2 + model->MOSk4*vgst);
    d1u0dvgst = -model->MOSk1/vgst2 + 2.0*model->MOSk3*vgst + model->MOSk4;
    eta = u0/model->MOSmaxDriftVel/L;

    t1 = u0*model->MOSa1/vgst/(L+model->MOSa2/(1+delta));
    t2 = u0*model->MOSa1*L/vgst/(L+model->MOSa2*0.5/(1+delta))
      /(L+model->MOSa2*0.5/(1+delta));
    t3 = vgst/(1+delta);
    t4 = sqrt((1.0+(-t1+t2)*t3)*(1.0+(-t1+t2)*t3)+2.0*(eta+eta+t1+t1-t2)*t3);
    ms->ms_vdsat = vdsat = (-(1.0+(-t1+t2)*t3)+t4)/(eta+eta+t1+t1-t2);
    t5 = (eta+eta)/u0/(1+delta);
    dvdsatdvgst = (-d1u0dvgst*u0+((1.0+(-t1+t2)*t3)*d1u0dvgst+t5
       + (eta+eta+t1+t1-t2)*t3*d1u0dvgst)/t4)/(eta+eta+t1+t1-t2)
       - vdsat*(-(t1+t1-t2)/vgst)/(eta+eta+t1+t1-t2);

    if (vgst <= 0) {
        cdrain = 0;
        here->MOSgm = 0;
        here->MOSgds = 0;
        here->MOSgmbs = 0;
    }
    else {
        if (here->MOSmode > 0)
            vds = ms->ms_vds;
        else
            vds = -ms->ms_vds;
	
        if (vdsat <= vds) {
	    /* saturation region */
	    t2 = 1.0/(vgst*L+model->MOSa2*vdsat);
	    ueff = u0/(1+eta*vdsat+u0*model->MOSa1*vdsat*t2);
	    dueffdvdsat = -ueff*ueff*(eta/u0+model->MOSa1*vgst*L*t2*t2);
	    dueffdvgst = -ueff*ueff*(d1u0dvgst+eta/u0*dvdsatdvgst+
              model->MOSa1*L*(vgst*dvdsatdvgst-vdsat)*t2*t2);
	    dueffdvdsatdvgst = -2.0*ueff*dueffdvgst*(eta/u0+
	      model->MOSa1*vgst*L*t2*t2)-ueff*ueff*(model->MOSa1*L*t2*t2-
	      model->MOSa1*vgst*L*2.0*t2*t2*t2*
              (L+model->MOSa2*dvdsatdvgst));
	    idsat = factor*ueff*(vgst-0.5*(1.0+delta)*vdsat)*vdsat;
	    didsatdvdsat = idsat*dueffdvdsat/ueff+
	      factor*ueff*(vgst-(1.0+delta)*vdsat);
	    didsatdvgst = idsat*(dueffdvgst/ueff+dvdsatdvgst/vdsat)+
	      factor*ueff*(1.0-0.5*(1.0+delta)*dvdsatdvgst)*vdsat;
	    vasat = idsat/didsatdvdsat;
	    dvasatdvgst = didsatdvgst/didsatdvdsat-
	      idsat/didsatdvdsat/didsatdvdsat*(factor*dueffdvdsat*
              (vdsat+vgst*dvdsatdvgst-(1.0+delta)*vdsat*dvdsatdvgst)+
	      idsat/ueff*dueffdvdsatdvgst +
	      factor*ueff*(1.0-(1.0+delta)*dvdsatdvgst));

	    t1 = 1/(1.0-ueff*vdsat/L/model->MOSmaxDriftVel);
	    lambda = 0.5*L/model->MOSem/litl*t1;
	    va = vasat + (L*t1-lambda*(vds - vdsat))*
	      (vds - vdsat)/model->MOSpclm/litl;
	    dvadvds = (L*t1-2.0*lambda*(vds-vdsat))/model->MOSpclm/litl;
	    dvadvgst = dvasatdvgst+L*t1*t1*(-(dueffdvgst*vdsat+
              ueff*dvdsatdvgst)/L/model->MOSmaxDriftVel)*
              (1.0-0.5*(vds-vdsat)/model->MOSem/litl)*
              (vds-vdsat)/model->MOSpclm/litl-dvadvds*dvdsatdvgst;

	    here->MOSgm = didsatdvgst*(1.0+(vds-vdsat)/va)+
	      idsat*(-dvdsatdvgst/va-(vds-vdsat)/va/va*dvadvgst);
	    here->MOSgds = idsat*(1.0/va-(vds-vdsat)/va/va*dvadvds);
            here->MOSgmbs = delta*here->MOSgm;
	    cdrain = idsat*(1.0+(vds-vdsat)/va);
        }
        else {
            /* linear region */

	    ueff = u0/(1+eta*vds+
		       u0*model->MOSa1*vds/(vgst*L+model->MOSa2*vds));
	    cdrain = factor*ueff*(vgst-0.5*(1+delta)*vds)*vds;
	    dueffdvgst = -ueff*ueff*(d1u0dvgst-model->MOSa1*L*vds/
              (vgst*L+model->MOSa2*vds)/(vgst*L+model->MOSa2*vds));
	    dueffdvds = -ueff*ueff*(eta/u0+model->MOSa1*vgst/L/(vgst+
              model->MOSa2*vds/L)/(vgst+model->MOSa2*vds/L));
	    here->MOSgm = factor*vds*(ueff+
	      (vgst-0.5*(1+delta)*vds)*dueffdvgst);
	    here->MOSgds = factor*ueff*(vgst-
              (1.0+delta)*vds)+cdrain*dueffdvds/ueff;
            here->MOSgmbs = delta*here->MOSgm;
       }

    }
    return (cdrain);

}

/**/

