/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "inddefs.h"
#include "uflags.h"

static IFparm INDpTable[] = { /* parameters */ 
 IOPAP("inductance",IND_IND,    IF_REAL,"Inductance of inductor"),
 IOPAU("ic",        IND_IC,     IF_REAL,"Initial current through inductor"),
 OP(   "flux",      IND_FLUX,   IF_REAL,"Flux through inductor"),
 OP(   "v",         IND_VOLT,   IF_REAL,"Terminal voltage of inductor"),
 OPR(  "volt",      IND_VOLT,   IF_REAL,"Terminal voltage of inductor"),
 OP(   "i",         IND_CURRENT,IF_REAL,"Current through the inductor"),
 OPR(  "current",   IND_CURRENT,IF_REAL,"Current through the inductor"),
 OP(   "p",         IND_POWER,  IF_REAL,
                                "instantaneous power stored by the inductor")
};

/* model parameters */
/* static IFparm INDmPTable[] = { }; */

static char *INDnames[] = {
    "L+",
    "L-"
};

static IFkeys INDkeys[] = {
    { 'l', NUMELEMS(INDnames), INDnames, 0, 0 },
};


static int INDkSize = NUMELEMS(INDkeys);
static int INDpTSize = NUMELEMS(INDpTable);
static int INDmPTSize = 0;
static int INDiSize = sizeof(INDinstance);
static int INDmSize = sizeof(INDmodel);

SPICEdev INDinfo = {
    {
        "Inductor",
        "Inductors",

        &INDkSize,
        INDkeys,
        1,
        NULL,
        INDparse,

        &INDpTSize,
        INDpTable,

        0/*&INDmPTSize,
        NULL/*INDmPTable*/,
    },

    INDparam,
    NULL,
    INDload,
    INDsetup,
    INDsetup,
    NULL,
    INDtrunc,
    INDfindBr,
    INDacLoad,
    NULL,
    GENdestroy,
    GENmDelete,
    GENdelete,
    INDgetic,
    INDask,
    NULL,
    INDpzLoad,
    NULL,
    NULL,  /* DISTO */
    NULL,  /* NOISE */

    &INDiSize,
    &INDmSize
};

#ifdef MUTUAL

static IFparm MUTpTable[] = { /* parameters */ 
 IOPAP("k",          MUT_COEFF,IF_REAL,     "Mutual inductance"),
 IOPR( "coefficient",MUT_COEFF,IF_REAL,     "Mutual inductance"),
 IOP(  "inductor1",  MUT_IND1, IF_INSTANCE, "First coupled inductor"),
 IOP(  "inductor2",  MUT_IND2, IF_INSTANCE, "Second coupled inductor"),
};

/* model parameters */
/* static IFparm MUTmPTable[] = { }; */

static IFkeys MUTkeys[] = {
    { 'k', 0, NULL, 0, 2 },
};


static int MUTkSize = NUMELEMS(MUTkeys);
static int MUTpTSize = NUMELEMS(MUTpTable);
static int MUTmPTSize = 0;
static int MUTiSize = sizeof(MUTinstance);
static int MUTmSize = sizeof(MUTmodel);

SPICEdev MUTinfo = {
    {   
        "mutual",
        "Mutual inductors",

        &MUTkSize,
        MUTkeys,
        1,
        NULL,
        MUTparse,

        &MUTpTSize,
        MUTpTable,

        0/*&INDmPTSize,
        NULL/*MUTmPTable*/,
    },

    MUTparam,
    NULL,
    NULL,/* load handled by INDload */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    GENdestroy,
    GENmDelete,
    GENdelete,
    NULL,
    MUTask,
    NULL,
    NULL, /* PZ */
    NULL,
    NULL, /* DISTO */
    NULL, /* NOISE */

    &MUTiSize,
    &MUTmSize
};

#endif /*MUTUAL*/
