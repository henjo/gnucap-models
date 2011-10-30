/**** BSIM3v3.3.0, Released by Xuemei Xi 07/29/2005 ****/

/**********
 * Copyright 2004 Regents of the University of California. All rights reserved.
 * File: b3del.c of BSIM3v3.3.0
 * Author: 1995 Min-Chie Jeng and Mansun Chan
 * Author: 1997-1999 Weidong Liu.
 * Author: 2001 Xuemei Xi
 * Modified by Xuemei Xi, 10/05, 12/14, 2001.
 **********/

static char rcsid[] = "$Id: b3del.c, v3.3.0 07/29/2005 18:00:00 Xuemei Xi Release $";

#include "spice.h"
#include <stdio.h>
#include "util.h"
#include "bsim3def.h"
#include "sperror.h"
#include "gendefs.h"
#include "suffix.h"


int
BSIM3delete(inModel,name,inInst)
GENmodel *inModel;
IFuid name;
GENinstance **inInst;
{
BSIM3instance **fast = (BSIM3instance**)inInst;
BSIM3model *model = (BSIM3model*)inModel;
BSIM3instance **prev = NULL;
BSIM3instance *here;

    for (; model ; model = model->BSIM3nextModel) 
    {    prev = &(model->BSIM3instances);
         for (here = *prev; here ; here = *prev) 
	 {    if (here->BSIM3name == name || (fast && here==*fast))
	      {   *prev= here->BSIM3nextInstance;
                  FREE(here);
                  return(OK);
              }
              prev = &(here->BSIM3nextInstance);
         }
    }
    return(E_NODEV);
}


