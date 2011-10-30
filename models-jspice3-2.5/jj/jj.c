/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Author: 1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "jjdefs.h"

static IFparm JJpTable[] = { /* parameters */ 
 IOP("area",   JJ_AREA,     IF_REAL,    "Area factor"),
 IOP("ic",     JJ_IC,       IF_REALVEC, "Initial condition vector"),
 IOP("phi",    JJ_ICP,      IF_REAL,    "Initial condition phase"),
 IOP("vj",     JJ_ICV,      IF_REAL,    "Initial condition voltage"),
 IOP("control",JJ_CON,      IF_INSTANCE,"Control inductor or voltage source"),
 OP( "icrit",  JJ_QUEST_CRT,IF_REAL,    "Maximum critical current"),
 OP( "cap",    JJ_QUEST_CAP,IF_REAL,    "Capacitance"),
 OP( "g0",     JJ_QUEST_G0, IF_REAL,    "Subgap conductance"),
 OP( "gn",     JJ_QUEST_GN, IF_REAL,    "Normal conductance"),
 OP( "gs",     JJ_QUEST_GS, IF_REAL,    "Quasiparticle onset conductance"),
 OP( "g1",     JJ_QUEST_G1, IF_REAL,    "NbN quasiparticle parameter"),
 OP( "g2",     JJ_QUEST_G2, IF_REAL,    "NbN quasiparticle parameter"),
};

static IFparm JJmPTable[] = { /* model parameters */
 OP( "jj",     JJ_MOD_JJ,    IF_FLAG,   "Model name"),
 IOP("rtype",  JJ_MOD_RT,    IF_INTEGER,"Quasiparticle current model"),
 IOP("cct",    JJ_MOD_IC,    IF_INTEGER,"Critical current model"),
 IOP("vg",     JJ_MOD_VG,    IF_REAL,   "Gap voltage"),
 IOP("delv",   JJ_MOD_DV,    IF_REAL,   "Delta gap voltage"),
 IOP("icrit",  JJ_MOD_CRT,   IF_REAL,   "Max crit current per unit area"),
 IOP("cap",    JJ_MOD_CAP,   IF_REAL,   "Capacitance per unit area"),
 IOP("r0",     JJ_MOD_R0,    IF_REAL,   "Subgap resistance for unit area"),
 IOP("rn",     JJ_MOD_RN,    IF_REAL,   "Normal resistance for unit area"),
 IOP("icon",   JJ_MOD_CCS,   IF_REAL,   "Control current first null"),
 OP( "vless",  JJ_MQUEST_VL, IF_REAL,   "Gap threshold voltage"),
 OP( "vmore",  JJ_MQUEST_VM, IF_REAL,   "Gap knee voltage"),
 OP( "vdp",    JJ_MQUEST_VDP,IF_REAL,   "Dropback voltage"),
};

static char *JJnames[] = {
    "B+",
    "B-",
    "Ph"
};

static char *JJmodNames[] = {
    "jj",
    NULL
};

static IFkeys JJkeys[] = {
    { 'b', NUMELEMS(JJnames), JJnames, 1, 0 },
};


static int JJkSize = NUMELEMS(JJkeys);
static int JJpTSize = NUMELEMS(JJpTable);
static int JJmPTSize = NUMELEMS(JJmPTable);
static int JJiSize = sizeof(JJinstance);
static int JJmSize = sizeof(JJmodel);


SPICEdev JJinfo = {
    {
        "JJ",
        "Josephson Junction model",

        &JJkSize,
        JJkeys,
        1,
        JJmodNames,
        JJparse,

        &JJpTSize,
        JJpTable,

        &JJmPTSize,
        JJmPTable,
    },

    JJparam,
    JJmParam,
    JJload,
    JJsetup,
    JJsetup,
    NULL /* JJtemp */,
    JJtrunc,
    NULL,
    NULL /* JJacLoad */,
    JJaccept,
    GENdestroy,
    GENmDelete,
    GENdelete,
    JJgetic,
    JJask,
    JJmAsk,
    NULL /* JJpzLoad */,
    NULL /* JJconvTest */,
    NULL /* JJdisto */,
    NULL /* JJnoise */,

    &JJiSize,
    &JJmSize
};
