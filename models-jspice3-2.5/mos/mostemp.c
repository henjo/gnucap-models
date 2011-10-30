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


/* assuming silicon - make definition for epsilon of silicon */
#define EPSSIL (11.7 * 8.854214871e-12)

/* (cm**3/m**3) */
#define CM3PM3 1e6

/* (cm**2/m**2) */
#define CM2PM2 1e4

/* (cm**2/m**2) */
#define M2PCM2 1e-4

#ifdef __STDC__
static int mos_dopsetup(MOSmodel*,double,double);
#else
static int mos_dopsetup();
#endif


int
MOStemp(inModel,ckt)

GENmodel *inModel;
CKTcircuit *ckt;
{
    MOSmodel *model = (MOSmodel *)inModel;
    MOSinstance *here;
    double egfet,egfet1;
    double fact1,fact2;
    double vt,vtnom;
    double pbfact,pbfact1,pbo;
    double ratio;
    double gmanew,gmaold;
    double capfact;
    double tmp;
    int error;


    /* loop through all the resistor models */
    for ( ; model != NULL; model = model->MOSnextModel) {
        
        /* perform model defaulting */

        if (!model->MOStnomGiven) {
            model->MOStnom = ckt->CKTnomTemp;
        }
        fact1  = model->MOStnom/REFTEMP;
        vtnom  = model->MOStnom*CONSTKoverQ;
        egfet1 = 1.16 - (7.02e-4*model->MOStnom*model->MOStnom)/
                (model->MOStnom + 1108);
        tmp    = model->MOStnom*CONSTboltz;
        tmp    = -egfet1/(tmp+tmp) + 1.1150877/(CONSTboltz*(REFTEMP+REFTEMP));
        pbfact1= -2*vtnom*(1.5*log(fact1) + CHARGE*tmp);


        if (!model->MOSoxideThicknessGiven ||
                model->MOSoxideThickness == 0) {

            if (model->MOSlevel == 1 || model->MOSlevel == 6) {
                model->MOSoxideCapFactor = 0;
                goto l1skip;
            }
            model->MOSoxideThickness = 1e-7;
        }
        model->MOSoxideCapFactor = 3.9 * 8.854214871e-12/
                model->MOSoxideThickness;

        if (!model->MOSsurfaceMobilityGiven)
            model->MOSsurfaceMobility = 600;

        if (model->MOSlevel == 6) {
            if (!model->MOSkcGiven) {
                model->MOSkc = .5*model->MOSsurfaceMobility * 
                    model->MOSoxideCapFactor * M2PCM2;
            }
        }
        else {
            if (!model->MOStransconductanceGiven) {
                model->MOStransconductance = model->MOSsurfaceMobility * 
                    model->MOSoxideCapFactor * M2PCM2;
            }
        }

        if (model->MOSsubstrateDopingGiven) {

            error = mos_dopsetup(model,vtnom,egfet1);
            if (error) return (error);

            if (model->MOSlevel == 2) {
                model->MOSxd = sqrt((EPSSIL+EPSSIL)/
                    (CHARGE*model->MOSsubstrateDoping*CM3PM3));
            }
            else if (model->MOSlevel == 3) {
                model->MOSalpha = (EPSSIL+EPSSIL)/
                    (CHARGE*model->MOSsubstrateDoping*CM3PM3);
                model->MOSxd = sqrt(model->MOSalpha);
            }
        }
l1skip:

        if (model->MOSlevel == 2) {
            if (!model->MOSbulkCapFactorGiven) {
                model->MOSbulkCapFactor = sqrt(EPSSIL*CHARGE*
                    model->MOSsubstrateDoping*CM3PM3
                    /(2*model->MOSbulkJctPotential));
            }
        }
        else if (model->MOSlevel == 3) {
            model->MOSnarrowFactor =
                model->MOSdelta * 0.5 * M_PI * EPSSIL / 
                model->MOSoxideCapFactor ;
        }
        
        /* loop through all instances of the model */
        for (here = model->MOSinstances; here!= NULL; 
                here = here->MOSnextInstance) {

            fact2 = here->MOStemp/REFTEMP;
            vt    = here->MOStemp * CONSTKoverQ;
            ratio = here->MOStemp/model->MOStnom;
            egfet = 1.16- (7.02e-4*here->MOStemp*here->MOStemp)/
                    (here->MOStemp + 1108);
            tmp   = here->MOStemp * CONSTboltz;
            tmp   = -egfet/(tmp+tmp) + 1.1150877/(CONSTboltz*(REFTEMP+REFTEMP));
            pbfact= -2*vt*(1.5*log(fact2) + CHARGE*tmp);

            tmp = 1/(ratio*sqrt(ratio));
            if (model->MOSlevel == 6)
                here->MOStTransconductance = model->MOSkc*tmp;
            else
                here->MOStTransconductance = model->MOStransconductance*tmp;
            here->MOStSurfMob = model->MOSsurfaceMobility*tmp;

            tmp = (model->MOSphi - pbfact1)/fact1;
            here->MOStPhi = fact2*tmp + pbfact;

            if (model->MOStype > 0) {
                here->MOStVbi =
                    model->MOSvt0 - model->MOSgamma*sqrt(model->MOSphi) +
                        .5*(egfet1 - egfet + here->MOStPhi - model->MOSphi);
                here->MOStVto =
                    here->MOStVbi + model->MOSgamma*sqrt(here->MOStPhi);
            }
            else {
                here->MOStVbi = 
                    model->MOSvt0 + model->MOSgamma*sqrt(model->MOSphi) +
                        .5*(egfet1 - egfet - (here->MOStPhi - model->MOSphi));
                here->MOStVto =
                    here->MOStVbi - model->MOSgamma*sqrt(here->MOStPhi);
            }

            tmp = exp(-egfet/vt + egfet1/vtnom);
            here->MOStSatCur = model->MOSjctSatCur*tmp;
            here->MOStSatCurDens = model->MOSjctSatCurDensity*tmp;

            pbo = (model->MOSbulkJctPotential - pbfact1)/fact1;
            gmaold = (model->MOSbulkJctPotential - pbo)/pbo;
            capfact = 1/(1 + model->MOSbulkJctBotGradingCoeff*
                    (4e-4*(model->MOStnom - REFTEMP) - gmaold));
            here->MOStCbd = model->MOScapBD * capfact;
            here->MOStCbs = model->MOScapBS * capfact;
            here->MOStCj  = model->MOSbulkCapFactor * capfact;

            capfact = 1/(1 + model->MOSbulkJctSideGradingCoeff*
                    (4e-4*(model->MOStnom - REFTEMP) - gmaold));
            here->MOStCjsw = model->MOSsideWallCapFactor * capfact;
            here->MOStBulkPot = fact2*pbo + pbfact;

            gmanew = (here->MOStBulkPot-pbo)/pbo;
            capfact = (1 + model->MOSbulkJctBotGradingCoeff*
                    (4e-4*(here->MOStemp - REFTEMP) - gmanew));
            here->MOStCbd *= capfact;
            here->MOStCbs *= capfact;
            here->MOStCj  *= capfact;
            capfact = (1 + model->MOSbulkJctSideGradingCoeff*
                    (4e-4*(here->MOStemp - REFTEMP) - gmanew));
            here->MOStCjsw *= capfact;
            here->MOStDepCap = model->MOSfwdCapDepCoeff * here->MOStBulkPot;

            if ( (here->MOStSatCurDens == 0) ||
                    (here->MOSdrainArea == 0) ||
                    (here->MOSsourceArea == 0) ) {
                here->MOSsourceVcrit = here->MOSdrainVcrit =
                        vt*log(vt/(CONSTroot2*here->MOStSatCur));
                here->MOStDrainSatCur = here->MOStSatCur;
                here->MOStSourceSatCur = here->MOStSatCur;
            }
            else {
                here->MOStDrainSatCur =
                    here->MOStSatCurDens*here->MOSdrainArea;
                here->MOStSourceSatCur =
                    here->MOStSatCurDens*here->MOSsourceArea;
                here->MOSdrainVcrit =
                        vt*log(vt/(CONSTroot2*here->MOStDrainSatCur));
                here->MOSsourceVcrit =
                        vt*log(vt/(CONSTroot2*here->MOStSourceSatCur));
            }

            if (model->MOScapBDGiven) {
                here->MOSCbd = here->MOStCbd;
            }
            else {
                if (model->MOSbulkCapFactorGiven) {
                    here->MOSCbd = here->MOStCj*here->MOSdrainArea;
                }
                else {
                    here->MOSCbd = 0;
                }
            }
            if (model->MOSsideWallCapFactorGiven) {
                here->MOSCbdsw = here->MOStCjsw*here->MOSdrainPerimeter;
            }
            else {
                here->MOSCbdsw = 0;
            }

            if (model->MOScapBSGiven) {
                here->MOSCbs = here->MOStCbs;
            }
            else {
                if(model->MOSbulkCapFactorGiven) {
                    here->MOSCbs = here->MOStCj*here->MOSsourceArea;
                }
                else {
                    here->MOSCbs = 0;
                }
            }
            if (model->MOSsideWallCapFactorGiven) {
                here->MOSCbssw = here->MOStCjsw*here->MOSsourcePerimeter;
            }
            else {
                here->MOSCbssw = 0;
            }

            MOSfd(model,here);
            MOSfs(model,here);


            /* cache a few useful parameters */

            here->MOSgateSourceOverlapCap =
                model->MOSgateSourceOverlapCapFactor * here->MOSw;
            here->MOSgateDrainOverlapCap =
                model->MOSgateDrainOverlapCapFactor * here->MOSw;
            here->MOSgateBulkOverlapCap =
                model->MOSgateBulkOverlapCapFactor * here->MOSeffectiveLength;
            here->MOSbeta = here->MOStTransconductance *
                here->MOSw/here->MOSeffectiveLength;
            here->MOSoxideCap =
                model->MOSoxideCapFactor * here->MOSeffectiveLength * 
                    here->MOSw;

        }
    }
    return (OK);
}


static int
mos_dopsetup(model,vtnom,egfet1)

MOSmodel * model;
double vtnom,egfet1;
{
    double wkfngs;
    double wkfng;
    double vfb;
    double doping = model->MOSsubstrateDoping * CM3PM3;

    if (doping > 1.45e16) {
        if (!model->MOSphiGiven) {
            model->MOSphi = 2*vtnom*log(doping/1.45e16);
            model->MOSphi = MAX(.1,model->MOSphi);
        }
        if (!model->MOSgateTypeGiven) {
            model->MOSgateType = 1;
        }
        if (!model->MOSgammaGiven) {
            model->MOSgamma =
                sqrt(2*EPSSIL*CHARGE*doping)/model->MOSoxideCapFactor;
        }
        if (!model->MOSvt0Given) {

            wkfng = 3.2;
            if (model->MOSgateType != 0) {
                if (model->MOStype > 0)
                    wkfng = 3.25 + .5*egfet1*(1 - model->MOSgateType);
                else
                    wkfng = 3.25 + .5*egfet1*(1 + model->MOSgateType);
            }
            if (model->MOStype > 0)
                wkfngs = wkfng - (3.25 + .5*(egfet1 + model->MOSphi));
            else
                wkfngs = wkfng - (3.25 + .5*(egfet1 - model->MOSphi));

            if (!model->MOSsurfaceStateDensityGiven) {
                model->MOSsurfaceStateDensity = 0;
                vfb = wkfngs;
            }
            else
                vfb = wkfngs -
                    model->MOSsurfaceStateDensity*CM2PM2*CHARGE/
                        model->MOSoxideCapFactor;

            if (model->MOStype > 0)
                model->MOSvt0 = vfb + 
                    (model->MOSgamma*sqrt(model->MOSphi) + model->MOSphi);
            else
                model->MOSvt0 = vfb -
                    (model->MOSgamma*sqrt(model->MOSphi) + model->MOSphi);
        }
    }
    else {
        model->MOSsubstrateDoping = 0;
        (*(SPfrontEnd->IFerror))(ERR_FATAL,"%s: Nsub < Ni",
                &(model->MOSmodName));
        return (E_BADPARM);
    }
    return (OK);
}


void
MOSfd(model,here)

MOSmodel *model;
MOSinstance *here;
{
    double arg;     /* 1 - fc */
    double sargsw;  /* (1-fc) ^^ (-mjsw) */
    double sarg;    /* (1-fc) ^^ (-mj) */
    double tmp1,tmp2;
    double pot;

    arg    = 1 - model->MOSfwdCapDepCoeff;
    sarg   = exp( (-model->MOSbulkJctBotGradingCoeff) * log(arg) );
    sargsw = exp( (-model->MOSbulkJctSideGradingCoeff) * log(arg) );

    if (model->MOSlevel == 3)
        pot = model->MOSbulkJctPotential;
    else
        pot = here->MOStBulkPot;

    tmp1   = (1 - model->MOSfwdCapDepCoeff*
                (1 + model->MOSbulkJctBotGradingCoeff))/arg;
    tmp2   = 1/(arg*pot);

    here->MOSf2d = (here->MOSCbd*sarg + here->MOSCbdsw*sargsw)*tmp1;

    here->MOSf3d =
        (here->MOSCbd*model->MOSbulkJctBotGradingCoeff*sarg +
        here->MOSCbdsw*model->MOSbulkJctSideGradingCoeff*sargsw)*tmp2;

    here->MOSf4d =
        here->MOSCbd*pot*(1 - arg*sarg)/
                (1 - model->MOSbulkJctBotGradingCoeff)
        + here->MOSCbdsw*pot*(1 - arg*sargsw)/
                (1 - model->MOSbulkJctSideGradingCoeff)
        - here->MOStDepCap*(here->MOSf2d + .5*here->MOSf3d*here->MOStDepCap);
}


void
MOSfs(model,here)

MOSmodel *model;
MOSinstance *here;
{
    double arg;     /* 1 - fc */
    double sargsw;  /* (1-fc) ^^ (-mjsw) */
    double sarg;    /* (1-fc) ^^ (-mj) */
    double tmp1,tmp2;
    double pot;

    arg    = 1 - model->MOSfwdCapDepCoeff;
    sarg   = exp( (-model->MOSbulkJctBotGradingCoeff) * log(arg) );
    sargsw = exp( (-model->MOSbulkJctSideGradingCoeff) * log(arg) );

    if (model->MOSlevel == 3)
        pot = model->MOSbulkJctPotential;
    else
        pot = here->MOStBulkPot;

    tmp1   = (1 - model->MOSfwdCapDepCoeff*
                (1 + model->MOSbulkJctBotGradingCoeff))/arg;
    tmp2   = 1/(arg*pot);

    here->MOSf2s = (here->MOSCbs*sarg + here->MOSCbssw*sargsw)*tmp1;

    here->MOSf3s =
        (here->MOSCbs*model->MOSbulkJctBotGradingCoeff*sarg +
        here->MOSCbssw*model->MOSbulkJctSideGradingCoeff*sargsw)*tmp2;

    here->MOSf4s =
        here->MOSCbs*pot*(1 - arg*sarg)/
                (1 - model->MOSbulkJctBotGradingCoeff)
        + here->MOSCbssw*pot*(1 - arg*sargsw)/
                (1 - model->MOSbulkJctSideGradingCoeff)
        - here->MOStDepCap*(here->MOSf2s + .5*here->MOSf3s*here->MOStDepCap);

    /* UCB has this for last 2 lines in MOS3.  error?
                (here->MOS3tBulkPot*here->MOS3tBulkPot)
        -here->MOS3tBulkPot * here->MOS3f2s;
    */
}
