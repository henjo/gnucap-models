/**********
Copyright 1992 Regents of the University of California.  All rights reserved.
Author:	1987 Kartikeya Mayaram, U. C. Berkeley CAD Group
**********/

/*
 * Function to load the COMPLEX circuit matrix using the small signal
 * parameters saved during a previous DC operating point analysis.
 */

#include "ngspice.h"
#include "cktdefs.h"
#include "numddefs.h"
#include "sperror.h"
#include "complex.h"
#include "../../../ciderlib/oned/onedext.h"
#include "cidersupt.h"
#include "numglobs.h"
#include "suffix.h"

/* External Declarations */
extern int ONEacDebug; 

int
NUMDacLoad(inModel, ckt)
  GENmodel *inModel;
  CKTcircuit *ckt;
{
  register NUMDmodel *model = (NUMDmodel *) inModel;
  register NUMDinstance *inst;
  SPcomplex y;
  double startTime;

  /* loop through all the diode models */
  for (; model != NULL; model = model->NUMDnextModel) {
    FieldDepMobility = model->NUMDmodels->MODLfieldDepMobility;
    Srh = model->NUMDmodels->MODLsrh;
    Auger = model->NUMDmodels->MODLauger;
    AvalancheGen = model->NUMDmodels->MODLavalancheGen;
    AcAnalysisMethod = model->NUMDmethods->METHacAnalysisMethod;
    MobDeriv = model->NUMDmethods->METHmobDeriv;
    ONEacDebug = model->NUMDoutputs->OUTPacDebug;

    for (inst = model->NUMDinstances; inst != NULL;
	inst = inst->NUMDnextInstance) {
      if (inst->NUMDowner != ARCHme) continue;

      startTime = SPfrontEnd->IFseconds();
      /* Get Temp.-Dep. Global Parameters */
      GLOBgetGlobals(&(inst->NUMDglobals));

      model->NUMDmethods->METHacAnalysisMethod =
	  NUMDadmittance(inst->NUMDpDevice,
	  ckt->CKTomega, &y);

      *(inst->NUMDposPosPtr) += y.real;
      *(inst->NUMDposPosPtr + 1) += y.imag;
      *(inst->NUMDnegNegPtr) += y.real;
      *(inst->NUMDnegNegPtr + 1) += y.imag;
      *(inst->NUMDnegPosPtr) -= y.real;
      *(inst->NUMDnegPosPtr + 1) -= y.imag;
      *(inst->NUMDposNegPtr) -= y.real;
      *(inst->NUMDposNegPtr + 1) -= y.imag;
      if (ckt->CKTomega != 0.0) {
	inst->NUMDc11 = y.imag / ckt->CKTomega;
      } else {
	inst->NUMDc11 = 0.0;	/* XXX What else can be done?! */
      }
      inst->NUMDy11r = y.real;
      inst->NUMDy11i = y.imag;
      inst->NUMDsmSigAvail = TRUE;
      inst->NUMDpDevice->pStats->totalTime[STAT_AC] +=
	  SPfrontEnd->IFseconds() - startTime;
    }
  }
  return (OK);
}
