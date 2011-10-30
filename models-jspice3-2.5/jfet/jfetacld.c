/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/
/********** new in 3f2
Sydney University mods Copyright(c) 1989 Anthony E. Parker, David J. Skellern
    Laboratory for Communication Science Engineering
    Sydney University Department of Electrical Engineering, Australia
**********/

#include "spice.h"
#include <stdio.h>
#include "jfetdefs.h"
#include "util.h"
#include "sperror.h"


int
JFETacLoad(inModel,ckt)
    GENmodel *inModel;
    register CKTcircuit *ckt;
{
    register JFETmodel *model = (JFETmodel*)inModel;
    register JFETinstance *here;
    double gdpr;
    double gspr;
    double gm;
    double gds;
    double ggs;
    double xgs;
    double ggd;
    double xgd;

    for( ; model != NULL; model = model->JFETnextModel ) {
        
        for( here = model->JFETinstances; here != NULL; 
                here = here->JFETnextInstance) {


            gdpr=model->JFETdrainConduct * here->JFETarea;
            gspr=model->JFETsourceConduct * here->JFETarea;
            gm= *(ckt->CKTstate0 + here->JFETgm) ;
            gds= *(ckt->CKTstate0 + here->JFETgds) ;
            ggs= *(ckt->CKTstate0 + here->JFETggs) ;
            xgs= *(ckt->CKTstate0 + here->JFETqgs) * ckt->CKTomega ;
            ggd= *(ckt->CKTstate0 + here->JFETggd) ;
            xgd= *(ckt->CKTstate0 + here->JFETqgd) * ckt->CKTomega ;
            *(here->JFETdrainDrainPtr ) += gdpr;
            *(here->JFETgateGatePtr ) += ggd+ggs;
            *(here->JFETgateGatePtr +1) += xgd+xgs;
            *(here->JFETsourceSourcePtr ) += gspr;
            *(here->JFETdrainPrimeDrainPrimePtr ) += gdpr+gds+ggd;
            *(here->JFETdrainPrimeDrainPrimePtr +1) += xgd;
            *(here->JFETsourcePrimeSourcePrimePtr ) += gspr+gds+gm+ggs;
            *(here->JFETsourcePrimeSourcePrimePtr +1) += xgs;
            *(here->JFETdrainDrainPrimePtr ) -= gdpr;
            *(here->JFETgateDrainPrimePtr ) -= ggd;
            *(here->JFETgateDrainPrimePtr +1) -= xgd;
            *(here->JFETgateSourcePrimePtr ) -= ggs;
            *(here->JFETgateSourcePrimePtr +1) -= xgs;
            *(here->JFETsourceSourcePrimePtr ) -= gspr;
            *(here->JFETdrainPrimeDrainPtr ) -= gdpr;
            *(here->JFETdrainPrimeGatePtr ) += (-ggd+gm);
            *(here->JFETdrainPrimeGatePtr +1) -= xgd;
            *(here->JFETdrainPrimeSourcePrimePtr ) += (-gds-gm);
            *(here->JFETsourcePrimeGatePtr ) += (-ggs-gm);
            *(here->JFETsourcePrimeGatePtr +1) -= xgs;
            *(here->JFETsourcePrimeSourcePtr ) -= gspr;
            *(here->JFETsourcePrimeDrainPrimePtr ) -= gds;

        }
    }
    return(OK);
}
