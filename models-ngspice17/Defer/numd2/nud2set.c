/**********
Copyright 1991 Regents of the University of California.  All rights reserved.
Author:	1987 Kartikeya Mayaram, U. C. Berkeley CAD Group
$Id: nud2set.c,v 1.2 2005/05/23 07:00:23 sjborley Exp $
**********/

#include "ngspice.h"
#include "cktdefs.h"
#include "smpdefs.h"
#include "numd2def.h"
#include "numconst.h"
#include "numenum.h"
#include "meshext.h"
#include "sperror.h"
#include "../../../ciderlib/twod/twoddefs.h"
#include "../../../ciderlib/twod/twodext.h"
#include "cidersupt.h"
#include "ciderinp.h"
#include "suffix.h"

#define NIL(type)   ((type *)0)
#define TSCALLOC(var, size, type)\
if (size && (!(var =(type *)calloc(1, (unsigned)(size)*sizeof(type))))) {\
   return(E_NOMEM);\
}

int
NUMD2setup(matrix, inModel, ckt, states)
  register SMPmatrix *matrix;
  GENmodel *inModel;
  CKTcircuit *ckt;
  int *states;
/*
 * load the structure with those pointers needed later for fast matrix
 * loading
 */
{
  register NUMD2model *model = (NUMD2model *) inModel;
  register NUMD2instance *inst;
  METHcard *methods;
  MODLcard *models;
  OPTNcard *options;
  OUTPcard *outputs;
  char *icFileName = NULL;
  int nameLen;
  int error, xIndex;
  int xMeshSize, yMeshSize;
  TWOdevice *pDevice;
  TWOcoord *xCoordList = NIL(TWOcoord);
  TWOcoord *yCoordList = NIL(TWOcoord);
  TWOdomain *domainList = NIL(TWOdomain);
  TWOelectrode *electrodeList = NIL(TWOelectrode);
  TWOmaterial *pM, *pMaterial = NIL(TWOmaterial), *materialList = NIL(TWOmaterial);
  DOPprofile *profileList = NIL(DOPprofile);
  DOPtable *dopTableList = NIL(DOPtable);
  double startTime;


  /* loop through all the models */
  for (; model != NULL; model = model->NUMD2nextModel) {
    if (!model->NUMD2pInfo) {
      TSCALLOC(model->NUMD2pInfo, 1, TWOtranInfo);
    }
    methods = model->NUMD2methods;
    if (!methods) {
      TSCALLOC(methods, 1, METHcard);
      model->NUMD2methods = methods;
    }
    models = model->NUMD2models;
    if (!models) {
      TSCALLOC(models, 1, MODLcard);
      model->NUMD2models = models;
    }
    options = model->NUMD2options;
    if (!options) {
      TSCALLOC(options, 1, OPTNcard);
      model->NUMD2options = options;
    }
    outputs = model->NUMD2outputs;
    if (!outputs) {
      TSCALLOC(outputs, 1, OUTPcard);
      model->NUMD2outputs = outputs;
    }
    if (!methods->METHvoltPredGiven) {
      methods->METHvoltPred = FALSE;
    }
    if (!methods->METHmobDerivGiven) {
      methods->METHmobDeriv = TRUE;
    }
    if (!methods->METHoneCarrierGiven) {
      methods->METHoneCarrier = FALSE;
    }
    if (!methods->METHacAnalysisMethodGiven) {
      methods->METHacAnalysisMethod = SOR;
    }
    if (!methods->METHdabstolGiven) {
      methods->METHdabstol = DABSTOL2D;
    }
    if (!methods->METHdreltolGiven) {
      methods->METHdreltol = ckt->CKTreltol;
    }
    if (!methods->METHitLimGiven) {
      methods->METHitLim = 50;
    }
    if (!methods->METHomegaGiven || methods->METHomega <= 0.0) {
      methods->METHomega = 2.0 * M_PI /* radians/sec */ ;
    }
    if (!options->OPTNdefaGiven || options->OPTNdefa <= 0.0) {
      options->OPTNdefa = 1.0e4 /* cm^2 */ ;
    }
    if (!options->OPTNdeflGiven || options->OPTNdefl <= 0.0) {
      options->OPTNdefl = 1.0e2 /* cm */ ;
    }
    if (!options->OPTNdefwGiven && options->OPTNdefaGiven) {
      options->OPTNdefw = options->OPTNdefa / options->OPTNdefl;
    } else if (!options->OPTNdefwGiven || options->OPTNdefw <= 0.0) {
      options->OPTNdefw = 1.0e2 /* cm */ ;
    }
    if (!options->OPTNdeviceTypeGiven) {
      options->OPTNdeviceType = OPTN_DIODE;
    }
    if (!options->OPTNicFileGiven) {
      options->OPTNicFile = NULL;
      options->OPTNunique = FALSE;		/* Can't form a unique name. */
    }
    if (!options->OPTNuniqueGiven) {
      options->OPTNunique = FALSE;
    }
    OneCarrier = methods->METHoneCarrier;

    /* Set up the rest of the card lists */
    if ((error = MODLsetup(model->NUMD2models)))
      return (error);
    BandGapNarrowing = models->MODLbandGapNarrowing;
    ConcDepLifetime = models->MODLconcDepLifetime;
    TempDepMobility = models->MODLtempDepMobility;
    ConcDepMobility = models->MODLconcDepMobility;
    SurfaceMobility = models->MODLsurfaceMobility;

    if ((error = OUTPsetup(model->NUMD2outputs)))
      return (error);
    if ((error = MATLsetup(model->NUMD2materials, &materialList)))
      return (error);
    if ((error = MOBsetup(model->NUMD2mobility, materialList)))
      return (error);
    if ((error = MESHsetup('x', model->NUMD2xMeshes, &xCoordList, &xMeshSize)))
      return (error);
    if ((error = MESHsetup('y', model->NUMD2yMeshes, &yCoordList, &yMeshSize)))
      return (error);
    if ((error = DOMNsetup(model->NUMD2domains, &domainList,
	    xCoordList, yCoordList, materialList)))
      return (error);
    if ((error = BDRYsetup(model->NUMD2boundaries,
	    xCoordList, yCoordList, domainList)))
      return (error);
    if ((error = ELCTsetup(model->NUMD2electrodes, &electrodeList,
	    xCoordList, yCoordList)))
      return (error);
    /* Make sure electrodes are OK. */
    checkElectrodes(electrodeList, 2);	/* NUMD2 has 4 electrodes */

    if ((error = CONTsetup(model->NUMD2contacts, electrodeList)))
      return (error);
    if ((error = DOPsetup(model->NUMD2dopings, &profileList,
	    &dopTableList, xCoordList, yCoordList)))
      return (error);
    model->NUMD2matlInfo = materialList;
    model->NUMD2profiles = profileList;
    model->NUMD2dopTables = dopTableList;

    /* loop through all the instances of the model */
    for (inst = model->NUMD2instances; inst != NULL;
	inst = inst->NUMD2nextInstance) {
      if (inst->NUMD2owner != ARCHme) goto matrixpointers;

      startTime = SPfrontEnd->IFseconds();

      if (!inst->NUMD2printGiven) {
	inst->NUMD2print = 0;
      } else if (inst->NUMD2print <= 0) {
	inst->NUMD2print = 1;
      }
      if (!inst->NUMD2icFileGiven) {
	if (options->OPTNunique) {
	  nameLen = strlen(options->OPTNicFile) + strlen(inst->NUMD2name) + 1;
	  TSCALLOC(icFileName, nameLen+1, char);
	  sprintf(icFileName, "%s.%s", options->OPTNicFile, inst->NUMD2name);
	  icFileName[nameLen] = '\0';
          inst->NUMD2icFile = icFileName;
	} else if (options->OPTNicFile != NULL) {
	  nameLen = strlen(options->OPTNicFile);
	  TSCALLOC(icFileName, nameLen+1, char);
	  icFileName = strcpy(icFileName, options->OPTNicFile);
	  inst->NUMD2icFile = icFileName;
	} else {
	  inst->NUMD2icFile = NULL;
	}
      }
      inst->NUMD2state = *states;
      *states += NUMD2numStates;

      if (!inst->NUMD2pDevice) {
	/* Assign the mesh and profile info to each instance. */
	TSCALLOC(pDevice, 1, TWOdevice);
	TSCALLOC(pDevice->pStats, 1, TWOstats);
	pDevice->name = inst->NUMD2name;
	pDevice->solverType = SLV_NONE;
	pDevice->numXNodes = xMeshSize;
	pDevice->numYNodes = yMeshSize;
	pDevice->xScale = MESHmkArray(xCoordList, xMeshSize);
	pDevice->yScale = MESHmkArray(yCoordList, yMeshSize);
	pDevice->abstol = methods->METHdabstol;
	pDevice->reltol = methods->METHdreltol;
	TSCALLOC(pDevice->elemArray, pDevice->numXNodes, TWOelem **);
	for (xIndex = 1; xIndex < pDevice->numXNodes; xIndex++) {
	  TSCALLOC(pDevice->elemArray[xIndex], pDevice->numYNodes, TWOelem *);
	}

	/* Create a copy of material data that can change with temperature. */
	pDevice->pMaterials = NIL(TWOmaterial);
	for (pM = materialList; pM != NIL(TWOmaterial); pM = pM->next) {
	  if (pDevice->pMaterials == NIL(TWOmaterial)) {
	    TSCALLOC(pMaterial, 1, TWOmaterial);
	    pDevice->pMaterials = pMaterial;
	  } else {
	    TSCALLOC(pMaterial->next, 1, TWOmaterial);
	    pMaterial = pMaterial->next;
	  }
	  /* Copy everything, then fix the incorrect pointer. */
	  bcopy((char *) pM, (char *) pMaterial, sizeof(TWOmaterial));
	  pMaterial->next = NIL(TWOmaterial);
	}

	/* Generate the mesh structure for the device. */
	TWObuildMesh(pDevice, domainList, electrodeList, pDevice->pMaterials);

	/* Store the device info in the instance. */
	inst->NUMD2pDevice = pDevice;
      }
      /* Now update the state pointers. */
      TWOgetStatePointers(inst->NUMD2pDevice, states);

      /* Wipe out statistics from previous runs (if any). */
      bzero((char *) inst->NUMD2pDevice->pStats, sizeof(TWOstats));

      inst->NUMD2pDevice->pStats->totalTime[STAT_SETUP] +=
	  SPfrontEnd->IFseconds() - startTime;

      /* macro to make elements with built in test for out of memory */
#define TSTALLOC(ptr,first,second) \
if ((inst->ptr = SMPmakeElt(matrix,inst->first,inst->second))==(double *)NULL){\
  return(E_NOMEM);\
}
matrixpointers:
      TSTALLOC(NUMD2posPosPtr, NUMD2posNode, NUMD2posNode)
      TSTALLOC(NUMD2negNegPtr, NUMD2negNode, NUMD2negNode)
      TSTALLOC(NUMD2negPosPtr, NUMD2negNode, NUMD2posNode)
      TSTALLOC(NUMD2posNegPtr, NUMD2posNode, NUMD2negNode)
    }
    /* Clean up lists */
    killCoordInfo(xCoordList);
    killCoordInfo(yCoordList);
    killDomainInfo(domainList);
    killElectrodeInfo(electrodeList);
  }
  return (OK);
}
