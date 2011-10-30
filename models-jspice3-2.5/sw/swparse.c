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
SWparse(type,ckt,tabp,currentp)

int  type;
GENERIC *ckt;
GENERIC *tabp;
GENERIC *currentp;
{
    /* switch parser */

    /* VOLTAGE CONTROLLED SWITCH */
    /*   Sname <node> <node> <node> <node> [<modname>] [IC] */

    /* CURRENT CONTROLLED SWITCH */
    /*   Wname <node> <node> <vctrl> [<modname>] [IC] */

    INPtables *tab = (INPtables*)tabp;
    card *current = (card*)currentp;

    char *line;     /* the part of the current line left to parse */
    char *name;     /* the device's name */
    char *model;    /* the name of the resistor's model */
    char *nname1;   /* the first node's name */
    char *nname2;   /* the second node's name */
    char *nname3;   /* the third node's name */
    char *nname4;   /* the fourth node's name */
    GENERIC *node1; /* the first node's node pointer */
    GENERIC *node2; /* the second node's node pointer */
    GENERIC *node3; /* the third node's node pointer */
    GENERIC *node4; /* the fourth node's node pointer */
    int error;      /* error code temporary */
    INPmodel *thismodel; /* pointer to model structure describing our model */
    GENERIC *mdfast;     /* pointer to the actual model */
    GENERIC *fast;  /* pointer to the actual instance */
    IFvalue ptemp;  /* a value structure to package resistance into */
    IFvalue *parm;  /* pointer to a value structure for functions to return */
    int waslead;    /* flag to indicate that funny unlabeled number was found */
    double leadval; /* actual value of unlabeled number */
    IFuid uid;      /* uid of default model */
    char key;

    line = current->line;
    key = *line;
    if (islower(key))
        key = toupper(key);

    INPgetTok(&line,&name,1);
    INPinsert(&name,tab);

    INPgetTok(&line,&nname1,1);
    INPtermInsert(ckt,&nname1,tab,&node1);

    INPgetTok(&line,&nname2,1);
    INPtermInsert(ckt,&nname2,tab,&node2);

    if (key == 'S') {
        /* voltage controlled switch */

        INPgetTok(&line,&nname3,1);
        INPtermInsert(ckt,&nname3,tab,&node3);

        INPgetTok(&line,&nname4,1);
        INPtermInsert(ckt,&nname4,tab,&node4);
    }
    else {
        /* current controlled switch */

        parm = INPgetValue(ckt,&line,IF_INSTANCE,tab);
        ptemp.uValue = parm->uValue;
    }
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
        if (!tab->defSmod) {
            /* create deafult S model */
            IFnewUid(ckt,&uid,(IFuid)NULL,"S",UID_MODEL,(GENERIC**)NULL);
            IFC(newModel,(ckt,type,&(tab->defSmod),uid))
        }
        mdfast = tab->defSmod;
    }
    IFC(newInstance,(ckt,mdfast,&fast,name))
    IFC(bindNode,(ckt,fast,1,node1))
    IFC(bindNode,(ckt,fast,2,node2))

    if (key == 'S') {
        IFC(bindNode,(ckt,fast,3,node3))
        IFC(bindNode,(ckt,fast,4,node4))
    }
    else {
        GCA(INPpName,("control",&ptemp,ckt,type,fast))
    }

    PARSECALL((&line,ckt,type,fast,&leadval,&waslead,tab))
    if (waslead) {
        /* ignore a number */
    }
}
