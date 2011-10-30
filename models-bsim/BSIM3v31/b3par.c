/* $Id: b3par.c,v 3.1 96/12/08 19:58:18 yuhua Release $  */
/* 
$Log:	b3par.c,v $
 * Revision 3.1  96/12/08  19:58:18  yuhua
 * BSIM3v3.1 release
 * 
*/
static char rcsid[] = "$Id: b3par.c,v 3.1 96/12/08 19:58:18 yuhua Release $";

/*************************************/

/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1995 Min-Chie Jeng and Mansun Chan.
File: b3par.c
**********/

#include "spice.h"
#include <stdio.h>
#include "ifsim.h"
#include "bsim3def.h"
#include "util.h"
#include "sperror.h"
#include "suffix.h"

int
BSIM3param(param,value,inst,select)
int param;
IFvalue *value;
GENinstance *inst;
IFvalue *select;
{
    BSIM3instance *here = (BSIM3instance*)inst;
    switch(param) 
    {   case BSIM3_W:
            here->BSIM3w = value->rValue;
            here->BSIM3wGiven = TRUE;
            break;
        case BSIM3_L:
            here->BSIM3l = value->rValue;
            here->BSIM3lGiven = TRUE;
            break;
        case BSIM3_AS:
            here->BSIM3sourceArea = value->rValue;
            here->BSIM3sourceAreaGiven = TRUE;
            break;
        case BSIM3_AD:
            here->BSIM3drainArea = value->rValue;
            here->BSIM3drainAreaGiven = TRUE;
            break;
        case BSIM3_PS:
            here->BSIM3sourcePerimeter = value->rValue;
            here->BSIM3sourcePerimeterGiven = TRUE;
            break;
        case BSIM3_PD:
            here->BSIM3drainPerimeter = value->rValue;
            here->BSIM3drainPerimeterGiven = TRUE;
            break;
        case BSIM3_NRS:
            here->BSIM3sourceSquares = value->rValue;
            here->BSIM3sourceSquaresGiven = TRUE;
            break;
        case BSIM3_NRD:
            here->BSIM3drainSquares = value->rValue;
            here->BSIM3drainSquaresGiven = TRUE;
            break;
        case BSIM3_OFF:
            here->BSIM3off = value->iValue;
            break;
        case BSIM3_IC_VBS:
            here->BSIM3icVBS = value->rValue;
            here->BSIM3icVBSGiven = TRUE;
            break;
        case BSIM3_IC_VDS:
            here->BSIM3icVDS = value->rValue;
            here->BSIM3icVDSGiven = TRUE;
            break;
        case BSIM3_IC_VGS:
            here->BSIM3icVGS = value->rValue;
            here->BSIM3icVGSGiven = TRUE;
            break;
        case BSIM3_NQSMOD:
            here->BSIM3nqsMod = value->iValue;
            here->BSIM3nqsModGiven = TRUE;
            break;
        case BSIM3_IC:
            switch(value->v.numValue){
                case 3:
                    here->BSIM3icVBS = *(value->v.vec.rVec+2);
                    here->BSIM3icVBSGiven = TRUE;
                case 2:
                    here->BSIM3icVGS = *(value->v.vec.rVec+1);
                    here->BSIM3icVGSGiven = TRUE;
                case 1:
                    here->BSIM3icVDS = *(value->v.vec.rVec);
                    here->BSIM3icVDSGiven = TRUE;
                    break;
                default:
                    return(E_BADPARM);
            }
            break;
        default:
            return(E_BADPARM);
    }
    return(OK);
}



