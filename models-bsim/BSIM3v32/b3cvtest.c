/* $Id: b3cvtest.c, v3.2 1998/6/16 18:00:00 Weidong Liu Release $  */
/*
 $Log:  b3cvtest.c, v3.2 $
 * Revision 3.2 1998/6/16  18:00:00  Weidong 
 * BSIM3v3.2 release
 *
*/
static char rcsid[] = "$Id: b3cvtest.c, v3.2 1998/6/16 18:00:00 Weidong Liu Release $";

/*************************************/

/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1995 Min-Chie Jeng and Mansun Chan.
File: b3cvtest.c
**********/

#include "spice.h"
#include <stdio.h>
#include <math.h>
#include "util.h"
#include "cktdefs.h"
#include "bsim3def.h"
#include "trandefs.h"
#include "const.h"
#include "devdefs.h"
#include "sperror.h"
#include "suffix.h"


int
BSIM3convTest(inModel,ckt)
GENmodel *inModel;
register CKTcircuit *ckt;
{
register BSIM3model *model = (BSIM3model*)inModel;
register BSIM3instance *here;
double delvbd, delvbs, delvds, delvgd, delvgs, vbd, vbs, vds;
double cbd, cbhat, cbs, cd, cdhat, tol, vgd, vgdo, vgs;

    /*  loop through all the BSIM3 device models */
    for (; model != NULL; model = model->BSIM3nextModel)
    {    /* loop through all the instances of the model */
         for (here = model->BSIM3instances; here != NULL ;
              here=here->BSIM3nextInstance) 
	 {    vbs = model->BSIM3type 
		  * (*(ckt->CKTrhsOld+here->BSIM3bNode) 
		  - *(ckt->CKTrhsOld+here->BSIM3sNodePrime));
              vgs = model->BSIM3type
		  * (*(ckt->CKTrhsOld+here->BSIM3gNode) 
		  - *(ckt->CKTrhsOld+here->BSIM3sNodePrime));
              vds = model->BSIM3type
		  * (*(ckt->CKTrhsOld+here->BSIM3dNodePrime) 
		  - *(ckt->CKTrhsOld+here->BSIM3sNodePrime));
              vbd = vbs - vds;
              vgd = vgs - vds;
              vgdo = *(ckt->CKTstate0 + here->BSIM3vgs) 
		   - *(ckt->CKTstate0 + here->BSIM3vds);
              delvbs = vbs - *(ckt->CKTstate0 + here->BSIM3vbs);
              delvbd = vbd - *(ckt->CKTstate0 + here->BSIM3vbd);
              delvgs = vgs - *(ckt->CKTstate0 + here->BSIM3vgs);
              delvds = vds - *(ckt->CKTstate0 + here->BSIM3vds);
              delvgd = vgd-vgdo;

              cd = here->BSIM3cd - here->BSIM3cbd;
              if (here->BSIM3mode >= 0)
	      {   cd += here->BSIM3csub;
		  cdhat = cd - here->BSIM3gbd * delvbd 
			+ (here->BSIM3gmbs + here->BSIM3gbbs) * delvbs
			+ (here->BSIM3gm + here->BSIM3gbgs) * delvgs
			+ (here->BSIM3gds + here->BSIM3gbds) * delvds;
              }
	      else
	      {   cdhat = cd + (here->BSIM3gmbs - here->BSIM3gbd) * delvbd 
			+ here->BSIM3gm * delvgd - here->BSIM3gds * delvds;
              }

            /*
             *  check convergence
             */
              if ((here->BSIM3off == 0)  || (!(ckt->CKTmode & MODEINITFIX)))
	      {   tol = ckt->CKTreltol * MAX(FABS(cdhat), FABS(cd))
		      + ckt->CKTabstol;
                  if (FABS(cdhat - cd) >= tol)
		  {   ckt->CKTnoncon++;
                      return(OK);
                  } 
                  cbs = here->BSIM3cbs;
                  cbd = here->BSIM3cbd;
                  if (here->BSIM3mode >= 0)
		  {   cbhat = cbs + cbd - here->BSIM3csub
			    + here->BSIM3gbd * delvbd 
		            + (here->BSIM3gbs - here->BSIM3gbbs) * delvbs
			    - here->BSIM3gbgs * delvgs
			    - here->BSIM3gbds * delvds;
		  }
		  else
		  {   cbhat = cbs + cbd - here->BSIM3csub 
		            + here->BSIM3gbs * delvbs
			    + (here->BSIM3gbd - here->BSIM3gbbs) * delvbd 
			    - here->BSIM3gbgs * delvgd
			    + here->BSIM3gbds * delvds;
		  }
                  tol = ckt->CKTreltol * MAX(FABS(cbhat), 
			FABS(cbs + cbd - here->BSIM3csub)) + ckt->CKTabstol;
                  if (FABS(cbhat - (cbs + cbd - here->BSIM3csub)) > tol) 
		  {   ckt->CKTnoncon++;
                      return(OK);
                  }
              }
         }
    }
    return(OK);
}


