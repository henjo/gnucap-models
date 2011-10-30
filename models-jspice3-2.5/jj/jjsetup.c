/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Author: 1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "jjdefs.h"
#include "sperror.h"
#include "util.h"
#include "cktext.h"

static double def_vm  = 0.03;     /* default Ic * Rsubgap */
static double def_icr = 0.0017;   /* default Ic * Rn      */

int
JJsetup(matrix,inModel,ckt,states)

SMPmatrix *matrix;
GENmodel *inModel;
CKTcircuit *ckt;
int *states;
{
    JJmodel *model = (JJmodel *)inModel;
    JJinstance *here;
    int error;
    char *emsg, *name;
    double temp;

    /*  loop through all the junction models */
    for ( ; model != NULL; model = model->JJnextModel) {

        if (!model->JJrtypeGiven)  model->JJrtype  = 1;
        if (!model->JJictypeGiven) model->JJictype = 1;
        if (!model->JJvgGiven)     model->JJvg     = 3e-3;
        if (!model->JJdelvGiven)   model->JJdelv   = 1e-4;
        if (!model->JJccsensGiven) model->JJccsens = 1e-2;
        if (!model->JJcritiGiven)  model->JJcriti  = 1e-3;
        if (!model->JJcapGiven)    model->JJcap    = 1e-12;

        if (model->JJdelv <= 0) {
            (*(SPfrontEnd->IFerror))(ERR_WARNING,
                "Warning: delv entry was <= 0, reset to 1e-5\n",
                (IFuid *)NULL);
            model->JJdelv = 1e-5;
        }
        if (model->JJccsens <= 0) {
            (*(SPfrontEnd->IFerror))(ERR_WARNING,
                "Warning: icon entry was <= 0, reset to 1e3\n",
                (IFuid *)NULL);
            model->JJccsens = 1e3;
        }

        if (!model->JJr0Given) {
            if (model->JJcriti)
                model->JJr0 = def_vm/model->JJcriti;
            else
                model->JJr0 = def_vm/1e-3;
        }
        if (!model->JJrnGiven) {
            if (model->JJcriti)
                model->JJrn = def_icr/model->JJcriti;
            else
                model->JJrn = def_icr/1e-3;
        }

        if (model->JJrn > model->JJr0)
            model->JJrn = model->JJr0;

        temp = model->JJdelv/2.0;
        if (model->JJvg < temp) {
            (*(SPfrontEnd->IFerror))(ERR_WARNING,
                "Warning: vg entry was < delv/2, reset to delv/2\n",
                (IFuid *)NULL);
            model->JJvg = temp;
        }
        model->JJvless  = model->JJvg - temp;
        model->JJvmore  = model->JJvg + temp;

        temp = model->JJvg/2;
        if (model->JJcap != 0.0) {
            model->JJvdpbak = sqrt(3.291e-16 * model->JJcriti / model->JJcap);
            if (model->JJvdpbak > temp) model->JJvdpbak = temp;
        }
        else
            model->JJvdpbak = temp;

        /* loop through all the instances of the model */
        for (here = model->JJinstances; here != NULL;
                here = here->JJnextInstance) {
            
            if (!here->JJareaGiven) here->JJarea = 1;

            here->JJg0 = here->JJarea / model->JJr0;
            here->JJgn = here->JJarea / model->JJrn;
            here->JJgs = (here->JJgn * model->JJvmore -
                          here->JJg0 * model->JJvless) / model->JJdelv;

            if (model->JJrtype == 3) {

                /* 5th order polynomial model for NbN.
                 *
                 * cj = g0*vj + g1*vj**3 + g2*vj**5,
                 * gj = dcj/dvj = g0 + 3*g1*vj**2 + 5*g2*vj**4.
                 *
                 * Required:
                 * (1) cj(vmore) = g0*vmore + g1*vmore**3 + g2*vmore**5
                 *                 = gn*vmore.
                 * (2) gj(vmore)
                 *               = g0 + 3*g1*vmore**2 + 5*g2*vmore**4
                 *               = gs.
                 * (3) gj(0) = g0 (trivially satisfied).
                 *
                 * thus: -4*g0 - 2*g1*vmore**2 = (gs - 5*gn), or
                 *        g1 = (5*gn - gs - 4*g0)/(2*vmore**2),
                 *
                 * and   -2*g0 + 2*g2*vmore**4 = (gs - 3*gn), or
                 *        g2 = (gs - 3*gn + 2*g0)/(2*vmore**4).
                 *
                 */

                here->JJg1 = 2.5*here->JJgn - 0.5*here->JJgs - 2.0*here->JJg0;

                temp = 1.0/model->JJvmore;
                temp *= temp;
                here->JJg1 *= temp;

                here->JJg2 = 0.5*here->JJgs - 1.5*here->JJgn + here->JJg0;

                temp *= temp;
                here->JJg2 *= temp;
            }
            else {
                here->JJg1 = 0;
                here->JJg2 = 0;
            }    

            here->JJcriti = model->JJcriti * here->JJarea;
            here->JJcap   = model->JJcap   * here->JJarea;

            /* jj's are "present" only if one has critical current */
            if (here->JJcriti > 0 && model->JJictype != 0)
                ckt->CKTjjPresent = 1;

            here->JJstate = *states;

            /* state vector size */
            *states += 7;

            here->JJinitControl = 0;
            if (here->JJcontrolGiven) {
	        here->JJbranch = 0; /*CKTfndBranch(ckt,here->JJcontrol); //BUG//*/

                if (here->JJbranch == 0) {
                    emsg = MALLOC(200);
                    (void) sprintf(emsg,
"Warning: %s control current modulated by non-existent\n\
or non-branch device %s, ignored.",here->JJname,here->JJcontrol);
                    (*(SPfrontEnd->IFerror))(ERR_WARNING,emsg,(IFuid *)NULL);
                    FREE(emsg);
                    here->JJcontrol = NULL;
                }
            }

            TSTALLOC(JJposPosPtr,JJposNode,JJposNode)
            TSTALLOC(JJnegNegPtr,JJnegNode,JJnegNode)
            TSTALLOC(JJnegPosPtr,JJnegNode,JJposNode)
            TSTALLOC(JJposNegPtr,JJposNode,JJnegNode)
            if (here->JJcontrol) {
                TSTALLOC(JJposIbrPtr,JJposNode,JJbranch)
                TSTALLOC(JJnegIbrPtr,JJnegNode,JJbranch)
            }
            if (here->JJphsNode) {
                TSTALLOC(JJphsPhsPtr,JJphsNode,JJphsNode)
                /* preload constant */
                if (ckt->CKTpreload)
                    *(here->JJphsPhsPtr) = 1;
            }
        }
    }

    return (OK);
}
