/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1987 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "inpdefs.h"
#include "inpmacs.h"

void
BJTparse(type,ckt,tabp,currentp)

int type;
GENERIC *ckt;
GENERIC *tabp;
GENERIC *currentp;
{
    /* bipolar transistor parser */

    /* Qname <node> <node> <node> [<node>] <model> [<val>] [OFF]
     *       [IC=<val>,<val>]
     */

    INPtables *tab = (INPtables*)tabp;
    card *current = (card*)currentp;

    char *line;     /* the part of the current line left to parse */
    char *name;     /* the device's name */
    char *nname1;   /* the first node's name */
    char *nname2;   /* the second node's name */
    char *nname3;   /* the third node's name */
    char *nname4;   /* the fourth node's name */
    GENERIC *node1; /* the first node's node pointer */
    GENERIC *node2; /* the second node's node pointer */
    GENERIC *node3; /* the third node's node pointer */
    GENERIC *node4; /* the fourth node's node pointer */
    int error;      /* error code temporary */
    GENERIC *fast;  /* pointer to the actual instance */
    IFvalue ptemp;  /* a value structure to package resistance into */
    int waslead;    /* flag to indicate that unlabeled number was found */
    double leadval; /* actual value of unlabeled number */
    char *model;    /* the name of the model */
    INPmodel *thismodel; /* pointer to model description for user's model */
    GENERIC *mdfast;     /* pointer to the actual model */
    IFuid uid;      /* uid of default model */

    char *groundname = "0";
    char *gname;
    GENERIC *gnode;

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
    if (!*model || INPlookMod(model) ) {
        /* only 3 nodes */
        INPgetTok(&groundname,&gname,1);
        INPgndInsert(ckt,&gname,tab,&node4);
    }
    else {
        nname4 = model;
        INPtermInsert(ckt,&nname4,tab,&node4);
        INPgetTok(&line,&model,1);
    }
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
        if (!tab->defQmod) {
            /* create default Q model */
            IFnewUid(ckt,&uid,(IFuid)NULL,"Q",UID_MODEL,(GENERIC**)NULL);
            IFC(newModel,(ckt,type,&(tab->defQmod),uid))
        }
        mdfast = tab->defQmod;
    }
    IFC(newInstance,(ckt,mdfast,&fast,name))
    IFC(bindNode,(ckt,fast,1,node1))
    IFC(bindNode,(ckt,fast,2,node2))
    IFC(bindNode,(ckt,fast,3,node3))
    IFC(bindNode,(ckt,fast,4,node4))
    PARSECALL((&line,ckt,type,fast,&leadval,&waslead,tab))
    if (waslead) {
        ptemp.rValue = leadval;
        GCA(INPpName,("area",&ptemp,ckt,type,fast))
    }
}
