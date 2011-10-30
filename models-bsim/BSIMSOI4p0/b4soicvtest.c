/***  B4SOI 11/30/2005 Xuemei (Jane) Xi Release   ***/

static char rcsid[] = "$Id: b4soicvtest.c  11/30/2005 Jane Xi Release $";

/**********
 * Copyright 2005 Regents of the University of California.  All rights reserved.
 * Authors: 1998 Samuel Fung, Dennis Sinitsky and Stephen Tang
 * Authors: 1999-2004 Pin Su, Hui Wan, Wei Jin, b3soicvtest.c
 * Authors: 2005- Hui Wan, Xuemei Xi, Ali Niknejad, Chenming Hu.
 * File: b4soicvtest.c
 * Modified by Hui Wan, Xuemei Xi 11/30/2005
 **********/

#include "spice.h"
#include <stdio.h>
#include <math.h>
#include "util.h"
#include "cktdefs.h"
#include "b4soidef.h"
#include "trandefs.h"
#include "const.h"
#include "devdefs.h"
#include "sperror.h"
#include "suffix.h"


int
B4SOIconvTest(inModel,ckt)
GENmodel *inModel;
register CKTcircuit *ckt;
{
register B4SOImodel *model = (B4SOImodel*)inModel;
register B4SOIinstance *here;
double delvbd, delvbs, delvds, delvgd, delvgs, vbd, vbs, vds;
double cbd, cbhat, cbs, cd, cdhat, tol, vgd, vgdo, vgs;

    /*  loop through all the B4SOI device models */
    for (; model != NULL; model = model->B4SOInextModel)
    {    /* loop through all the instances of the model */
         for (here = model->B4SOIinstances; here != NULL ;
              here=here->B4SOInextInstance) 
	 {    vbs = model->B4SOItype 
		  * (*(ckt->CKTrhsOld+here->B4SOIbNode) 
		  - *(ckt->CKTrhsOld+here->B4SOIsNodePrime));
              vgs = model->B4SOItype
		  * (*(ckt->CKTrhsOld+here->B4SOIgNode) 
		  - *(ckt->CKTrhsOld+here->B4SOIsNodePrime));
              vds = model->B4SOItype
		  * (*(ckt->CKTrhsOld+here->B4SOIdNodePrime) 
		  - *(ckt->CKTrhsOld+here->B4SOIsNodePrime));
              vbd = vbs - vds;
              vgd = vgs - vds;
              vgdo = *(ckt->CKTstate0 + here->B4SOIvgs) 
		   - *(ckt->CKTstate0 + here->B4SOIvds);
              delvbs = vbs - *(ckt->CKTstate0 + here->B4SOIvbs);
              delvbd = vbd - *(ckt->CKTstate0 + here->B4SOIvbd);
              delvgs = vgs - *(ckt->CKTstate0 + here->B4SOIvgs);
              delvds = vds - *(ckt->CKTstate0 + here->B4SOIvds);
              delvgd = vgd-vgdo;

              cd = here->B4SOIcd;
              if (here->B4SOImode >= 0)
	      {   cdhat = cd - here->B4SOIgjdb * delvbd 
			+ here->B4SOIgmbs * delvbs + here->B4SOIgm * delvgs
			+ here->B4SOIgds * delvds;
              }
	      else
	      {   cdhat = cd - (here->B4SOIgjdb - here->B4SOIgmbs) * delvbd 
			- here->B4SOIgm * delvgd + here->B4SOIgds * delvds;
              }

            /*
             *  check convergence
             */
              if ((here->B4SOIoff == 0)  || (!(ckt->CKTmode & MODEINITFIX)))
	      {   tol = ckt->CKTreltol * MAX(FABS(cdhat), FABS(cd))
		      + ckt->CKTabstol;
                  if (FABS(cdhat - cd) >= tol)
		  {   ckt->CKTnoncon++;
                      return(OK);
                  } 
                  cbs = here->B4SOIcjs;
                  cbd = here->B4SOIcjd;
                  cbhat = cbs + cbd + here->B4SOIgjdb * delvbd 
		        + here->B4SOIgjsb * delvbs;
                  tol = ckt->CKTreltol * MAX(FABS(cbhat), FABS(cbs + cbd))
		      + ckt->CKTabstol;
                  if (FABS(cbhat - (cbs + cbd)) > tol) 
		  {   ckt->CKTnoncon++;
                      return(OK);
                  }
              }
         }
    }
    return(OK);
}

