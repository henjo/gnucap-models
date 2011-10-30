/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "diodefs.h"
#include "sperror.h"


int
DIOpzLoad(inModel,ckt,s)

GENmodel *inModel;
CKTcircuit *ckt;
SPcomplex *s;
{
    DIOmodel *model = (DIOmodel*)inModel;
    double gspr;
    double geq;
    double xceq;
    DIOinstance *here;

    /* loop through all the diode models */
    for ( ; model != NULL; model = model->DIOnextModel) {

        /* loop through all the instances of the model */
        for (here = model->DIOinstances; here != NULL;
                here = here->DIOnextInstance) {

            gspr = model->DIOresist*here->DIOarea;
            geq  = here->DIOgd;
            xceq = here->DIOcap;

            *(here->DIOposPosPtr  ) += gspr;
            *(here->DIOnegNegPtr  ) += geq + xceq*s->real;
            *(here->DIOnegNegPtr+1) += xceq*s->imag;
            *(here->DIOposPrimePosPrimePtr  ) += geq + gspr + xceq*s->real;
            *(here->DIOposPrimePosPrimePtr+1) += xceq*s->imag;
            *(here->DIOposPosPrimePtr  ) -= gspr;
            *(here->DIOnegPosPrimePtr  ) -= geq + xceq*s->real;
            *(here->DIOnegPosPrimePtr+1) -= xceq*s->imag;
            *(here->DIOposPrimePosPtr  ) -= gspr;
            *(here->DIOposPrimeNegPtr  ) -= geq + xceq*s->real;
            *(here->DIOposPrimeNegPtr+1) -= xceq*s->imag;
        }
    }
    return (OK);
}
