/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "diodefs.h"
#include "uflags.h"

static IFparm DIOpTable[] = { /* parameters */ 
 IOPU( "area",    DIO_AREA,   IF_REAL, "Area factor"),
 IOPAU("ic",      DIO_IC,     IF_REAL, "Initial device voltage"),
 IOPU( "off",     DIO_OFF,    IF_FLAG, "Initially off"),
 OP(   "id",      DIO_CURRENT,IF_REAL, "Diode current"),
 OPR(  "c",       DIO_CURRENT,IF_REAL, "Diode current"),
 OP(   "vd",      DIO_VOLTAGE,IF_REAL, "Diode voltage"),
 OPR(  "voltage", DIO_VOLTAGE,IF_REAL, "Diode voltage"),
 OPU(  "charge",  DIO_CHARGE, IF_REAL, "Diode capacitor charge"),
 OPU(  "capcur",  DIO_CAPCUR, IF_REAL, "Diode capacitor current"),
 OP(   "gd",      DIO_CONDUCT,IF_REAL, "Diode conductance"),
 OPU(  "p",       DIO_POWER,  IF_REAL, "Diode power"),
 IOPU( "temp",    DIO_TEMP,   IF_REAL, "Instance temperature"),
 OP(   "cd",      DIO_CAP,    IF_REAL, "Diode capacitance")
};

static IFparm DIOmPTable[] = { /* model parameters */
 IOP( "is",  DIO_MOD_IS,  IF_REAL, "Saturation current"),
 IOP( "rs",  DIO_MOD_RS,  IF_REAL, "Ohmic resistance"),
 IOP( "n",   DIO_MOD_N,   IF_REAL, "Emission Coefficient"),
 IOPA("tt",  DIO_MOD_TT,  IF_REAL, "Transit Time"),
 IOPA("cjo", DIO_MOD_CJO, IF_REAL, "Junction capacitance"),
 IOPR("cj0", DIO_MOD_CJO, IF_REAL, "Junction capacitance"),
 IOP( "vj",  DIO_MOD_VJ,  IF_REAL, "Junction potential"),
 IOP( "m",   DIO_MOD_M,   IF_REAL, "Grading coefficient"),
 IOP( "eg",  DIO_MOD_EG,  IF_REAL, "Activation energy"),
 IOP( "xti", DIO_MOD_XTI, IF_REAL, "Saturation current temperature exp."),
 IOP( "fc",  DIO_MOD_FC,  IF_REAL, "Forward bias junction fit parameter"),
 IOP( "bv",  DIO_MOD_BV,  IF_REAL, "Reverse breakdown voltage"),
 IOP( "ibv", DIO_MOD_IBV, IF_REAL, "Current at reverse breakdown voltage"),
 OPU( "cond",DIO_MOD_COND,IF_REAL, "Ohmic conductance"),
 IP(  "d",   DIO_MOD_D,   IF_FLAG, "Diode model"),
 IOPU("tnom",DIO_MOD_TNOM,IF_REAL, "Parameter measurement temperature"),
 IP(  "kf",  DIO_MOD_KF,  IF_REAL, "flicker noise coefficient"),
 IP(  "af",  DIO_MOD_AF,  IF_REAL, "flicker noise exponent")
};

static char *DIOnames[] = {
    "D+",
    "D-"
};

static char *DIOmodNames[] = {
    "d",
    NULL
};

static IFkeys DIOkeys[] = {
    { 'd', NUMELEMS(DIOnames), DIOnames, 0, 0 },
};


static int DIOkSize = NUMELEMS(DIOkeys);
static int DIOpTSize = NUMELEMS(DIOpTable);
static int DIOmPTSize = NUMELEMS(DIOmPTable);
static int DIOiSize = sizeof(DIOinstance);
static int DIOmSize = sizeof(DIOmodel);


SPICEdev DIOinfo = {
    {
        "Diode",
        "Junction Diode model",

        &DIOkSize,
        DIOkeys,
        1,
        DIOmodNames,
        DIOparse,

        &DIOpTSize,
        DIOpTable,

        &DIOmPTSize,
        DIOmPTable,
    },

    DIOparam,
    DIOmParam,
    DIOload,
    DIOsetup,
    DIOsetup,
    DIOtemp,
    DIOtrunc,
    NULL,
    DIOacLoad,
    NULL,
    GENdestroy,
    GENmDelete,
    GENdelete,
    DIOgetic,
    DIOask,
    DIOmAsk,
    DIOpzLoad,
    DIOconvTest,
    DIOdisto,
    DIOnoise,

    &DIOiSize,
    &DIOmSize
};
