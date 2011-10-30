/**********
Copyright 1991 Regents of the University of California. All rights reserved.
Authors: 1987 Karti Mayaram, 1991 David Gates
**********/

#ifndef NUMD2_H
#define NUMD2_H "NUMD2defs.h $Revision: 1.1 $  on $Date: 2003/08/11 19:35:56 $ "

/* data structures used to describe 2D numerical diodes */

/* circuit level includes */
#include "ifsim.h"
#include "cktdefs.h"
#include "gendefs.h"

/* device level includes */
#include "twomesh.h"
#include "twodev.h"
#include "profile.h"
#include "numglobs.h"
#include "carddefs.h"

/* information needed per instance */
typedef struct sNUMD2instance {
  struct sNUMD2model *NUMD2modPtr;	/* back pointer to model */
  struct sNUMD2instance *NUMD2nextInstance;	/* pointer to next instance
						 * of current model */
  IFuid NUMD2name;		/* pointer to character string naming this
				 * instance */
  int NUMD2owner;		/* number of owner process */
  int NUMD2state;		/* pointer to start of state vector for diode */

#define NUMD2voltage NUMD2state
#define NUMD2id NUMD2state+1
#define NUMD2conduct NUMD2state+2
#define NUMD2numStates 3

  int NUMD2posNode;		/* number of positive node of diode */
  int NUMD2negNode;		/* number of negative node of diode */
  TWOdevice *NUMD2pDevice;
  GLOBvalues NUMD2globals;	/* Temp.-Dep. Global Parameters */
  int NUMD2type;		/* device type pn or np */
  double NUMD2width;		/* width factor for the diode */
  double NUMD2area;		/* area factor for the diode */
  double NUMD2temp;		/* instance temperature */
  double NUMD2c11;		/* small-signal capacitance */
  double NUMD2y11r;		/* small-signal admittance, real part */
  double NUMD2y11i;		/* small-signal admittance, imag part */
  int NUMD2print;		/* number of timesteps after which print
				 * internal */
  char *NUMD2icFile;            /* Name of initial condition file */
  double *NUMD2negPosPtr;	/* pointer to sparse matrix at
				 * (negative,positive) */
  double *NUMD2posNegPtr;	/* pointer to sparse matrix at
				 * (positive,negative) */
  double *NUMD2posPosPtr;	/* pointer to sparse matrix at
				 * (positive,positive) */
  double *NUMD2negNegPtr;	/* pointer to sparse matrix at
				 * (negative,negative) */

  int NUMD2off;			/* 'off' flag for diode */
  unsigned NUMD2smSigAvail:1;	/* flag to indicate small-signal done */
  unsigned NUMD2widthGiven:1;	/* flag to indicate width was specified */
  unsigned NUMD2areaGiven:1;	/* flag to indicate area was specified */
  unsigned NUMD2icFileGiven:1;	/* flag to indicate init. cond. file given */
  unsigned NUMD2tempGiven:1;	/* flag to indicate temp was specified */
  unsigned NUMD2printGiven:1;	/* flag to indicate if print was specified */
} NUMD2instance;


/* per model data */

typedef struct sNUMD2model {	/* model structure for a diode */
  int NUMD2modType;		/* type index of this device type */
  struct sNUMD2model *NUMD2nextModel;	/* pointer to next possible model in
					 * linked list */
  NUMD2instance *NUMD2instances;/* pointer to list of instances that have
				 * this model */
  IFuid NUMD2modName;		/* pointer to character string naming this
				 * model */
  /* Everything below here is numerical-device-specific */
  MESHcard *NUMD2xMeshes;	/* list of xmesh cards */
  MESHcard *NUMD2yMeshes;	/* list of ymesh cards */
  DOMNcard *NUMD2domains;	/* list of domain cards */
  BDRYcard *NUMD2boundaries;	/* list of boundary cards */
  DOPcard *NUMD2dopings;	/* list of doping cards */
  ELCTcard *NUMD2electrodes;	/* list of electrode cards */
  CONTcard *NUMD2contacts;	/* list of contact cards */
  MODLcard *NUMD2models;	/* list of model cards */
  MATLcard *NUMD2materials;	/* list of material cards */
  MOBcard *NUMD2mobility;	/* list of mobility cards */
  METHcard *NUMD2methods;	/* list of method cards */
  OPTNcard *NUMD2options;	/* list of option cards */
  OUTPcard *NUMD2outputs;	/* list of output cards */
  TWOtranInfo *NUMD2pInfo;	/* transient analysis information */
  DOPprofile *NUMD2profiles;	/* expanded list of doping profiles */
  DOPtable *NUMD2dopTables;	/* list of tables used by profiles */
  TWOmaterial *NUMD2matlInfo;	/* list of material info structures */
} NUMD2model;

/* type of 2D diode */
#define PN  1
#define NP -1

/* device parameters */
#define NUMD2_WIDTH 1
#define NUMD2_AREA 2
#define NUMD2_IC_FILE 3
#define NUMD2_OFF 4
#define NUMD2_PRINT 5
#define NUMD2_TEMP 6
#define NUMD2_VD 7
#define NUMD2_ID 8

#define NUMD2_G11 9
#define NUMD2_C11 10
#define NUMD2_Y11 11
#define NUMD2_G12 12
#define NUMD2_C12 13
#define NUMD2_Y12 14
#define NUMD2_G21 15
#define NUMD2_C21 16
#define NUMD2_Y21 17
#define NUMD2_G22 18
#define NUMD2_C22 19
#define NUMD2_Y22 20

/* model parameters */
/* NOTE: all true model parameters have been moved to IFcardInfo structures */
#define NUMD2_MOD_NUMD 101

/* device questions */

/* model questions */

#include "numd2ext.h"

#endif				/* NUMD2_H */
