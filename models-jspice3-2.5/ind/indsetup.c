/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

/* load the inductor structure with those pointers needed later 
 * for fast matrix loading 
 */

#include "spice.h"
#include <stdio.h>
#include <math.h>
#include "inddefs.h"
#include "util.h"
#include "sperror.h"
#include "cktext.h"


int
INDsetup(matrix,inModel,ckt,states)

SMPmatrix *matrix;
GENmodel *inModel;
CKTcircuit *ckt;
int *states;
{
    INDmodel *model = (INDmodel*)inModel;
    INDinstance *here;
    int error;
    CKTnode *tmp;

#ifdef MUTUAL
    MUTmodel *mutmodel;
    int ktype;
#endif /* MUTUAL */

    /*  loop through all the inductor models */
    for ( ; model != NULL; model = model->INDnextModel) {

#ifdef MUTUAL
        /* save the mutual indutor model head, this is redundant if
         * there is more than one inductor model
         */
        mutmodel = (MUTmodel*)(ckt->CKThead[CKTtypelook("mutual")]);
        model->INDmutModels = (GENmodel*)mutmodel;
#endif

        /* loop through all the instances of the model */
        for (here = model->INDinstances; here != NULL;
                here = here->INDnextInstance) {
            
            if (here->INDbrEq == 0) {
                /* SRW -- used to call this ...#internal, changed for
                 * compatability with voltage sources, i.e. i(l1) maps
                 * to l1#branch.
                 */
                error = CKTmkCur(ckt,&tmp,here->INDname,"branch");
                if (error)
                    return (error);
                here->INDbrEq = tmp->number;
            }

            TSTALLOC(INDibrIbrptr,INDbrEq,INDbrEq)
            TSTALLOC(INDposIbrptr,INDposNode,INDbrEq)
            TSTALLOC(INDnegIbrptr,INDnegNode,INDbrEq)
            TSTALLOC(INDibrNegptr,INDbrEq,INDnegNode)
            TSTALLOC(INDibrPosptr,INDbrEq,INDposNode)

            here->INDflux = *states;
            *states += 2 ;

            if (ckt->CKTpreload) {
                /* preload constants */

                if (here->INDposNode) {
                    *(here->INDposIbrptr) += 1;
                    *(here->INDibrPosptr) += 1;
                }
                if (here->INDnegNode) {
                    *(here->INDnegIbrptr) -= 1;
                    *(here->INDibrNegptr) -= 1;
                }
            }
        }
    }

#ifdef MUTUAL

    ktype = CKTtypelook("Inductor");

    /*  loop through all the mutual inductor models */
    for ( ; mutmodel != NULL; mutmodel = mutmodel->MUTnextModel) {

        MUTinstance *here;

        /* loop through all the instances of the model */
        for (here = mutmodel->MUTinstances; here != NULL;
                here = here->MUTnextInstance) {
            
            error =
                CKTfndDev((GENERIC*)ckt,&ktype,(GENERIC**)&(here->MUTind1),
                    here->MUTindName1, (GENERIC *)NULL,(char *)NULL);
            if (error && error != E_NODEV && error != E_NOMOD)
                return (error);
            if (error) {
                IFuid namarray[2];
                namarray[0] = here->MUTname;
                namarray[1] = here->MUTindName1;
                (*(SPfrontEnd->IFerror))(ERR_WARNING,
                    "%s: coupling to non-existant inductor %s.",
                    namarray);
            }
            error =
                CKTfndDev((GENERIC*)ckt,&ktype,(GENERIC**)&(here->MUTind2),
                    here->MUTindName2,(GENERIC *)NULL,(char *)NULL);
            if (error && error != E_NODEV && error != E_NOMOD)
                return (error);
            if (error) {
                IFuid namarray[2];
                namarray[0] = here->MUTname;
                namarray[1] = here->MUTindName2;
                (*(SPfrontEnd->IFerror))(ERR_WARNING,
                    "%s: coupling to non-existant inductor %s.",
                    namarray);
            }

            here->MUTfactor = here->MUTcoupling *
                sqrt(here->MUTind1->INDinduct *
                    here->MUTind2->INDinduct);

            TSTALLOC(MUTbr1br2,MUTind1->INDbrEq,MUTind2->INDbrEq)
            TSTALLOC(MUTbr2br1,MUTind2->INDbrEq,MUTind1->INDbrEq)
        }
    }
#endif /* MUTUAL */

    return (OK);
}
