/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Author: 1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "jjdefs.h"
#include "const.h"
#include "sperror.h"
#include "util.h"
#include "niext.h"

#if __GNUC__
#ifdef i386
#define ASM_SINCOS
#endif
#endif

struct jjstuff {
    double js_vj;
    double js_phi;
    double js_ci;
    double js_gqt;
    double js_crhs;
    double js_crt;
    double js_dcrt;
    double js_pfac;
    double js_ddv;
};


#ifdef __STDC__
static void jj_limiting(CKTcircuit*,JJmodel*,JJinstance*,struct jjstuff*);
static void jj_iv(JJmodel*,JJinstance*,struct jjstuff*);
static void jj_ic(JJmodel*,JJinstance*,struct jjstuff*);
static void jj_load(CKTcircuit*,JJmodel*,JJinstance*,struct jjstuff*);
#else
static void jj_limiting();
static void jj_iv();
static void jj_ic();
static void jj_load();
#endif


int
JJload(inModel,ckt)

GENmodel *inModel;
CKTcircuit *ckt;
{
    
    JJmodel *model = (JJmodel *)inModel;
    JJinstance *here;
    struct jjstuff js;
    double absvj;
    double temp;
    int error;  /* used in NI_SUM macro */

    if (ckt->CKTmode & MODEDC)
        return (OK);

    js.js_pfac = ckt->CKTdelta / JJcphi;
    if (ckt->CKTorder > 1)
        js.js_pfac *= .5;

    if (ckt->CKTmode & MODEINITFLOAT) {
        double absold, absdvj, maxvj;

        for ( ; model != NULL; model = model->JJnextModel) {

            for (here = model->JJinstances; here != NULL;
                    here = here->JJnextInstance) {

                js.js_ci  = (here->JJcontrol) ?
                        *(ckt->CKTrhsOld + here->JJbranch) : 0;

                js.js_vj  = *(ckt->CKTrhsOld + here->JJposNode) -
                        *(ckt->CKTrhsOld + here->JJnegNode);

                absvj  = js.js_vj < 0 ? -js.js_vj : js.js_vj;
                temp   = *(ckt->CKTstate0 + here->JJvoltage);
                absold = temp < 0 ? -temp : temp;
                maxvj  = MAX(absvj,absold);
                temp  -= js.js_vj;
                absdvj = temp < 0 ? -temp : temp;

                if (maxvj >= model->JJvless) {
                    temp = model->JJdelv * 0.5;
                    if (absdvj > temp) {
                        js.js_ddv = temp;
                        jj_limiting(ckt,model,here,&js);
                        absvj  = js.js_vj < 0 ? -js.js_vj : js.js_vj;
                        maxvj  = MAX(absvj,absold);
                        temp   = js.js_vj -
                                    *(ckt->CKTstate0 + here->JJvoltage);
                        absdvj = temp < 0 ? -temp : temp;
                    }
                }

                /* check convergence */

                if (!ckt->CKTnoncon) {
                    double tol;

                    tol = ckt->CKTreltol*maxvj + ckt->CKTabstol;
                    if (absdvj > tol) {
                        ckt->CKTnoncon++;
                    }
                }

                js.js_phi = *(ckt->CKTstate1 + here->JJphase);
                temp = js.js_vj;
                if (ckt->CKTorder > 1)
                    temp += *(ckt->CKTstate1 + here->JJvoltage);
                js.js_phi += js.js_pfac*temp;

                js.js_crhs = 0;
                js.js_dcrt = 0;
                js.js_crt  = here->JJcriti;
            
                /*
                 * compute quasiparticle current and derivatives
                 */
                if (model->JJrtype == 1) {

                    if (absvj < model->JJvless)
                        js.js_gqt = here->JJg0;
                    else if (absvj < model->JJvmore) {
                        js.js_gqt = here->JJgs;
                        if (js.js_vj >= 0)
                            js.js_crhs =
                                (here->JJg0 - js.js_gqt)*model->JJvless;
                        else
                            js.js_crhs =
                                (js.js_gqt - here->JJg0)*model->JJvless;
                    }
                    else
                        js.js_gqt = here->JJgn;
                }
                else {
                    jj_iv(model,here,&js);
                }
                if (model->JJictype != 1) {
                    jj_ic(model,here,&js);
                }
                jj_load(ckt,model,here,&js);
            }
        }
        return (OK);
    }

    if (ckt->CKTmode & MODEINITPRED) {
        double y0, y1, y2, y3, rag0, *dtfac;

        rag0  = ckt->CKTorder == 1 ? ckt->CKTdelta : .5*ckt->CKTdelta;
        dtfac = ckt->CKTpred;

        for ( ; model != NULL; model = model->JJnextModel) {

            for (here = model->JJinstances; here != NULL;
                    here = here->JJnextInstance) {

                y1 = *(ckt->CKTstate1 + here->JJdvdt);
                y2 = *(ckt->CKTstate2 + here->JJdvdt);
                y3 = *(ckt->CKTstate3 + here->JJdvdt);

                y0 = y1*dtfac[0] + y2*dtfac[1] + y3*dtfac[2]
                    + *(ckt->CKTstate0 + here->JJdvdt)*dtfac[3];

                temp = *(ckt->CKTstate1 + here->JJvoltage);
                if (ckt->CKTorder != 1)
                    y0 += y1;
                js.js_vj  = temp + rag0*y0;

                js.js_phi = *(ckt->CKTstate1 + here->JJphase);
                temp = js.js_vj;
                if (ckt->CKTorder > 1)
                    temp += *(ckt->CKTstate1 + here->JJvoltage);
                js.js_phi += js.js_pfac*temp;

                if (here->JJcontrol) {

                    y1 = *(ckt->CKTstate1 + here->JJconI);
                    y2 = *(ckt->CKTstate2 + here->JJconI);
                    y3 = *(ckt->CKTstate3 + here->JJconI);

                    js.js_ci = y1*dtfac[0] + y2*dtfac[1] + y3*dtfac[2]
                        + *(ckt->CKTstate0 + here->JJconI)*dtfac[3];
                }
                else {
                    js.js_ci = 0;
                }

                NI_SUM(ckt,here->JJdelVdelT,here->JJvoltage);

                js.js_crhs = 0;
                js.js_dcrt = 0;
                js.js_crt  = here->JJcriti;
            
                if (model->JJrtype == 1) {
                    absvj = js.js_vj < 0 ? -js.js_vj : js.js_vj;

                    if (absvj < model->JJvless)
                        js.js_gqt = here->JJg0;
                    else if (absvj < model->JJvmore) {
                        js.js_gqt = here->JJgs;
                        if (js.js_vj >= 0)
                            js.js_crhs =
                                (here->JJg0 - js.js_gqt)*model->JJvless;
                        else
                            js.js_crhs =
                                (js.js_gqt - here->JJg0)*model->JJvless;
                    }
                    else
                        js.js_gqt = here->JJgn;
                }
                else {
                    jj_iv(model,here,&js);
                }
                if (model->JJictype != 1) {
                    jj_ic(model,here,&js);
                }
                jj_load(ckt,model,here,&js);
            }
        }
        return (OK);
    }

    if (ckt->CKTmode & MODEINITTRAN) {

        for ( ; model != NULL; model = model->JJnextModel) {

            for (here = model->JJinstances; here != NULL;
                    here = here->JJnextInstance) {

                js.js_vj  = here->JJinitVoltage;
                js.js_phi = here->JJinitPhase;
                js.js_ci  = here->JJinitControl;

                *(ckt->CKTstate1 + here->JJvoltage) = js.js_vj;
                *(ckt->CKTstate1 + here->JJphase)   = js.js_phi;
                *(ckt->CKTstate1 + here->JJconI)    = js.js_ci;

                NI_SUM(ckt,here->JJdelVdelT,here->JJvoltage);

                js.js_crhs = 0;
                js.js_dcrt = 0;
                js.js_crt  = here->JJcriti;
            
                if (model->JJrtype == 1) {
                    absvj = js.js_vj < 0 ? -js.js_vj : js.js_vj;

                    if (absvj < model->JJvless)
                        js.js_gqt = here->JJg0;
                    else if (absvj < model->JJvmore) {
                        js.js_gqt = here->JJgs;
                        if (js.js_vj >= 0)
                            js.js_crhs =
                                (here->JJg0 - js.js_gqt)*model->JJvless;
                        else
                            js.js_crhs =
                                (js.js_gqt - here->JJg0)*model->JJvless;
                    }
                    else
                        js.js_gqt = here->JJgn;
                }
                else {
                    jj_iv(model,here,&js);
                }
                if (model->JJictype != 1) {
                    jj_ic(model,here,&js);
                }
                jj_load(ckt,model,here,&js);
            }
        }
    }
    return (OK);
}


static void
jj_limiting(ckt,model,here,js)

CKTcircuit *ckt;
JJmodel *model;
JJinstance *here;
struct jjstuff *js;
{
    double vprev;
    double absold;
    double vtop;
    double vbot;

    /* limit new junction voltage */

    vprev  = *(ckt->CKTstate0 + here->JJvoltage);
    absold = vprev < 0 ? -vprev : vprev;
                                    
    vtop = vprev + js->js_ddv;
    vbot = vprev - js->js_ddv;
    if (absold < model->JJvless) {
        vtop = MAX(vtop,model->JJvless);
        vbot = MIN(vbot,-model->JJvless);
    }
    else if (vprev > model->JJvmore) {
        vtop += 1.0;
        vbot = MIN(vbot,model->JJvmore);
    }
    else if (vprev < -model->JJvmore) {
        vtop = MAX(vtop,-model->JJvmore);
        vbot -= 1.0;
    }
    if (js->js_vj > vtop)
        js->js_vj = vtop;
    else if (js->js_vj < vbot)
        js->js_vj = vbot;
}


static void
jj_iv(model,here,js)

JJmodel *model;
JJinstance *here;
struct jjstuff *js;
{
    double vj;
    double absvj;

    vj    = js->js_vj;
    absvj = vj < 0 ? -vj : vj;

    if (model->JJrtype == 3) {

        if (absvj < model->JJvmore) {
            /* cj   = g0*vj + g1*vj**3 + g2*vj**5,
             * crhs = cj - vj*gqt
             */
            double temp1 = vj*vj;
            double temp2 = temp1*temp1;
            js->js_gqt = here->JJg0 + 3.0*here->JJg1*temp1 +
                        5.0*here->JJg2*temp2;
            temp1  *= vj;
            temp2  *= vj;
            js->js_crhs = -2.0*temp1*here->JJg1 - 4.0*temp2*here->JJg2;
        }
        else
            js->js_gqt = here->JJgn;
    }

    else if (model->JJrtype == 2) {
        double avj, gam, expgam, exngam, xp, xn;

        avj = absvj / model->JJdelv;
        gam = avj - model->JJvg/model->JJdelv;
        if (gam > 30.0) gam = 30.0;
        if (gam < -30.0) gam = -30.0;
        expgam = exp(gam);
        exngam = 1.0 / expgam;
        xp     = 1.0 + expgam;
        xn     = 1.0 + exngam;
        js->js_crhs  = vj*(here->JJg0 + here->JJgn*expgam)/xp;
        js->js_gqt   = here->JJgn*(xn + avj*exngam)/(xn*xn) +
                        here->JJg0*(xp - avj*expgam)/(xp*xp);
        js->js_crhs -= js->js_gqt*vj;
    }

    else if (model->JJrtype == 4) {
        double vless, vmore, vg, gs;
        double temp = 1;

        if (here->JJcontrol)
            temp = js->js_ci < 0 ? -js->js_ci : js->js_ci;
        if (temp > 1) temp = 1;
        js->js_crt *= temp;
        gs = here->JJgs*temp;
        if (gs < here->JJgn)
            gs = here->JJgn;
        vg = model->JJvg*temp;
        temp = .5*model->JJdelv;
        vless = vg - temp;
        vmore = vg + temp;

        if (vless > 0) {
            if (absvj < vless)
                js->js_gqt = here->JJg0;
            else if (absvj < vmore) {
                js->js_gqt = gs;
                if (vj >= 0)
                    js->js_crhs  = (here->JJg0 - js->js_gqt)*vless;
                else
                    js->js_crhs  = (js->js_gqt - here->JJg0)*vless;
            }
            else
                js->js_gqt = here->JJgn;
        }
        else
            js->js_gqt = here->JJgn;

        if (model->JJictype > 1) model->JJictype = 0;
    }
    else {
        js->js_gqt = 0;
    }
}


static void
jj_ic(model,here,js)

/* Nondefault supercurrent
 * For shaped junction types, parameters scale with
 * area as in small junctions, control current does not scale.
 */
JJmodel *model;
JJinstance *here;
struct jjstuff *js;
{
    double ci;
    double temp;

    ci = js->js_ci;

    if (model->JJictype == 2) {

        if (ci != 0.0) {
            double ang, xx = js->js_crt;
            ang  = M_PI * ci / model->JJccsens;
            js->js_crt *= sin(ang)/ang;
            js->js_dcrt = xx*(cos(ang) - js->js_crt)/ci;
        }
    }

    else if (model->JJictype == 3) {

        temp = ci < 0 ? -ci : ci;
        if (temp < model->JJccsens) {
            js->js_dcrt = js->js_crt / model->JJccsens;
            js->js_crt *= (1.0 - temp/model->JJccsens);
            if (ci > 0.0) js->js_dcrt = -js->js_dcrt;
            if (ci == 0.0) js->js_dcrt = 0.0;
        }
        else js->js_crt = 0.0;
    }

    else if (model->JJictype == 4) {

        temp = ci < 0 ? -ci : ci;
        if (temp < model->JJccsens) {
            temp = model->JJccsens + model->JJccsens;
            js->js_dcrt = -js->js_crt / temp;
            js->js_crt *= (model->JJccsens - ci)/temp;
            if (ci == 0.0) js->js_dcrt = 0.0;
        }
        else js->js_crt = 0.0;
    }
    else {
        js->js_crt = 0.0;
    }
}


static void
jj_load(ckt,model,here,js)

CKTcircuit *ckt;
JJmodel *model;
JJinstance *here;
struct jjstuff *js;
{
    double si;
    double gcs;
    double gqt;
    double crhs;
    double crt;
    double temp;

    gqt  = js->js_gqt;
    crhs = js->js_crhs;
    crt  = js->js_crt;

    *(ckt->CKTstate0 + here->JJvoltage) = js->js_vj;
    *(ckt->CKTstate0 + here->JJphase)   = js->js_phi;
    *(ckt->CKTstate0 + here->JJconI)    = js->js_ci;

#ifdef ASM_SINCOS
    asm("fsincos" : "=t" (temp), "=u"  (si) : "0" (js->js_phi));
    /*
    sincos(js->js_phi,&si,&temp);
    */
    gcs   = js->js_pfac*crt*temp;
#else
    gcs   = js->js_pfac*crt*cos(js->js_phi);
    si    = sin(js->js_phi);
#endif

    crt  *= si;
    crhs += crt - gcs*js->js_vj;
    gqt  += gcs + ckt->CKTag[0]*here->JJcap;
    crhs += here->JJdelVdelT*here->JJcap;

    /* load matrix, rhs vector */
    if (here->JJphsNode)
        *(ckt->CKTrhs + here->JJphsNode) = js->js_phi +
            (2*M_PI)* *(int*)(ckt->CKTstate1 + here->JJphsInt);

    if (!here->JJnegNode) {
        *(here->JJposPosPtr) += gqt;
        if (!here->JJcontrol) {
            *(ckt->CKTrhs + here->JJposNode) -= crhs;
        }
        else {
            temp = js->js_dcrt*si;
            *(here->JJposIbrPtr) += temp;
            crhs -= temp*js->js_ci;
            *(ckt->CKTrhs + here->JJposNode) -= crhs;
        }
    }
    else if (!here->JJposNode) {
        *(here->JJnegNegPtr) += gqt;
        if (!here->JJcontrol) {
            *(ckt->CKTrhs + here->JJnegNode) += crhs;
        }
        else {
            temp = js->js_dcrt*si;
            *(here->JJnegIbrPtr) -= temp;
            crhs -= temp*js->js_ci;
            *(ckt->CKTrhs + here->JJnegNode) += crhs;
        }
    }
    else {
        *(here->JJposPosPtr) += gqt;
        *(here->JJposNegPtr) -= gqt;
        *(here->JJnegPosPtr) -= gqt;
        *(here->JJnegNegPtr) += gqt;
        if (!here->JJcontrol) {
            *(ckt->CKTrhs + here->JJposNode) -= crhs;
            *(ckt->CKTrhs + here->JJnegNode) += crhs;
        }
        else {
            temp = js->js_dcrt*si;
            *(here->JJposIbrPtr) += temp;
            *(here->JJnegIbrPtr) -= temp;
            crhs -= temp*js->js_ci;
            *(ckt->CKTrhs + here->JJposNode) -= crhs;
            *(ckt->CKTrhs + here->JJnegNode) += crhs;
        }
    }
}
