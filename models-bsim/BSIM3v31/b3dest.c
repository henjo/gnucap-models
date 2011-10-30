/* $Id: b3dest.c,v 3.1 96/12/08 19:54:27 yuhua Release $  */
/* 
$Log:	b3dest.c,v $
 * Revision 3.1  96/12/08  19:54:27  yuhua
 * BSIM3v3.1 release
 * 
*/
static char rcsid[] = "$Id: b3dest.c,v 3.1 96/12/08 19:54:27 yuhua Release $";

/*************************************/

/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1995 Min-Chie Jeng and Mansun Chan.
File: b3dest.c
**********/

#include "spice.h"
#include <stdio.h>
#include "util.h"
#include "bsim3def.h"
#include "suffix.h"

void
BSIM3destroy(inModel)
GENmodel **inModel;
{
BSIM3model **model = (BSIM3model**)inModel;
BSIM3instance *here;
BSIM3instance *prev = NULL;
BSIM3model *mod = *model;
BSIM3model *oldmod = NULL;

    for (; mod ; mod = mod->BSIM3nextModel)
    {    if(oldmod) FREE(oldmod);
         oldmod = mod;
         prev = (BSIM3instance *)NULL;
         for (here = mod->BSIM3instances; here; here = here->BSIM3nextInstance)
	 {    if(prev) FREE(prev);
              prev = here;
         }
         if(prev) FREE(prev);
    }
    if(oldmod) FREE(oldmod);
    *model = NULL;
    return;
}



