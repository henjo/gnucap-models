/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1990 Jaijeet S. Roychowdhury
         1993 Stephen R. Whiteley
****************************************************************************/

/*
 * This file defines the LTRA data structures that are available to the
 * next level(s) up the calling hierarchy
 */

#include "spice.h"
#include <stdio.h>
#include <math.h>
#include "ltradefs.h"
#include "uflags.h"


static IFparm LTRApTable[] = { /* parameters */ 
 IOPAU("v1", LTRA_V1, IF_REAL,   "Initial voltage at end 1"),
 IOPAU("i1", LTRA_I1, IF_REAL,   "Initial current at end 1"),
 IOPAU("v2", LTRA_V2, IF_REAL,   "Initial voltage at end 2"),
 IOPAU("i2", LTRA_I2, IF_REAL,   "Initial current at end 2"),
 IP(   "ic", LTRA_IC, IF_REALVEC,"Initial condition vector:v1,i1,v2,i2"),
 OPU(  "pos_node1", LTRA_POS_NODE1,IF_INTEGER,
                                       "Positive node of end 1 of line"),
 OPU(  "neg_node1", LTRA_NEG_NODE1,IF_INTEGER,
                                       "Negative node of end 1 of line"),
 OPU(  "pos_node2", LTRA_POS_NODE2,IF_INTEGER,
                                       "Positive node of end 2 of line"),
 OPU(  "neg_node2", LTRA_NEG_NODE2,IF_INTEGER,
                                       "Negative node of end 2 of line"),
 OPU(  "br_eq1",    LTRA_BR_EQ1,   IF_INTEGER, "Branch equation 1"),
 OPU(  "br_eq2",    LTRA_BR_EQ2,   IF_INTEGER, "Branch equation 2"),
};

static IFparm LTRAmPTable[] = { /* model parameters */
 IP(   "ltra", LTRA_MOD_LTRA,   IF_FLAG, "LTRA model"),
 IOPU( "r",    LTRA_MOD_R,      IF_REAL, "Resistance per metre"),
 IOPAU("l",    LTRA_MOD_L,      IF_REAL, "Inductance per metre"),
 IOPU( "g",    LTRA_MOD_G,      IF_REAL, "Conductance per metre"),
 IOPAU("c",    LTRA_MOD_C,      IF_REAL, "Capacitance per metre"),
 IOPU( "len",  LTRA_MOD_LEN,    IF_REAL, "length of line"),
 IOP(  "rel",  LTRA_MOD_RELTOL, IF_REAL,
                                "Rel. rate of change of deriv. for bkpt"),
 IOP(  "abs",  LTRA_MOD_ABSTOL, IF_REAL,
                                "Abs. rate of change of deriv. for bkpt"),
 IOPU( "nocontrol",  LTRA_MOD_NOCONTROL,  IF_FLAG, "No timestep control"),
 IOPU( "halfcontrol",LTRA_MOD_HALFCONTROL,  IF_FLAG, "Half timestep control"),
 IOPU( "fullcontrol",LTRA_MOD_FULLCONTROL,  IF_FLAG, "Full timestep control"),
 IOPU( "steplimit",  LTRA_MOD_STEPLIMIT,  IF_FLAG,
                        "always limit timestep to 0.8*(delay of line)"),
 IOPU( "nosteplimit",LTRA_MOD_NOSTEPLIMIT,IF_FLAG,
                        "don't always limit timestep to 0.8*(delay of line)"),
 IOPU( "lininterp",  LTRA_MOD_LININTERP,  IF_FLAG,
                        "use linear interpolation"),
 IOPU( "quadinterp", LTRA_MOD_QUADINTERP, IF_FLAG,
                        "use quadratic interpolation"),
 IOPU( "mixedinterp",LTRA_MOD_MIXEDINTERP,IF_FLAG,
                        "use linear or quadratic interpolation"),
 IOPU( "truncnr",    LTRA_MOD_TRUNCNR,    IF_FLAG,
                        "use N-R iterations for timestep calculation"),
 IOPU( "truncdontcut",LTRA_MOD_TRUNCDONTCUT, IF_FLAG,
                        "don't limit timestep for accuracy"),
 IOPAU("compactrel", LTRA_MOD_STLINEREL,  IF_REAL,
                        "special reltol for straight line checking"),
 IOPAU("compactabs", LTRA_MOD_STLINEABS,  IF_REAL,
                        "special abstol for straight line checking"),
 OP(   "z0",    LTRA_MOD_Z0,    IF_REAL, "transmission line impedance"),
 OP(   "delay", LTRA_MOD_TD,    IF_REAL, "transmission line delay")
};

static char *LTRAnames[] = {
    "P1+",
    "P1-",
    "P2+",
    "P2-"
};

static char *LTRAmodNames[] = {
    "ltra",
    NULL
};

static IFkeys LTRAkeys[] = {
    { 'o', NUMELEMS(LTRAnames), LTRAnames, 0, 0 },
};


static int LTRAkSize = NUMELEMS(LTRAkeys);
static int LTRApTSize = NUMELEMS(LTRApTable);
static int LTRAmPTSize = NUMELEMS(LTRAmPTable);
static int LTRAiSize = sizeof(LTRAinstance);
static int LTRAmSize = sizeof(LTRAmodel);


SPICEdev LTRAinfo = {
    { "LTRA",
        "Lossy transmission line",

        &LTRAkSize,
        LTRAkeys,
        1,
        LTRAmodNames,
        LTRAparse,

        &LTRApTSize,
        LTRApTable,

        &LTRAmPTSize,
        LTRAmPTable,
    },

    LTRAparam,
    LTRAmParam,
    LTRAload,
    LTRAsetup,
    LTRAsetup,
    NULL,
    LTRAtrunc,
    NULL,
    LTRAacLoad,
    LTRAaccept,
    LTRAdestroy,
    LTRAmDelete,
    LTRAdelete,
    NULL,     /* getic */
    LTRAask,
    LTRAmAsk,
    NULL,    /* pzLoad */
    NULL,    /* convTest */
    NULL,    /* disto */
    NULL,    /* noise */

    &LTRAiSize,
    &LTRAmSize
};
