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
#include "util.h"


int
MOSmParam(param,value,inModel)

int param;
IFvalue *value;
GENmodel *inModel;
{
    MOSmodel *model = (MOSmodel *)inModel;

    switch (param) {

        case MOS_MOD_LEVEL:
            model->MOSlevel = value->iValue;
            model->MOSlevelGiven = TRUE;
            break;
        case MOS_MOD_TNOM:
            model->MOStnom = value->rValue+CONSTCtoK;
            model->MOStnomGiven = TRUE;
            break;
        case MOS_MOD_VTO:
            model->MOSvt0 = value->rValue;
            model->MOSvt0Given = TRUE;
            break;
        case MOS_MOD_KP:
            model->MOStransconductance = value->rValue;
            model->MOStransconductanceGiven = TRUE;
            break;
        case MOS_MOD_GAMMA:
            model->MOSgamma = value->rValue;
            model->MOSgammaGiven = TRUE;
            break;
        case MOS_MOD_PHI:
            model->MOSphi = value->rValue;
            model->MOSphiGiven = TRUE;
            break;
        case MOS_MOD_RD:
            model->MOSdrainResistance = value->rValue;
            model->MOSdrainResistanceGiven = TRUE;
            break;
        case MOS_MOD_RS:
            model->MOSsourceResistance = value->rValue;
            model->MOSsourceResistanceGiven = TRUE;
            break;
        case MOS_MOD_CBD:
            model->MOScapBD = value->rValue;
            model->MOScapBDGiven = TRUE;
            break;
        case MOS_MOD_CBS:
            model->MOScapBS = value->rValue;
            model->MOScapBSGiven = TRUE;
            break;
        case MOS_MOD_IS:
            model->MOSjctSatCur = value->rValue;
            model->MOSjctSatCurGiven = TRUE;
            break;
        case MOS_MOD_PB:
            model->MOSbulkJctPotential = value->rValue;
            model->MOSbulkJctPotentialGiven = TRUE;
            break;
        case MOS_MOD_CGSO:
            model->MOSgateSourceOverlapCapFactor = value->rValue;
            model->MOSgateSourceOverlapCapFactorGiven = TRUE;
            break;
        case MOS_MOD_CGDO:
            model->MOSgateDrainOverlapCapFactor = value->rValue;
            model->MOSgateDrainOverlapCapFactorGiven = TRUE;
            break;
        case MOS_MOD_CGBO:
            model->MOSgateBulkOverlapCapFactor = value->rValue;
            model->MOSgateBulkOverlapCapFactorGiven = TRUE;
            break;
        case MOS_MOD_CJ:
            model->MOSbulkCapFactor = value->rValue;
            model->MOSbulkCapFactorGiven = TRUE;
            break;
        case MOS_MOD_MJ:
            model->MOSbulkJctBotGradingCoeff = value->rValue;
            model->MOSbulkJctBotGradingCoeffGiven = TRUE;
            break;
        case MOS_MOD_CJSW:
            model->MOSsideWallCapFactor = value->rValue;
            model->MOSsideWallCapFactorGiven = TRUE;
            break;
        case MOS_MOD_MJSW:
            model->MOSbulkJctSideGradingCoeff = value->rValue;
            model->MOSbulkJctSideGradingCoeffGiven = TRUE;
            break;
        case MOS_MOD_JS:
            model->MOSjctSatCurDensity = value->rValue;
            model->MOSjctSatCurDensityGiven = TRUE;
            break;
        case MOS_MOD_TOX:
            model->MOSoxideThickness = value->rValue;
            model->MOSoxideThicknessGiven = TRUE;
            break;
        case MOS_MOD_LD:
            model->MOSlatDiff = value->rValue;
            model->MOSlatDiffGiven = TRUE;
            break;
        case MOS_MOD_RSH:
            model->MOSsheetResistance = value->rValue;
            model->MOSsheetResistanceGiven = TRUE;
            break;
        case MOS_MOD_U0:
            model->MOSsurfaceMobility = value->rValue;
            model->MOSsurfaceMobilityGiven = TRUE;
            break;
        case MOS_MOD_FC:
            model->MOSfwdCapDepCoeff = value->rValue;
            model->MOSfwdCapDepCoeffGiven = TRUE;
            break;
        case MOS_MOD_NSS:
            model->MOSsurfaceStateDensity = value->rValue;
            model->MOSsurfaceStateDensityGiven = TRUE;
            break;
        case MOS_MOD_NSUB:
            model->MOSsubstrateDoping = value->rValue;
            model->MOSsubstrateDopingGiven = TRUE;
            break;
        case MOS_MOD_TPG:
            model->MOSgateType = value->iValue;
            model->MOSgateTypeGiven = TRUE;
            break;
        case MOS_MOD_NMOS:
            if(value->iValue) {
                model->MOStype = 1;
                model->MOStypeGiven = TRUE;
            }
            break;
        case MOS_MOD_PMOS:
            if(value->iValue) {
                model->MOStype = -1;
                model->MOStypeGiven = TRUE;
            }
            break;
        case MOS_MOD_KF:
            model->MOSfNcoef = value->rValue;
            model->MOSfNcoefGiven = TRUE;
            break;
        case MOS_MOD_AF:
            model->MOSfNexp = value->rValue;
            model->MOSfNexpGiven = TRUE;
            break;
        case MOS_MOD_LAMBDA:
            /* levels 1 and 2 */
            model->MOSlambda = value->rValue;
            model->MOSlambdaGiven = TRUE;
            break;
        case MOS_MOD_UEXP:
            /* level 2 */
            model->MOScritFieldExp = value->rValue;
            model->MOScritFieldExpGiven = TRUE;
            break;
        case MOS_MOD_NEFF:
            /* level 2 */
            model->MOSchannelCharge = value->rValue;
            model->MOSchannelChargeGiven = TRUE;
            break;
        case MOS_MOD_UCRIT:
            /* level 2 */
            model->MOScritField = value->rValue;
            model->MOScritFieldGiven = TRUE;
            break;
        case MOS_MOD_NFS:
            /* levels 2 and 3 */
            model->MOSfastSurfaceStateDensity = value->rValue;
            model->MOSfastSurfaceStateDensityGiven = TRUE;
            break;
        case MOS_MOD_DELTA:
            /* levels 2 and 3 */
            model->MOSnarrowFactor = value->rValue;
            model->MOSnarrowFactorGiven = TRUE;
            break;
        case MOS_MOD_VMAX:
            /* levels 2 and 3 */
            model->MOSmaxDriftVel = value->rValue;
            model->MOSmaxDriftVelGiven = TRUE;
            break;
        case MOS_MOD_XJ:
            /* levels 2 and 3 */
            model->MOSjunctionDepth = value->rValue;
            model->MOSjunctionDepthGiven = TRUE;
            break;
        case MOS_MOD_ETA:
            /* level 3 */
            model->MOSeta = value->rValue;
            model->MOSetaGiven = TRUE;
            break;
        case MOS_MOD_THETA:
            /* level 3 */
            model->MOStheta = value->rValue;
            model->MOSthetaGiven = TRUE;
            break;
        case MOS_MOD_KAPPA:
            /* level 3 */
            model->MOSkappa = value->rValue;
            model->MOSkappaGiven = TRUE;
            break;
        case MOS_MOD_KV:
            /* level 6 */
            model->MOSkv = value->rValue;
            model->MOSkvGiven = TRUE;
            break;
        case MOS_MOD_NV:
            /* level 6 */
            model->MOSnv = value->rValue;
            model->MOSnvGiven = TRUE;
            break;
        case MOS_MOD_KC:
            /* level 6 */
            model->MOSkc = value->rValue;
            model->MOSkcGiven = TRUE;
            break;
        case MOS_MOD_NC:
            /* level 6 */
            model->MOSnc = value->rValue;
            model->MOSncGiven = TRUE;
            break;
        case MOS_MOD_GAMMA1:
            /* level 6 */
            model->MOSgamma1 = value->rValue;
            model->MOSgamma1Given = TRUE;
            break;
        case MOS_MOD_SIGMA:
            /* level 6 */
            model->MOSsigma = value->rValue;
            model->MOSsigmaGiven = TRUE;
            break;
        case MOS_MOD_LAMDA0:
            /* level 6 */
            model->MOSlamda0 = value->rValue;
            model->MOSlamda0Given = TRUE;
            break;
        case MOS_MOD_LAMDA1:
            /* level 6 */
            model->MOSlamda1 = value->rValue;
            model->MOSlamda1Given = TRUE;
            break;
        case MOS_MOD_A1:
            /* level 8, CryoMOS by Luong Huynh 1/19/95 */
            model->MOSa1 = value->rValue;
            model->MOSa1Given = TRUE;
            break;
        case MOS_MOD_A2:
            /* level 8, CryoMOS by Luong Huynh 1/19/95 */
            model->MOSa2 = value->rValue;
            model->MOSa2Given = TRUE;
            break;
        case MOS_MOD_K1:
            /* level 8, CryoMOS by Luong Huynh 1/19/95 */
            model->MOSk1 = value->rValue;
            model->MOSk1Given = TRUE;
            break;
        case MOS_MOD_K2:
            /* level 8, CryoMOS by Luong Huynh 1/19/95 */
            model->MOSk2 = value->rValue;
            model->MOSk2Given = TRUE;
            break;
        case MOS_MOD_K3:
            /* level 8, CryoMOS by Luong Huynh 1/19/95 */
            model->MOSk3 = value->rValue;
            model->MOSk3Given = TRUE;
            break;
        case MOS_MOD_K4:
            /* level 8, CryoMOS by Luong Huynh 1/19/95 */
            model->MOSk4 = value->rValue;
            model->MOSk4Given = TRUE;
            break;
        case MOS_MOD_PCLM:
            /* level 8, CryoMOS by Luong Huynh 1/19/95 */
            model->MOSpclm = value->rValue;
            model->MOSpclmGiven = TRUE;
            break;
        case MOS_MOD_EM:
            /* level 8, CryoMOS by Luong Huynh 1/19/95 */
            model->MOSem = value->rValue;
            model->MOSemGiven = TRUE;
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}
