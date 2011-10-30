/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1987 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "inddefs.h"
#include "util.h"
#include "sperror.h"
#include "cktext.h"


int
INDfindBr(ckt,inModel,name)

CKTcircuit *ckt;
GENmodel *inModel;
IFuid name;
{
    INDmodel *model = (INDmodel *)inModel;
    INDinstance *here;
    int error;
    CKTnode *tmp;

    for ( ; model != NULL; model = model->INDnextModel) {
        for (here = model->INDinstances; here != NULL;
                here = here->INDnextInstance) {
            if (here->INDname == name) {
                if (here->INDbrEq == 0) {
                    error = CKTmkCur(ckt,&tmp,here->INDname,"branch");
                    if (error) return(error);
                    here->INDbrEq = tmp->number;
                }
                return(here->INDbrEq);
            }
        }
    }
    return(0);
}

