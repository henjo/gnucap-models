/***  B4SOI 03/06/2009 Wenwei Yang Release   ***/

static char rcsid[] = "$Id: b4soirunc.c  03/06/2009 Wenwei Yang Release $";

/**********
 * Copyright 2009 Regents of the University of California.  All rights reserved.
 * Authors: 1998 Samuel Fung, Dennis Sinitsky and Stephen Tang
 * Authors: 1999-2004 Pin Su, Hui Wan, Wei Jin, b3soitrunc.c
 * Authors: 2005- Hui Wan, Xuemei Xi, Ali Niknejad, Chenming Hu.
 * Authors: 2009- Wenwei Yang, Chung-Hsun Lin, Ali Niknejad, Chenming Hu.
 * File: b4soitrunc.c
 * Modified by Hui Wan, Xuemei Xi 11/30/2005
 * Modified by Wenwei Yang, Chung-Hsun Lin, Darsen Lu 03/06/2009
 **********/

#include "spice.h"
#include <stdio.h>
#include <math.h>
#include "cktdefs.h"
#include "b4soidef.h"
#include "sperror.h"
#include "suffix.h"


int
B4SOItrunc(inModel,ckt,timeStep)
GENmodel *inModel;
register CKTcircuit *ckt;
double *timeStep;
{
register B4SOImodel *model = (B4SOImodel*)inModel;
register B4SOIinstance *here;

#ifdef STEPDEBUG
    double debugtemp;
#endif /* STEPDEBUG */

    for (; model != NULL; model = model->B4SOInextModel)
    {    for (here = model->B4SOIinstances; here != NULL;
	      here = here->B4SOInextInstance)
	 {
#ifdef STEPDEBUG
            debugtemp = *timeStep;
#endif /* STEPDEBUG */
            CKTterr(here->B4SOIqb,ckt,timeStep);
            CKTterr(here->B4SOIqg,ckt,timeStep);
            CKTterr(here->B4SOIqd,ckt,timeStep);
#ifdef STEPDEBUG
            if(debugtemp != *timeStep)
	    {  printf("device %s reduces step from %g to %g\n",
                       here->B4SOIname,debugtemp,*timeStep);
            }
#endif /* STEPDEBUG */
        }
    }
    return(OK);
}



