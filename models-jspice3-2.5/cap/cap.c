/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "capdefs.h"
#include "uflags.h"

static IFparm CAPpTable[] = { /* parameters */ 
 IOPAP("capacitance", CAP_CAP,    IF_REAL, "Device capacitance"),
 IOPAU("ic",          CAP_IC,     IF_REAL, "Initial capacitor voltage"),
 IOPAU("w",           CAP_WIDTH,  IF_REAL, "Device width"),
 IOPAU("l",           CAP_LENGTH, IF_REAL, "Device length"),
 OP(   "i",           CAP_CURRENT,IF_REAL, "Device current"),
 OPR(  "c",           CAP_CURRENT,IF_REAL, "Device current"),
 OP(   "p",           CAP_POWER,  IF_REAL, "Instantaneous device power")
};

static IFparm CAPmPTable[] = { /* names of model parameters */
 IOPA( "cj",     CAP_MOD_CJ,      IF_REAL, "Bottom Capacitance per area"),
 IOPA( "cjsw",   CAP_MOD_CJSW,    IF_REAL, "Sidewall capacitance per meter"),
 IOPX( "defw",   CAP_MOD_DEFWIDTH,IF_REAL, "Default width"),
 IP(   "c",      CAP_MOD_C,       IF_FLAG, "Capacitor model"),
 IOPA( "narrow", CAP_MOD_NARROW,  IF_REAL, "width correction factor")
};

static char *CAPnames[] = {
    "C+",
    "C-"
};

static char *CAPmodNames[] = {
    "c",
    NULL
};

static IFkeys CAPkeys[] = {
    { 'c', NUMELEMS(CAPnames), CAPnames, 0, 0 },
};


static int CAPkSize = NUMELEMS(CAPkeys);
static int CAPpTSize = NUMELEMS(CAPpTable);
static int CAPmPTSize = NUMELEMS(CAPmPTable);
static int CAPiSize = sizeof(CAPinstance);
static int CAPmSize = sizeof(CAPmodel);


SPICEdev CAPinfo = {
    {   "Capacitor",
        "Fixed capacitor",

        &CAPkSize,
        CAPkeys,
        1,
        CAPmodNames,
        CAPparse,

        &CAPpTSize,
        CAPpTable,

        &CAPmPTSize,
        CAPmPTable,
    },
    CAPparam,
    CAPmParam,
    CAPload,
    CAPsetup,
    CAPsetup,
    CAPtemp,
    CAPtrunc,
    NULL,
    CAPacLoad,
    NULL,
    GENdestroy,
    GENmDelete,
    GENdelete,
    CAPgetic,
    CAPask,
    CAPmAsk,
    CAPpzLoad,
    NULL,
    NULL,  /* DISTO */
    NULL,  /* NOISE */

    &CAPiSize,
    &CAPmSize
};
