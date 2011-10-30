/**** BSIM4.5.0 Released by Xuemei (Jane) Xi 07/29/2005 ****/

/**********
 * Copyright 2005 Regents of the University of California. All rights reserved.
 * File: b4del.c of BSIM4.5.0.
 * Author: 2000 Weidong Liu
 * Authors: 2001- Xuemei Xi, Mohan Dunga, Ali Niknejad, Chenming Hu.
 * Project Director: Prof. Chenming Hu.
 **********/

#include "spice.h"
#include <stdio.h>
#include "util.h"
#include "bsim4def.h"
#include "sperror.h"
#include "gendefs.h"
#include "suffix.h"


int
BSIM4delete(inModel,name,inInst)
GENmodel *inModel;
IFuid name;
GENinstance **inInst;
{
BSIM4instance **fast = (BSIM4instance**)inInst;
BSIM4model *model = (BSIM4model*)inModel;
BSIM4instance **prev = NULL;
BSIM4instance *here;

    for (; model ; model = model->BSIM4nextModel) 
    {    prev = &(model->BSIM4instances);
         for (here = *prev; here ; here = *prev) 
	 {    if (here->BSIM4name == name || (fast && here==*fast))
	      {   *prev= here->BSIM4nextInstance;
                  FREE(here);
                  return(OK);
              }
              prev = &(here->BSIM4nextInstance);
         }
    }
    return(E_NODEV);
}
