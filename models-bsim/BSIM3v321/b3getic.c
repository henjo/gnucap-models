/* $Id: b3getic.c, v3.2.1 1999/4/20 18:00:00 Weidong Liu Release $  */
/*
 $Log:  b3getic.c, v3.2.1 $
 * Revision 3.2.1 1999/4/20  18:00:00  Weidong
 * BSIM3v3.2.1 release
 *
*/
static char rcsid[] = "$Id: b3getic.c, v3.2.1 1999/4/20 18:00:00 Weidong Liu Release $";

/*************************************/

/**********
Copyright 1999 Regents of the University of California.  All rights reserved.
Author: 1995 Min-Chie Jeng and Mansun Chan.
Author: 1997-1999 Weidong Liu.
File: b3getic.c
**********/

#include "spice.h"
#include <stdio.h>
#include "cktdefs.h"
#include "bsim3def.h"
#include "sperror.h"
#include "suffix.h"


int
BSIM3getic(inModel,ckt)
GENmodel *inModel;
CKTcircuit *ckt;
{
BSIM3model *model = (BSIM3model*)inModel;
BSIM3instance *here;

    for (; model ; model = model->BSIM3nextModel) 
    {    for (here = model->BSIM3instances; here; here = here->BSIM3nextInstance)
	 {    if(!here->BSIM3icVBSGiven) 
	      {  here->BSIM3icVBS = *(ckt->CKTrhs + here->BSIM3bNode) 
				  - *(ckt->CKTrhs + here->BSIM3sNode);
              }
              if (!here->BSIM3icVDSGiven) 
	      {   here->BSIM3icVDS = *(ckt->CKTrhs + here->BSIM3dNode) 
				   - *(ckt->CKTrhs + here->BSIM3sNode);
              }
              if (!here->BSIM3icVGSGiven) 
	      {   here->BSIM3icVGS = *(ckt->CKTrhs + here->BSIM3gNode) 
				   - *(ckt->CKTrhs + here->BSIM3sNode);
              }
         }
    }
    return(OK);
}


