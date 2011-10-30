/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1989 Takayasu Sakurai
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include <math.h>
#include "mosdefs.h"
#include "const.h"
#include "sperror.h"
#include "util.h"
#include "cktext.h"


int
MOSsetup(matrix,inModel,ckt,states)

SMPmatrix *matrix;
GENmodel *inModel;
CKTcircuit *ckt;
int *states;
/* load the MOS device structure with those pointers needed later 
 * for fast matrix loading 
 */
{
    register MOSmodel *model = (MOSmodel *)inModel;
    register MOSinstance *here;
    int error;
    CKTnode *tmp;

    /*  loop through all the MOS device models */
    for( ; model != NULL; model = model->MOSnextModel ) {

        if (!model->MOStypeGiven) {
            model->MOStype = NMOS;
        }

        if (!model->MOSlevelGiven ||
                (model->MOSlevel != 1 &&
                model->MOSlevel != 2 &&
                model->MOSlevel != 3 &&
                model->MOSlevel != 6 &&
		/* level 8, CryoMOS by Luong Huynh, 1/19/95 */
		model->MOSlevel != 8)) {
            model->MOSlevel = 1;
        }

        /* tnom */

        /* vto */
        if (!model->MOSvt0Given) {
            model->MOSvt0 = 0;
        }

        /* kp */
        if (!model->MOStransconductanceGiven) {
            model->MOStransconductance = 2e-5;
        }

        /* gamma */
        if (!model->MOSgammaGiven) {
            model->MOSgamma = 0;
        }

        /* phi */
        if (!model->MOSphiGiven) {
            model->MOSphi = .6;
        }

        /* rd */
        if (!model->MOSdrainResistanceGiven) {
            model->MOSdrainResistance = 0;
        }

        /* rs */
        if (!model->MOSsourceResistanceGiven) {
            model->MOSsourceResistance = 0;
        }

        /* cbd */
        if (!model->MOScapBDGiven) {
            model->MOScapBD = 0;
        }

        /* cbs */
        if (!model->MOScapBSGiven) {
            model->MOScapBS = 0;
        }

        /* is */
        if (!model->MOSjctSatCurGiven) {
            model->MOSjctSatCur = 1e-14;
        }

        /* pb */
        if (!model->MOSbulkJctPotentialGiven) {
            model->MOSbulkJctPotential = .8;
        }

        /* cgso */
        if (!model->MOSgateSourceOverlapCapFactorGiven) {
            model->MOSgateSourceOverlapCapFactor = 0;
        }

        /* cgdo */
        if (!model->MOSgateDrainOverlapCapFactorGiven) {
            model->MOSgateDrainOverlapCapFactor = 0;
        }

        /* cgbo */
        if (!model->MOSgateBulkOverlapCapFactorGiven) {
            model->MOSgateBulkOverlapCapFactor = 0;
        }

        /* cj */
        if (!model->MOSbulkCapFactorGiven) {
            model->MOSbulkCapFactor = 0;
        }

        /* mj */
        if (!model->MOSbulkJctBotGradingCoeffGiven) {
            model->MOSbulkJctBotGradingCoeff = .5;
        }

        /* cjsw */
        if (!model->MOSsideWallCapFactorGiven) {
            model->MOSsideWallCapFactor = 0;
        }

        /* mjsw */
        if (!model->MOSbulkJctSideGradingCoeffGiven) {
            if (model->MOSlevel == 1 || model->MOSlevel == 6)
                model->MOSbulkJctSideGradingCoeff = .5;
            else
                model->MOSbulkJctSideGradingCoeff = .33;
        }

        /* js */
        if (!model->MOSjctSatCurDensityGiven) {
            model->MOSjctSatCurDensity = 0;
        }

        /* tox */
        if (!model->MOSoxideThicknessGiven) {
            if (model->MOSlevel == 2 || model->MOSlevel == 3)
                model->MOSoxideThickness = 1e-7;
            else
                model->MOSoxideThickness = 0;
        }

        /* ld */
        if (!model->MOSlatDiffGiven) {
            model->MOSlatDiff = 0;
        }

        /* rsh */
        if (!model->MOSsheetResistanceGiven) {
            model->MOSsheetResistance = 0;
        }

        /* u0 */

        /* fc */
        if (!model->MOSfwdCapDepCoeffGiven) {
            model->MOSfwdCapDepCoeff = .5;
        }

        /* nss */
        /* nsub */
        /* tpg */

        /* kf */
        if (!model->MOSfNcoefGiven) {
            model->MOSfNcoef = 0;
        }

        /* af */
        if (!model->MOSfNexpGiven) {
            model->MOSfNexp = 1;
        }

        /* lambda, 1,2 and 6*/
        if (!model->MOSlambdaGiven) {
            model->MOSlambda = 0;
        }

        /* uexp, 2 */
        if (!model->MOScritFieldExpGiven) {
            model->MOScritFieldExp = 0;
        }

        /* neff, 2 */
        if (!model->MOSchannelChargeGiven) {
            model->MOSchannelCharge = 1;
        }

        /* ucrit, 2 */
        if (!model->MOScritFieldGiven) {
            model->MOScritField = 1e4;
        }

        /* nfs, 2 and 3 */
        if (!model->MOSfastSurfaceStateDensityGiven) {
            model->MOSfastSurfaceStateDensity = 0;
        }

        /* delta, 2 and 3 */
        if (!model->MOSdeltaGiven) {
            model->MOSdelta = 0;
        }
        if (!model->MOSnarrowFactorGiven) {
            model->MOSnarrowFactor = 0;
        }

        /* vmax, 2 and 3 */
        if (!model->MOSmaxDriftVelGiven) {
            model->MOSmaxDriftVel = 0;
        }

        /* xj, 2 and 3 */
        if (!model->MOSjunctionDepthGiven) {
            model->MOSjunctionDepth = 0;
        }

        /* eta, 3 */
        if (!model->MOSetaGiven) {
            model->MOSeta = 0;
        }

        /* theta, 3 */
        if (!model->MOSthetaGiven) {
            model->MOStheta = 0;
        }

        /* kappa, 3 */
        if (!model->MOSkappaGiven) {
            model->MOSkappa = .2;
        }

        /* level 6 */
        if (!model->MOSkvGiven) {
            model->MOSkv = 2;
        }
        if (!model->MOSnvGiven) {
            model->MOSnv = 0.5;
        }
        if (!model->MOSkcGiven) {
            model->MOSkc = 5e-5;
        }
        if (!model->MOSncGiven) {
            model->MOSnc = 1;
        }
        if (!model->MOSlamda0Given) {
            model->MOSlamda0 = 0;
            if (model->MOSlambdaGiven) {
                model->MOSlamda0 = model->MOSlambda;
            }
        }
        if (!model->MOSlamda1Given) {
            model->MOSlamda1 = 0;
        }
        if (!model->MOSsigmaGiven) {
            model->MOSsigma = 0;
        }
        if(!model->MOSgamma1Given) {
            model->MOSgamma1 = 0;
        }

	/* level 8, CryoMOS by Luong Huynh 1/19/95 */
        if (!model->MOSa1Given) {
            model->MOSa1 = 0;
        }
        if (!model->MOSa2Given) {
            model->MOSa2 = 0;
        }
        if (!model->MOSk1Given) {
            model->MOSk1 = 0;
        }
        if (!model->MOSk2Given) {
            model->MOSk2 = 0;
        }
        if (!model->MOSk3Given) {
            model->MOSk3 = 0;
        }
        if (!model->MOSk4Given) {
            model->MOSk4 = 0;
        }
        if (!model->MOSemGiven) {
            model->MOSem = 0;
        }
        if (!model->MOSpclmGiven) {
            model->MOSpclm = 0;
        }


        /* loop through all the instances of the model */
        for (here = model->MOSinstances; here != NULL;
                here = here->MOSnextInstance) {

            here->MOSmode = 1;
            here->MOSvon = 0;

            /* allocate a chunk of the state vector */
            here->MOSstates = *states;
            *states += MOSnumStates;

            /* temp */
            if (!here->MOStempGiven) {
                here->MOStemp = ckt->CKTtemp;
            }

            /* pd */
            if (!here->MOSdrainPerimeterGiven) {
                here->MOSdrainPerimeter = 0;
            }

            /* nrd */
            if (!here->MOSdrainSquaresGiven ||
                    here->MOSdrainSquares == 0) {
                here->MOSdrainSquares = 1;
            }

            /* ps */
            if (!here->MOSsourcePerimeterGiven) {
                here->MOSsourcePerimeter = 0;
            }

            /* nrs */
            if (!here->MOSsourceSquaresGiven ||
                    here->MOSsourceSquares == 0) {
                here->MOSsourceSquares = 1;
            }

            /* ic: vbs */
            if (!here->MOSicVBSGiven) {
                here->MOSicVBS = 0;
            }

            /* ic: vds */
            if (!here->MOSicVDSGiven) {
                here->MOSicVDS = 0;
            }

            /* ic: vgs */
            if (!here->MOSicVGSGiven) {
                here->MOSicVGS = 0;
            }

            /* no assigned parameter? */
            if (!here->MOSvdsatGiven) {
                here->MOSvdsat = 0;
            }

            if (!here->MOSlGiven) {
                here->MOSl = ckt->CKTdefaultMosL;
            }
            if (!here->MOSwGiven) {
                here->MOSw = ckt->CKTdefaultMosW;
            }
            if (!here->MOSdrainAreaGiven) {
                here->MOSdrainArea = ckt->CKTdefaultMosAD;
            }
            
            if (!here->MOSsourceAreaGiven) {
                here->MOSsourceArea = ckt->CKTdefaultMosAS;
            }

            if (model->MOSdrainResistanceGiven) {
                if (model->MOSdrainResistance != 0) {
                    here->MOSdrainConductance =
                        1/model->MOSdrainResistance;
                }
                else {
                    here->MOSdrainConductance = 0;
                }
            }
            else if (model->MOSsheetResistanceGiven) {
                if (model->MOSsheetResistance != 0) {
                    here->MOSdrainConductance = 
                        1/(model->MOSsheetResistance*
                            here->MOSdrainSquares);
                }
                else {
                    here->MOSdrainConductance = 0;
                }
            }
            else {
                here->MOSdrainConductance = 0;
            }
            if (model->MOSsourceResistanceGiven) {
                if (model->MOSsourceResistance != 0) {
                    here->MOSsourceConductance =
                        1/model->MOSsourceResistance;
                }
                else {
                    here->MOSsourceConductance = 0;
                }
            }
            else if (model->MOSsheetResistanceGiven) {
                if (model->MOSsheetResistance != 0) {
                    here->MOSsourceConductance = 
                        1/(model->MOSsheetResistance*
                            here->MOSsourceSquares);
                }
                else {
                    here->MOSsourceConductance = 0;
                }
            }
            else {
                here->MOSsourceConductance = 0;
            }

            here->MOSeffectiveLength = here->MOSl - 2*model->MOSlatDiff;
            if (here->MOSeffectiveLength < 0) {
                (*(SPfrontEnd->IFerror))(ERR_WARNING,
                        "%s: effective channel length less than zero",
                        &(here->MOSname));
                here->MOSeffectiveLength = 0;
            }

            /* assign internal nodes */

            if ((model->MOSdrainResistance != 0
                || model->MOSsheetResistance != 0)
                    && here->MOSdNodePrime == 0) {
                error = CKTmkVolt(ckt,&tmp,here->MOSname,"internal#drain");
                if (error) return(error);
                here->MOSdNodePrime = tmp->number;
            }
            else {
                here->MOSdNodePrime = here->MOSdNode;
            }

            if (((model->MOSsourceResistance != 0) || 
                    ((here->MOSsourceSquares != 0) &&
                     (model->MOSsheetResistance != 0))) && 
                        (here->MOSsNodePrime == 0)) {
                error = CKTmkVolt(ckt,&tmp,here->MOSname,"internal#source");
                if (error) return(error);
                here->MOSsNodePrime = tmp->number;
            }
            else {
                here->MOSsNodePrime = here->MOSsNode;
            }

            TSTALLOC(MOSDdPtr,   MOSdNode,      MOSdNode)
            TSTALLOC(MOSGgPtr,   MOSgNode,      MOSgNode)
            TSTALLOC(MOSSsPtr,   MOSsNode,      MOSsNode)
            TSTALLOC(MOSBbPtr,   MOSbNode,      MOSbNode)
            TSTALLOC(MOSDPdpPtr, MOSdNodePrime, MOSdNodePrime)
            TSTALLOC(MOSSPspPtr, MOSsNodePrime, MOSsNodePrime)
            TSTALLOC(MOSDdpPtr,  MOSdNode,      MOSdNodePrime)
            TSTALLOC(MOSGbPtr,   MOSgNode,      MOSbNode)
            TSTALLOC(MOSGdpPtr,  MOSgNode,      MOSdNodePrime)
            TSTALLOC(MOSGspPtr,  MOSgNode,      MOSsNodePrime)
            TSTALLOC(MOSSspPtr,  MOSsNode,      MOSsNodePrime)
            TSTALLOC(MOSBdpPtr,  MOSbNode,      MOSdNodePrime)
            TSTALLOC(MOSBspPtr,  MOSbNode,      MOSsNodePrime)
            TSTALLOC(MOSDPspPtr, MOSdNodePrime, MOSsNodePrime)
            TSTALLOC(MOSDPdPtr,  MOSdNodePrime, MOSdNode)
            TSTALLOC(MOSBgPtr,   MOSbNode,      MOSgNode)
            TSTALLOC(MOSDPgPtr,  MOSdNodePrime, MOSgNode)
            TSTALLOC(MOSSPgPtr,  MOSsNodePrime, MOSgNode)
            TSTALLOC(MOSSPsPtr,  MOSsNodePrime, MOSsNode)
            TSTALLOC(MOSDPbPtr,  MOSdNodePrime, MOSbNode)
            TSTALLOC(MOSSPbPtr,  MOSsNodePrime, MOSbNode)
            TSTALLOC(MOSSPdpPtr, MOSsNodePrime, MOSdNodePrime)

        }
    }
    return (OK);
}
