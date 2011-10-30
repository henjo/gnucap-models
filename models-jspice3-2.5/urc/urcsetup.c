/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include <math.h>
#include "urcdefs.h"
#include "sperror.h"
#include "util.h"
#include "cktext.h"


/* ARGSUSED */
int
URCsetup(matrix,inModel,ckt,state)
    SMPmatrix *matrix;
    GENmodel *inModel;
    CKTcircuit *ckt;
    int *state;
        /* create the resistors/capacitors used to model the URC
         */
{
    register URCmodel *model = (URCmodel *)inModel;
    register URCinstance *here;
    int rtype;
    int ctype;
    int dtype;
    CKTnode * lowl;
    CKTnode * lowr;
    CKTnode * hil;
    CKTnode * hir;
    char *nameelt;
    char *namehi;
    CKTnode *nodehi;
    CKTnode *nodelo;
    char *namelo;
    double r;
    double c;
    IFvalue ptemp;
    double p;
    double r0;
    double c0;
    double i0;
    double r1;
    double c1;
    double i1;
    double rd;
    double wnorm;
    double prop;
    int i;
    GENinstance  *fast;
    GENmodel *modfast; /* capacitor or diode model */
    GENmodel *rmodfast; /* resistor model */
    int error;
    IFuid dioUid;
    IFuid resUid;
    IFuid capUid;
    IFuid eltUid;


    rtype = CKTtypelook("Resistor");
    ctype = CKTtypelook("Capacitor");
    dtype = CKTtypelook("Diode");
    /*  loop through all the URC models */
    for( ; model != NULL; model = model->URCnextModel ) {
        if(!model->URCkGiven)
             model->URCk = 1.5;
        if(!model->URCfmaxGiven)
             model->URCfmax = 1e9;
        if(!model->URCrPerLGiven)
             model->URCrPerL = 1000;
        if(!model->URCcPerLGiven)
             model->URCcPerL = 1e-12;

/* may need to put in limits:  k>=1.1, freq <=1e9, rperl >=.1 */

        /* loop through all the instances of the model */
        for (here = model->URCinstances; here != NULL ;
                here=here->URCnextInstance) {
            p = model->URCk;
            r0 = here->URClength * model->URCrPerL;
            c0 = here->URClength * model->URCcPerL;
            i0 = here->URClength * model->URCisPerL;
            if(!here->URClumpsGiven) {
                wnorm = model->URCfmax * r0 * c0 * 2.0 * M_PI;
                here->URClumps=MAX(3.0,log(wnorm*(((p-1)/p)*((p-1)/p)))/log(p));
                if(wnorm <35) here->URClumps=3;
                /* may want to limit lumps to <= 100 or something like that */
            }
            r1 = (r0*(p-1))/((2*(pow(p,(double)here->URClumps)))-2);
            c1 = (c0 * (p-1))/((pow(p,(double)(here->URClumps-1)))*(p+1)-2);
            i1 = (i0 * (p-1))/((pow(p,(double)(here->URClumps-1)))*(p+1)-2);
            rd = here->URClength * here->URClumps * model->URCrsPerL;
            /* may want to check that c1 > 0 */
            prop=1;
            if(model->URCisPerLGiven) {
                error = (*(SPfrontEnd->IFnewUid))(ckt,&dioUid,here->URCname,
                        "diodemod",UID_MODEL,(GENERIC**)NULL);
                if(error) return(error);
                modfast = (GENmodel *)NULL;
                error = CKTmodCrt((GENERIC *)ckt,dtype,(GENERIC**)&modfast,
                        dioUid);
                if(error) return(error);
                ptemp.rValue = c1;
                error= CKTpModName("cjo",&ptemp,ckt,dtype,dioUid,&modfast);
                if(error) return(error);
                ptemp.rValue = rd;
                error = CKTpModName("rs",&ptemp,ckt,dtype,dioUid,&modfast);
                if(error) return(error);
                ptemp.rValue = i1;
                error = CKTpModName("is",&ptemp,ckt,dtype,dioUid,&modfast);
                if(error) return(error);
            } else {
                error = (*(SPfrontEnd->IFnewUid))((GENERIC*)ckt,&capUid,
                        here->URCname, "capmod",UID_MODEL,(GENERIC**)NULL);
                if(error) return(error);
                modfast = (GENmodel *)NULL;
                error = CKTmodCrt((GENERIC *)ckt,ctype,(GENERIC**)&modfast,
                        capUid);
                if(error) return(error);
            }
            error = (*(SPfrontEnd->IFnewUid))(ckt,&resUid,here->URCname,
                    "resmod",UID_MODEL,(GENERIC**)NULL);
            if(error) return(error);
            rmodfast = (GENmodel *)NULL;
            error = CKTmodCrt((GENERIC *)ckt,rtype,(GENERIC**)&rmodfast,resUid);
            if(error) return(error);
            lowl = CKTnum2nod(ckt,here->URCposNode);
            hir = CKTnum2nod(ckt,here->URCnegNode);
            for(i=1;i<=here->URClumps;i++) {
                namehi = (char *)MALLOC(10*sizeof(char));
                (void)sprintf(namehi,"hi%d",i);
                error = CKTmkVolt(ckt,(CKTnode**)&nodehi,here->URCname,namehi);
                if(error) return(error);
                hil = nodehi;
                if(i==here->URClumps) {
                    lowr = hil;
                } else {
                    namelo = (char *)MALLOC(10*sizeof(char));
                    (void)sprintf(namelo,"lo%d",i);
                    error = CKTmkVolt(ckt,(CKTnode**)&nodelo,here->URCname,
                            namelo);
                    if(error) return(error);
                    lowr = nodelo;
                }
                r = prop*r1;
                c = prop*c1;

                nameelt = (char *)MALLOC(10*sizeof(char));
                (void)sprintf(nameelt,"rlo%d",i);
                error = (*(SPfrontEnd->IFnewUid))(ckt,&eltUid,here->URCname,
                        nameelt,UID_INSTANCE, (GENERIC **)NULL);
                if(error) return(error);
                error = CKTcrtElt((GENERIC *)ckt,(GENERIC*)rmodfast,
                        (GENERIC**)&fast,eltUid);
                if(error) return(error);
                error = CKTbindNode((GENERIC*)ckt,(GENERIC*)fast,1,lowl);
                if(error) return(error);
                error = CKTbindNode((GENERIC*)ckt,(GENERIC*)fast,2,lowr);
                if(error) return(error);
                ptemp.rValue = r;
                error = CKTpName("resistance",&ptemp,ckt,rtype,nameelt,&fast);
                if(error) return(error);

                nameelt = (char *)MALLOC(10*sizeof(char));
                (void)sprintf(nameelt,"rhi%d",i);
                error = (*(SPfrontEnd->IFnewUid))(ckt,&eltUid,here->URCname,
                        nameelt,UID_INSTANCE, (GENERIC **)NULL);
                if(error) return(error);
                error = CKTcrtElt((GENERIC *)ckt,(GENERIC *)rmodfast,
                        (GENERIC**)&fast,eltUid);
                if(error) return(error);
                error = CKTbindNode((GENERIC*)ckt,(GENERIC*)fast,1,hil);
                if(error) return(error);
                error = CKTbindNode((GENERIC*)ckt,(GENERIC*)fast,2,hir);
                if(error) return(error);
                ptemp.rValue = r;
                error = CKTpName("resistance",&ptemp,ckt,rtype,nameelt,&fast);
                if(error) return(error);

                if(model->URCisPerLGiven) {
                    /* use diode */
                    nameelt = (char *)MALLOC(10*sizeof(char));
                    (void)sprintf(nameelt,"dlo%d",i);
                    error = (*(SPfrontEnd->IFnewUid))(ckt,&eltUid,
                            here->URCname,nameelt,UID_INSTANCE, 
                            (GENERIC **)NULL);
                    if(error) return(error);
                    error = CKTcrtElt((GENERIC *)ckt,(GENERIC*)modfast,
                            (GENERIC**)&fast, eltUid);
                    if(error) return(error);
                    error = CKTbindNode((GENERIC*)ckt,(GENERIC*)fast,1,lowr);
                    if(error) return(error);
                    error = CKTbindNode((GENERIC*)ckt,(GENERIC*)fast,2,
                            (GENERIC *)CKTnum2nod(ckt, here->URCgndNode));
                    if(error) return(error);
                    ptemp.rValue = prop;
                    error = CKTpName("area",&ptemp,ckt,dtype,nameelt,&fast);
                    if(error) return(error);
                } else {
                    /* use simple capacitor */
                    nameelt = (char *)MALLOC(10*sizeof(char));
                    (void)sprintf(nameelt,"clo%d",i);
                    error = (*(SPfrontEnd->IFnewUid))(ckt,&eltUid,here->URCname
                            ,nameelt,UID_INSTANCE, (GENERIC **)NULL);
                    if(error) return(error);
                    error = CKTcrtElt((GENERIC *)ckt,(GENERIC*)modfast,
                            (GENERIC**)&fast, eltUid);
                    if(error) return(error);
                    error = CKTbindNode((GENERIC*)ckt,(GENERIC*)fast,1,lowr);
                    if(error) return(error);
                    error = CKTbindNode((GENERIC*)ckt,(GENERIC*)fast,2,
                            (GENERIC *)CKTnum2nod(ckt, here->URCgndNode));
                    if(error) return(error);
                    ptemp.rValue = c;
                    error = CKTpName("capacitance",&ptemp,ckt,ctype,nameelt,
                            &fast);
                    if(error) return(error);
                }

                if(i!=here->URClumps){
                    if(model->URCisPerLGiven) {
                        /* use diode */
                        nameelt = (char *)MALLOC(10*sizeof(char));
                        (void)sprintf(nameelt,"dhi%d",i);
                        error = (*(SPfrontEnd->IFnewUid))(ckt,&eltUid,
                                here->URCname,nameelt,UID_INSTANCE,
                                (GENERIC **)NULL);
                        if(error) return(error);
                        error = CKTcrtElt((GENERIC *)ckt,(GENERIC *)modfast,
                                (GENERIC**) &fast,eltUid);
                        if(error) return(error);
                        error = CKTbindNode((GENERIC*)ckt,(GENERIC*)fast,1,hil);
                        if(error) return(error);
                        error = CKTbindNode((GENERIC*)ckt,(GENERIC*)fast,2,
                                (GENERIC *)CKTnum2nod(ckt, here->URCgndNode));
                        if(error) return(error);
                        ptemp.rValue = prop;
                        error=CKTpName("area",&ptemp,ckt,dtype,nameelt,&fast);
                        if(error) return(error);
                    } else {
                        /* use simple capacitor */
                        nameelt = (char *)MALLOC(10*sizeof(char));
                        (void)sprintf(nameelt,"chi%d",i);
                        error = (*(SPfrontEnd->IFnewUid))(ckt,&eltUid,
                                here->URCname,nameelt,UID_INSTANCE,
                                (GENERIC **)NULL);
                        if(error) return(error);
                        error = CKTcrtElt((GENERIC *)ckt,(GENERIC *)modfast,
                                (GENERIC**)&fast,eltUid);
                        if(error) return(error);
                        error = CKTbindNode((GENERIC*)ckt,(GENERIC*)fast,1,hil);
                        if(error) return(error);
                        error = CKTbindNode((GENERIC*)ckt,(GENERIC*)fast,2,
                                (GENERIC*)CKTnum2nod(ckt, here->URCgndNode));
                        if(error) return(error);
                        ptemp.rValue = c;
                        error =CKTpName("capacitance",&ptemp,ckt,ctype,nameelt,
                                &fast);
                        if(error) return(error);
                    }
                }
                prop *= p;
                lowl = lowr;
                hir = hil;
            }
        }
    }
    return(OK);
}
