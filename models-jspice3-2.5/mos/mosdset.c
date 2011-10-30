/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1988 Jaijeet S Roychowdhury
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include <math.h>
#include "mosdefs.h"
#include "distodef.h"
#include "const.h"
#include "sperror.h"
#include "util.h"
#include "distoext.h"

#define P16 .16666666667
#define MOD_D_DELTA 1e-5


#ifdef __STDC__
static void mos_deriv(MOSmodel*,MOSinstance*,struct mosstuff*,Dderivs*);
static double mos_exec(MOSmodel*,MOSinstance*,struct mosstuff*,
    double,double,double);
#else
static void mos_deriv();
static double mos_exec();
#endif

int
MOSdSetup(inModel,ckt)

GENmodel *inModel;
CKTcircuit *ckt;
/* actually load the current value into the 
 * sparse matrix previously provided 
 */
{
    MOSmodel *model = (MOSmodel *)inModel;
    MOSinstance *here;
    struct mosstuff ms;
    double arg;
    double cdrain;
    double evb;
    double vgst;
    double sarg;
    double sargsw;
    double lcapgs2;
    double lcapgd2; 
    double lcapgb2;
    double lcapgs3;  
    double lcapgd3; 
    double lcapgb3;
    double lgbs, lgbs2, lgbs3;
    double lgbd, lgbd2, lgbd3;
    double lcapbs, lcapbs2, lcapbs3;
    double lcapbd, lcapbd2, lcapbd3;
    Dderivs cd;

    /* loop through all the MOS device models */
    for ( ; model != NULL; model = model->MOSnextModel) {

        /* loop through all the instances of the model */
        for (here = model->MOSinstances; here != NULL ;
                here = here->MOSnextInstance) {

            ms.ms_vt = CONSTKoverQ * here->MOStemp;

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


            /*
             * bulk-source and bulk-drain diodes
             * here we just evaluate the ideal diode current and the
             * corresponding derivative (conductance).
             */

            if (ms.ms_vbs <= 0) {
                lgbs  = here->MOStSourceSatCur/ms.ms_vt + ckt->CKTgmin;
                lgbs2 = lgbs3 = 0;
            }
            else {
                evb   = exp(ms.ms_vbs/ms.ms_vt);
                lgbs  = here->MOStSourceSatCur*evb/ms.ms_vt + ckt->CKTgmin;
                if (model->MOStype > 0) {
                    lgbs2 = 0.5*(lgbs - ckt->CKTgmin)/ms.ms_vt;
                    lgbs3 = lgbs2/(ms.ms_vt*3);
                }
                else {
                    lgbs2 = -0.5*(lgbs - ckt->CKTgmin)/ms.ms_vt;
                    lgbs3 = -lgbs2/(ms.ms_vt*3);
                }

            }
            if (ms.ms_vbd <= 0) {
                lgbd  = here->MOStDrainSatCur/ms.ms_vt + ckt->CKTgmin;
                lgbd2 = lgbd3 = 0;
            }
            else {
                evb   = exp(ms.ms_vbd/ms.ms_vt);
                lgbd  = here->MOStDrainSatCur*evb/ms.ms_vt + ckt->CKTgmin;
                if (model->MOStype > 0) {
                    lgbd2 = 0.5*(lgbd - ckt->CKTgmin)/ms.ms_vt;
                    lgbd3 = lgbd2/(ms.ms_vt*3);
                }
                else {
                    lgbd2 = -0.5*(lgbd - ckt->CKTgmin)/ms.ms_vt;
                    lgbd3 = -lgbd2/(ms.ms_vt*3);
                }
            }

            /* now to determine whether the user was able to correctly
             * identify the source and drain of his device
             */
            if (ms.ms_vds >= 0) {
                /* normal mode */
                here->MOSmode = 1;
            }
            else {
                /* inverse mode */
                here->MOSmode = -1;
            }

            mos_deriv(model,here,&ms,&cd);

            /*
             *  charge storage elements
             *  bulk-drain and bulk-source depletion capacitances
             */

            if (ms.ms_vbs < here->MOStDepCap) {
                arg = 1 - ms.ms_vbs/here->MOStBulkPot;

                SARGS(arg,model->MOSbulkJctBotGradingCoeff,
                    model->MOSbulkJctSideGradingCoeff,sarg,sargsw);

                lcapbs = here->MOSCbs*sarg + here->MOSCbssw*sargsw;
                lcapbs2 = 0.5/here->MOStBulkPot*
                    (here->MOSCbs*model->MOSbulkJctBotGradingCoeff*sarg +
                    here->MOSCbssw*model->MOSbulkJctSideGradingCoeff*sargsw)
                    /arg;
                if (model->MOStype < 0)
                    lcapbs2 = -lcapbs2;
                lcapbs3 = here->MOSCbs*sarg*
                    model->MOSbulkJctBotGradingCoeff*
                    (model->MOSbulkJctBotGradingCoeff+1);
                lcapbs3 += here->MOSCbssw*sargsw*
                    model->MOSbulkJctSideGradingCoeff*
                    (model->MOSbulkJctSideGradingCoeff+1);
                lcapbs3 = lcapbs3/(6*here->MOStBulkPot*
                    here->MOStBulkPot*arg*arg);
            }
            else {
                lcapbs  = here->MOSf2s + here->MOSf3s*ms.ms_vbs;
                lcapbs2 = 0.5*here->MOSf3s;
                lcapbs3 = 0;
            }

            if (ms.ms_vbd < here->MOStDepCap) {
                arg = 1 - ms.ms_vbd/here->MOStBulkPot;

                SARGS(arg,model->MOSbulkJctBotGradingCoeff,
                    model->MOSbulkJctSideGradingCoeff,sarg,sargsw);

                lcapbd = here->MOSCbd*sarg + here->MOSCbdsw*sargsw;
                lcapbd2 = model->MOStype*0.5/here->MOStBulkPot*
                    (here->MOSCbd*model->MOSbulkJctBotGradingCoeff*sarg +
                    here->MOSCbdsw*model->MOSbulkJctSideGradingCoeff*sargsw)
                    /arg;
                if (model->MOStype < 0)
                    lcapbd2 = -lcapbd2;
                lcapbd3 = here->MOSCbd*sarg*
                    model->MOSbulkJctBotGradingCoeff*
                    (model->MOSbulkJctBotGradingCoeff+1);
                lcapbd3 += here->MOSCbdsw*sargsw*
                    model->MOSbulkJctSideGradingCoeff*
                    (model->MOSbulkJctSideGradingCoeff+1);
                lcapbd3 = lcapbd3/(6*here->MOStBulkPot*
                    here->MOStBulkPot*arg*arg);
            }
            else {
                lcapbd  = here->MOSf2d + ms.ms_vbd * here->MOSf3d;
                lcapbd2 = 0.5*here->MOSf3d;
                lcapbd3 = 0;
            }

            /*
             *     meyer's capacitor model
             */
     
            /* von, vgst and vdsat have already been adjusted for 
             * possible source-drain interchange
             */
            lcapgb2 = 0;
            lcapgb3 = 0;
            lcapgs2 = 0;
            lcapgs3 = 0;
            lcapgd2 = 0;
            lcapgd3 = 0;

            if (here->MOSmode > 0)
                vgst = ms.ms_vgs - ms.ms_von;
            else
                vgst = ms.ms_vgd - ms.ms_von;

            if (vgst > -here->MOStPhi) {

                if (vgst <= -.5*here->MOStPhi) {
                    lcapgb2 = -here->MOSoxideCap/(4*here->MOStPhi);
                }
                else if (vgst <= 0) {
                    lcapgb2 = -here->MOSoxideCap/(4*here->MOStPhi);
                    lcapgs2 = here->MOSoxideCap/(3*here->MOStPhi);
                }
                else {
                    double vds;

                    if (here->MOSmode > 0)
                        vds = ms.ms_vds;
                    else
                        vds = -ms.ms_vds;

                    if (ms.ms_vdsat > vds) {
                        double vddif, vddif1, vddif2, x1, x2;

                        vddif   = 2.0*ms.ms_vdsat - vds;
                        vddif1  = ms.ms_vdsat - vds;
                        vddif2  = vddif*vddif;
                        x1      = 1/(3*vddif*vddif2);
                        x2      = 1/(9*vddif2*vddif2);

                        lcapgd2 = -ms.ms_vdsat*vds*here->MOSoxideCap*x1;
                        lcapgd3 = -vds*
                            here->MOSoxideCap*(vddif - 6*ms.ms_vdsat)*x2;
                        lcapgs2 = -vddif1*vds*here->MOSoxideCap*x1;
                        lcapgs3 = -vds*
                            here->MOSoxideCap*(vddif - 6*vddif1)*x2;
                    }
                }
                if (model->MOStype < 0) {
                    lcapgb2 = -lcapgb2;
                    lcapgs2 = -lcapgs2;
                    lcapgd2 = -lcapgd2;
                }
            }

            /* the b-s and b-d diodes need no processing ...  */
            here->capbs2 = lcapbs2;
            here->capbs3 = lcapbs3;
            here->capbd2 = lcapbd2;
            here->capbd3 = lcapbd3;
            here->gbs2   = lgbs2;
            here->gbs3   = lgbs3;
            here->gbd2   = lgbd2;
            here->gbd3   = lgbd3;
            here->capgb2 = lcapgb2;
            here->capgb3 = lcapgb3;

            /*
             *   process to get Taylor coefficients, taking into
             *   account type and mode.
             */

            if (here->MOSmode == 1) {
                /* normal mode - no source-drain interchange */

                here->capgs2 = lcapgs2;
                here->capgs3 = lcapgs3;
                here->capgd2 = lcapgd2;
                here->capgd3 = lcapgd3;

                if (model->MOStype > 0) {
                    here->cdr_x2  = .5*cd.d2_p2;
                    here->cdr_y2  = .5*cd.d2_q2;
                    here->cdr_z2  = .5*cd.d2_r2;
                    here->cdr_xy  = cd.d2_pq;
                    here->cdr_yz  = cd.d2_qr;
                    here->cdr_xz  = cd.d2_pr;
                }
                else {
                    here->cdr_x2  = -.5*cd.d2_p2;
                    here->cdr_y2  = -.5*cd.d2_q2;
                    here->cdr_z2  = -.5*cd.d2_r2;
                    here->cdr_xy  = -cd.d2_pq;
                    here->cdr_yz  = -cd.d2_qr;
                    here->cdr_xz  = -cd.d2_pr;
                }
                here->cdr_x3  = cd.d3_p3*P16;
                here->cdr_y3  = cd.d3_q3*P16;
                here->cdr_z3  = cd.d3_r3*P16;
                here->cdr_x2z = .5*cd.d3_p2r;
                here->cdr_x2y = .5*cd.d3_p2q;
                here->cdr_y2z = .5*cd.d3_q2r;
                here->cdr_xy2 = .5*cd.d3_pq2;
                here->cdr_xz2 = .5*cd.d3_pr2;
                here->cdr_yz2 = .5*cd.d3_qr2;
                here->cdr_xyz = cd.d3_pqr;


            }
            else {
                /* inverse mode - source and drain interchanged */
                here->capgs2 = lcapgd2;
                here->capgs3 = lcapgd3;
                here->capgd2 = lcapgs2;
                here->capgd3 = lcapgs3;

                if (model->MOStype > 0) {
                    here->cdr_x2 = -.5*cd.d2_p2;
                    here->cdr_y2 = -.5*cd.d2_q2;
                    here->cdr_z2 = -.5*(cd.d2_p2 + cd.d2_q2 + cd.d2_r2 +
                                    2*(cd.d2_pq + cd.d2_pr + cd.d2_qr));
                    here->cdr_xy = -cd.d2_pq;
                    here->cdr_yz = cd.d2_pq + cd.d2_q2 + cd.d2_qr;
                    here->cdr_xz = cd.d2_p2 + cd.d2_pq + cd.d2_pr;
                }
                else {
                    here->cdr_x2 = .5*cd.d2_p2;
                    here->cdr_y2 = .5*cd.d2_q2;
                    here->cdr_z2 = .5*(cd.d2_p2 + cd.d2_q2 + cd.d2_r2 +
                                    2*(cd.d2_pq + cd.d2_pr + cd.d2_qr));
                    here->cdr_xy = cd.d2_pq;
                    here->cdr_yz = -(cd.d2_pq + cd.d2_q2 + cd.d2_qr);
                    here->cdr_xz = -(cd.d2_p2 + cd.d2_pq + cd.d2_pr);
                }

                here->cdr_x3 = -P16*cd.d3_p3;
                here->cdr_y3 = -P16*cd.d3_q3;
                here->cdr_z3 = P16*(cd.d3_p3 + cd.d3_q3 + cd.d3_r3 + 
                                3*(cd.d3_p2q + cd.d3_p2r + cd.d3_pq2 +
                                cd.d3_q2r + cd.d3_pr2 + cd.d3_qr2) +
                                6*cd.d3_pqr);
                here->cdr_x2z = .5*(cd.d3_p3 + cd.d3_p2q + cd.d3_p2r);
                here->cdr_x2y = -.5*cd.d3_p2q;
                here->cdr_y2z = .5*(cd.d3_pq2 + cd.d3_q3 + cd.d3_q2r);
                here->cdr_xy2 = -.5*cd.d3_pq2;
                here->cdr_xz2 = -.5*(cd.d3_p3 + 2*(cd.d3_p2q + cd.d3_p2r +
                                cd.d3_pqr) + cd.d3_pq2 + cd.d3_pr2);
                here->cdr_yz2 = -.5*(cd.d3_q3 + 2*(cd.d3_pq2 + cd.d3_q2r +
                                cd.d3_pqr) + cd.d3_p2q + cd.d3_qr2);
                here->cdr_xyz = cd.d3_p2q + cd.d3_pq2 + cd.d3_pqr;

            }
        }
    }
    return(OK);
}


static void
mos_deriv(model,here,ms,cd)

MOSmodel *model;
MOSinstance *here;
struct mosstuff *ms;
Dderivs *cd;
{
    double delta = MOD_D_DELTA, dd = 1.0/delta;

    double a222 = mos_exec(model,here,ms,0.0,0.0,0.0);

    double a322 = mos_exec(model,here,ms,delta,0.0,0.0);
    double a232 = mos_exec(model,here,ms,0.0,delta,0.0);
    double a223 = mos_exec(model,here,ms,0.0,0.0,delta);

    double a122 = mos_exec(model,here,ms,-delta,0.0,0.0);
    double a212 = mos_exec(model,here,ms,0.0,-delta,0.0);
    double a221 = mos_exec(model,here,ms,0.0,0.0,-delta);

    double a233 = mos_exec(model,here,ms,0.0,delta,delta);
    double a323 = mos_exec(model,here,ms,delta,0.0,delta);
    double a332 = mos_exec(model,here,ms,delta,delta,0.0);

    double a333 = mos_exec(model,here,ms,delta,delta,delta);

    double a123 = mos_exec(model,here,ms,-delta,0.0,delta);
    double a132 = mos_exec(model,here,ms,-delta,delta,0.0);
    double a213 = mos_exec(model,here,ms,0.0,-delta,delta);
    double a312 = mos_exec(model,here,ms,delta,-delta,0.0);
    double a321 = mos_exec(model,here,ms,delta,0.0,-delta);
    double a231 = mos_exec(model,here,ms,0.0,delta,-delta);


    double a022 = mos_exec(model,here,ms,-(delta+delta),0.0,0.0);
    double a202 = mos_exec(model,here,ms,0.0,-(delta+delta),0.0);
    double a220 = mos_exec(model,here,ms,0.0,0.0,-(delta+delta));

    cd->d2_p2  = ((a322 - a222)*dd - (a222 - a122)*dd)*dd;
    cd->d2_q2  = ((a232 - a222)*dd - (a222 - a212)*dd)*dd;
    cd->d2_r2  = ((a223 - a222)*dd - (a222 - a221)*dd)*dd;
    cd->d2_pq  = ((a332 - a322)*dd - (a232 - a222)*dd)*dd;
    cd->d2_qr  = ((a233 - a232)*dd - (a223 - a222)*dd)*dd;
    cd->d2_pr  = ((a323 - a322)*dd - (a223 - a222)*dd)*dd;

    cd->d3_p3  = (((a322 - a222)*dd - (a222 - a122)*dd)*dd -
                  ((a222 - a122)*dd - (a122 - a022)*dd)*dd)*dd;
    cd->d3_q3  = (((a232 - a222)*dd - (a222 - a212)*dd)*dd -
                  ((a222 - a212)*dd - (a212 - a202)*dd)*dd)*dd;
    cd->d3_r3  = (((a223 - a222)*dd - (a222 - a221)*dd)*dd -
                  ((a222 - a221)*dd - (a221 - a220)*dd)*dd)*dd;

    cd->d3_p2r = (((a323 - a223)*dd - (a223 - a123)*dd)*dd -
                  ((a322 - a222)*dd - (a222 - a122)*dd)*dd)*dd;

    cd->d3_p2q = (((a332 - a232)*dd - (a232 - a132)*dd)*dd -
                  ((a322 - a222)*dd - (a222 - a122)*dd)*dd)*dd;

    cd->d3_q2r = (((a233 - a223)*dd - (a223 - a213)*dd)*dd -
                  ((a232 - a222)*dd - (a222 - a212)*dd)*dd)*dd;

    cd->d3_pq2 = (((a332 - a232)*dd - (a322 - a222)*dd)*dd -
                  ((a322 - a222)*dd - (a312 - a212)*dd)*dd)*dd;

    cd->d3_pr2 = (((a323 - a223)*dd - (a322 - a222)*dd)*dd -
                  ((a322 - a222)*dd - (a321 - a221)*dd)*dd)*dd;

    cd->d3_qr2 = (((a233 - a223)*dd - (a232 - a222)*dd)*dd -
                  ((a232 - a222)*dd - (a231 - a221)*dd)*dd)*dd;

    cd->d3_pqr = (((a333 - a233)*dd - (a323 - a223)*dd)*dd -
                  ((a332 - a232)*dd - (a322 - a222)*dd)*dd)*dd;
        
}
/* from now on, p=vgs, q=vbs, r=vds  */

static double
mos_exec(model,here,ms,dvgs,dvbs,dvds)

MOSmodel *model;
MOSinstance *here;
struct mosstuff *ms;
double dvgs, dvbs, dvds;
{
    double vgs, vbs, vds;

    vgs = ms->ms_vgs;
    vbs = ms->ms_vbs;
    vds = ms->ms_vds;

    ms->ms_vgs += dvgs;
    ms->ms_vbs += dvbs;
    ms->ms_vds += dvds;
    ms->ms_vbd = ms->ms_vbs - ms->ms_vds;
    ms->ms_vgd = ms->ms_vgs - ms->ms_vds;
    ms->ms_vgb = ms->ms_vgs - ms->ms_vbs;

    if (model->MOSlevel == 1)
        ms->ms_cdrain = MOSeq1(model,here,ms);
    else if (model->MOSlevel == 2)
        ms->ms_cdrain = MOSeq2(model,here,ms);
    /* CryoMOS Luong Huynh, 1/19/95 */
    else if (model->MOSlevel == 8)
        ms->ms_cdrain = cryoMOSeq(model,here,ms);
    else
        ms->ms_cdrain = MOSeq3(model,here,ms);

    ms->ms_vgs = vgs;
    ms->ms_vbs = vbs;
    ms->ms_vds = vds;
    ms->ms_vbd = ms->ms_vbs - ms->ms_vds;
    ms->ms_vgd = ms->ms_vgs - ms->ms_vds;
    ms->ms_vgb = ms->ms_vgs - ms->ms_vbs;

    return (ms->ms_cdrain);
}
