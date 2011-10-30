/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1987 Mathew Lew and Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "bjtdefs.h"
#include "sperror.h"
#include "const.h"


/*ARGSUSED*/
int
BJTmAsk(ckt,instPtr,which,value)

CKTcircuit *ckt;
GENmodel *instPtr;
int which;
IFvalue *value;
{
    BJTmodel *here = (BJTmodel*)instPtr;

    switch(which) {

        case BJT_MOD_IS:
            value->rValue = here->BJTsatCur;
            break;
        case BJT_MOD_BF:
            value->rValue = here->BJTbetaF;
            break;
        case BJT_MOD_NF:
            value->rValue = here->BJTemissionCoeffF;
            break;
        case BJT_MOD_VAF:
            value->rValue = here->BJTearlyVoltF;
            break;
        case BJT_MOD_IKF:
            value->rValue = here->BJTrollOffF;
            break;
        case BJT_MOD_ISE:
            value->rValue = here->BJTleakBEcurrent;
            break;
        case BJT_MOD_NE:
            value->rValue = here->BJTleakBEemissionCoeff;
            break;
        case BJT_MOD_BR:
            value->rValue = here->BJTbetaR;
            break;
        case BJT_MOD_NR:
            value->rValue = here->BJTemissionCoeffR;
            break;
        case BJT_MOD_VAR:
            value->rValue = here->BJTearlyVoltR;
            break;
        case BJT_MOD_IKR:
            value->rValue = here->BJTrollOffR;
            break;
        case BJT_MOD_ISC:
            value->rValue = here->BJTleakBCcurrent;
            break;
        case BJT_MOD_NC:
            value->rValue = here->BJTleakBCemissionCoeff;
            break;
        case BJT_MOD_RB:
            value->rValue = here->BJTbaseResist;
            break;
        case BJT_MOD_IRB:
            value->rValue = here->BJTbaseCurrentHalfResist;
            break;
        case BJT_MOD_RBM:
            value->rValue = here->BJTminBaseResist;
            break;
        case BJT_MOD_RE:
            value->rValue = here->BJTemitterResist;
            break;
        case BJT_MOD_RC:
            value->rValue = here->BJTcollectorResist;
            break;
        case BJT_MOD_CJE:
            value->rValue = here->BJTdepletionCapBE;
            break;
        case BJT_MOD_VJE:
            value->rValue = here->BJTpotentialBE;
            break;
        case BJT_MOD_MJE:
            value->rValue = here->BJTjunctionExpBE;
            break;
        case BJT_MOD_TF:
            value->rValue = here->BJTtransitTimeF;
            break;
        case BJT_MOD_XTF:
            value->rValue = here->BJTtransitTimeBiasCoeffF;
            break;
        case BJT_MOD_VTF:
            value->rValue = here->BJTtransitTimeFVBC;
            break;
        case BJT_MOD_ITF:
            value->rValue = here->BJTtransitTimeHighCurrentF;
            break;
        case BJT_MOD_PTF:
            value->rValue = here->BJTexcessPhase;
            break;
        case BJT_MOD_CJC:
            value->rValue = here->BJTdepletionCapBC;
            break;
        case BJT_MOD_VJC:
            value->rValue = here->BJTpotentialBC;
            break;
        case BJT_MOD_MJC:
            value->rValue = here->BJTjunctionExpBC;
            break;
        case BJT_MOD_XCJC:
            value->rValue = here->BJTbaseFractionBCcap;
            break;
        case BJT_MOD_TR:
            value->rValue = here->BJTtransitTimeR;
            break;
        case BJT_MOD_CJS:
            value->rValue = here->BJTcapCS;
            break;
        case BJT_MOD_VJS:
            value->rValue = here->BJTpotentialSubstrate;
            break;
        case BJT_MOD_MJS:
            value->rValue = here->BJTexponentialSubstrate;
            break;
        case BJT_MOD_XTB:
            value->rValue = here->BJTbetaExp;
            break;
        case BJT_MOD_EG:
            value->rValue = here->BJTenergyGap;
            break;
        case BJT_MOD_XTI:
            value->rValue = here->BJTtempExpIS;
            break;
        case BJT_MOD_FC:
            value->rValue = here->BJTdepletionCapCoeff;
            break;
        case BJT_MOD_TNOM:
            value->rValue = here->BJTtnom-CONSTCtoK;
            break;
        case BJT_MOD_INVEARLYF:
            value->rValue = here->BJTinvEarlyVoltF;
            break;
        case BJT_MOD_INVEARLYR:
            value->rValue = here->BJTinvEarlyVoltR;
            break;
        case BJT_MOD_INVROLLOFFF:
            value->rValue = here->BJTinvRollOffF;
            break;
        case BJT_MOD_INVROLLOFFR:
            value->rValue = here->BJTinvRollOffR;
            break;
        case BJT_MOD_COLCONDUCT:
            value->rValue = here->BJTcollectorConduct;
            break;
        case BJT_MOD_EMITTERCONDUCT:
            value->rValue = here->BJTemitterConduct;
            break;
        case BJT_MOD_TRANSVBCFACT:
            value->rValue = here->BJTtransitTimeVBCFactor;
            break;
        case BJT_MOD_EXCESSPHASEFACTOR:
            value->rValue = here->BJTexcessPhaseFactor;
            break;
        case BJT_MOD_TYPE:
            if (here->BJTtype == NPN)
                value->sValue = "npn";
            else
                value->sValue = "pnp";
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}

