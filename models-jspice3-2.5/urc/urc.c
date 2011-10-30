/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "urcdefs.h"
#include "uflags.h"


static IFparm URCpTable[] = { /* parameters */ 
 IOPU("l",       URC_LEN,       IF_REAL,    "Length of transmission line"),
 IOPU("n",       URC_LUMPS,     IF_REAL,    "Number of lumps"),
 OPU( "pos_node",URC_POS_NODE,  IF_INTEGER, "Positive node of URC"),
 OPU( "neg_node",URC_NEG_NODE,  IF_INTEGER, "Negative node of URC"),
 OPU( "gnd",     URC_GND_NODE,  IF_INTEGER, "Ground node of URC")
};

static IFparm URCmPTable[] = { /* model parameters */
 IOP( "k",       URC_MOD_K,     IF_REAL, "Propagation constant"),
 IOPA("fmax",    URC_MOD_FMAX,  IF_REAL, "Maximum frequency of interest"),
 IOP( "rperl",   URC_MOD_RPERL, IF_REAL, "Resistance per unit length"),
 IOPA("cperl",   URC_MOD_CPERL, IF_REAL, "Capacitance per unit length"),
 IOP( "isperl",  URC_MOD_ISPERL,IF_REAL, "Saturation current per length"),
 IOP( "rsperl",  URC_MOD_RSPERL,IF_REAL, "Diode resistance per length"),
 IP(  "urc",     URC_MOD_URC,   IF_FLAG, "Uniform R.C. line model")
};

static char *URCnames[] = {
    "P1",
    "P2",
    "Ref"
};

static char *URCmodNames[] = {
    "urc",
    NULL
};

static IFkeys URCkeys[] = {
    { 'u', NUMELEMS(URCnames), URCnames, 0, 0 },
};


static int URCkSize = NUMELEMS(URCkeys);
static int URCpTSize = NUMELEMS(URCpTable);
static int URCmPTSize = NUMELEMS(URCmPTable);
static int URCiSize = sizeof(URCinstance);
static int URCmSize = sizeof(URCmodel);


SPICEdev URCinfo = {
    {
        "URC",      /* MUST precede both resistors and capacitors */
        "Uniform R.C. line",

        &URCkSize,
        URCkeys,
        1,
        URCmodNames,
        URCparse,

        &URCpTSize,
        URCpTable,

        &URCmPTSize,
        URCmPTable,
    },

    URCparam,
    URCmParam,
    NULL,
    URCsetup,
    URCsetup,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    GENdestroy,
    GENmDelete,
    GENdelete,
    NULL,
    URCask,
    URCmAsk,
    NULL,
    NULL,
    NULL,  /* DISTO */
    NULL,  /* NOISE */

    &URCiSize,
    &URCmSize
};
