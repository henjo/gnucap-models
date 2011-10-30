/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "tradefs.h"
#include "uflags.h"

static IFparm TRApTable[] = { /* parameters */ 
 IOPU(  "z0",  TRA_Z0,    IF_REAL, "Characteristic impedance"),
 IOPR(  "zo",  TRA_Z0,    IF_REAL, "Characteristic impedance"),
 IOPAU( "f",   TRA_FREQ,  IF_REAL, "Frequency"),
 IOPAU( "td",  TRA_TD,    IF_REAL, "Transmission delay"),
 IOPAU( "nl",  TRA_NL,    IF_REAL, "Normalized length at frequency given"),
 IOPAU( "v1",  TRA_V1,    IF_REAL, "Initial voltage at end 1"),
 IOPAU( "v2",  TRA_V2,    IF_REAL, "Initial voltage at end 2"),
 IOPAU( "i1",  TRA_I1,    IF_REAL, "Initial current at end 1"),
 IOPAU( "i2",  TRA_I2,    IF_REAL, "Initial current at end 2"),
 IP(    "ic",  TRA_IC,    IF_REALVEC,"Initial condition vector:v1,i1,v2,i2"),
 OP(    "rel", TRA_RELTOL,IF_REAL, "Rel. rate of change of deriv. for bkpt"),
 OP(    "abs", TRA_ABSTOL,IF_REAL, "Abs. rate of change of deriv. for bkpt"),

 OPU(   "pos_node1", TRA_POS_NODE1, IF_INTEGER,
                                   "Positive node of end 1 of t. line"),
 OPU(   "neg_node1", TRA_NEG_NODE1, IF_INTEGER,
                                   "Negative node of end 1 of t. line"),
 OPU(   "pos_node2", TRA_POS_NODE2, IF_INTEGER,
                                   "Positive node of end 2 of t. line"),
 OPU(   "neg_node2", TRA_NEG_NODE2, IF_INTEGER,
                                   "Negative node of end 2 of t. line"),
 OPU(   "delays",TRA_DELAY, IF_REALVEC, "Delayed values of excitation")
};

    /* model parameters */
/* static IFparm TRAmPTable[] = { }; */

static char *TRAnames[] = {
    "P1+",
    "P1-",
    "P2+",
    "P2-"
};

static IFkeys TRAkeys[] = {
    { 't', NUMELEMS(TRAnames), TRAnames, 0, 0 },
};


static int TRAkSize = NUMELEMS(TRAkeys);
static int TRApTSize = NUMELEMS(TRApTable);
static int TRAmPTSize = 0;
static int TRAiSize = sizeof(TRAinstance);
static int TRAmSize = sizeof(TRAmodel);


SPICEdev TRAinfo = {
    {
        "Tranline",
        "Lossless transmission line",

        &TRAkSize,
        TRAkeys,
        1,
        NULL,
        TRAparse,

        &TRApTSize,
        TRApTable,

        0/*&TRAmPTSize,
        NULL/*TRAmPTable/**/,
    },

    TRAparam,
    NULL,
    TRAload,
    TRAsetup,
    TRAsetup,
    TRAtemp,
    TRAtrunc,
    NULL,
    TRAacLoad,
    TRAaccept,
    GENdestroy,
    GENmDelete,
    GENdelete,
    NULL,
    TRAask,
    NULL,
    NULL,
    NULL,
    NULL,  /* DISTO */
    NULL,  /* NOISE */

    &TRAiSize,
    &TRAmSize
};
