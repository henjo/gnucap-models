/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Gordon M. Jacobs
         1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "swdefs.h"
#include "sperror.h"
#include "util.h"


int
SWload(inModel,ckt)

GENmodel *inModel;
CKTcircuit *ckt;
/* actually load the current values into the 
 * sparse matrix previously provided 
 */
{
    SWmodel *model = (SWmodel *) inModel;
    SWinstance *here;
    double g_now;
    double ctrl;
    double previous_state; 
    double current_state;

    /* loop through all the switch models */
    for ( ; model != NULL; model = model->SWnextModel) {

        /* loop through all the instances of the model */
        for (here = model->SWinstances; here != NULL;
                here = here->SWnextInstance) {

            /* decide the state of the switch */

            if (ckt->CKTmode & (MODEINITFIX|MODEINITJCT)) {

                if (here->SWzero_stateGiven)
                    /* switch specified "on" */
                    current_state = 1.0;
                else
                    current_state = 0.0;
                *(ckt->CKTstate0 + here->SWstate) = current_state;;

            }
            else if (ckt->CKTmode & (MODEINITSMSIG)) {

                previous_state = *(ckt->CKTstate0 + here->SWstate);
                current_state = previous_state;

            }
            else if (ckt->CKTmode & (MODEINITFLOAT)) {

                /* use state0 since INITTRAN or INITPRED already called */
                previous_state = *(ckt->CKTstate0 + here->SWstate);
                current_state = previous_state;

                if (here->SWcontName) {
                    ctrl = *(ckt->CKTrhsOld + here->SWcontBranch);
                    if (ctrl > (model->SWiThreshold+model->SWiHysteresis))
                        current_state = 1.0;
                    else if
                        (ctrl < (model->SWiThreshold-model->SWiHysteresis))
                        current_state = 0.0;
                }
                else {
                    ctrl = *(ckt->CKTrhsOld + here->SWposCntrlNode)
                            - *(ckt->CKTrhsOld + here->SWnegCntrlNode);
                    if (ctrl > (model->SWvThreshold+model->SWvHysteresis))
                        current_state = 1.0;
                    else if
                        (ctrl < (model->SWvThreshold-model->SWvHysteresis))
                        current_state = 0.0;
                }
                *(ckt->CKTstate0 + here->SWstate) = current_state;

                if (current_state != previous_state) {
                    ckt->CKTnoncon++;   /* ensure one more iteration */
                    ckt->CKTtroubleElt = (GENinstance *) here;
                }
            }
            else if (ckt->CKTmode & (MODEINITTRAN|MODEINITPRED) ) {

                previous_state = *(ckt->CKTstate1 + here->SWstate);
                current_state = previous_state;

                if (here->SWcontName) {
                    ctrl = *(ckt->CKTrhsOld + here->SWcontBranch);
                    if (ctrl > (model->SWiThreshold+model->SWiHysteresis))
                        current_state = 1.0;
                    else if
                        (ctrl < (model->SWiThreshold-model->SWiHysteresis))
                        current_state = 0.0;
                }
                else {
                    ctrl = *(ckt->CKTrhsOld + here->SWposCntrlNode)
                            - *(ckt->CKTrhsOld + here->SWnegCntrlNode);
                    if (ctrl > (model->SWvThreshold+model->SWvHysteresis))
                        current_state = 1.0;
                    else if
                        (ctrl < (model->SWvThreshold-model->SWvHysteresis))
                        current_state = 0.0;
                }
                *(ckt->CKTstate0 + here->SWstate) = current_state;
            }

            g_now = current_state?(model->SWonConduct):(model->SWoffConduct);
            here->SWcond = g_now;

            *(here->SWposPosptr) += g_now;
            *(here->SWposNegptr) -= g_now;
            *(here->SWnegPosptr) -= g_now;
            *(here->SWnegNegptr) += g_now;
        }
    }
    return (OK);
}
