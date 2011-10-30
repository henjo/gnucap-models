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
URCparse(type,ckt,tabp,currentp)

int type;
GENERIC *ckt;
GENERIC *tabp;
GENERIC *currentp;
{
    /* lossy line (lumped approx) parser */

    /* Uname <node> <node> <model> [l=<val>] [n=<val>] */

    INPtables *tab = (INPtables*)tabp;
    card *current = (card*)currentp;

    char *line;     /* the part of the current line left to parse */
    char *name;     /* the device's name */
    char *nname1;   /* the first node's name */
    char *nname2;   /* the second node's name */
    char *nname3;   /* the third node's name */
    GENERIC *node1; /* the first node's node pointer */
    GENERIC *node2; /* the second node's node pointer */
    GENERIC *node3; /* the third node's node pointer */
    int error;      /* error code temporary */
    GENERIC *fast;  /* pointer to the actual instance */
    int waslead;    /* flag to indicate that unlabeled number was found */
    double leadval; /* actual value of unlabeled number */
    char *model;    /* name of the model */
    INPmodel *thismodel; /* pointer to our model descriptor */
    GENERIC *mdfast;     /* pointer to the actual model */
    IFuid uid;      /* uid for default model */

    line = current->line;
    INPgetTok(&line,&name,1);
    INPinsert(&name,tab);

    INPgetTok(&line,&nname1,1);
    INPtermInsert(ckt,&nname1,tab,&node1);

    INPgetTok(&line,&nname2,1);
    INPtermInsert(ckt,&nname2,tab,&node2);

    INPgetTok(&line,&nname3,1);
    INPtermInsert(ckt,&nname3,tab,&node3);

    INPgetTok(&line,&model,1);
    INPinsert(&model,tab);

    current->error = INPgetMod(ckt,model,&thismodel,tab);
    if (thismodel != NULL) {
        if (type != thismodel->INPmodType) {
            LITERR(INPdevErr(NULL))
            return;
        }
        mdfast = (thismodel->INPmodfast);
    }
    else {
        if (!tab->defUmod) {
            /* create deafult U model */
            IFnewUid(ckt,&uid,(IFuid)NULL,"U",UID_MODEL,(GENERIC**)NULL);
            IFC(newModel,(ckt,type,&(tab->defUmod),uid))
        }
        mdfast = tab->defUmod;
    }
    IFC(newInstance,(ckt,mdfast,&fast,name))
    IFC(bindNode,(ckt,fast,1,node1))
    IFC(bindNode,(ckt,fast,2,node2))
    IFC(bindNode,(ckt,fast,3,node3))
    PARSECALL((&line,ckt,type,fast,&leadval,&waslead,tab))
}
