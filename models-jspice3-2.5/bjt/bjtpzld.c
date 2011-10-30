/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "bjtdefs.h"
#include "sperror.h"


int
BJTpzLoad(inModel,ckt,s)

GENmodel *inModel;
CKTcircuit *ckt;
SPcomplex *s;
{
    BJTmodel *model = (BJTmodel*)inModel;
    BJTinstance *here;
    double gcpr;
    double gepr;
    double gpi;
    double gmu;
    double go;
    double xgm;
    double gm;
    double gx;
    double xcpi;
    double xcmu;
    double xcbx;
    double xccs;
    double xcmcb;

    for ( ; model != NULL; model = model->BJTnextModel) {
        for (here = model->BJTinstances; here!= NULL; 
                here = here->BJTnextInstance) {
            
            gcpr = model->BJTcollectorResist * here->BJTarea;
            gepr = model->BJTemitterResist * here->BJTarea;
            gpi  = here->BJTgpi;
            gmu  = here->BJTgmu;
            gm   = here->BJTgm;
            go   = here->BJTgo;
            xgm  = 0;
            gx   = here->BJTgx;
            xcpi = here->BJTcapbe * ckt->CKTomega;
            xcmu = here->BJTcapbc * ckt->CKTomega;
            xcbx = here->BJTcapbx * ckt->CKTomega;
            xccs = here->BJTcapcs * ckt->CKTomega;
            xcmcb= here->BJTgeqcb * ckt->CKTomega;

            *(here->BJTcolColPtr)       += gcpr;
            *(here->BJTbaseBasePtr)     += gx + xcbx * s->real;
            *(here->BJTbaseBasePtr + 1) += xcbx * s->imag;
            *(here->BJTemitEmitPtr)     += gepr;

            *(here->BJTcolPrimeColPrimePtr) +=
                (gmu+go+gcpr) + (xcmu+xccs+xcbx) * (s->real);
            *(here->BJTcolPrimeColPrimePtr + 1) +=
                (xcmu+xccs+xcbx) * (s->imag);
            *(here->BJTbasePrimeBasePrimePtr) +=
                (gx+gpi+gmu) + (xcpi+xcmu+xcmcb) * (s->real);
            *(here->BJTbasePrimeBasePrimePtr + 1) +=
                (xcpi+xcmu+xcmcb) * (s->imag);
            *(here->BJTemitPrimeEmitPrimePtr) +=
                (gpi+gepr+gm+go) + (xcpi+xgm) * (s->real);
            *(here->BJTemitPrimeEmitPrimePtr + 1) +=
                (xcpi+xgm) * (s->imag);

            *(here->BJTcolColPrimePtr)   -= gcpr;
            *(here->BJTbaseBasePrimePtr) -= gx;
            *(here->BJTemitEmitPrimePtr) -= gepr;
            *(here->BJTcolPrimeColPtr)   -= gcpr;

            *(here->BJTcolPrimeBasePrimePtr) +=
                (-gmu+gm) + (-xcmu+xgm) * (s->real);
            *(here->BJTcolPrimeBasePrimePtr + 1) +=
                (-xcmu+xgm) * (s->imag);
            *(here->BJTcolPrimeEmitPrimePtr) +=
                (-gm-go) + (-xgm) * (s->real);
            *(here->BJTcolPrimeEmitPrimePtr + 1) += (-xgm) * (s->imag);
            *(here->BJTbasePrimeBasePtr) += (-gx);
            *(here->BJTbasePrimeColPrimePtr) +=
                (-gmu) + (-xcmu-xcmcb) * (s->real);
            *(here->BJTbasePrimeColPrimePtr + 1) +=
                (-xcmu-xcmcb) * (s->imag);
            *(here->BJTbasePrimeEmitPrimePtr) +=
                (-gpi) + (-xcpi) * (s->real);
            *(here->BJTbasePrimeEmitPrimePtr + 1) += (-xcpi) * (s->imag);
            *(here->BJTemitPrimeEmitPtr) += (-gepr);
            *(here->BJTemitPrimeColPrimePtr) +=
                (-go) + (xcmcb) * (s->real);
            *(here->BJTemitPrimeColPrimePtr + 1) += (xcmcb) * (s->imag);
            *(here->BJTemitPrimeBasePrimePtr) +=
                (-gpi-gm) + (-xcpi-xgm-xcmcb) * (s->real);
            *(here->BJTemitPrimeBasePrimePtr + 1) +=
                (-xcpi-xgm-xcmcb) * (s->imag);

            *(here->BJTsubstSubstPtr)        += xccs * s->real;
            *(here->BJTsubstSubstPtr + 1)    += xccs * s->imag;
            *(here->BJTcolPrimeSubstPtr)     -= xccs * s->real;
            *(here->BJTcolPrimeSubstPtr + 1) -= xccs * s->imag;
            *(here->BJTsubstColPrimePtr)     -= xccs * s->real;
            *(here->BJTsubstColPrimePtr + 1) -= xccs * s->imag;
            *(here->BJTbaseColPrimePtr)      -= xcbx * s->real;
            *(here->BJTbaseColPrimePtr + 1)  -= xcbx * s->imag;
            *(here->BJTcolPrimeBasePtr)      -= xcbx * s->real;
            *(here->BJTcolPrimeBasePtr + 1)  -= xcbx * s->imag;
        }
    }
    return (OK);
}
