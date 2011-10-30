/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1987 Kanwar Jit Singh
         1992 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "srcdefs.h"
#include "sperror.h"
#include "util.h"


/* ARGSUSED */
int
SRCparam(ckt,param,value,inst,select)

CKTcircuit *ckt;
int param;
IFvalue *value;
GENinstance *inst;
IFvalue *select;
{
    SRCinstance *here = (SRCinstance *)inst;

    switch (param) {
        case SRC_I:
            if (here->SRCtype)
                return (E_BADPARM);
            here->SRCtype = SRC_I;
            here->SRCtree = value->tValue;
            break;
        case SRC_V:
            if (here->SRCtype)
                return (E_BADPARM);
            here->SRCtype = SRC_V;
            here->SRCtree = value->tValue;
            break;
        case SRC_DEP:
            if (here->SRCdep)
                return (E_BADPARM);
            switch (value->iValue) {
                case 'A':
                    break;
                case 'I':
                    if (here->SRCtype)
                        return (E_BADPARM);
                    here->SRCtype = SRC_I;
                    break;
                case 'V':
                    if (here->SRCtype)
                        return (E_BADPARM);
                    here->SRCtype = SRC_V;
                    break;
                case 'E':
                    if (here->SRCtype)
                        return (E_BADPARM);
                    here->SRCtype = SRC_V;
                    here->SRCdep = SRC_VC;
                    break;
                case 'F':
                    if (here->SRCtype)
                        return (E_BADPARM);
                    here->SRCtype = SRC_I;
                    here->SRCdep = SRC_CC;
                    break;
                case 'G':
                    if (here->SRCtype)
                        return (E_BADPARM);
                    here->SRCtype = SRC_I;
                    here->SRCdep = SRC_VC;
                    break;
                case 'H':
                    if (here->SRCtype)
                        return (E_BADPARM);
                    here->SRCtype = SRC_V;
                    here->SRCdep = SRC_CC;
                    break;
                default:
                    return (E_BADPARM);
            }
            break;
        case SRC_DC:
            here->SRCdcValue = value->rValue;
            here->SRCdcGiven = TRUE;
            break;
        case SRC_AC:
            switch(value->v.numValue) {
                case 2:
                    here->SRCacPhase = *(value->v.vec.rVec+1);
                    here->SRCacPGiven = TRUE;
                case 1:
                    here->SRCacMag = *(value->v.vec.rVec);
                    here->SRCacMGiven = TRUE;
                case 0:
                    here->SRCacGiven = TRUE;
                    break;
                default:
                    return(E_BADPARM);
            }
            break;
        case SRC_AC_MAG:
            here->SRCacMag = value->rValue;
            here->SRCacMGiven = TRUE;
            here->SRCacGiven = TRUE;
            break;
        case SRC_AC_PHASE:
            here->SRCacPhase = value->rValue;
            here->SRCacPGiven = TRUE;
            here->SRCacGiven = TRUE;
            break;
        case SRC_FUNC:
            if (here->SRCtree) return E_BADPARM;
            here->SRCtree = value->tValue;
            break;
        case SRC_D_F1:
            here->SRCdF1given = TRUE;
            here->SRCdGiven = TRUE;
            switch (value->v.numValue) {
                case 2:
                    here->SRCdF1phase = *(value->v.vec.rVec+1);
                    here->SRCdF1mag = *(value->v.vec.rVec);
                    break;
                case 1:
                    here->SRCdF1mag = *(value->v.vec.rVec);
                    here->SRCdF1phase = 0.0;
                    break;
                case 0:
                    here->SRCdF1mag = 1.0;
                    here->SRCdF1phase = 0.0;
                    break;
                default:
                    return(E_BADPARM);
            }
            break;
        case SRC_D_F2:
            here->SRCdF2given = TRUE;
            here->SRCdGiven = TRUE;
            switch (value->v.numValue) {
                case 2:
                    here->SRCdF2phase = *(value->v.vec.rVec+1);
                    here->SRCdF2mag = *(value->v.vec.rVec);
                    break;
                case 1:
                    here->SRCdF2mag = *(value->v.vec.rVec);
                    here->SRCdF2phase = 0.0;
                    break;
                case 0:
                    here->SRCdF2mag = 1.0;
                    here->SRCdF2phase = 0.0;
                    break;
                default:
                    return(E_BADPARM);
            }
            break;
        case SRC_GAIN:
            if (here->SRCccCoeffGiven || here->SRCvcCoeffGiven)
                return (E_BADPARM);
            here->SRCcoeff = value->rValue;
            if (here->SRCdep == SRC_CC)
                here->SRCccCoeffGiven = TRUE;
            else if (here->SRCdep == SRC_VC)
                here->SRCvcCoeffGiven = TRUE;
            else
                return (E_BADPARM);
            break;
        case SRC_CONTROL:
            here->SRCcontName = value->uValue;
            break;
        default:
            return(E_BADPARM);
    }
    return (OK);
}
