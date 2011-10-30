/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1987 Kanwar Jit Singh
         1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "srcdefs.h"
#include "sperror.h"
#include "util.h"
#include "cktext.h"


int
SRCfindBr(ckt,inputModel,name)

CKTcircuit *ckt;
GENmodel *inputModel;
IFuid name;
{
    SRCinstance *here;
    SRCmodel *model = (SRCmodel*)inputModel;
    int error;
    CKTnode *tmp;

    for ( ; model != NULL; model = model->SRCnextModel) {
        for (here = model->SRCinstances; here != NULL;
                here = here->SRCnextInstance) {
            if (here->SRCname == name && here->SRCtype == SRC_V) {
                if (here->SRCbranch == 0) {
                    error = CKTmkCur(ckt,&tmp, here->SRCname,"branch");
                    if (error)
                        return (error);
                    here->SRCbranch = tmp->number;
                }
                return (here->SRCbranch);
            }
        }
    }
    return (0);
}
