/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1987 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/


    /* support routines for device models */

#include "spice.h"
#include "devdefs.h"
#include "optdefs.h"
#include <math.h>
#include "util.h"
#include "cktext.h"



double
DEVlimvds(vnew,vold)

/* DEVlimvds(vnew,vold)
 *  limit the per-iteration change of VDS
 */
double vnew;
double vold;
{

    if (vold >= 3.5) {
        if(vnew > vold) {
            vnew = MIN(vnew,(3 * vold) +2);
        }
        else {
            if (vnew < 3.5) {
                vnew = MAX(vnew,2);
            }
        }
    }
    else {
        if(vnew > vold) {
            vnew = MIN(vnew,4);
        }
        else {
            vnew = MAX(vnew,-.5);
        }
    }
    return (vnew);
}


double
DEVpnjlim(vnew,vold,vt,vcrit,icheck)

/* DEVpnjlim(vnew,vold,vt,vcrit,icheck)
 * limit the per-iteration change of PN junction  voltages 
 */
double vnew;
double vold;
double vt;
double vcrit;
int *icheck;
{
    double arg;

    if ((vnew > vcrit) && (FABS(vnew - vold) > (vt + vt))) {
        if(vold > 0) {
            arg = 1 + (vnew - vold) / vt;
            if(arg > 0) {
                vnew = vold + vt * log(arg);
            }
            else {
                vnew = vcrit;
            }
        }
        else {
            vnew = vt *log(vnew/vt);
        }
        *icheck = 1;
    }
    else {
        *icheck = 0;
    }
    return (vnew);
}


double
DEVfetlim(vnew,vold,vto)

/*
 * DEVfetlim(vnew,vold.vto)
 *
 * limit the per-iteration change of FET voltages 
 */
double vnew;
double vold;
double vto;
{
    double vtsthi;
    double vtstlo;
    double vtox;
    double delv;
    double vtemp;

    vtsthi = FABS(2*(vold-vto))+2;
    vtstlo = vtsthi/2 +2;
    vtox = vto + 3.5;
    delv = vnew-vold;

    if (vold >= vto) {
        if (vold >= vtox) {
            if (delv <= 0) {
                /* going off */
                if (vnew >= vtox) {
                    if (-delv >vtstlo) {
                        vnew =  vold - vtstlo;
                    }
                }
                else {
                    vnew = MAX(vnew,vto+2);
                }
            }
            else {
                /* staying on */
                if (delv >= vtsthi) {
                    vnew = vold + vtsthi;
                }
            }
        }
        else {
            /* middle region */
            if (delv <= 0) {
                /* decreasing */
                vnew = MAX(vnew,vto-.5);
            }
            else {
                /* increasing */
                vnew = MIN(vnew,vto+4);
            }
        }
    }
    else {
        /* off */
        if (delv <= 0) {
            if (-delv >vtsthi) {
                vnew = vold - vtsthi;
            } 
        }
        else {
            vtemp = vto + .5;
            if (vnew <= vtemp) {
                if (delv >vtstlo) {
                    vnew = vold + vtstlo;
                }
            }
            else {
                vnew = vtemp;
            }
        }
    }
    return (vnew);
}


void
DEVqmeyer(vgs,vgd,von,vdsat,capgs,capgd,capgb,phi,cox)

/*
 * DEVqmeyer(vgs,vgd,vgb,von,vdsat,capgs,capgd,capgb,phi,cox)
 *           
 *  Compute the MOS overlap capacitances as functions of the 
 *  device terminal voltages
 */
double vgs;    /* initial voltage gate-source */
double vgd;    /* initial voltage gate-drain */
double von;
double vdsat;
double *capgs;  /* non-constant portion of g-s overlap capacitance */
double *capgd;  /* non-constant portion of g-d overlap capacitance */
double *capgb;  /* non-constant portion of g-b overlap capacitance */
double phi;
double cox;     /* oxide capactiance */
{
    double vds;
    double vddif;
    double vddif1;
    double vddif2;
    double vgst;

    vgst = vgs-von;
    if (vgst <= -phi) {
        *capgb = cox/2;
        *capgs = 0;
        *capgd = 0;
    }
    else if (vgst <= -phi/2) {
        *capgb = -vgst*cox/(2*phi);
        *capgs = 0;
        *capgd = 0;
    }
    else if (vgst <= 0) {
        *capgb = -vgst*cox/(2*phi);
        *capgs = vgst*cox/(1.5*phi)+cox/3;
        *capgd = 0;
    }
    else  {
        vds = vgs-vgd;
        if (vdsat <= vds) {
            *capgs = cox/3;
            *capgd = 0;
            *capgb = 0;
        }
        else {
            vddif = 2.0*vdsat-vds;
            vddif1 = vdsat-vds/*-1.0e-12*/;
            vddif2 = vddif*vddif;
            *capgd = cox*(1.0-vdsat*vdsat/vddif2)/3;
            *capgs = cox*(1.0-vddif1*vddif1/vddif2)/3;
            *capgb = 0;
        }
    }
}


double DEVpred(ckt,loct)

/* predict a value for the capacitor at loct by
 * extrapolating from previous values
 */
CKTcircuit *ckt;
int loct;
{

#ifndef NEWTRUNC
    double xfact;
    xfact = ckt->CKTdelta/ckt->CKTdeltaOld[1];
    return( ( (1+xfact) * *(ckt->CKTstate1+loct) ) -
            (    xfact  * *(ckt->CKTstate2+loct) )  );
#endif /*NEWTRUNC*/

}

/************************************************************************
 * Routine added by S. R. Whiteley.
 ************************************************************************/

struct sCKTtable **
DEVtableHead(ckt)

GENERIC *ckt;
{
    return (&((CKTcircuit*)ckt)->CKTtableHead);
}


double *
DEVpredNew(ckt)

/* Compute the Lagrange factors for extrapolation to new time point.
 * Sets ckt->CKTpred[4] and returns pointer: diff = DEVpredNew(ckt).
 * Use: predicted = diff[0]*state1 + diff[1]*state2 + diff[2]*state3
 *                  + dif[3]*state0.
 */

CKTcircuit *ckt;
{
    double *diff;
    double d1,d2,d3,d4,c;
    STATistics *stat = (STATistics*)ckt->CKTstat;

    if (ckt == NULL)
        return (NULL);

    diff = ckt->CKTpred; /* double CKTpred[4] */
    d1 = ckt->CKTdelta;

    if (stat->STATaccepted >= 4) {

        /* third order extrapolation factors */

        double c1,c2,c3,c12,c23,c123,dd1,dd2;

        d1 = 1/d1;
        c1 = ckt->CKTdeltaOld[1]*d1;
        c2 = ckt->CKTdeltaOld[2]*d1;
        c3 = ckt->CKTdeltaOld[3]*d1;
        c12 = c1 + c2;
        c23 = c2 + c3;
        c123 = c12 + c3;

        d2 = 1 + c1;
        d3 = 1 + c12;
        d4 = 1 + c123;

        c  = c1*c2*c3*c12*c23*c123;
        c = 1/c;

        dd1 = d3*d4*c*c3;
        dd2 = d2*c*c1;

        diff[0] =  dd1 * d2 * c2  * c23;
        diff[1] = -dd1      * c12 * c123;
        diff[2] =  dd2 * d4 * c23 * c123;
        diff[3] = -dd2 * d3 * c2  * c12;

        /***************************************
         * i486 cpu cycles: * 16, / 73, +- 10. *
         *                                     *
         * expr (1): 24 *, 2 /, 6 +  (above)   *
         *  total: 590                         *
         * expr (2): 16 *, 5 /, 6 +            *
         *  total: 681                         *
         * expr (3): 15 *, 4 /, 12 +           *
         *  total: 652                         *
         *                                     *
         * Note that 1,2 better conditioned.   *
         ***************************************/

        /* expr (2)
        diff[0] =  d2*d3*d4/(( c1  )*( c12 )*( c123));
        diff[1] = -d3*d4   /(( c1  )*( c2  )*( c23 ));
        diff[2] =  d2*d4   /(( c12 )*( c2  )*( c3  ));
        diff[3] = -d2*d3   /(( c123)*( c23 )*( c3  ));
        */

        /* expr (3)
        d2 = d1 + ckt->CKTdeltaOld[1];
        d3 = d2 + ckt->CKTdeltaOld[2];
        d4 = d3 + ckt->CKTdeltaOld[3];

        c  = -d1*d2*d3*d4;
        diff[0] = c/((d1-d2)*(d1-d3)*(d1-d4)*d1);
        diff[1] = c/((d2-d1)*(d2-d3)*(d2-d4)*d2);
        diff[2] = c/((d3-d1)*(d3-d2)*(d3-d4)*d3);
        diff[3] = c/((d4-d1)*(d4-d2)*(d4-d3)*d4);
        */

    }
    else
        if (stat->STATaccepted >= 3) {

        /* second order extrapolation factors */
        d2 = d1 + ckt->CKTdeltaOld[1];
        d3 = d2 + ckt->CKTdeltaOld[2];

        diff[0] = d2*d3/((d1-d2)*(d1-d3));
        diff[1] = d1*d3/((d2-d1)*(d2-d3));
        diff[2] = d1*d2/((d3-d1)*(d3-d2));
        diff[3] = 0;
    }

    else {
        /* first order extrapolation factors */
        diff[1] = -d1/ckt->CKTdeltaOld[1];
        diff[0] = 1 - diff[1];
        diff[2] = diff[3] = 0;

    }
    return (diff);
}
