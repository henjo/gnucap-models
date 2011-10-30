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


int
SWsetup(matrix,inModel,ckt,states)

SMPmatrix *matrix;
GENmodel *inModel;
CKTcircuit *ckt;
int *states;
/* load the switch conductance with those pointers needed later 
 * for fast matrix loading 
 */
{
    SWmodel *model = (SWmodel *)inModel;
    SWinstance *here;

    /* loop through all the current source models */
    for ( ; model != NULL; model = model->SWnextModel) {
        /* Default Value Processing for Switch Model */
        if (!model->SWvThreshGiven) {
            model->SWvThreshold = 0;
        } 
        if (!model->SWvHystGiven) {
            model->SWvHysteresis = 0;
        } 
        if (!model->SWiThreshGiven) {
            model->SWiThreshold = 0;
        } 
        if (!model->SWiHystGiven) {
            model->SWiHysteresis = 0;
        } 
        if (!model->SWonGiven)  {
            model->SWonConduct = SW_ON_CONDUCTANCE;
            model->SWonResistance = 1.0/model->SWonConduct;
        } 
        if (!model->SWoffGiven)  {
            model->SWoffConduct = SW_OFF_CONDUCTANCE;
            model->SWoffResistance = 1.0/model->SWoffConduct;
        }

        /* loop through all the instances of the model */
        for (here = model->SWinstances; here != NULL;
                here = here->SWnextInstance) {

            if (here->SWcontName) {
                here->SWcontBranch = CKTfndBranch(ckt,here->SWcontName);
                if (here->SWcontBranch == 0) {
                    IFuid namarray[2];
                    namarray[0] = here->SWname;
                    namarray[1] = here->SWcontName;
                    (*(SPfrontEnd->IFerror))(ERR_FATAL,
                            "%s: unknown controlling source %s",namarray);
                    return(E_BADPARM);
                }
            }

            TSTALLOC(SWposPosptr, SWposNode, SWposNode)
            TSTALLOC(SWposNegptr, SWposNode, SWnegNode)
            TSTALLOC(SWnegPosptr, SWnegNode, SWposNode)
            TSTALLOC(SWnegNegptr, SWnegNode, SWnegNode)
            here->SWstate = *states;
            *states += SW_NUM_STATES;
        }
    }
    return (OK);
}
