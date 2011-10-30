/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "resdefs.h"
#include "uflags.h"

static IFparm RESpTable[] = { /* parameters */ 
 IOPP( "resistance", RES_RESIST, IF_REAL,"Resistance"),
 IOPZU("temp",       RES_TEMP,   IF_REAL,"Instance operating temperature"),
 IOPQU("w",          RES_WIDTH,  IF_REAL,"Width"),
 IOPZU("l",          RES_LENGTH, IF_REAL,"Length"),
 OP(   "c",          RES_CURRENT,IF_REAL,"Current"),
 OP(   "p",          RES_POWER,  IF_REAL,"Power"),
} ;

static IFparm RESmPTable[] = { /* model parameters */
 IOPQ( "rsh",    RES_MOD_RSH,      IF_REAL,"Sheet resistance"),
 IOPZ( "narrow", RES_MOD_NARROW,   IF_REAL,"Narrowing of resistor"),
 IOPQ( "tc1",    RES_MOD_TC1,      IF_REAL,"First order temp. coefficient"),
 IOPQO("tc2",    RES_MOD_TC2,      IF_REAL,"Second order temp. coefficient"),
 IOPX( "defw",   RES_MOD_DEFWIDTH, IF_REAL,"Default device width"),
 IOPXU("tnom",   RES_MOD_TNOM,     IF_REAL,"Parameter measurement temperature"),
 IP(   "r",      RES_MOD_R,        IF_FLAG,"Device is a resistor model"),
};

static char *RESnames[] = {
    "R+",
    "R-"
};

static char *RESmodNames[] = {
    "r",
    NULL
};

static IFkeys RESkeys[] = {
    { 'r', NUMELEMS(RESnames), RESnames, 0, 0 },
};


static int RESkSize = NUMELEMS(RESkeys);
static int RESpTSize = NUMELEMS(RESpTable);
static int RESmPTSize = NUMELEMS(RESmPTable);
static int RESiSize = sizeof(RESinstance);
static int RESmSize = sizeof(RESmodel);


SPICEdev RESinfo = {
    {
        "Resistor",
        "Simple linear resistor",

        &RESkSize,
        RESkeys,
        1,
        RESmodNames,
        RESparse,

        &RESpTSize,
        RESpTable,

        &RESmPTSize,
        RESmPTable,
    },

    RESparam,
    RESmParam,
    NULL,      /* values cached in matrix imag part */
    RESsetup,
    RESsetup,
    REStemp,
    NULL,
    NULL,
    RESacLoad,
    NULL,
    GENdestroy,
    GENmDelete,
    GENdelete,
    NULL,
    RESask,
    RESmAsk,
    RESpzLoad,
    NULL,
    NULL, /* Disto */
    RESnoise,

    &RESiSize,
    &RESmSize
};
