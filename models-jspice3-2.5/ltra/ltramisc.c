/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1990 Jaijeet S. Roychowdhury
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <math.h>
#include "ltradefs.h"
#include "util.h"

#ifndef M_PI
#define M_PI    3.14159265
#endif


#ifdef __STDC__
static double intlinfunc(double,double,double,double,double,double);
static double twiceintlinfunc(double,double,double,double,double,double,double);
static double thriceintlinfunc(double,double,double,double,double,double,double,double);
static double bessI0(double);
static double bessI1(double);
static double bessXX(double);
static double bessI1xOverX(double);
static double bessYY(double, double);
static double bessZZ(double, double);
static double ltra_rlcH1dashTwiceIntFunc(double,double);
static double ltra_rlcH3dashIntFunc(double,double,double);
static double ltra_rcH1dashTwiceIntFunc(double,double);
static double ltra_rcH2TwiceIntFunc(double,double);
#else
static double intlinfunc();
static double twiceintlinfunc();
static double thriceintlinfunc();
static double thriceintlinfunc();
static double bessI0();
static double bessI1();
static double bessXX();
static double bessI1xOverX();
static double bessYY();
static double bessZZ();
static double ltra_rlcH1dashTwiceIntFunc();
static double ltra_rlcH3dashIntFunc();
static double ltra_rcH1dashTwiceIntFunc();
static double ltra_rcH2TwiceIntFunc();
#endif

/*
 * Miscellaneous functions to do with lossy lines
 */


/*
 * LTRArcCoeffsSetup sets up the all coefficient lists for 
 * the special case where L=G=0
 */

void
LTRArcCoeffsSetup(ckt,InModel)

CKTcircuit *ckt;
GENmodel *InModel;
{
    LTRAmodel *model = (LTRAmodel*)InModel;
    double delta1, delta2;
    double h1dummy1, h1dummy2;
    double h2dummy1, h2dummy2;
    double h3dummy1, h3dummy2;
    double lolimit1, lolimit2, hilimit1, hilimit2;
    double h1lovalue1, h1lovalue2, h1hivalue1, h1hivalue2;
    double h2lovalue1, h2lovalue2, h2hivalue1, h2hivalue2;
    double h3lovalue1, h3lovalue2, h3hivalue1, h3hivalue2;
    double temp, temp2, temp3, temp4, temp5;
    double h1relval, h2relval, h3relval;
    int doh1 = 1, doh2 = 1, doh3 = 1;
    int i, auxindex;

    auxindex = ckt->CKTtimeIndex;

    /* the first coefficients */

    delta1   = ckt->CKTtime - *(ckt->CKTtimePoints + auxindex);
    lolimit1 = 0.0;
    hilimit1 = delta1;

    h1lovalue1 = 0.0;
    h1hivalue1 = sqrt(4*model->LTRAcByR*hilimit1/M_PI);
    h1dummy1   = h1hivalue1/delta1;
    h1relval   = FABS(h1dummy1*model->LTRAchopReltol);
    model->LTRAh1dashFirstCoeff = h1dummy1;

    temp  = model->LTRArclsqr/(4*hilimit1);
    temp2 = (temp >= 100.0 ? 0.0 : erfc(sqrt(temp)));
    temp3 = exp(-temp);
    temp4 = sqrt(model->LTRArclsqr);
    temp5 = sqrt(model->LTRAcByR);

    h2lovalue1 = 0.0;
    if (hilimit1 != 0.0)
        h2hivalue1 = (hilimit1 + model->LTRArclsqr*0.5)*temp2 -
            sqrt(hilimit1*model->LTRArclsqr/M_PI)*temp3;
    else
        h2hivalue1 = 0.0;
        
    h2dummy1 = h2hivalue1/delta1;
    h2relval = FABS(h2dummy1*model->LTRAchopReltol);
    model->LTRAh2FirstCoeff = h2dummy1;

    h3lovalue1 = 0.0;
    if (hilimit1 != 0.0) {
        temp = 2*sqrt(hilimit1/M_PI)*temp3 - temp4*temp2;
        h3hivalue1 = temp5*temp;
    }
    else
        h3hivalue1 = 0.0;

    h3dummy1 = h3hivalue1/delta1;
    h3relval = FABS(h3dummy1*model->LTRAchopReltol);
    model->LTRAh3dashFirstCoeff = h3dummy1;

    /* the coefficients for the rest of the timepoints */

    for (i = auxindex; i > 0; i--) {

        delta2   = delta1;   /* previous delta1 */
        lolimit2 = lolimit1; /* previous lolimit1 */
        hilimit2 = hilimit1; /* previous hilimit1 */

        delta1   = *(ckt->CKTtimePoints + i) - *(ckt->CKTtimePoints + i - 1);
        lolimit1 = hilimit2;
        hilimit1 = ckt->CKTtime - *(ckt->CKTtimePoints + i - 1);

        if (doh1) {
            h1lovalue2 = h1lovalue1; /* previous lovalue1 */
            h1hivalue2 = h1hivalue1; /* previous hivalue1 */
            h1dummy2   = h1dummy1;   /* previous dummy1 */

            h1lovalue1 = h1hivalue2;
            h1hivalue1 = sqrt(4*model->LTRAcByR*hilimit1/M_PI);
            h1dummy1 = (h1hivalue1 - h1lovalue1)/delta1;
            *(model->LTRAh1dashCoeffs + i) = h1dummy1 - h1dummy2;
            if (FABS(*(model->LTRAh1dashCoeffs + i)) < h1relval) doh1 = 0;
        }
        else
            *(model->LTRAh1dashCoeffs + i) = 0.0;

        if (doh2 || doh3) {
            temp  = model->LTRArclsqr/(4*hilimit1);
            temp2 = (temp >= 100.0 ? 0.0 : erfc(sqrt(temp)));
            temp3 = exp(-temp);
        }

        if (doh2) {
            h2lovalue2 = h2lovalue1; /* previous lovalue1 */
            h2hivalue2 = h2hivalue1; /* previous hivalue1 */
            h2dummy2   = h2dummy1;   /* previous dummy1 */

            h2lovalue1 = h2hivalue2;
            if (hilimit1 != 0.0)
                h2hivalue1 = (hilimit1 + model->LTRArclsqr*0.5)*temp2 -
                    sqrt(hilimit1*model->LTRArclsqr/M_PI)*temp3;
            else
                h2hivalue1 = 0.0;
            h2dummy1 = (h2hivalue1 - h2lovalue1)/delta1;
            *(model->LTRAh2Coeffs + i) = h2dummy1 - h2dummy2;
            if (FABS(*(model->LTRAh2Coeffs + i)) < h2relval) doh2 = 0;
        }
        else
            *(model->LTRAh2Coeffs + i) = 0.0;

        if (doh3) {
            h3lovalue2 = h3lovalue1; /* previous lovalue1 */
            h3hivalue2 = h3hivalue1; /* previous hivalue1 */
            h3dummy2   = h3dummy1;   /* previous dummy1 */

            h3lovalue1 = h3hivalue2;
            if (hilimit1 != 0.0) {
                temp = 2*sqrt(hilimit1/M_PI)*temp3 - temp4*temp2;
                h3hivalue1 = temp5*temp;
            }
            else
                h3hivalue1 = 0.0;
            h3dummy1 = (h3hivalue1 - h3lovalue1)/delta1;
            *(model->LTRAh3dashCoeffs + i) = h3dummy1 - h3dummy2;
            if (FABS(*(model->LTRAh3dashCoeffs + i)) < h3relval) doh3 = 0;
        }
        else
            *(model->LTRAh3dashCoeffs + i) = 0.0;
    }
}


void
LTRArlcCoeffsSetup(ckt,InModel)

CKTcircuit *ckt;
GENmodel *InModel;
{
    LTRAmodel *model = (LTRAmodel*)InModel;
    unsigned exact;
    double lolimit1, lolimit2, hilimit1, hilimit2;
    double delta1, delta2;
    double h1dummy1, h1dummy2;
    double h1lovalue1, h1lovalue2, h1hivalue1, h1hivalue2;
    double h2dummy1, h2dummy2;
    double h2lovalue1, h2lovalue2, h2hivalue1, h2hivalue2;
    double h3dummy1, h3dummy2;
    double h3lovalue1, h3lovalue2, h3hivalue1, h3hivalue2;
    double exparg, besselarg, bessi1overxterm, bessi0term;
    double expbetaTterm, alphasqTterm;
    double h1relval, h2relval, h3relval;
    int doh1 = 1, doh2 = 1, doh3 = 1;
    int i, auxindex;

    /*
     * we assume a piecewise linear function, and we calculate the
     * coefficients using this assumption in the integration of the
     * function
     */

    if (model->LTRAtd == 0.0) {
        auxindex = ckt->CKTtimeIndex;
    }
    else {

        if (ckt->CKTtime - model->LTRAtd <= 0.0) {
            auxindex = 0;
        }
        else {
            exact = 0;
            for (i = ckt->CKTtimeIndex; i >= 0; i--) {
                if (ckt->CKTtime - *(ckt->CKTtimePoints + i) ==
                        model->LTRAtd) {
                    exact = 1;
                    break;
                } 
                if (ckt->CKTtime - *(ckt->CKTtimePoints + i) >
                    model->LTRAtd) break;
            }

#ifdef LTRADEBUG
            if ((i < 0) || ((i == 0) && (exact == 1)))
                printf("LTRAcoeffSetup: i <= 0: some mistake!\n");
#endif

            if (exact == 1) {
                auxindex = i-1;
            }
            else {
                auxindex = i;
            }
        }
    }
    /* the first coefficient */

    if (auxindex != 0) {
        lolimit1 = model->LTRAtd;
        hilimit1 = ckt->CKTtime - *(ckt->CKTtimePoints + auxindex);
        delta1   = hilimit1 - lolimit1;

        h2lovalue1 = LTRArlcH2Func(model->LTRAtd,(GENmodel*)model);
        besselarg  = (hilimit1 > model->LTRAtd) ?
            model->LTRAalpha*sqrt(hilimit1*hilimit1 -
            model->LTRAtd*model->LTRAtd) : 0.0;
        exparg  = -model->LTRAbeta*hilimit1;
        bessi1overxterm = bessYY(besselarg, exparg);
        alphasqTterm = model->LTRAalpha*model->LTRAalpha*model->LTRAtd;
        h2hivalue1 =
            ((model->LTRAalpha == 0.0) || (hilimit1 < model->LTRAtd)) ? 0.0:
            alphasqTterm*bessi1overxterm;

        h2dummy1 = twiceintlinfunc(lolimit1,hilimit1,lolimit1,h2lovalue1,
            h2hivalue1,lolimit1,hilimit1)/delta1;
        model->LTRAh2FirstCoeff = h2dummy1;
        h2relval = FABS(model->LTRAchopReltol*h2dummy1);

        h3lovalue1 = 0.0; /* E3dash should be consistent with this */
        bessi0term = bessZZ(besselarg, exparg);
        expbetaTterm = exp(-model->LTRAbeta*model->LTRAtd);
        h3hivalue1 =
            ((hilimit1 <= model->LTRAtd) || (model->LTRAbeta == 0.0)) ? 0.0:
            bessi0term - expbetaTterm;
        h3dummy1 = intlinfunc(lolimit1,hilimit1,h3lovalue1,
            h3hivalue1,lolimit1,hilimit1)/delta1;
        model->LTRAh3dashFirstCoeff = h3dummy1;
        h3relval = FABS(h3dummy1*model->LTRAchopReltol);
    }
    else {
        model->LTRAh2FirstCoeff = model->LTRAh3dashFirstCoeff = 0.0;
    }

    lolimit1 = 0.0;
    hilimit1 = ckt->CKTtime - *(ckt->CKTtimePoints + ckt->CKTtimeIndex);
    delta1 = hilimit1 - lolimit1;
    exparg = -model->LTRAbeta*hilimit1;

    h1lovalue1 = 0.0;
    h1hivalue1 = (model->LTRAbeta == 0.0)? hilimit1:
        ((hilimit1 == 0.0) ? 0.0 : bessXX(-exparg)*hilimit1 - hilimit1);
    h1dummy1 = h1hivalue1/delta1;
    model->LTRAh1dashFirstCoeff = h1dummy1;
    h1relval = FABS(h1dummy1*model->LTRAchopReltol);


    /* the coefficients for the rest of the timepoints */

    for (i = ckt->CKTtimeIndex; i > 0; i--) {

        if (doh1 || doh2 || doh3) {
            lolimit2 = lolimit1; /* previous lolimit1 */
            hilimit2 = hilimit1; /* previous hilimit1 */
            delta2   = delta1;   /* previous delta1 */

            lolimit1 = hilimit2;
            hilimit1 = ckt->CKTtime - *(ckt->CKTtimePoints + i - 1);
            delta1   =
                *(ckt->CKTtimePoints + i) - *(ckt->CKTtimePoints + i - 1);

            exparg = -model->LTRAbeta*hilimit1;
        }

        if (doh1) {
            h1lovalue2 = h1lovalue1; /* previous lovalue1 */
            h1hivalue2 = h1hivalue1; /* previous hivalue1 */
            h1dummy2   = h1dummy1;   /* previous dummy1 */

            h1lovalue1 = h1hivalue2;
            h1hivalue1 = (model->LTRAbeta == 0.0)? hilimit1:
                ((hilimit1 == 0.0) ? 0.0:
            bessXX(-exparg)*hilimit1 - hilimit1);
            h1dummy1 = (h1hivalue1 - h1lovalue1)/delta1;

            *(model->LTRAh1dashCoeffs + i) = h1dummy1 - h1dummy2;
            if (FABS(*(model->LTRAh1dashCoeffs + i)) <= h1relval) doh1 = 0;
        }
        else
            *(model->LTRAh1dashCoeffs + i) = 0.0;

        if (i <= auxindex) {
    
            if (doh2 || doh3)
                besselarg = (hilimit1 > model->LTRAtd) ?
                    model->LTRAalpha*sqrt(hilimit1*hilimit1 -
                    model->LTRAtd*model->LTRAtd) : 0.0;

            if (doh2) {
                h2lovalue2 = h2lovalue1; /* previous lovalue1 */
                h2hivalue2 = h2hivalue1; /* previous hivalue1 */
                h2dummy2   = h2dummy1;   /* previous dummy1 */

                h2lovalue1 = h2hivalue2;
                bessi1overxterm = bessYY(besselarg, exparg);
                h2hivalue1 =
                    ((model->LTRAalpha == 0.0) ||
                    (hilimit1 < model->LTRAtd)) ? 0.0 :
                    alphasqTterm*bessi1overxterm;
                h2dummy1 = twiceintlinfunc(lolimit1,hilimit1,lolimit1,
                    h2lovalue1,h2hivalue1,lolimit1,hilimit1)/delta1;

                *(model->LTRAh2Coeffs + i) = h2dummy1 - h2dummy2 +
                    intlinfunc(lolimit2,hilimit2,
                    h2lovalue2,h2hivalue2,lolimit2,hilimit2);
                if (FABS(*(model->LTRAh2Coeffs + i)) <= h2relval) doh2 = 0;
            }
            else
                *(model->LTRAh2Coeffs + i) = 0.0;

            if (doh3) {
                h3lovalue2 = h3lovalue1; /* previous lovalue1 */
                h3hivalue2 = h3hivalue1; /* previous hivalue1 */
                h3dummy2   = h3dummy1;   /* previous dummy1 */
    
                h3lovalue1 = h3hivalue2;
                bessi0term = bessZZ(besselarg, exparg);
                h3hivalue1 =
                    ((hilimit1 <= model->LTRAtd) ||
                    (model->LTRAbeta == 0.0)) ? 0.0 :
                    bessi0term - expbetaTterm;
                h3dummy1 = intlinfunc(lolimit1,hilimit1,h3lovalue1,
                    h3hivalue1,lolimit1,hilimit1)/delta1;

                *(model->LTRAh3dashCoeffs + i) = h3dummy1 - h3dummy2;
                if (FABS(*(model->LTRAh3dashCoeffs + i)) <= h3relval)
                    doh3 = 0;
            }
            else
                *(model->LTRAh3dashCoeffs + i) = 0.0;
        } 
    }
    model->LTRAauxIndex = auxindex;
}


double
LTRArlcH2Func(time,InModel)

double time;
GENmodel *InModel;
{
    LTRAmodel *model = (LTRAmodel*)InModel;
    double besselarg, exparg, returnval;

    /*
     * result = 0, time < T
     *        = (alpha*T*e^{-beta*time})/sqrt(t^2 - T^2) *
     *                           I_1(alpha*sqrt(t^2 - T^2))
     */

    if (model->LTRAalpha == 0.0) return(0.0);
    if (time < model->LTRAtd) return(0.0);

    if (time != model->LTRAtd) { 
        besselarg = model->LTRAalpha*
            sqrt(time*time - model->LTRAtd*model->LTRAtd); 
    }
    else { 
        besselarg = 0.0;
    }
    exparg = -model->LTRAbeta*time;

    returnval = model->LTRAalpha*model->LTRAalpha*model->LTRAtd*
        exp(exparg)*bessI1xOverX(besselarg);
    return (returnval);
}


double
LTRArlcH3dashFunc(time,T,alpha,beta)

double time,T,alpha,beta;
{
    double exparg, besselarg, returnval;

    /*
     * result = 0, time < T
     *        = alpha*e^{-beta*time}*(t/sqrt(t^2-T^2)*
     *             I_1(alpha*sqrt(t^2-T^2)) - I_0(alpha*sqrt(t^2-T^2)))
     */

    if (alpha == 0.0) return(0.0);
    if (time < T) return(0.0);

    exparg = - beta*time;
    if (time != T) { 
        besselarg = alpha*sqrt(time*time - T*T); 
    }
    else { 
        besselarg = 0.0;
    }

    returnval = alpha*time*bessI1xOverX(besselarg) - bessI0(besselarg);
    returnval *= alpha*exp(exparg);
    return (returnval);
}

/*
 * LTRAstraightLineCheck - takes the co-ordinates of three points,
 * finds the area of the triangle enclosed by these points and
 * compares this area with the area of the quadrilateral formed by
 * the line between the first point and the third point, the
 * perpendiculars from the first and third points to the x-axis, and
 * the x-axis. If within reltol, then it returns 1, else 0. The
 * purpose of this function is to determine if three points lie
 * acceptably close to a straight line. This area criterion is used
 * because it is related to integrals and convolution
 */

int
LTRAstraightLineCheck(x1,y1,x2,y2,x3,y3,reltol,abstol)
double x1,y1,x2,y2,x3,y3,reltol,abstol;

{
double TRarea, QUADarea1,QUADarea2,QUADarea3, area;

    /* this should work if y1,y2,y3 all have the same sign and x1,x2,x3
     * are in increasing order
     */

    QUADarea1 = (FABS(y2)+FABS(y1))*0.5*FABS(x2-x1);
    QUADarea2 = (FABS(y3)+FABS(y2))*0.5*FABS(x3-x2);
    QUADarea3 = (FABS(y3)+FABS(y1))*0.5*FABS(x3-x1);
    TRarea = FABS(QUADarea3 - QUADarea1 - QUADarea2);
    area = QUADarea1 + QUADarea2;
    if (area*reltol + abstol > TRarea)
        return(1);
    return(0);
}


/* i is the index of the latest value, 
 * a,b,c values correspond to values at t_{i-2}, t{i-1} and t_i
 */

#define SECONDDERIV(i,a,b,c) (oof = (i==ckt->CKTtimeIndex+1?curtime:\
*(ckt->CKTtimePoints+i)),\
(( c - b )/(oof-*(ckt->CKTtimePoints+i-1)) -\
( b - a )/(*(ckt->CKTtimePoints+i-1)-\
*(ckt->CKTtimePoints+i-2)))/(oof - \
*(ckt->CKTtimePoints+i-2)))

/*
 * LTRAlteCalculate - returns sum of the absolute values of the total
 * local truncation error of the 2 equations for the LTRAline
 */


double
LTRAlteCalculate(ckt,genmodel,geninstance,curtime)

CKTcircuit *ckt;
GENmodel *genmodel;
GENinstance *geninstance;
double curtime;
{
    LTRAmodel *model = (LTRAmodel *) genmodel;
    LTRAinstance *instance = (LTRAinstance *) geninstance;
    double h1dashTfirstCoeff; 
    double h2TfirstCoeff; 
    double h3dashTfirstCoeff; 
    double dashdash;
    double oof;
    double hilimit1, lolimit1, hivalue1, lovalue1, f1i, g1i;
    double eq1LTE = 0.0, eq2LTE = 0.0;
    int auxindex, tdover, i, exact;

    switch(model->LTRAspecialCase) {

        case LTRA_MOD_LC:
        case LTRA_MOD_RG:
            return(0.0);

        case LTRA_MOD_RLC:

            if (curtime > model->LTRAtd) {
                tdover = 1;

                exact = 0;
                for (i = ckt->CKTtimeIndex ; i>= 0; i--) {
                    if (curtime - *(ckt->CKTtimePoints + i) ==
                            model->LTRAtd) {
                        exact = 1;
                        break;
                    } 
                    if (curtime - *(ckt->CKTtimePoints + i)
                            > model->LTRAtd)
                        break;
                }

#ifdef LTRADEBUG
                if ((i < 0) || ((i==0) && (exact==1)))
                printf("LTRAlteCalculate: i <= 0: some mistake!\n");
#endif

                if (exact == 1) {
                    auxindex = i-1;
                }
                else {
                    auxindex = i;
                }

            }
            else {
                tdover = 0;
            }

            hilimit1 = curtime - *(ckt->CKTtimePoints + ckt->CKTtimeIndex);
            lolimit1 = 0.0;
            hivalue1 = ltra_rlcH1dashTwiceIntFunc(hilimit1,model->LTRAbeta);
            lovalue1 = 0.0;

            f1i = hivalue1;
            g1i = intlinfunc(lolimit1,hilimit1,lovalue1,hivalue1,
                lolimit1,hilimit1);
            h1dashTfirstCoeff = 0.5 * f1i *
                (curtime - *(ckt->CKTtimePoints+ckt->CKTtimeIndex)) - g1i;

            if (tdover) {
                hilimit1 = curtime - *(ckt->CKTtimePoints + auxindex);
                lolimit1 = *(ckt->CKTtimePoints + ckt->CKTtimeIndex) -
                    *(ckt->CKTtimePoints + auxindex);
                lolimit1 = MAX(model->LTRAtd,lolimit1);

                /* are the following really doing the operations in the
                    write-up? */
                hivalue1 = LTRArlcH2Func(hilimit1,(GENmodel*)model);
                lovalue1 = LTRArlcH2Func(lolimit1,(GENmodel*)model);
                f1i = twiceintlinfunc(lolimit1,hilimit1,lolimit1,lovalue1,
                    hivalue1,lolimit1,hilimit1);
                g1i = thriceintlinfunc(lolimit1,hilimit1,lolimit1,lolimit1,
                    lovalue1,hivalue1,lolimit1,hilimit1);
                h2TfirstCoeff = 0.5*f1i*(curtime-model->LTRAtd -
                    *(ckt->CKTtimePoints+auxindex)) - g1i;

                hivalue1 = ltra_rlcH3dashIntFunc(hilimit1,model->LTRAtd,
                    model->LTRAbeta);
                lovalue1 = ltra_rlcH3dashIntFunc(lolimit1,model->LTRAtd,
                    model->LTRAbeta);
                f1i = intlinfunc(lolimit1,hilimit1,lovalue1,hivalue1,
                    lolimit1,hilimit1);
                g1i = twiceintlinfunc(lolimit1,hilimit1,lolimit1,lovalue1,
                    hivalue1,lolimit1,hilimit1);
                h3dashTfirstCoeff = 0.5*f1i*(curtime - model->LTRAtd -
                    *(ckt->CKTtimePoints+auxindex)) - g1i;
            }


            /* LTEs for convolution with v1 */
            /* get divided differences for v1 (2nd derivative estimates) */

            /* 
             * no need to subtract operating point values because
             * taking differences anyway
             */

            dashdash = SECONDDERIV(ckt->CKTtimeIndex+1,
                *(instance->LTRAv1+ckt->CKTtimeIndex-1),
                *(instance->LTRAv1+ckt->CKTtimeIndex),
                *(ckt->CKTrhsOld + instance->LTRAposNode1) -
                *(ckt->CKTrhsOld + instance->LTRAnegNode1));
            eq1LTE += model->LTRAadmit*FABS(dashdash * 
                h1dashTfirstCoeff);

            /* not bothering to interpolate since everything is approximate
             anyway */

            if (tdover) {

                dashdash = SECONDDERIV(auxindex+1,
                    *(instance->LTRAv1 + auxindex - 1),
                    *(instance->LTRAv1 + auxindex),
                    *(instance->LTRAv1 + auxindex + 1)) ;

                eq2LTE += model->LTRAadmit*FABS(dashdash * 
                    h3dashTfirstCoeff);
            }
            /* end LTEs for convolution with v1 */

            /* LTEs for convolution with v2 */
            /* get divided differences for v2 (2nd derivative estimates) */

            dashdash = SECONDDERIV(ckt->CKTtimeIndex+1,
                *(instance->LTRAv2+ckt->CKTtimeIndex-1),
                *(instance->LTRAv2+ckt->CKTtimeIndex),
                *(ckt->CKTrhsOld + instance->LTRAposNode2) -
                *(ckt->CKTrhsOld + instance->LTRAnegNode2));

            eq2LTE += model->LTRAadmit*FABS(dashdash * 
                h1dashTfirstCoeff);


            if (tdover) {
                dashdash = SECONDDERIV(auxindex+1,
                    *(instance->LTRAv2 + auxindex - 1),
                    *(instance->LTRAv2 + auxindex),
                    *(instance->LTRAv2 + auxindex + 1)) ;

                eq1LTE += model->LTRAadmit*FABS(dashdash * 
                    h3dashTfirstCoeff);
            }

            /* end LTEs for convolution with v2 */

            /* LTE for convolution with i1 */
            /* get divided differences for i1 (2nd derivative estimates) */

            if (tdover) {
                dashdash = SECONDDERIV(auxindex+1,
                    *(instance->LTRAi1 + auxindex - 1),
                    *(instance->LTRAi1 + auxindex),
                    *(instance->LTRAi1 + auxindex + 1)) ;

                eq2LTE += FABS(dashdash * h2TfirstCoeff);
            }
            /* end LTE for convolution with i1 */

            /* LTE for convolution with i2 */
            /* get divided differences for i2 (2nd derivative estimates) */

            if (tdover) {
                dashdash = SECONDDERIV(auxindex+1,
                    *(instance->LTRAi2 + auxindex - 1),
                    *(instance->LTRAi2 + auxindex),
                    *(instance->LTRAi2 + auxindex + 1)) ;

                eq1LTE += FABS(dashdash * h2TfirstCoeff);
            }
            /* end LTE for convolution with i1 */

            break;

        case LTRA_MOD_RC:

            hilimit1 = curtime - *(ckt->CKTtimePoints + ckt->CKTtimeIndex);
            lolimit1 = 0.0;

            hivalue1 = ltra_rcH1dashTwiceIntFunc(hilimit1,model->LTRAcByR);
            lovalue1 = 0.0;

            f1i = hivalue1;
            g1i = intlinfunc(lolimit1,hilimit1,lovalue1,hivalue1,lolimit1,
                hilimit1);
/*
            h1dashTfirstCoeff = 0.5*f1i*(curtime -
                *(ckt->CKTtimePoints+ckt->CKTtimeIndex)) - g1i;
*/
            h2TfirstCoeff = 0.5*f1i*(curtime -
                *(ckt->CKTtimePoints+ckt->CKTtimeIndex)) - g1i;

            hivalue1 = ltra_rcH2TwiceIntFunc(hilimit1,model->LTRArclsqr);
            lovalue1 = 0.0;

            f1i = hivalue1;
            g1i = intlinfunc(lolimit1,hilimit1,lovalue1,hivalue1,lolimit1,
                hilimit1);
            h1dashTfirstCoeff = 0.5*f1i*(curtime -
                *(ckt->CKTtimePoints+ckt->CKTtimeIndex)) - g1i;

            hivalue1 = ltra_rcH2TwiceIntFunc(hilimit1,model->LTRArclsqr);
            lovalue1 = 0.0;

            f1i = hivalue1;
            g1i = intlinfunc(lolimit1,hilimit1,lovalue1,
                hivalue1,lolimit1,hilimit1);
/*
            h1dashTfirstCoeff = 0.5*f1i*(curtime -
                *(ckt->CKTtimePoints+ckt->CKTtimeIndex)) - g1i;
*/
            h3dashTfirstCoeff = 0.5*f1i*(curtime -
                *(ckt->CKTtimePoints+ckt->CKTtimeIndex)) - g1i;


            /* LTEs for convolution with v1 */
            /* get divided differences for v1 (2nd derivative estimates) */

            /* 
             * no need to subtract operating point values because
             * taking differences anyway
             */

            dashdash = SECONDDERIV(ckt->CKTtimeIndex+1,
                *(instance->LTRAv1+ckt->CKTtimeIndex-1),
                *(instance->LTRAv1+ckt->CKTtimeIndex),
                *(ckt->CKTrhsOld + instance->LTRAposNode1) -
                *(ckt->CKTrhsOld + instance->LTRAnegNode1));
            eq1LTE += FABS(dashdash * h1dashTfirstCoeff);
            eq2LTE += FABS(dashdash * h3dashTfirstCoeff);
            
            /* end LTEs for convolution with v1 */

            /* LTEs for convolution with v2 */
            /* get divided differences for v2 (2nd derivative estimates) */

            dashdash = SECONDDERIV(ckt->CKTtimeIndex+1,
                *(instance->LTRAv2+ckt->CKTtimeIndex-1),
                *(instance->LTRAv2+ckt->CKTtimeIndex),
                *(ckt->CKTrhsOld + instance->LTRAposNode2) -
                *(ckt->CKTrhsOld + instance->LTRAnegNode2));

            eq2LTE += FABS(dashdash * h1dashTfirstCoeff);
            eq1LTE += FABS(dashdash * h3dashTfirstCoeff);
            
            /* end LTEs for convolution with v2 */

            /* LTE for convolution with i1 */
            /* get divided differences for i1 (2nd derivative estimates) */

            dashdash = SECONDDERIV(ckt->CKTtimeIndex+1,
                *(instance->LTRAi1 + ckt->CKTtimeIndex - 1),
                *(instance->LTRAi1 + ckt->CKTtimeIndex),
                *(ckt->CKTrhsOld + instance->LTRAbrEq1));

            eq2LTE += FABS(dashdash * h2TfirstCoeff);

            /* end LTE for convolution with i1 */

            /* LTE for convolution with i2 */
            /* get divided differences for i2 (2nd derivative estimates) */

            dashdash = SECONDDERIV(ckt->CKTtimeIndex+1,
                *(instance->LTRAi2 + ckt->CKTtimeIndex - 1),
                *(instance->LTRAi2 + ckt->CKTtimeIndex),
                *(ckt->CKTrhsOld + instance->LTRAbrEq2));

            eq1LTE += FABS(dashdash * h2TfirstCoeff);
            
            /* end LTE for convolution with i1 */

            break;

        default:
            return (1);
    }

#ifdef LTRADEBUG
    fprintf(stdout,"%s: LTE/input for Eq1 at time %g is: %g\n",
        instance->LTRAname, curtime, eq1LTE/instance->LTRAinput1);
    fprintf(stdout,"%s: LTE/input for Eq2 at time %g is: %g\n",
        instance->LTRAname, curtime, eq2LTE/instance->LTRAinput1);
    fprintf(stdout,"\n");
#endif

    return (FABS(eq1LTE) + FABS(eq2LTE));
}



/* intlinfunc returns int lolimit-hilimit h(tau) dtau, where
 * h(tau) is assumed to be linear, with values lovalue and hivalue
 * tau = t1 and t2 respectively
 * this is used only locally
 */

static double
intlinfunc(lolimit,hilimit,lovalue,hivalue,t1,t2)

double lolimit, hilimit, lovalue, hivalue, t1,t2;
{
    double width, m;

    width = t2 - t1;
    if (width == 0.0) return(0.0);
    m = (hivalue - lovalue)/width;

    return ((hilimit-lolimit)*lovalue + 0.5*m*((hilimit-t1)*(hilimit-t1)
        - (lolimit - t1)*(lolimit - t1)));
}


/* twiceintlinfunc returns int lolimit-hilimit int otherlolimit-tau 
 * h(tau') d tau' d tau , where
 * h(tau') is assumed to be linear, with values lovalue and hivalue
 * tau = t1 and t2 respectively
 * this is used only locally
 */

static double
twiceintlinfunc(lolimit,hilimit,otherlolimit,lovalue,hivalue,t1,t2)

double lolimit, hilimit, lovalue, hivalue, t1,t2, otherlolimit;
{
    double width, m, dummy;
    double temp1, temp2, temp3;

    width = t2 - t1;
    if (width == 0.0) return(0.0);
    m = (hivalue - lovalue)/width;

    temp1 = hilimit - t1;
    temp2 = lolimit - t1;
    temp3 = otherlolimit - t1;
    dummy = lovalue*((hilimit - otherlolimit)*(hilimit - otherlolimit) -
        (lolimit - otherlolimit)*(lolimit - otherlolimit));
    dummy += m*((temp1*temp1*temp1 - temp2*temp2*temp2)/3.0 -
        temp3*temp3*(hilimit - lolimit));
    return(dummy*0.5);
}


/* thriceintlinfunc returns int lolimit-hilimit int secondlolimit-tau
 * int thirdlolimit-tau' h(tau'') d tau'' d tau' d tau , where
 * h(tau'') is assumed to be linear, with values lovalue and hivalue
 * tau = t1 and t2 respectively
 * this is used only locally
 */

static double
thriceintlinfunc(lolimit,hilimit,secondlolimit,thirdlolimit,lovalue,
    hivalue,t1,t2)

double lolimit, hilimit, lovalue, hivalue, t1,t2, secondlolimit,
    thirdlolimit;
{
    double width, m, dummy;
    double temp1, temp2, temp3, temp4;
    double temp5, temp6, temp7, temp8, temp9, temp10;

    width = t2 - t1;
    if (width == 0.0) return(0.0);
    m = (hivalue - lovalue)/width;

    temp1 = hilimit - t1;
    temp2 = lolimit - t1;
    temp3 = secondlolimit - t1;
    temp4 = thirdlolimit - t1;
    temp5 = hilimit - thirdlolimit;
    temp6 = lolimit - thirdlolimit;
    temp7 = secondlolimit - thirdlolimit;
    temp8 = hilimit - lolimit;
    temp9 = hilimit - secondlolimit;
    temp10 = lolimit - secondlolimit;
    dummy = lovalue*((temp5*temp5*temp5 - temp6*temp6*temp6)/3 -
        temp7*temp5*temp8);
    dummy += m*(((temp1*temp1*temp1*temp1 - temp2*temp2*temp2*temp2)*0.25 -
        temp3*temp3*temp3*temp8)/3 - temp4*temp4*0.5*(temp9*temp9 -
        temp10*temp10));
    return(dummy*0.5);
}

    
/*
 * 
 * These are from the book Numerical Recipes in C
 *
 */

static double
bessI0(x)

double x;
{
    double ax,ans;
    double y;
    
    if ((ax = fabs(x)) < 3.75) {
        y = x/3.75;
        y *= y;
        ans = 1.0+y*(3.5156229+y*(3.0899424+y*(1.2067492
            +y*(0.2659732+y*(0.360768e-1+y*0.45813e-2)))));
    }
    else {
        y = 3.75/ax;
        ans = (exp(ax)/sqrt(ax))*(0.39894228+y*(0.1328592e-1
            +y*(0.225319e-2+y*(-0.157565e-2+y*(0.916281e-2
            +y*(-0.2057706e-1+y*(0.2635537e-1+y*(-0.1647633e-1
            +y*0.392377e-2))))))));
    }
    return (ans);
}
    

/*
static double
bessI1(x)

double x;
{
    double ax,ans;
    double y;
    
    if ((ax = fabs(x)) < 3.75) {
        y = x/3.75;
        y *= y;
        ans = ax*(0.5+y*(0.87890594+y*(0.51498869+y*(0.15084934
            +y*(0.2658733e-1+y*(0.301532e-2+y*0.32411e-3))))));
    }
    else {
        y = 3.75/ax;
        ans = 0.2282967e-1+y*(-0.2895312e-1+y*(0.1787654e-1
            -y*0.420059e-2));
        ans = 0.39894228+y*(-0.3988024e-1+y*(-0.362018e-2
            +y*(0.163801e-2+y*(-0.1031555e-1+y*ans))));
        ans *= (exp(ax)/sqrt(ax));
    }
    return (x < 0.0 ? -ans : ans);
}
*/


// return (bessI1(x) + bessI0(x))*exp(-x) assuming x > 0
//
static double
bessXX(double x)
{
    double ax = fabs(x);
    if (ax < 3.75) {
        double y = x/3.75;
        y *= y;
        double ans1 = 1.0+y*(3.5156229+y*(3.0899424+y*(1.2067492
            +y*(0.2659732+y*(0.360768e-1+y*0.45813e-2)))));
        double ans2 = ax*(0.5+y*(0.87890594+y*(0.51498869+y*(0.15084934
            +y*(0.2658733e-1+y*(0.301532e-2+y*0.32411e-3))))));
        if (x > 0)
            ans1 += ans2;
        else
            ans1 -= ans2;
        return (ans1 * exp(-x));
    }       
    else {
        double y = 3.75/ax;
        double ans1 = (0.39894228+y*(0.1328592e-1
            +y*(0.225319e-2+y*(-0.157565e-2+y*(0.916281e-2
            +y*(-0.2057706e-1+y*(0.2635537e-1+y*(-0.1647633e-1
            +y*0.392377e-2))))))));
        double ans2 = 0.2282967e-1+y*(-0.2895312e-1+y*(0.1787654e-1
            -y*0.420059e-2));
        ans2 = 0.39894228+y*(-0.3988024e-1+y*(-0.362018e-2
            +y*(0.163801e-2+y*(-0.1031555e-1+y*ans2))));
        if (x > 0)
            ans1 += ans2;
        else
            ans1 -= ans2;
        ans1 /= sqrt(ax);
        return (ans1);
    }
}


static double
bessI1xOverX(x)

double x;
{
    double ax,ans;
    double y;
    
    if ((ax = fabs(x)) < 3.75) {
        y = x/3.75;
        y *= y;
        ans = 0.5+y*(0.87890594+y*(0.51498869+y*(0.15084934
            +y*(0.2658733e-1+y*(0.301532e-2+y*0.32411e-3)))));
    }
    else {
        y = 3.75/ax;
        ans = 0.2282967e-1+y*(-0.2895312e-1+y*(0.1787654e-1
            -y*0.420059e-2));
        ans = 0.39894228+y*(-0.3988024e-1+y*(-0.362018e-2
            +y*(0.163801e-2+y*(-0.1031555e-1+y*ans))));
        ans *= (exp(ax)/(ax*sqrt(ax)));
    }
    return (ans);
}


static double
bessYY(double x, double z)
{
    double ans, ax = fabs(x);
    if (ax < 3.75) {
        double y = x/3.75;
        y *= y;
        ans = 0.5+y*(0.87890594+y*(0.51498869+y*(0.15084934
            +y*(0.2658733e-1+y*(0.301532e-2+y*0.32411e-3)))));
        ans *= exp(z);
    }
    else {
        double y = 3.75/ax;
        ans = 0.2282967e-1+y*(-0.2895312e-1+y*(0.1787654e-1
            -y*0.420059e-2));
        ans = 0.39894228+y*(-0.3988024e-1+y*(-0.362018e-2
            +y*(0.163801e-2+y*(-0.1031555e-1+y*ans))));
        ans *= (exp(ax + z)/(ax*sqrt(ax)));
    }
    return (ans);
}


static double
bessZZ(double x, double z)
{
    double ans, ax = fabs(x);
    if (ax < 3.75) {
        double y = x/3.75;
        y *= y;
        ans = 1.0+y*(3.5156229+y*(3.0899424+y*(1.2067492
            +y*(0.2659732+y*(0.360768e-1+y*0.45813e-2)))));
        ans *= exp(z);
    }
    else {
        double y = 3.75/ax;
        ans = (exp(ax + z)/sqrt(ax))*(0.39894228+y*(0.1328592e-1
            +y*(0.225319e-2+y*(-0.157565e-2+y*(0.916281e-2
            +y*(-0.2057706e-1+y*(0.2635537e-1+y*(-0.1647633e-1
            +y*0.392377e-2))))))));
    }
    return (ans);
}


/* ltra_rlcH1dashTwiceIntFunc - twice repeated integral of h1dash for the
 * special case of G = 0
 */

static double
ltra_rlcH1dashTwiceIntFunc(time,beta)

double time,beta;
{
    double arg, returnval;

    /* result = time * e^{- beta*time} * {I_0(beta*time) +
     * I_1(beta*time)} - time
     */

    if (beta == 0.0) return (time);
    arg = beta*time;
    if (arg == 0.0) return (0.0);

    returnval = (bessXX(arg)*time - time);
    return (returnval);
}


/* ltra_rlcH3dashIntFunc - twice repeated integral of h1dash for the
 * special case of G = 0
 */

static double
ltra_rlcH3dashIntFunc(time,T,beta)

double time, T, beta;
{
    double exparg, besselarg;
    double returnval;

    if (time <= T) return(0.0);
    if (beta == 0.0) return(0.0);
    exparg = -beta*time;
    besselarg = beta*sqrt(time*time - T*T);
    returnval = (bessZZ(besselarg, exparg) - exp(-beta*T));
    return (returnval);
}


static double 
ltra_rcH1dashTwiceIntFunc(time,cbyr)

double time, cbyr;
{
    return (sqrt(4*cbyr*time/M_PI));
}


static double
ltra_rcH2TwiceIntFunc(time,rclsqr)

double time, rclsqr;
{
    double temp;

    if (time != 0.0) {
        temp = rclsqr/(4*time);
        return ((time + rclsqr*0.5)*erfc(sqrt(temp)) -
            sqrt(time*rclsqr/M_PI)*exp(-temp));
    }
    return(0.0);
}
