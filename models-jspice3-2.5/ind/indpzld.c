/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "inddefs.h"
#include "sperror.h"


/* ARGSUSED */
int
INDpzLoad(inModel,ckt,s)

GENmodel *inModel;
CKTcircuit *ckt;
SPcomplex *s;
{
    INDmodel *model = (INDmodel*)inModel;
    INDinstance *here;
    double val;

#ifdef MUTUAL
    MUTmodel *mutmodel;
    MUTinstance *muthere;

    mutmodel = (MUTmodel*)model->INDmutModels;
    for ( ; mutmodel != NULL; mutmodel = mutmodel->MUTnextModel) {
        for (muthere = mutmodel->MUTinstances; muthere != NULL; 
                muthere = muthere->MUTnextInstance) {
    
            val = muthere->MUTfactor;
            *(muthere->MUTbr1br2)   -= val * s->real;
            *(muthere->MUTbr1br2+1) -= val * s->imag;
            *(muthere->MUTbr2br1)   -= val * s->real;
            *(muthere->MUTbr2br1+1) -= val * s->imag;
        }
    }
#endif /*MUTUAL*/

    for ( ; model != NULL; model = model->INDnextModel) {
        for (here = model->INDinstances; here != NULL; 
                here = here->INDnextInstance) {
    
            val = here->INDinduct;
            *(here->INDposIbrptr) += 1;
            *(here->INDnegIbrptr) -= 1;
            *(here->INDibrPosptr) += 1;
            *(here->INDibrNegptr) -= 1;
            *(here->INDibrIbrptr)   -= val * s->real;
            *(here->INDibrIbrptr+1) -= val * s->imag;
        }
    }
    return (OK);
}
