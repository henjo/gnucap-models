/***  B4SOI 11/30/2005 Xuemei (Jane) Xi Release   ***/

static char rcsid[] = "$Id: b4soidest.c  11/30/2005 Jane Xi Release $";

/**********
 * Copyright 2005 Regents of the University of California.  All rights reserved.
 * Authors: 1998 Samuel Fung, Dennis Sinitsky and Stephen Tang
 * Authors: 1999-2004 Pin Su, Hui Wan, Wei Jin, b3soidest.c
 * Authors: 2005- Hui Wan, Xuemei Xi, Ali Niknejad, Chenming Hu.
 * File: b4soidest.c
 * Modified by Hui Wan, Xuemei Xi 11/30/2005
 **********/

#include "spice.h"
#include <stdio.h>
#include "util.h"
#include "b4soidef.h"
#include "suffix.h"

void
B4SOIdestroy(inModel)
GENmodel **inModel;
{
B4SOImodel **model = (B4SOImodel**)inModel;
B4SOIinstance *here;
B4SOIinstance *prev = NULL;
B4SOImodel *mod = *model;
B4SOImodel *oldmod = NULL;

    for (; mod ; mod = mod->B4SOInextModel)
    {    if(oldmod) FREE(oldmod);
         oldmod = mod;
         prev = (B4SOIinstance *)NULL;
         for (here = mod->B4SOIinstances; here; here = here->B4SOInextInstance)
	 {    if(prev) FREE(prev);
              prev = here;
         }
         if(prev) FREE(prev);
    }
    if(oldmod) FREE(oldmod);
    *model = NULL;
    return;
}



