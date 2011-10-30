/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1987 Kanwar Jit Singh
         1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "srcdefs.h"
#include "tskdefs.h"
#include "sperror.h"
#include "util.h"
#include "cktext.h"


/* ARGSUSED */
int
SRCask(ckt,inst,which,value,select)

/*
 * This routine gives access to the internal device parameters
 * of Sources
 */

CKTcircuit *ckt;
GENinstance *inst;
int which;
IFvalue *value;
IFvalue *select;
{
    SRCinstance *here = (SRCinstance*)inst;
    static char *msg =  "Current and power not available in ac analysis";

    switch (which) {
        case SRC_DEP:
            value->iValue = here->SRCdep;
            break;
        case SRC_DC:
            value->rValue = here->SRCdcValue;
            break;
        case SRC_AC:
            value->v.numValue = 2;
            value->v.vec.rVec = here->SRCacVec;
            break;
        case SRC_AC_MAG:
            value->rValue = here->SRCacMag;
            break;
        case SRC_AC_PHASE:
            value->rValue = here->SRCacPhase;
            break;
        case SRC_FUNC:
            value->tValue = here->SRCtree;
            break;
        case SRC_GAIN:
            value->rValue = here->SRCcoeff;
            break;
        case SRC_CONTROL:
            value->uValue = here->SRCcontName;
            break;
        case SRC_POS_NODE:
            value->iValue = here->SRCposNode;
            break;
        case SRC_NEG_NODE:
            value->iValue = here->SRCnegNode;
            break;
        case SRC_AC_REAL:
            value->rValue = here->SRCacReal;
            break;
        case SRC_AC_IMAG:
            value->rValue = here->SRCacImag;
            break;
        case SRC_CURRENT:
            if (ckt->CKTcurrentAnalysis & DOING_AC) {
                errMsg = MALLOC(strlen(msg)+1);
                errRtn = "SRCask";
                strcpy(errMsg,msg);
                return (E_ASKCURRENT);
            }
            if (ckt->CKTrhsOld)
                value->rValue = *(ckt->CKTrhsOld+here->SRCbranch);
            break;
        case SRC_POWER:
            if (ckt->CKTcurrentAnalysis & DOING_AC) {
                errMsg = MALLOC(strlen(msg)+1);
                errRtn = "SRCask";
                strcpy(errMsg,msg);
                return (E_ASKPOWER);
            }
            if (ckt->CKTrhsOld)
                value->rValue = (*(ckt->CKTrhsOld+here->SRCposNode)
                    - *(ckt->CKTrhsOld + here->SRCnegNode)) * 
                    -*(ckt->CKTrhsOld + here->SRCbranch);
            break;
        case SRC_CONT_P_NODE:
            value->iValue = here->SRCcontPosNode;
            break;
        case SRC_CONT_N_NODE:
            value->iValue = here->SRCcontNegNode;
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}
