/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Author: 1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "jjdefs.h"
#include "sperror.h"
#include "util.h"

int
JJaccept(ckt,inModel)

CKTcircuit *ckt;
GENmodel *inModel;
{
    JJmodel *model = (JJmodel *)inModel;
    JJinstance *here;
    double phi,ag0;
    double vj,vmax = 0;
    int pint;

    ag0 = ckt->CKTag[0];

    for ( ; model != NULL; model = model->JJnextModel) {

        if (model->JJictype != 0 && vmax < model->JJvdpbak)
            vmax = model->JJvdpbak;

        for (here = model->JJinstances; here != NULL;
                here = here->JJnextInstance) {

            vj = *(ckt->CKTstate0 + here->JJvoltage);

            /* update */
            *(ckt->CKTstate0 + here->JJdvdt) = here->JJdelVdelT + ag0*vj;

            /* keep phase  > -2*PI and < 2*PI */
            phi = *(ckt->CKTstate0 + here->JJphase);
            pint = *(int *)(ckt->CKTstate1 + here->JJphsInt);
            if (phi > 2*M_PI) {
                phi -= 2*M_PI;
                pint++;
            }
            else if (phi < -2*M_PI) {
                phi += 2*M_PI;
                pint--;
            }
            *(ckt->CKTstate0 + here->JJphase) = phi;
            *(int *)(ckt->CKTstate0 + here->JJphsInt) = pint;

            /* find max vj for time step */
            if (model->JJictype != 0 && here->JJcriti > 0) {
                if (vj < 0)
                    vj = -vj;
                if (vmax < vj) vmax = vj;
            }

            if (here->JJphsNode)
                *(ckt->CKTrhsOld + here->JJphsNode) = phi + (2*M_PI)*pint;
        }
    }
    /* set next time step */
    if (ckt->CKTjjPresent) /* not set if jj's have cct=0 */
        ckt->CKTdelta = JJdphi/vmax;
    return (OK);
}
