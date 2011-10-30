/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1987 Thomas L. Quarles
$Id: nud2ask.c,v 1.2 2005/05/23 07:00:23 sjborley Exp $
**********/

#include "ngspice.h"
#include "const.h"
#include "ifsim.h"
#include "cktdefs.h"
#include "devdefs.h"
#include "numd2def.h"
#include "sperror.h"
#include "suffix.h"


/* Check out this one */
extern int NUMD2initSmSig(NUMD2instance *);

/* ARGSUSED */
int
NUMD2ask(ckt, inInst, which, value, select)
  CKTcircuit *ckt;
  GENinstance *inInst;
  int which;
  IFvalue *value;
  IFvalue *select;
{
  NUMD2instance *inst = (NUMD2instance *) inInst;
  switch (which) {
  case NUMD2_WIDTH:
    value->rValue = inst->NUMD2width;
    return (OK);
  case NUMD2_AREA:
    value->rValue = inst->NUMD2area;
    break;
  case NUMD2_TEMP:
    value->rValue = inst->NUMD2temp - CONSTCtoK;
    return (OK);
  case NUMD2_VD:
    value->rValue = *(ckt->CKTstate0 + inst->NUMD2voltage);
    return (OK);
  case NUMD2_ID:
    value->rValue = *(ckt->CKTstate0 + inst->NUMD2id);
    return (OK);
  case NUMD2_G11:
    value->rValue = *(ckt->CKTstate0 + inst->NUMD2conduct);
    return (OK);
  case NUMD2_G12:
    value->rValue = -*(ckt->CKTstate0 + inst->NUMD2conduct);
    return (OK);
  case NUMD2_G21:
    value->rValue = -*(ckt->CKTstate0 + inst->NUMD2conduct);
    return (OK);
  case NUMD2_G22:
    value->rValue = *(ckt->CKTstate0 + inst->NUMD2conduct);
    return (OK);
  case NUMD2_C11:
    if (!inst->NUMD2smSigAvail
	&& ckt->CKTcurrentAnalysis != DOING_TRAN) {
      NUMD2initSmSig(inst);
    }
    value->rValue = inst->NUMD2c11;
    return (OK);
  case NUMD2_C12:
    if (!inst->NUMD2smSigAvail
	&& ckt->CKTcurrentAnalysis != DOING_TRAN) {
      NUMD2initSmSig(inst);
    }
    value->rValue = -inst->NUMD2c11;
    return (OK);
  case NUMD2_C21:
    if (!inst->NUMD2smSigAvail
	&& ckt->CKTcurrentAnalysis != DOING_TRAN) {
      NUMD2initSmSig(inst);
    }
    value->rValue = -inst->NUMD2c11;
    return (OK);
  case NUMD2_C22:
    if (!inst->NUMD2smSigAvail
	&& ckt->CKTcurrentAnalysis != DOING_TRAN) {
      NUMD2initSmSig(inst);
    }
    value->rValue = inst->NUMD2c11;
    return (OK);
  case NUMD2_Y11:
    if (!inst->NUMD2smSigAvail
	&& ckt->CKTcurrentAnalysis != DOING_TRAN) {
      NUMD2initSmSig(inst);
    }
    value->cValue.real = inst->NUMD2y11r;
    value->cValue.imag = inst->NUMD2y11i;
    return (OK);
  case NUMD2_Y12:
    if (!inst->NUMD2smSigAvail
	&& ckt->CKTcurrentAnalysis != DOING_TRAN) {
      NUMD2initSmSig(inst);
    }
    value->cValue.real = -inst->NUMD2y11r;
    value->cValue.imag = -inst->NUMD2y11i;
    return (OK);
  case NUMD2_Y21:
    if (!inst->NUMD2smSigAvail
	&& ckt->CKTcurrentAnalysis != DOING_TRAN) {
      NUMD2initSmSig(inst);
    }
    value->cValue.real = -inst->NUMD2y11r;
    value->cValue.imag = -inst->NUMD2y11i;
    return (OK);
  case NUMD2_Y22:
    if (!inst->NUMD2smSigAvail
	&& ckt->CKTcurrentAnalysis != DOING_TRAN) {
      NUMD2initSmSig(inst);
    }
    value->cValue.real = inst->NUMD2y11r;
    value->cValue.imag = inst->NUMD2y11i;
    return (OK);
  default:
    return (E_BADPARM);
  }
  /* NOTREACHED */
  return (E_BADPARM);
}
