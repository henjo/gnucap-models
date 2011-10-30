/* $Id: BSIMFD2.1 99/9/27 Pin Su Release $  */
/*
$Log:   b3soipar.c, FD2.1 $
 * Revision 2.1  99/9/27 Pin Su 
 * BSIMFD2.1 release
 *
*/
static char rcsid[] = "$Id: b3soipar.c, FD2.1 99/9/27 Pin Su Release $";

/*************************************/

/**********
Copyright 1999 Regents of the University of California.  All rights reserved.
Author: 1998 Samuel Fung, Dennis Sinitsky and Stephen Tang
File: b3soipar.c          98/5/01
**********/


#include "spice.h"
#include <stdio.h>
#include "ifsim.h"
#include "b3soidef.h"
#include "util.h"
#include "sperror.h"
#include "suffix.h"

int
B3SOIparam(param,value,inst,select)
int param;
IFvalue *value;
GENinstance *inst;
IFvalue *select;
{
    B3SOIinstance *here = (B3SOIinstance*)inst;
    switch(param) 
    {   case B3SOI_W:
            here->B3SOIw = value->rValue;
            here->B3SOIwGiven = TRUE;
            break;
        case B3SOI_L:
            here->B3SOIl = value->rValue;
            here->B3SOIlGiven = TRUE;
            break;
        case B3SOI_AS:
            here->B3SOIsourceArea = value->rValue;
            here->B3SOIsourceAreaGiven = TRUE;
            break;
        case B3SOI_AD:
            here->B3SOIdrainArea = value->rValue;
            here->B3SOIdrainAreaGiven = TRUE;
            break;
        case B3SOI_PS:
            here->B3SOIsourcePerimeter = value->rValue;
            here->B3SOIsourcePerimeterGiven = TRUE;
            break;
        case B3SOI_PD:
            here->B3SOIdrainPerimeter = value->rValue;
            here->B3SOIdrainPerimeterGiven = TRUE;
            break;
        case B3SOI_NRS:
            here->B3SOIsourceSquares = value->rValue;
            here->B3SOIsourceSquaresGiven = TRUE;
            break;
        case B3SOI_NRD:
            here->B3SOIdrainSquares = value->rValue;
            here->B3SOIdrainSquaresGiven = TRUE;
            break;
        case B3SOI_OFF:
            here->B3SOIoff = value->iValue;
            break;
        case B3SOI_IC_VBS:
            here->B3SOIicVBS = value->rValue;
            here->B3SOIicVBSGiven = TRUE;
            break;
        case B3SOI_IC_VDS:
            here->B3SOIicVDS = value->rValue;
            here->B3SOIicVDSGiven = TRUE;
            break;
        case B3SOI_IC_VGS:
            here->B3SOIicVGS = value->rValue;
            here->B3SOIicVGSGiven = TRUE;
            break;
        case B3SOI_IC_VES:
            here->B3SOIicVES = value->rValue;
            here->B3SOIicVESGiven = TRUE;
            break;
        case B3SOI_IC_VPS:
            here->B3SOIicVPS = value->rValue;
            here->B3SOIicVPSGiven = TRUE;
            break;
        case B3SOI_BJTOFF:
            here->B3SOIbjtoff = value->iValue;
            here->B3SOIbjtoffGiven= TRUE;
            break;
        case B3SOI_DEBUG:
            here->B3SOIdebugMod = value->iValue;
            here->B3SOIdebugModGiven= TRUE;
            break;
        case B3SOI_RTH0:
            here->B3SOIrth0= value->rValue;
            here->B3SOIrth0Given = TRUE;
            break;
        case B3SOI_CTH0:
            here->B3SOIcth0= value->rValue;
            here->B3SOIcth0Given = TRUE;
            break;
        case B3SOI_NRB:
            here->B3SOIbodySquares = value->rValue;
            here->B3SOIbodySquaresGiven = TRUE;
            break;
        case B3SOI_IC:
            switch(value->v.numValue){
                case 5:
                    here->B3SOIicVPS = *(value->v.vec.rVec+4);
                    here->B3SOIicVPSGiven = TRUE;
                case 4:
                    here->B3SOIicVES = *(value->v.vec.rVec+3);
                    here->B3SOIicVESGiven = TRUE;
                case 3:
                    here->B3SOIicVBS = *(value->v.vec.rVec+2);
                    here->B3SOIicVBSGiven = TRUE;
                case 2:
                    here->B3SOIicVGS = *(value->v.vec.rVec+1);
                    here->B3SOIicVGSGiven = TRUE;
                case 1:
                    here->B3SOIicVDS = *(value->v.vec.rVec);
                    here->B3SOIicVDSGiven = TRUE;
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



