/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1989 Takayasu Sakurai
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "mosdefs.h"
#include "const.h"
#include "sperror.h"

int
MOSmAsk(ckt,inModel,param,value)

CKTcircuit *ckt;
GENmodel *inModel;
int param;
IFvalue *value;
{
    MOSmodel *model = (MOSmodel *)inModel;

    switch (param) {

        case MOS_MOD_LEVEL:
            value->iValue = model->MOSlevel;
            break;
        case MOS_MOD_TNOM:
            value->rValue = model->MOStnom - CONSTCtoK;
            break;
        case MOS_MOD_VTO:
            value->rValue = model->MOSvt0;
            break;
        case MOS_MOD_KP:
            value->rValue = model->MOStransconductance;
            break;
        case MOS_MOD_GAMMA:
            value->rValue = model->MOSgamma;
            break;
        case MOS_MOD_PHI:
            value->rValue = model->MOSphi;
            break;
        case MOS_MOD_RD:
            value->rValue = model->MOSdrainResistance;
            break;
        case MOS_MOD_RS:
            value->rValue = model->MOSsourceResistance;
            break;
        case MOS_MOD_CBD:
            value->rValue = model->MOScapBD;
            break;
        case MOS_MOD_CBS:
            value->rValue = model->MOScapBS;
            break;
        case MOS_MOD_IS:
            value->rValue = model->MOSjctSatCur;
            break;
        case MOS_MOD_PB:
            value->rValue = model->MOSbulkJctPotential;
            break;
        case MOS_MOD_CGSO:
            value->rValue = model->MOSgateSourceOverlapCapFactor;
            break;
        case MOS_MOD_CGDO:
            value->rValue = model->MOSgateDrainOverlapCapFactor;
            break;
        case MOS_MOD_CGBO:
            value->rValue = model->MOSgateBulkOverlapCapFactor;
            break;
        case MOS_MOD_CJ:
            value->rValue = model->MOSbulkCapFactor;
            break;
        case MOS_MOD_MJ:
            value->rValue = model->MOSbulkJctBotGradingCoeff;
            break;
        case MOS_MOD_CJSW:
            value->rValue = model->MOSsideWallCapFactor;
            break;
        case MOS_MOD_MJSW:
            value->rValue = model->MOSbulkJctSideGradingCoeff;
            break;
        case MOS_MOD_JS:
            value->rValue = model->MOSjctSatCurDensity;
            break;
        case MOS_MOD_TOX:
            value->rValue = model->MOSoxideThickness;
            break;
        case MOS_MOD_LD:
            value->rValue = model->MOSlatDiff;
            break;
        case MOS_MOD_RSH:
            value->rValue = model->MOSsheetResistance;
            break;
        case MOS_MOD_U0:
            value->rValue = model->MOSsurfaceMobility;
            break;
        case MOS_MOD_FC:
            value->rValue = model->MOSfwdCapDepCoeff;
            break;
        case MOS_MOD_NSUB:
            value->rValue = model->MOSsubstrateDoping;
            break;
        case MOS_MOD_TPG:
            value->rValue = model->MOSgateType;
            break;
        case MOS_MOD_NSS:
            value->rValue = model->MOSsurfaceStateDensity;
            break;
        case MOS_MOD_TYPE:
            if (model->MOStype > 0)
                value->sValue = "nmos";
            else
                value->sValue = "pmos";
            break;
        case MOS_MOD_KF:
            value->rValue = model->MOSfNcoef;
            break;
        case MOS_MOD_AF:
            value->rValue = model->MOSfNexp;
            break;
        case MOS_MOD_LAMBDA:
            /* levels 1,2 and 6 */
            value->rValue = model->MOSlambda;
            break;
        case MOS_MOD_UEXP:
            /* level 2 */
            value->rValue = model->MOScritFieldExp;
            break;
        case MOS_MOD_NEFF:
            /* level 2 */
            value->rValue = model->MOSchannelCharge;
            break;
        case MOS_MOD_UCRIT:
            /* level 2 */
            value->rValue = model->MOScritField;
            break;
        case MOS_MOD_NFS:
            /* levels 2 and 3 */
            value->rValue = model->MOSfastSurfaceStateDensity;
            break;
        case MOS_MOD_DELTA:
            /* levels 2 and 3 */
            value->rValue = model->MOSnarrowFactor;
            break;
        case MOS_MOD_VMAX:
            /* levels 2 and 3 */
            value->rValue = model->MOSmaxDriftVel;
            break;
        case MOS_MOD_XJ:
            /* levels 2 and 3 */
            value->rValue = model->MOSjunctionDepth;
            break;
        case MOS_MOD_ETA:
            /* level 3 */
            value->rValue = model->MOSeta;
            break;
        case MOS_MOD_THETA:
            /* level 3 */
            value->rValue = model->MOStheta;
            break;
        case MOS_MOD_ALPHA:
            /* level 3 */
            value->rValue = model->MOSalpha;
            break;
        case MOS_MOD_KAPPA:
            /* level 3 */
            value->rValue = model->MOSkappa;
            break;
        case MOS_MOD_XD:
            /* level 3 */
            value->rValue = model->MOSxd;
            break;
        case MOS_DELTA:
            /* level 3 */
            value->rValue = model->MOSdelta;
            return(OK);
        case MOS_MOD_KV:
            /* level 6 */
            value->rValue = model->MOSkv;
            break;
        case MOS_MOD_NV:
            /* level 6 */
            value->rValue = model->MOSnv;
            break;
        case MOS_MOD_KC:
            /* level 6 */
            value->rValue = model->MOSkc;
            break;
        case MOS_MOD_NC:
            /* level 6 */
            value->rValue = model->MOSnc;
            break;
        case MOS_MOD_GAMMA1:
            /* level 6 */
            value->rValue = model->MOSgamma1;
            break;
        case MOS_MOD_SIGMA:
            /* level 6 */
            value->rValue = model->MOSsigma;
            break;
        case MOS_MOD_LAMDA0:
            /* level 6 */
            value->rValue = model->MOSlamda0;
            break;
        case MOS_MOD_LAMDA1:
            /* level 6 */
            value->rValue = model->MOSlamda1;
            break;
	case MOS_MOD_A1:
	    /* level 8, CryoMOS by Luong 1/19/95 */
	    value->rValue = model->MOSa1;
	    break;
	case MOS_MOD_A2:
	    /* level 8, CryoMOS by Luong 1/19/95 */
	    value->rValue = model->MOSa2;
	    break;
	case MOS_MOD_K1:
	    /* level 8, CryoMOS by Luong 1/19/95 */
	    value->rValue = model->MOSk1;
	    break;
	case MOS_MOD_K2:
	    /* level 8, CryoMOS by Luong 1/19/95 */
	    value->rValue = model->MOSk2;
	    break;
	case MOS_MOD_K3:
	    /* level 8, CryoMOS by Luong 1/19/95 */
	    value->rValue = model->MOSk3;
	    break;
	case MOS_MOD_K4:
	    /* level 8, CryoMOS by Luong 1/19/95 */
	    value->rValue = model->MOSk4;
	    break;
	case MOS_MOD_PCLM:
	    /* level 8, CryoMOS by Luong 1/19/95 */
	    value->rValue = model->MOSpclm;
	    break;
	case MOS_MOD_EM:
	    /* level 8, CryoMOS by Luong 1/19/95 */
	    value->rValue = model->MOSem;
	    break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}
