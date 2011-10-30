/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Author: 1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "jjdefs.h"
#include "sperror.h"

/* default max phase change is 2*PI/10 */
/* init to 2.07e-16 in options.c */
extern double JJdphi;

int
JJgetic(inModel,ckt)

/*
 * grab initial conditions out of rhs array.  User specified, so use
 * external nodes to get values.
 */

GENmodel *inModel;
CKTcircuit *ckt;
{
    JJmodel *model = (JJmodel *)inModel;
    JJinstance *here;
    JJmodel *tempmodel;
    double temp, vmax;

    for (tempmodel = model; model; model = model->JJnextModel) {
        for (here = model->JJinstances; here; here = here->JJnextInstance) {
            if (!here->JJinitVoltGiven)
                here->JJinitVoltage = 
                        *(ckt->CKTrhs + here->JJposNode) - 
                        *(ckt->CKTrhs + here->JJnegNode);
            if (!here->JJinitPhaseGiven) {
                if (here->JJphsNode)
                    here->JJinitPhase = *(ckt->CKTrhs + here->JJphsNode);
                else
                    here->JJinitPhase = 0;
            }
            if (here->JJcontrol)
                here->JJinitControl = *(ckt->CKTrhs + here->JJbranch);
        }
    }

    /* find initial time delta */

    /* jj's are "present" only if one has critical current */
    if (ckt->CKTjjPresent) {
        vmax = 0;

        for (model = tempmodel; model != NULL; model = model->JJnextModel) {

            if (model->JJictype != 0) {
                if (vmax < model->JJvdpbak) {
                    vmax = model->JJvdpbak;
                }
                for (here = model->JJinstances; here != NULL;
                        here = here->JJnextInstance) {

                    if (here->JJcriti > 0) {
                        temp = here->JJinitVoltage;
                        if (temp < 0)
                            temp = -temp;
                        if (vmax < temp) vmax = temp;
                    }
                }
            }
        }

        if (vmax > 0) {
            temp = .1*JJdphi/vmax;
            if (ckt->CKTinitDelta < temp)
                ckt->CKTinitDelta = temp;
        }
    }
    return (OK);
}
