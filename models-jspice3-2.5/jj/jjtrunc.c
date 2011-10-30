/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Author: 1993 Stephen R. Whiteley
****************************************************************************/

/*
 * This is called *only* when cct=0 for all junctions in the
 * circuit.  Otherwise the time step is set in JJacct().
 */

#include "spice.h"
#include <stdio.h>
#include "jjdefs.h"
#include "sperror.h"
#include "util.h"


int
JJtrunc(inModel,ckt,timeStep)

GENmodel *inModel;
CKTcircuit *ckt;
double *timeStep;
{
    JJmodel *model = (JJmodel *)inModel;
    JJinstance *here;

    for ( ; model!= NULL; model = model->JJnextModel) {
        for (here = model->JJinstances ; here != NULL;
            here = here->JJnextInstance) {

            CKTterr(here->JJvoltage,ckt,timeStep);

        }
    }
    return (OK);
}
