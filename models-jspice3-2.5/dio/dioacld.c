/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "diodefs.h"
#include "sperror.h"
#include "util.h"


int
DIOacLoad(inModel,ckt)

GENmodel *inModel;
CKTcircuit *ckt;
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

            gspr = model->DIOconductance*here->DIOarea;
            geq  = here->DIOgd;
            xceq = here->DIOcap*ckt->CKTomega;

            *(here->DIOposPosPtr  )   += gspr;
            *(here->DIOnegNegPtr  )   += geq;
            *(here->DIOnegNegPtr+1)   += xceq;
            *(here->DIOposPrimePosPrimePtr  ) += geq + gspr;
            *(here->DIOposPrimePosPrimePtr+1) += xceq;
            *(here->DIOposPosPrimePtr  ) -= gspr;
            *(here->DIOnegPosPrimePtr  ) -= geq;
            *(here->DIOnegPosPrimePtr+1) -= xceq;
            *(here->DIOposPrimePosPtr  ) -= gspr;
            *(here->DIOposPrimeNegPtr  ) -= geq;
            *(here->DIOposPrimeNegPtr+1) -= xceq;
        }
    }
    return (OK);

}
