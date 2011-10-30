/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1987 Kanwar Jit Singh
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "srcdefs.h"
#include "uflags.h"

/* Arbitrary source */
static IFparm SRCpTable[] = { /* parameters */ 
 IP(    "i",       SRC_I,         IF_PARSETREE, "Current source "),
 IP(    "v",       SRC_V,         IF_PARSETREE, "Voltage source"),
 IOP(   "type",    SRC_DEP,       IF_INTEGER,   "Type of dependency"),
 IOPP(  "dc",      SRC_DC,        IF_REAL,      "D.C. source value"),
 IOP(   "ac",      SRC_AC,        IF_REALVEC,   "AC magnitude, phase vector"),
 IOPAAU("acmag",   SRC_AC_MAG,    IF_REAL,      "A.C. Magnitude"),
 IOPAAU("acphase", SRC_AC_PHASE,  IF_REAL,      "A.C. Phase"),
 IOP(   "function",SRC_FUNC,      IF_PARSETREE, "Function specification"),
 IP(    "distof1", SRC_D_F1,      IF_REALVEC,   "f1 input for distortion"),
 IP(    "distof2", SRC_D_F2,      IF_REALVEC,   "f2 input for distortion"),
 IOP(   "gain",    SRC_GAIN,      IF_REAL    ,  "Gain of source"),
 IOP(   "control", SRC_CONTROL,   IF_INSTANCE,  "Name of controlling source"),
 OPU(   "pos_node",SRC_POS_NODE,  IF_INTEGER,   "Positive node of source"),
 OPU(   "neg_node",SRC_NEG_NODE,  IF_INTEGER,   "Negative node of source"),
 OPU(   "acreal",  SRC_AC_REAL,   IF_REAL,      "AC real part"),
 OPU(   "acimag",  SRC_AC_IMAG,   IF_REAL,      "AC imaginary part"),
 OP(    "c",       SRC_CURRENT,   IF_REAL,      "Voltage source current"),
 OP(    "p",       SRC_POWER,     IF_REAL,      "Instantaneous power"),
 OP(    "cont_p_node",SRC_CONT_P_NODE,IF_INTEGER,
                                        "Positive node of contr. source"),
 OP(    "cont_n_node",SRC_CONT_N_NODE,IF_INTEGER,
                                        "Negative node of contr. source")
};


/* model parameters */
/* static IFparm SRCmPTable[] = { }; */

static char *SRCnames1[] = {
    "src+",
    "src-",
};

static char *SRCnames2[] = {
    "src+",
    "src-",
    "srcC+",
    "srcC-"
};

static IFkeys SRCkeys[] = {
    { 'a', NUMELEMS(SRCnames1), SRCnames1, 0, 0 },
    { 'v', NUMELEMS(SRCnames1), SRCnames1, 0, 0 },
    { 'i', NUMELEMS(SRCnames1), SRCnames1, 0, 0 },
    { 'e', NUMELEMS(SRCnames2), SRCnames2, 0, 0 },
    { 'f', NUMELEMS(SRCnames1), SRCnames1, 0, 1 },
    { 'g', NUMELEMS(SRCnames2), SRCnames2, 0, 0 },
    { 'h', NUMELEMS(SRCnames1), SRCnames1, 0, 1 },
};

static int SRCkSize = NUMELEMS(SRCkeys);
static int SRCpTSize = NUMELEMS(SRCpTable);
static int SRCmPTSize = 0;
static int SRCiSize = sizeof(SRCinstance);
static int SRCmSize = sizeof(SRCmodel);


SPICEdev SRCinfo = {
    {
        "Source",
        "General source model ",

        &SRCkSize,
        SRCkeys,
        1,
        NULL,
        SRCparse,

        &SRCpTSize,
        SRCpTable,

        0/*&SRCmPTSize,
        NULL/*SRCmPTable*/,
    },

    SRCparam,
    NULL,
    SRCload,
    SRCsetup,
    SRCsetup,
    NULL,
    NULL,
    SRCfindBr,
    SRCacLoad,   /* ac and normal load functions NOT identical */
    NULL,
    SRCdestroy,
    SRCmDelete,
    SRCdelete,
    NULL,
    SRCask,
    NULL,
    SRCpzLoad,
    SRCconvTest,
    NULL,  /* DISTO */
    NULL,  /* NOISE */

    &SRCiSize,
    &SRCmSize
};
