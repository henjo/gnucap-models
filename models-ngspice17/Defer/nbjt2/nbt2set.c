/**********
Copyright 1991 Regents of the University of California.  All rights reserved.
Author:	1987 Kartikeya Mayaram, U. C. Berkeley CAD Group
$Id: nbt2set.c,v 1.2 2005/05/21 13:33:49 sjborley Exp $
**********/

#include "ngspice.h"
#include "cktdefs.h"
#include "smpdefs.h"
#include "nbjt2def.h"
#include "numconst.h"
#include "numenum.h"
#include "meshext.h"
#include "sperror.h"
#include "../../../ciderlib/twod/twoddefs.h"
#include "../../../ciderlib/twod/twodext.h"
#include "ciderinp.h"
#include "cidersupt.h"
#include "suffix.h"

#define NIL(type)   ((type *)0)
#define TSCALLOC(var, size, type)\
if (size && (!(var =(type *)calloc(1, (unsigned)(size)*sizeof(type))))) {\
   return(E_NOMEM);\
}

int 
NBJT2setup(matrix, inModel, ckt, states)
  register SMPmatrix *matrix;
  GENmodel *inModel;
  CKTcircuit *ckt;
  int *states;
/*
 * load the structure with those pointers needed later for fast matrix
 * loading
 */
{
  register NBJT2model *model = (NBJT2model *) inModel;
  register NBJT2instance *inst;
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
  for (; model != NULL; model = model->NBJT2nextModel) {
    if (!model->NBJT2pInfo) {
      TSCALLOC(model->NBJT2pInfo, 1, TWOtranInfo);
    }
    methods = model->NBJT2methods;
    if (!methods) {
      TSCALLOC(methods, 1, METHcard);
      model->NBJT2methods = methods;
    }
    models = model->NBJT2models;
    if (!models) {
      TSCALLOC(models, 1, MODLcard);
      model->NBJT2models = models;
    }
    options = model->NBJT2options;
    if (!options) {
      TSCALLOC(options, 1, OPTNcard);
      model->NBJT2options = options;
    }
    outputs = model->NBJT2outputs;
    if (!outputs) {
      TSCALLOC(outputs, 1, OUTPcard);
      model->NBJT2outputs = outputs;
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
      options->OPTNdeviceType = OPTN_BIPOLAR;
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
    if ((error = MODLsetup(model->NBJT2models)))
      return (error);
    BandGapNarrowing = models->MODLbandGapNarrowing;
    ConcDepLifetime = models->MODLconcDepLifetime;
    TempDepMobility = models->MODLtempDepMobility;
    ConcDepMobility = models->MODLconcDepMobility;
    SurfaceMobility = models->MODLsurfaceMobility;

    if ((error = OUTPsetup(model->NBJT2outputs)))
      return (error);
    if ((error = MATLsetup(model->NBJT2materials, &materialList)))
      return (error);
    if ((error = MOBsetup(model->NBJT2mobility, materialList)))
      return (error);
    if ((error = MESHsetup('x', model->NBJT2xMeshes, &xCoordList, &xMeshSize)))
      return (error);
    if ((error = MESHsetup('y', model->NBJT2yMeshes, &yCoordList, &yMeshSize)))
      return (error);
    if ((error = DOMNsetup(model->NBJT2domains, &domainList,
	    xCoordList, yCoordList, materialList)))
      return (error);
    if ((error = BDRYsetup(model->NBJT2boundaries,
	    xCoordList, yCoordList, domainList)))
      return (error);
    if ((error = ELCTsetup(model->NBJT2electrodes, &electrodeList,
	    xCoordList, yCoordList)))
      return (error);
    /* Make sure electrodes are OK. */
    checkElectrodes(electrodeList, 3);	/* NBJT2 has 3 electrodes */

    if ((error = CONTsetup(model->NBJT2contacts, electrodeList)))
      return (error);
    if ((error = DOPsetup(model->NBJT2dopings, &profileList,
	    &dopTableList, xCoordList, yCoordList)))
      return (error);
    model->NBJT2matlInfo = materialList;
    model->NBJT2profiles = profileList;
    model->NBJT2dopTables = dopTableList;

    /* loop through all the instances of the model */
    for (inst = model->NBJT2instances; inst != NULL;
	inst = inst->NBJT2nextInstance) {
      if (inst->NBJT2owner != ARCHme) goto matrixpointers;

      startTime = SPfrontEnd->IFseconds();

      if (!inst->NBJT2printGiven) {
	inst->NBJT2print = 0;
      } else if (inst->NBJT2print <= 0) {
	inst->NBJT2print = 1;
      }
      if (!inst->NBJT2icFileGiven) {
	if (options->OPTNunique) {
	  nameLen = strlen(options->OPTNicFile) + strlen(inst->NBJT2name) + 1;
	  TSCALLOC(icFileName, nameLen+1, char);
	  sprintf(icFileName, "%s.%s", options->OPTNicFile, inst->NBJT2name);
	  icFileName[nameLen] = '\0';
          inst->NBJT2icFile = icFileName;
	} else if (options->OPTNicFile != NULL) {
	  nameLen = strlen(options->OPTNicFile);
	  TSCALLOC(icFileName, nameLen+1, char);
	  icFileName = strcpy(icFileName, options->OPTNicFile);
	  inst->NBJT2icFile = icFileName;
	} else {
	  inst->NBJT2icFile = NULL;
	}
      }
      inst->NBJT2state = *states;
      *states += NBJT2numStates;

      if (!inst->NBJT2pDevice) {
	/* Assign the mesh and profile info to each instance. */
	TSCALLOC(pDevice, 1, TWOdevice);
	TSCALLOC(pDevice->pStats, 1, TWOstats);
	pDevice->name = inst->NBJT2name;
	pDevice->solverType = SLV_NONE;
	pDevice->numXNodes = xMeshSize;
	pDevice->numYNodes = yMeshSize;
	pDevice->xScale = MESHmkArray(xCoordList, xMeshSize);
	pDevice->yScale = MESHmkArray(yCoordList, yMeshSize);
	pDevice->abstol = methods->METHdabstol;
	pDevice->reltol = methods->METHdreltol;
	pDevice->rhsImag = NIL(double);
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
	inst->NBJT2pDevice = pDevice;
      }
      /* Now update the state pointers. */
      TWOgetStatePointers(inst->NBJT2pDevice, states);

      /* Wipe out statistics from previous runs (if any). */
      bzero((char *) inst->NBJT2pDevice->pStats, sizeof(TWOstats));

      inst->NBJT2pDevice->pStats->totalTime[STAT_SETUP] +=
	  SPfrontEnd->IFseconds() - startTime;

      /* macro to make elements with built in test for out of memory */
#define TSTALLOC(ptr,first,second) \
if ((inst->ptr = SMPmakeElt(matrix,inst->first,inst->second))==(double *)NULL){\
  return(E_NOMEM);\
}

matrixpointers:
      TSTALLOC(NBJT2colColPtr, NBJT2colNode, NBJT2colNode)
      TSTALLOC(NBJT2colBasePtr, NBJT2colNode, NBJT2baseNode)
      TSTALLOC(NBJT2colEmitPtr, NBJT2colNode, NBJT2emitNode)
      TSTALLOC(NBJT2baseColPtr, NBJT2baseNode, NBJT2colNode)
      TSTALLOC(NBJT2baseBasePtr, NBJT2baseNode, NBJT2baseNode)
      TSTALLOC(NBJT2baseEmitPtr, NBJT2baseNode, NBJT2emitNode)
      TSTALLOC(NBJT2emitColPtr, NBJT2emitNode, NBJT2colNode)
      TSTALLOC(NBJT2emitBasePtr, NBJT2emitNode, NBJT2baseNode)
      TSTALLOC(NBJT2emitEmitPtr, NBJT2emitNode, NBJT2emitNode)
    }
    /* Clean up lists */
    killCoordInfo(xCoordList);
    killCoordInfo(yCoordList);
    killDomainInfo(domainList);
    killElectrodeInfo(electrodeList);
  }
  return (OK);
}
