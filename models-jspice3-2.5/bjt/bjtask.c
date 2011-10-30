/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Mathew Lew and Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

/*
 * This routine gives access to the internal device 
 * parameters for BJTs
 */

#include "spice.h"
#include <math.h>
#include <stdio.h>
#include "bjtdefs.h"
#include "tskdefs.h"
#include "const.h"
#include "util.h"
#include "sperror.h"


/*ARGSUSED*/
int
BJTask(ckt,instPtr,which,value,select)

CKTcircuit *ckt;
GENinstance *instPtr;
int which;
IFvalue *value;
IFvalue *select;
{
    BJTinstance *here = (BJTinstance*)instPtr;
    double tmp;
    static char *msg = "Current and power not available for ac analysis";

    switch (which) {

        case BJT_AREA:
            value->rValue = here->BJTarea;
            break;
        case BJT_OFF:
            value->iValue = here->BJToff;
            break;
        case BJT_IC_VBE:
            value->rValue = here->BJTicVBE;
            break;
        case BJT_IC_VCE:
            value->rValue = here->BJTicVCE;
            break;
        case BJT_TEMP:
            value->rValue = here->BJTtemp - CONSTCtoK;
            break;
        case BJT_QUEST_FT:
            if (ckt->CKTstate0) {
                tmp = MAX(*(ckt->CKTstate0 + here->BJTcqbc),
                    *(ckt->CKTstate0 + here->BJTcqbx));
                value->rValue = here->BJTgm/(2 * M_PI *
                    MAX(*(ckt->CKTstate0 + here->BJTcqbe),tmp));
            }
            break;
        case BJT_QUEST_COLNODE:
            value->iValue = here->BJTcolNode;
            break;
        case BJT_QUEST_BASENODE:
            value->iValue = here->BJTbaseNode;
            break;
        case BJT_QUEST_EMITNODE:
            value->iValue = here->BJTemitNode;
            break;
        case BJT_QUEST_SUBSTNODE:
            value->iValue = here->BJTsubstNode;
            break;
        case BJT_QUEST_COLPRIMENODE:
            value->iValue = here->BJTcolPrimeNode;
            break;
        case BJT_QUEST_BASEPRIMENODE:
            value->iValue = here->BJTbasePrimeNode;
            break;
        case BJT_QUEST_EMITPRIMENODE:
            value->iValue = here->BJTemitPrimeNode;
            break;
        case BJT_QUEST_VBE:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->BJTvbe);
            break;
        case BJT_QUEST_VBC:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->BJTvbc);
            break;
        case BJT_QUEST_CC:
            if (ckt->CKTstate0)
                value->rValue = here->BJTcc;
            break;
        case BJT_QUEST_CB:
            if (ckt->CKTstate0)
                value->rValue = here->BJTcb;
            break;
        case BJT_QUEST_GPI:
            if (ckt->CKTstate0)
                value->rValue = here->BJTgpi;
            break;
        case BJT_QUEST_GMU:
            if (ckt->CKTstate0)
                value->rValue = here->BJTgmu;
            break;
        case BJT_QUEST_GM:
            if (ckt->CKTstate0)
                value->rValue = here->BJTgm;
            break;
        case BJT_QUEST_GO:
            if (ckt->CKTstate0)
                value->rValue = here->BJTgo;
            break;
        case BJT_QUEST_QBE:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->BJTqbe);
            break;
        case BJT_QUEST_CQBE:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->BJTcqbe);
            break;
        case BJT_QUEST_QBC:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->BJTqbc);
            break;
        case BJT_QUEST_CQBC:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->BJTcqbc);
            break;
        case BJT_QUEST_QCS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->BJTqcs);
            break;
        case BJT_QUEST_CQCS:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->BJTcqcs);
            break;
        case BJT_QUEST_QBX:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->BJTqbx);
            break;
        case BJT_QUEST_CQBX:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->BJTcqbx);
            break;
        case BJT_QUEST_GX:
            if (ckt->CKTstate0)
                value->rValue = here->BJTgx;
            break;
        case BJT_QUEST_CEXBC:
            if (ckt->CKTstate0)
                value->rValue = *(ckt->CKTstate0 + here->BJTcexbc);
            break;
        case BJT_QUEST_GEQCB:
            if (ckt->CKTstate0)
                value->rValue = here->BJTgeqcb;
            break;
        case BJT_QUEST_GCCS:
            if (ckt->CKTstate0)
                value->rValue = here->BJTgccs;
            break;
        case BJT_QUEST_GEQBX:
            if (ckt->CKTstate0)
                value->rValue = here->BJTgeqbx;
            break;
        case BJT_QUEST_CE:
            if (ckt->CKTcurrentAnalysis & DOING_AC) {
                errMsg = MALLOC(strlen(msg)+1);
                errRtn = "BJTask";
                strcpy(errMsg,msg);
                return (E_ASKCURRENT);
            }
            if (ckt->CKTstate0) {
                value->rValue = -here->BJTcc;
                value->rValue -= here->BJTcb;
                if ((ckt->CKTcurrentAnalysis & DOING_TRAN) && 
                        !(ckt->CKTmode & MODETRANOP)) {
                    value->rValue += *(ckt->CKTstate0 + here->BJTcqcs);
                }
            }
            break;
        case BJT_QUEST_CS:  
            if (ckt->CKTcurrentAnalysis & DOING_AC) {
                errMsg = MALLOC(strlen(msg)+1);
                errRtn = "BJTask";
                strcpy(errMsg,msg);
                return (E_ASKCURRENT);
            }
            if (ckt->CKTcurrentAnalysis & DOING_TRAN) {
                if (!(ckt->CKTmode & MODETRANOP)) {
                    if (ckt->CKTstate0)
                        value->rValue = -*(ckt->CKTstate0 + here->BJTcqcs);
                }
            }
            break;
        case BJT_QUEST_POWER:
            if (ckt->CKTcurrentAnalysis & DOING_AC) {
                errMsg = MALLOC(strlen(msg)+1);
                errRtn = "BJTask";
                strcpy(errMsg,msg);
                return (E_ASKPOWER);
            }
            if (ckt->CKTstate0 && ckt->CKTrhsOld) {
                value->rValue = here->BJTcc *               
                        *(ckt->CKTrhsOld + here->BJTcolNode);
                value->rValue += here->BJTcb *        
                        *(ckt->CKTrhsOld + here->BJTbaseNode);
                if ((ckt->CKTcurrentAnalysis & DOING_TRAN) &&
                    !(ckt->CKTmode & MODETRANOP)) {
                    value->rValue -= *(ckt->CKTstate0 + here->BJTcqcs) *
                            *(ckt->CKTrhsOld + here->BJTsubstNode);
                }
                tmp = -here->BJTcc;
                tmp -= here->BJTcb;
                if ((ckt->CKTcurrentAnalysis & DOING_TRAN) && 
                        !(ckt->CKTmode & MODETRANOP)) {
                    tmp += *(ckt->CKTstate0 + here->BJTcqcs);
                }
                value->rValue += tmp * *(ckt->CKTrhsOld + 
                        here->BJTemitNode);
            }
            break;
        case BJT_QUEST_CPI:
            value->rValue = here->BJTcapbe;
            break;
        case BJT_QUEST_CMU:
            value->rValue = here->BJTcapbc;
            break;
        case BJT_QUEST_CBX:
            value->rValue = here->BJTcapbx;
            break;
        case BJT_QUEST_CCS:
            value->rValue = here->BJTcapcs;
            break;
        default:
            return (E_BADPARM);
    }
    return (OK);
}

