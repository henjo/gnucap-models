/**********
Copyright 1991 Regents of the University of California.  All rights reserved.
Author:	1987 Kartikeya Mayaram, U. C. Berkeley CAD Group
Author:	1991 David A. Gates, U. C. Berkeley CAD Group
**********/

#include "ngspice.h"
#include "cktdefs.h"
#include "numosdef.h"
#include "numenum.h"
#include "carddefs.h"
#include "sperror.h"
#include "../../../ciderlib/twod/twoddefs.h"
#include "../../../ciderlib/twod/twodext.h"
#include "cidersupt.h"
#include "suffix.h"

#define NIL(type)   ((type *)0)

int
NUMOStemp(inModel, ckt)
  GENmodel *inModel;
  register CKTcircuit *ckt;
/*
 * perform the temperature update
 */
{
  register NUMOSmodel *model = (NUMOSmodel *) inModel;
  register NUMOSinstance *inst;
  METHcard *methods;
  MODLcard *models;
  OPTNcard *options;
  OUTPcard *outputs;
  TWOmaterial *pM, *pMaterial, *pNextMaterial;
  double startTime;


  /* loop through all the models */
  for (; model != NULL; model = model->NUMOSnextModel) {
    methods = model->NUMOSmethods;
    models = model->NUMOSmodels;
    options = model->NUMOSoptions;
    outputs = model->NUMOSoutputs;

    if (!options->OPTNtnomGiven) {
      options->OPTNtnom = ckt->CKTnomTemp;
    }
    for (pM = model->NUMOSmatlInfo; pM != NIL(TWOmaterial);
	pM = pM->next) {
      pM->tnom = options->OPTNtnom;
    }
    BandGapNarrowing = models->MODLbandGapNarrowing;
    ConcDepLifetime = models->MODLconcDepLifetime;
    TempDepMobility = models->MODLtempDepMobility;
    ConcDepMobility = models->MODLconcDepMobility;
    SurfaceMobility = models->MODLsurfaceMobility;
    MatchingMobility = models->MODLmatchingMobility;
    OneCarrier = methods->METHoneCarrier;

    for (inst = model->NUMOSinstances; inst != NULL;
	inst = inst->NUMOSnextInstance) {
      if (inst->NUMOSowner != ARCHme) continue;

      startTime = SPfrontEnd->IFseconds();

      if (!inst->NUMOStempGiven) {
	inst->NUMOStemp = ckt->CKTtemp;
      }
      if (!inst->NUMOSareaGiven || inst->NUMOSarea <= 0.0) {
	inst->NUMOSarea = 1.0;
      }
      if (!inst->NUMOSwidthGiven || inst->NUMOSwidth <= 0.0) {
	inst->NUMOSwidth = 1.0;
      }
      inst->NUMOSpDevice->width =
	  inst->NUMOSarea * inst->NUMOSwidth * options->OPTNdefw;

      /* Compute and save globals for this instance. */
      GLOBcomputeGlobals(&(inst->NUMOSglobals), inst->NUMOStemp);

      /* Calculate new sets of material parameters. */
      pM = model->NUMOSmatlInfo;
      pMaterial = inst->NUMOSpDevice->pMaterials;
      for (; pM != NULL; pM = pM->next, pMaterial = pMaterial->next) {

	/* Copy everything, then fix the incorrect pointer. */
	pNextMaterial = pMaterial->next;
	bcopy((char *) pM, (char *) pMaterial, sizeof(TWOmaterial));
	pMaterial->next = pNextMaterial;

	/* Now do the temperature dependence. */
	MATLtempDep(pMaterial, pMaterial->tnom);
	if (outputs->OUTPmaterial) {
	  printMaterialInfo(pMaterial);
	}
      }

      /* Assign doping to the mesh. */
      TWOsetDoping(inst->NUMOSpDevice, model->NUMOSprofiles,
	  model->NUMOSdopTables);

      /* Assign physical parameters to the mesh. */
      TWOsetup(inst->NUMOSpDevice);

      /* Assign boundary condition parameters. */
      TWOsetBCparams(inst->NUMOSpDevice, model->NUMOSboundaries);

      /* Normalize everything. */
      TWOnormalize(inst->NUMOSpDevice);

      /* Find the device's type. */
      if (inst->NUMOSpDevice->pFirstContact->pNodes[0]->netConc < 0.0) {
	inst->NUMOStype = P_CH;
	if (OneCarrier) {
	  methods->METHoneCarrier = P_TYPE;
	}
      } else {
	inst->NUMOStype = N_CH;
	if (OneCarrier) {
	  methods->METHoneCarrier = N_TYPE;
	}
      }
      inst->NUMOSpDevice->pStats->totalTime[STAT_SETUP] +=
	  SPfrontEnd->IFseconds() - startTime;
    }
  }
  return (OK);
}
