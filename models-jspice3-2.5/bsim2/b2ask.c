/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1988 Hong J. Park
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include <math.h>
#include "bsim2def.h"
#include "sperror.h"
#include "util.h"


/*ARGSUSED*/
int
B2ask(ckt,inst,which,value,select)

CKTcircuit *ckt;
GENinstance *inst;
int which;
IFvalue *value;
IFvalue *select;
{
    B2instance *here = (B2instance*)inst;

    switch (which) {

        case BSIM2_L:
            value->rValue = here->B2l;
            break;
        case BSIM2_W:
            value->rValue = here->B2w;
            break;
        case BSIM2_AS:
            value->rValue = here->B2sourceArea;
            break;
        case BSIM2_AD:
            value->rValue = here->B2drainArea;
            break;
        case BSIM2_PS:
            value->rValue = here->B2sourcePerimeter;
            break;
        case BSIM2_PD:
            value->rValue = here->B2drainPerimeter;
            break;
        case BSIM2_NRS:
            value->rValue = here->B2sourceSquares;
            break;
        case BSIM2_NRD:
            value->rValue = here->B2drainSquares;
            break;
        case BSIM2_OFF:
            value->rValue = here->B2off;
            break;
        case BSIM2_IC_VBS:
            value->rValue = here->B2icVBS;
            break;
        case BSIM2_IC_VDS:
            value->rValue = here->B2icVDS;
            break;
        case BSIM2_IC_VGS:
            value->rValue = here->B2icVGS;
            break;
        case BSIM2_DNODE:
            value->iValue = here->B2dNode;
            break;
        case BSIM2_GNODE:
            value->iValue = here->B2gNode;
            break;
        case BSIM2_SNODE:
            value->iValue = here->B2sNode;
            break;
        case BSIM2_BNODE:
            value->iValue = here->B2bNode;
            break;
        case BSIM2_DNODEPRIME:
            value->iValue = here->B2dNodePrime;
            break;
        case BSIM2_SNODEPRIME:
            value->iValue = here->B2sNodePrime;
            break;
        case BSIM2_SOURCECONDUCT:
            value->rValue = here->B2sourceConductance;
            break;
        case BSIM2_DRAINCONDUCT:
            value->rValue = here->B2drainConductance;
            break;
        case BSIM2_VBD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B2vbd);
            break;
        case BSIM2_VBS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B2vbs);
            break;
        case BSIM2_VGS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B2vgs);
            break;
        case BSIM2_VDS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B2vds);
            break;
        case BSIM2_CD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B2cd); 
            break;
        case BSIM2_CBS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B2cbs); 
            break;
        case BSIM2_CBD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B2cbd); 
            break;
        case BSIM2_GM:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B2gm); 
            break;
        case BSIM2_GDS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B2gds); 
            break;
        case BSIM2_GMBS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B2gmbs); 
            break;
        case BSIM2_GBD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B2gbd); 
            break;
        case BSIM2_GBS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B2gbs); 
            break;
        case BSIM2_QB:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B2qb); 
            break;
        case BSIM2_CQB:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B2cqb); 
            break;
        case BSIM2_QG:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B2qg); 
            break;
        case BSIM2_CQG:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B2cqg); 
            break;
        case BSIM2_QD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B2qd); 
            break;
        case BSIM2_CQD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B2cqd); 
            break;
        case BSIM2_CGG:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B2cggb); 
            break;
        case BSIM2_CGD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B2cgdb); 
            break;
        case BSIM2_CGS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B2cgsb); 
            break;
        case BSIM2_CBG:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B2cbgb); 
            break;
        case BSIM2_CAPBD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B2capbd); 
            break;
        case BSIM2_CQBD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B2cqbd); 
            break;
        case BSIM2_CAPBS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B2capbs); 
            break;
        case BSIM2_CQBS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B2cqbs); 
            break;
        case BSIM2_CDG:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B2cdgb); 
            break;
        case BSIM2_CDD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B2cddb); 
            break;
        case BSIM2_CDS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B2cdsb); 
            break;
        case BSIM2_VON:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B2vono); 
            break;
        case BSIM2_QBS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B2qbs); 
            break;
        case BSIM2_QBD:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->B2qbd); 
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}


