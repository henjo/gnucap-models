/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

/*
 * Function to load the COMPLEX circuit matrix using the
 * small signal parameters saved during a previous DC operating
 * point analysis.
 */

#include "spice.h"
#include <stdio.h>
#include <math.h>
#include "bjtdefs.h"
#include "sperror.h"
#include "util.h"


int
BJTacLoad(inModel,ckt)

GENmodel *inModel;
CKTcircuit *ckt;
{
    BJTinstance *here;
    BJTmodel *model = (BJTmodel*)inModel;
    double gcpr;
    double gepr;
    double gpi;
    double gmu;
    double go;
    double xgm;
    double td;
    double arg;
    double gm;
    double gx;
    double xcpi;
    double xcmu;
    double xcbx;
    double xccs;
    double xcmcb;

    for ( ; model != NULL; model = model->BJTnextModel) {
        for (here = model->BJTinstances; here != NULL; 
                here = here->BJTnextInstance) {
            
            gcpr = model->BJTcollectorConduct * here->BJTarea;
            gepr = model->BJTemitterConduct * here->BJTarea;
            gpi  = here->BJTgpi;
            gmu  = here->BJTgmu;
            gm   = here->BJTgm;
            go   = here->BJTgo;
            xgm  = 0;
            td   = model->BJTexcessPhaseFactor;
            if (td != 0) {
                arg = td*ckt->CKTomega;
                gm += go;
                xgm = -gm*sin(arg);
                gm  = gm*cos(arg) - go;
            }
            gx   = here->BJTgx;
            xcpi = here->BJTcapbe * ckt->CKTomega;
            xcmu = here->BJTcapbc * ckt->CKTomega;
            xcbx = here->BJTcapbx * ckt->CKTomega;
            xccs = here->BJTcapcs * ckt->CKTomega;
            xcmcb= here->BJTgeqcb * ckt->CKTomega;

            *(here->BJTcolColPtr)                 += gcpr;
            *(here->BJTbaseBasePtr)               += gx;
            *(here->BJTbaseBasePtr + 1)           += xcbx;
            *(here->BJTemitEmitPtr)               += gepr;
            *(here->BJTcolPrimeColPrimePtr)       += gmu + go + gcpr;
            *(here->BJTcolPrimeColPrimePtr + 1)   += xcmu + xccs + xcbx;
            *(here->BJTbasePrimeBasePrimePtr)     += gx + gpi + gmu;
            *(here->BJTbasePrimeBasePrimePtr + 1) += xcpi + xcmu + xcmcb;
            *(here->BJTemitPrimeEmitPrimePtr)     += gpi + gepr + gm + go;
            *(here->BJTemitPrimeEmitPrimePtr + 1) += xcpi + xgm;
            *(here->BJTcolColPrimePtr)            -= gcpr;
            *(here->BJTbaseBasePrimePtr)          -= gx;
            *(here->BJTemitEmitPrimePtr)          -= gepr;
            *(here->BJTcolPrimeColPtr)            -= gcpr;
            *(here->BJTcolPrimeBasePrimePtr)      += -gmu + gm;
            *(here->BJTcolPrimeBasePrimePtr + 1)  += -xcmu + xgm;
            *(here->BJTcolPrimeEmitPrimePtr)      -= gm + go;
            *(here->BJTcolPrimeEmitPrimePtr + 1)  -= xgm;
            *(here->BJTbasePrimeBasePtr)          -= gx;
            *(here->BJTbasePrimeColPrimePtr)      -= gmu;
            *(here->BJTbasePrimeColPrimePtr + 1)  -= xcmu +xcmcb;
            *(here->BJTbasePrimeEmitPrimePtr)     -= gpi;
            *(here->BJTbasePrimeEmitPrimePtr + 1) -= xcpi;
            *(here->BJTemitPrimeEmitPtr)          -= gepr;
            *(here->BJTemitPrimeColPrimePtr)      -= go;
            *(here->BJTemitPrimeColPrimePtr + 1)  += xcmcb;
            *(here->BJTemitPrimeBasePrimePtr)     -= gpi + gm;
            *(here->BJTemitPrimeBasePrimePtr + 1) -= xcpi + xgm + xcmcb;
            *(here->BJTsubstSubstPtr + 1)         += xccs;
            *(here->BJTcolPrimeSubstPtr + 1)      -= xccs;
            *(here->BJTsubstColPrimePtr + 1)      -= xccs;
            *(here->BJTbaseColPrimePtr + 1)       -= xcbx;
            *(here->BJTcolPrimeBasePtr + 1)       -= xcbx;
        }
    }
    return (OK);
}
