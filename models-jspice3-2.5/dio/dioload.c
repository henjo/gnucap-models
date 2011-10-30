/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "diodefs.h"
#include "const.h"
#include "util.h"
#include "sperror.h"
#include "niext.h"


#ifdef __STDC__
static int  dio_bypass(CKTcircuit*,DIOmodel*,DIOinstance*);
static int  dio_limiting(CKTcircuit*,DIOmodel*,DIOinstance*);
static int  dio_pnjlim(double*,double,double,double);
static void dio_iv(CKTcircuit*,DIOmodel*,DIOinstance*);
static void dio_cap(CKTcircuit*,DIOmodel*,DIOinstance*);
static void dio_load(CKTcircuit*,DIOmodel*,DIOinstance*);
#else
static int  dio_bypass();
static int  dio_limiting();
static void dio_iv();
static void dio_cap();
static void dio_load();
#endif


int
DIOload(inModel,ckt)

/* actually load the current stamp into the 
 * sparse matrix previously provided 
 */
GENmodel *inModel;
CKTcircuit *ckt;
{
    DIOmodel *model = (DIOmodel*)inModel;
    DIOinstance *here;
    double ceq;
    double geq;
    int error;

    if (ckt->CKTmode & MODEINITFLOAT) {

        /* loop through all the diode models */
        for ( ; model != NULL; model = model->DIOnextModel) {

            /* loop through all the instances of the model */
            for (here = model->DIOinstances; here != NULL;
                    here = here->DIOnextInstance) {

                here->DIOvd = *(ckt->CKTrhsOld + here->DIOposPrimeNode) -
                        *(ckt->CKTrhsOld + here->DIOnegNode);

                /*  
                 *   bypass if solution has not changed
                 */
                if (ckt->CKTbypass && dio_bypass(ckt,model,here))
                    continue;

                (void)dio_limiting(ckt,model,here);

                dio_iv(ckt,model,here);

                if (ckt->CKTmode & MODETRAN) {
                    dio_cap(ckt,model,here);
                    /*
                     *   transient analysis
                     */
                    NI_INTEG(ckt,geq,ceq,here->DIOcap,here->DIOcapCharge);
                    here->DIOgd += geq;
                    here->DIOcd += *(ckt->CKTstate0 + here->DIOcapCurrent);
                }
                dio_load(ckt,model,here);
            }
        }
        return (OK);
    }

    if (ckt->CKTmode & MODEINITPRED) {
        double xf1;
        double xf2;

        xf2 = -ckt->CKTdelta/ckt->CKTdeltaOld[1];
        xf1 = 1 - xf2;

        /* loop through all the diode models */
        for ( ; model != NULL; model = model->DIOnextModel) {

            /* loop through all the instances of the model */
            for (here = model->DIOinstances; here != NULL;
                    here = here->DIOnextInstance) {

                here->DIOvd = xf1* *(ckt->CKTstate1 + here->DIOvoltage)
                        + xf2* *(ckt->CKTstate2 + here->DIOvoltage);

                *(ckt->CKTstate0 + here->DIOvoltage) =
                    *(ckt->CKTstate1 + here->DIOvoltage);

                (void)dio_limiting(ckt,model,here);
                dio_iv(ckt,model,here);
                if (ckt->CKTmode & MODETRAN) {
                    dio_cap(ckt,model,here);
                    NI_INTEG(ckt,geq,ceq,here->DIOcap,here->DIOcapCharge);
                    here->DIOgd += geq;
                    here->DIOcd += *(ckt->CKTstate0 + here->DIOcapCurrent);
                }
                dio_load(ckt,model,here);
            }
        }
        return (OK);
    }

    if (ckt->CKTmode & MODEINITFIX) {

        /* loop through all the diode models */
        for ( ; model != NULL; model = model->DIOnextModel) {

            /* loop through all the instances of the model */
            for (here = model->DIOinstances; here != NULL;
                    here = here->DIOnextInstance) {

                if (here->DIOoff)
                    here->DIOvd = 0;
                else {
                    here->DIOvd = *(ckt->CKTrhsOld + here->DIOposPrimeNode) -
                        *(ckt->CKTrhsOld + here->DIOnegNode);

                    if (dio_limiting(ckt,model,here))
                        ckt->CKTnoncon++;
                }
                dio_iv(ckt,model,here);
                dio_load(ckt,model,here);
            }
        }
        return (OK);
    }

    if (ckt->CKTmode & MODEINITTRAN) {

        /* loop through all the diode models */
        for ( ; model != NULL; model = model->DIOnextModel) {

            /* loop through all the instances of the model */
            for (here = model->DIOinstances; here != NULL;
                    here = here->DIOnextInstance) {

                here->DIOvd = *(ckt->CKTstate1 + here->DIOvoltage);

                dio_iv(ckt,model,here);
                dio_cap(ckt,model,here);
                *(ckt->CKTstate1 + here->DIOcapCharge) = 
                    *(ckt->CKTstate0 + here->DIOcapCharge);
                NI_INTEG(ckt,geq,ceq,here->DIOcap,here->DIOcapCharge);
                here->DIOgd += geq;
                here->DIOcd += *(ckt->CKTstate0 + here->DIOcapCurrent);
                dio_load(ckt,model,here);
            }
        }
        return (OK);
    }

    if (ckt->CKTmode & MODEINITSMSIG) {

        /* loop through all the diode models */
        for ( ; model != NULL; model = model->DIOnextModel) {

            /* loop through all the instances of the model */
            for (here = model->DIOinstances; here != NULL;
                    here = here->DIOnextInstance) {

                here->DIOvd = *(ckt->CKTstate0 + here->DIOvoltage);
                dio_iv(ckt,model,here);
                dio_cap(ckt,model,here);
            }
        }
        return (OK);
    }

    if ((ckt->CKTmode & MODEINITJCT) && 
            (ckt->CKTmode & MODETRANOP) && (ckt->CKTmode & MODEUIC)) {

        /* loop through all the diode models */
        for ( ; model != NULL; model = model->DIOnextModel) {

            /* loop through all the instances of the model */
            for (here = model->DIOinstances; here != NULL;
                    here = here->DIOnextInstance) {

                *(ckt->CKTstate0 + here->DIOvoltage) = here->DIOinitCond;
            }
        }
        return (OK);
    }

    if (ckt->CKTmode & MODEINITJCT) {

        /* loop through all the diode models */
        for ( ; model != NULL; model = model->DIOnextModel) {

            /* loop through all the instances of the model */
            for (here = model->DIOinstances; here != NULL;
                    here = here->DIOnextInstance) {

                if (here->DIOoff)
                    here->DIOvd = 0;
                else
                    here->DIOvd = here->DIOtVcrit;
                dio_iv(ckt,model,here);
                dio_load(ckt,model,here);
            }
        }
    }
    return (OK);
}


static int
dio_bypass(ckt,model,here)

CKTcircuit *ckt;
DIOmodel *model;
DIOinstance *here;
{
    double delvd, cdhat, tol, A1, A2;

    delvd = here->DIOvd - *(ckt->CKTstate0 + here->DIOvoltage);

    A1  = FABS(here->DIOvd);
    A2  = FABS(*(ckt->CKTstate0 + here->DIOvoltage));
    tol = ckt->CKTvoltTol + ckt->CKTreltol*MAX(A1,A2);

    if (FABS(delvd) < tol) {
        cdhat = here->DIOcd + here->DIOgd*delvd;
        A1 = FABS(cdhat);
        A2 = FABS(here->DIOcd);
        tol = ckt->CKTabstol + ckt->CKTreltol*MAX(A1,A2);
        A1 = cdhat - here->DIOcd;
        if (FABS(A1) < tol) {
            here->DIOvd = *(ckt->CKTstate0 + here->DIOvoltage);
            dio_load(ckt,model,here);
            return (1);
        }
    }
    return (0);
}


static int
dio_limiting(ckt,model,here)

CKTcircuit *ckt;
DIOmodel *model;
DIOinstance *here;
{
    double temp;
    int rt;

    if (model->DIObreakdownVoltageGiven) {
        temp = 10*here->DIOvte - here->DIOtBrkdwnV;
        if (here->DIOvd < MIN(0,temp)) {

            temp = -(here->DIOvd + here->DIOtBrkdwnV);
            rt = dio_pnjlim(&temp,
                -(*(ckt->CKTstate0 + here->DIOvoltage) + here->DIOtBrkdwnV),
                here->DIOvte,here->DIOtVcrit);
            here->DIOvd = -(temp + here->DIOtBrkdwnV);
            return (rt);
        }
    }
    return (dio_pnjlim(&here->DIOvd,*(ckt->CKTstate0 + here->DIOvoltage),
                here->DIOvte,here->DIOtVcrit));
    
}


static int
dio_pnjlim(vnew,vold,vt,vcrit)

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
dio_iv(ckt,model,here)

/*
 *   compute dc current and derivitives
 */
CKTcircuit *ckt;
DIOmodel *model;
DIOinstance *here;
{
    double arg;
    double vte;
    double vd;
    double csat;

    vte  = here->DIOvte;
    csat = here->DIOtSatCur;
    vd   = here->DIOvd;

    if (vd >= -3*vte) {
        arg = exp(vd/vte);
        here->DIOcd = csat*(arg-1) + ckt->CKTgmin*vd;
        here->DIOgd = csat*arg/vte + ckt->CKTgmin;
    }
    else if ((!(here->DIOtBrkdwnV)) || vd >= -here->DIOtBrkdwnV) {
        arg = 3*vte/(vd*CONSTe);
        arg = arg * arg * arg;
        here->DIOcd = -csat*(1+arg) + ckt->CKTgmin*vd;
        here->DIOgd = csat*3*arg/vd + ckt->CKTgmin;
    }
    else {
        arg = exp(-(here->DIOtBrkdwnV + vd)/vte);
        here->DIOcd = -csat*arg + ckt->CKTgmin*vd;
        here->DIOgd = csat*arg/vte + ckt->CKTgmin;
    }
}


static void
dio_cap(ckt,model,here)

/*
 *   charge storage elements
 */
CKTcircuit *ckt;
DIOmodel *model;
DIOinstance *here;
{
    double czero;
    double czof2;
    double arg;
    double sarg;
    double vd;

    vd = here->DIOvd;
    czero = here->DIOtJctCap*here->DIOarea;

    if (vd < here->DIOtDepCap) {
        arg = 1 - vd/model->DIOjunctionPot;

        if (model->DIOgradingCoeff == .5)
            sarg = 1/sqrt(arg);
        else
            sarg = exp(-model->DIOgradingCoeff*log(arg));

        *(ckt->CKTstate0 + here->DIOcapCharge) = 
                model->DIOtransitTime*here->DIOcd +
                model->DIOjunctionPot*czero*(1 - arg*sarg)/
                    (1 - model->DIOgradingCoeff);
        here->DIOcap = 
                model->DIOtransitTime*here->DIOgd + czero*sarg;
    }
    else {
        czof2 = czero/model->DIOf2;
        *(ckt->CKTstate0 + here->DIOcapCharge) = 
                model->DIOtransitTime*here->DIOcd + czero*here->DIOtF1 +
                czof2*(model->DIOf3*(vd - here->DIOtDepCap) +
                (model->DIOgradingCoeff/
                    (model->DIOjunctionPot + model->DIOjunctionPot))*
                        (vd*vd - here->DIOtDepCap*here->DIOtDepCap));
        here->DIOcap = 
                model->DIOtransitTime*here->DIOgd +
                czof2*(model->DIOf3 +
                    model->DIOgradingCoeff*vd/model->DIOjunctionPot);
    }
}


static void
dio_load(ckt,model,here)

/*
 *   load matrix, rhs, and state
 */
CKTcircuit *ckt;
DIOmodel *model;
DIOinstance *here;
{
    double gspr;
    double cdeq;
    double gd;

    gd = here->DIOgd;

    *(ckt->CKTstate0 + here->DIOvoltage) = here->DIOvd;

    gspr = model->DIOconductance*here->DIOarea;
    cdeq = here->DIOcd - gd*here->DIOvd;

    *(ckt->CKTrhs + here->DIOnegNode)      += cdeq;
    *(ckt->CKTrhs + here->DIOposPrimeNode) -= cdeq;

    *(here->DIOposPosPtr)           += gspr;
    *(here->DIOnegNegPtr)           += gd;
    *(here->DIOposPrimePosPrimePtr) += gd + gspr;
    *(here->DIOposPosPrimePtr)      -= gspr;
    *(here->DIOnegPosPrimePtr)      -= gd;
    *(here->DIOposPrimePosPtr)      -= gspr;
    *(here->DIOposPrimeNegPtr)      -= gd;
}
