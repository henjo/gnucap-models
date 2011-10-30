/***  B4SOI 03/06/2009 Wenwei Yang Release   ***/

static char rcsid[] = "$Id: b4soidel.c  03/06/2009 Wenwei Yang Release $";

/**********
 * Copyright 2009 Regents of the University of California.  All rights reserved.
 * Authors: 1998 Samuel Fung, Dennis Sinitsky and Stephen Tang
 * Authors: 1999-2004 Pin Su, Hui Wan, Wei Jin, b3soidel.c
 * Authors: 2005- Hui Wan, Xuemei Xi, Ali Niknejad, Chenming Hu.
 * Authors: 2009- Wenwei Yang, Chung-Hsun Lin, Ali Niknejad, Chenming Hu.
 * File: b4soidel.c
 * Modified by Hui Wan, Xuemei Xi 11/30/2005
 * Modified by Wenwei Yang, Chung-Hsun Lin, Darsen Lu 03/06/2009
 **********/

#include "spice.h"
#include <stdio.h>
#include "util.h"
#include "b4soidef.h"
#include "sperror.h"
#include "gendefs.h"
#include "suffix.h"


int
B4SOIdelete(inModel,name,inInst)
GENmodel *inModel;
IFuid name;
GENinstance **inInst;
{
B4SOIinstance **fast = (B4SOIinstance**)inInst;
B4SOImodel *model = (B4SOImodel*)inModel;
B4SOIinstance **prev = NULL;
B4SOIinstance *here;

    for (; model ; model = model->B4SOInextModel) 
    {    prev = &(model->B4SOIinstances);
         for (here = *prev; here ; here = *prev) 
	 {    if (here->B4SOIname == name || (fast && here==*fast))
	      {   *prev= here->B4SOInextInstance;
                  FREE(here);
                  return(OK);
              }
              prev = &(here->B4SOInextInstance);
         }
    }
    return(E_NODEV);
}


