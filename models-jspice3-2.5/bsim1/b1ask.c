/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1988 Hong J. Park
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include <math.h>
#include "bsim1def.h"
#include "sperror.h"
#include "util.h"


/*ARGSUSED*/
int
B1ask(ckt,inst,which,value,select)

CKTcircuit *ckt;
GENinstance *inst;
int which;
IFvalue *value;
IFvalue *select;
{
    B1instance *here = (B1instance*)inst;

    switch (which) {

        case BSIM1_L:
            value->rValue = here->B1l;
            break;
        case BSIM1_W:
            value->rValue = here->B1w;
            break;
        case BSIM1_AS:
            value->rValue = here->B1sourceArea;
            break;
        case BSIM1_AD:
            value->rValue = here->B1drainArea;
            break;
        case BSIM1_PS:
            value->rValue = here->B1sourcePerimeter;
            break;
        case BSIM1_PD:
            value->rValue = here->B1drainPerimeter;
            break;
        case BSIM1_NRS:
            value->rValue = here->B1sourceSquares;
            break;
        case BSIM1_NRD:
            value->rValue = here->B1drainSquares;
            break;
        case BSIM1_OFF:
            value->rValue = here->B1off;
            break;
        case BSIM1_IC_VBS:
            value->rValue = here->B1icVBS;
            break;
        case BSIM1_IC_VDS:
            value->rValue = here->B1icVDS;
            break;
        case BSIM1_IC_VGS:
            value->rValue = here->B1icVGS;
            break;
        case BSIM1_DNODE:
            value->iValue = here->B1dNode;
            break;
        case BSIM1_GNODE:
            value->iValue = here->B1gNode;
            break;
        case BSIM1_SNODE:
            value->iValue = here->B1sNode;
            break;
        case BSIM1_BNODE:
            value->iValue = here->B1bNode;
            break;
        case BSIM1_DNODEPRIME:
            value->iValue = here->B1dNodePrime;
            break;
        case BSIM1_SNODEPRIME:
            value->iValue = here->B1sNodePrime;
            break;
        case BSIM1_SOURCECONDUCT:
            value->rValue = here->B1sourceConductance;
            break;
        case BSIM1_DRAINCONDUCT:
            value->rValue = here->B1drainConductance;
            break;
        case BSIM1_VBD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B1vbd);
            break;
        case BSIM1_VBS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B1vbs);
            break;
        case BSIM1_VGS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B1vgs);
            break;
        case BSIM1_VDS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B1vds);
            break;
        case BSIM1_CD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B1cd); 
            break;
        case BSIM1_CBS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B1cbs); 
            break;
        case BSIM1_CBD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B1cbd); 
            break;
        case BSIM1_GM:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B1gm); 
            break;
        case BSIM1_GDS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B1gds); 
            break;
        case BSIM1_GMBS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B1gmbs); 
            break;
        case BSIM1_GBD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B1gbd); 
            break;
        case BSIM1_GBS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B1gbs); 
            break;
        case BSIM1_QB:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B1qb); 
            break;
        case BSIM1_CQB:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B1cqb); 
            break;
        case BSIM1_QG:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B1qg); 
            break;
        case BSIM1_CQG:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B1cqg); 
            break;
        case BSIM1_QD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B1qd); 
            break;
        case BSIM1_CQD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B1cqd); 
            break;
        case BSIM1_CGG:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B1cggb); 
            break;
        case BSIM1_CGD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B1cgdb); 
            break;
        case BSIM1_CGS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B1cgsb); 
            break;
        case BSIM1_CBG:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B1cbgb); 
            break;
        case BSIM1_CAPBD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B1capbd); 
            break;
        case BSIM1_CQBD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B1cqbd); 
            break;
        case BSIM1_CAPBS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B1capbs); 
            break;
        case BSIM1_CQBS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B1cqbs); 
            break;
        case BSIM1_CDG:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B1cdgb); 
            break;
        case BSIM1_CDD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B1cddb); 
            break;
        case BSIM1_CDS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B1cdsb); 
            break;
        case BSIM1_VON:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B1vono); 
            break;
        case BSIM1_QBS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B1qbs); 
            break;
        case BSIM1_QBD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B1qbd); 
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}

