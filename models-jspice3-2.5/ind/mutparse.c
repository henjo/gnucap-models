/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1987 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "inpdefs.h"
#include "inpmacs.h"

void
MUTparse(type,ckt,tabp,currentp)

int  type;
GENERIC *ckt;
GENERIC *tabp;
GENERIC *currentp;
{
    /* mutual inductance parser */

    /* Kname Lname Lname <val> */

    INPtables *tab = (INPtables*)tabp;
    card *current = (card*)currentp;

    char *line;     /* the part of the current line left to parse */
    char *name;     /* the device's name */
    int error;      /* error code temporary */
    GENERIC *fast;  /* pointer to the actual instance */
    IFvalue ptemp;  /* a value structure to package resistance into */
    IFvalue *parm;  /* ptr to a value structure for function return values */
    int waslead;    /* flag to indicate that unlabeled number was found */
    double leadval; /* actual value of unlabeled number */
    IFuid uid;      /* uid for default model */

    line = current->line;
    INPgetTok(&line,&name,1);
    INPinsert(&name,tab);

    if (!tab->defKmod) {
        /* create deafult K model */
        IFnewUid(ckt,&uid,(IFuid)NULL,"K",UID_MODEL,(GENERIC**)NULL);
        IFC(newModel,(ckt,type,&(tab->defKmod),uid))
    }
    IFC(newInstance,(ckt,tab->defKmod,&fast,name))

    parm = INPgetValue(ckt,&line,IF_INSTANCE,tab);
    GCA(INPpName,("inductor1",parm,ckt,type,fast))
    parm = INPgetValue(ckt,&line,IF_INSTANCE,tab);
    GCA(INPpName,("inductor2",parm,ckt,type,fast))

    PARSECALL((&line,ckt,type,fast,&leadval,&waslead,tab))
    if (waslead) {
        ptemp.rValue = leadval;
        GCA(INPpName,("coefficient",&ptemp,ckt,type,fast))
    }
}
