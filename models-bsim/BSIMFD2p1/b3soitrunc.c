/* $Id: BSIMFD2.1 99/9/27 Pin Su Release $  */
/*
$Log:   b3soitrunc.c, FD2.1 $
 * Revision 2.1  99/9/27 Pin Su 
 * BSIMFD2.1 release
 *
*/
static char rcsid[] = "$Id: b3soitrunc.c, FD2.1 99/9/27 Pin Su Release $";

/*************************************/

/**********
Copyright 1999 Regents of the University of California.  All rights reserved.
Author: 1998 Samuel Fung, Dennis Sinitsky and Stephen Tang
File: b3soitrunc.c          98/5/01
**********/


#include "spice.h"
#include <stdio.h>
#include <math.h>
#include "cktdefs.h"
#include "b3soidef.h"
#include "sperror.h"
#include "suffix.h"


int
B3SOItrunc(inModel,ckt,timeStep)
GENmodel *inModel;
register CKTcircuit *ckt;
double *timeStep;
{
register B3SOImodel *model = (B3SOImodel*)inModel;
register B3SOIinstance *here;

#ifdef STEPDEBUG
    double debugtemp;
#endif /* STEPDEBUG */

    for (; model != NULL; model = model->B3SOInextModel)
    {    for (here = model->B3SOIinstances; here != NULL;
	      here = here->B3SOInextInstance)
	 {
#ifdef STEPDEBUG
            debugtemp = *timeStep;
#endif /* STEPDEBUG */
            CKTterr(here->B3SOIqb,ckt,timeStep);
            CKTterr(here->B3SOIqg,ckt,timeStep);
            CKTterr(here->B3SOIqd,ckt,timeStep);
#ifdef STEPDEBUG
            if(debugtemp != *timeStep)
	    {  printf("device %s reduces step from %g to %g\n",
                       here->B3SOIname,debugtemp,*timeStep);
            }
#endif /* STEPDEBUG */
        }
    }
    return(OK);
}



