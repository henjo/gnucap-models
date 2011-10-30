/**********
Copyright 1988-1999 Regents of the University of California.  All rights reserved.
Author: 1988 Thomas L. Quarles
Modified by Pin Su and Jan Feng	99/2/15
**********/

#include "spice.h"
#include <stdio.h>
#include "ifsim.h"
#include "inpdefs.h"
#include "inpmacs.h"
#include "fteext.h"
#include "suffix.h"

void
INP2M(ckt,tab,current)
    GENERIC *ckt;
    INPtables *tab;
    card *current;
{

    /* Mname <node> <node> <node> <node> <model> [L=<val>]
     *       [W=<val>] [AD=<val>] [AS=<val>] [PD=<val>]
     *       [PS=<val>] [NRD=<val>] [NRS=<val>] [OFF] 
     *       [IC=<val>,<val>,<val>] 
     */

int type;   /* the type the model says it is */
char *line; /* the part of the current line left to parse */
char *name; /* the resistor's name */
char *nname1;   /* the first node's name */
char *nname2;   /* the second node's name */
char *nname3;   /* the third node's name */
char *nname4;   /* the fourth node's name */
char *nname5;   /* the fifth node's name */
char *nname6;   /* the sixth node's name */
char *nname7;   /* the seventh node's name */
GENERIC *node1; /* the first node's node pointer */
GENERIC *node2; /* the second node's node pointer */
GENERIC *node3; /* the third node's node pointer */
GENERIC *node4; /* the fourth node's node pointer */
GENERIC *node5; /* the fifth node's node pointer */
GENERIC *node6; /* the sixth node's node pointer */
GENERIC *node7; /* the seventh node's node pointer */
int error;      /* error code temporary */
int nodeflag;   /* flag indicating 4 or 5 nodes */
GENERIC *fast;  /* pointer to the actual instance */
int waslead;    /* flag to indicate that funny unlabeled number was found */
double leadval; /* actual value of unlabeled number */
char *model;    /* the name of the model */
INPmodel *thismodel;    /* pointer to model description for user's model */
GENERIC *mdfast;    /* pointer to the actual model */
IFuid uid;      /* uid for default model */

    nodeflag = 0;  /*  initially specify a 4 terminal device  */
    line = current->line;
    INPgetTok(&line,&name,1);
    INPinsert(&name,tab);
    INPgetTok(&line,&nname1,1);
    INPtermInsert(ckt,&nname1,tab,&node1);
    INPgetTok(&line,&nname2,1);
    INPtermInsert(ckt,&nname2,tab,&node2);
    INPgetTok(&line,&nname3,1);
    INPtermInsert(ckt,&nname3,tab,&node3);
    INPgetTok(&line,&nname4,1);
    INPtermInsert(ckt,&nname4,tab,&node4);

/*  See if 5th token after device specification is a model name  */

    INPgetTok(&line,&nname5,1);  /*  get 5th token  */
    thismodel = (INPmodel *)NULL;
    INPgetMod(ckt,nname5,&thismodel,tab);
    if (thismodel == NULL) {  /*  5th token is not a model in the table  */
        nodeflag = 1;  /*  now specify a 5 node device  */
        INPgetTok(&line,&nname6,1);     /*  get next token  */
        thismodel = (INPmodel *)NULL;
        INPgetMod(ckt,nname6,&thismodel,tab);
        if(thismodel == NULL) {  /*  6th token is not a model in the table  */
           nodeflag = 2;  /*  now specify a 6 node device  */
           INPgetTok(&line,&nname7,1);     /*  get next token  */
           thismodel = (INPmodel *)NULL;
           INPgetMod(ckt,nname7,&thismodel,tab);
           if(thismodel == NULL) {  /*  7th token is not a model in the table  */
             nodeflag = 3;  /*  now specify a 7 node device  */
             INPgetTok(&line,&model,1);  /*  get model name  */
	     INPgetMod(ckt,model,&thismodel,tab);  /*  get pointer to the model  */
             if (thismodel != NULL) {
	        if (thismodel->INPmodType != INPtypelook("B3SOI")) {
	                /*  if model is not variable node B3SOI model, error!  */
                   LITERR("only level 9 B3SOI can have 7 nodes")
                   return;
	        }
	        else {  /*  if looking at B3SOI model, allocate the 7th node  */
                   INPtermInsert(ckt,&nname5,tab,&node5);
                   INPtermInsert(ckt,&nname6,tab,&node6);
                   INPtermInsert(ckt,&nname7,tab,&node7);
	        }
             }
           }
           else {/*  7th token is a model - only have 6 terminal device  */
	        if (thismodel->INPmodType != INPtypelook("B3SOI")) {
	                /*  if model is not variable node B3SOI model, error!  */
                   LITERR("only level 9 B3SOI can have 6 nodes")
                   return;
	        }
	        else {  /*  if looking at B3SOI model, allocate the 6th node  */
                   INPtermInsert(ckt,&nname5,tab,&node5);
                   INPtermInsert(ckt,&nname6,tab,&node6);
                   model = nname7;
	        }
           }
        }
        else { /*  6th token is a model - only have 5 terminal device  */
	        if (thismodel->INPmodType != INPtypelook("B3SOI")) {
	                /*  if model is not variable node B3SOI model, error!  */
                   LITERR("only level 9 B3SOI can have 5 nodes")
                   return;
	        }
	        else {  /*  if looking at B3SOI model, allocate the 5th node  */
                   INPtermInsert(ckt,&nname5,tab,&node5);
                   model = nname6;  /*  make model point to the correct token  */
	        }
        }
    }
    else {  /*  5th token is a model - only have 4 terminal device  */
        model = nname5;  /*  make model point to the correct token  */
    }


    INPinsert(&model,tab);
    thismodel = (INPmodel *)NULL;
    current->error = INPgetMod(ckt,model,&thismodel,tab);
    if(thismodel != NULL) {
        if (thismodel->INPmodType != INPtypelook("Mos1")
                && thismodel->INPmodType != INPtypelook("Mos2")
                && thismodel->INPmodType != INPtypelook("Mos3")
                && thismodel->INPmodType != INPtypelook("Mos5")
                && thismodel->INPmodType != INPtypelook("Mos6")
                && thismodel->INPmodType != INPtypelook("Mos8")
                && thismodel->INPmodType != INPtypelook("BSIM1")
                && thismodel->INPmodType != INPtypelook("BSIM2")
                && thismodel->INPmodType != INPtypelook("BSIM3")
		&& thismodel->INPmodType != INPtypelook("B3SOI")
		)
	{
            LITERR("incorrect model type")
            return;
        }
        type = thismodel->INPmodType;
        mdfast = (thismodel->INPmodfast);
    } 
    else {
	type = INPtypelook("Mos1");
	if(type < 0 ) {
	    LITERR("Device type MOS1 not supported by this binary\n")
	    return;
	}
        if(!tab->defMmod) {
            /* create default M model */
            IFnewUid(ckt,&uid,(IFuid)NULL,"M",UID_MODEL,(GENERIC**)NULL);
            IFC(newModel,(ckt,type,&(tab->defMmod),uid))
        }
        mdfast = tab->defMmod;
    }
    IFC(newInstance,(ckt,mdfast,&fast,name))
    IFC(bindNode,(ckt,fast,1,node1))
    IFC(bindNode,(ckt,fast,2,node2))
    IFC(bindNode,(ckt,fast,3,node3))
    IFC(bindNode,(ckt,fast,4,node4))
    if (thismodel->INPmodType == INPtypelook("B3SOI")) {

      switch (nodeflag) 
      {
	case 0:  
          ((GENinstance *)fast)->GENnode5 = -1;
          ((GENinstance *)fast)->GENnode6 = -1;
          ((GENinstance *)fast)->GENnode7 = -1;
           break;
	case 1:  
          IFC(bindNode,(ckt,fast,5,node5))
          ((GENinstance *)fast)->GENnode6 = -1;
          ((GENinstance *)fast)->GENnode7 = -1;
           break;
	case 2:  
          IFC(bindNode,(ckt,fast,5,node5))
          IFC(bindNode,(ckt,fast,6,node6))
          ((GENinstance *)fast)->GENnode7 = -1;
           break;
	case 3:  
          IFC(bindNode,(ckt,fast,5,node5))
          IFC(bindNode,(ckt,fast,6,node6))
          IFC(bindNode,(ckt,fast,7,node7))
          break;
        default: 
          break;
       }
    }


    PARSECALL((&line,ckt,type,fast,&leadval,&waslead,tab))
    if(waslead) {
        LITERR(" error:  no unlabeled parameter permitted on mosfet\n")
    }
}

