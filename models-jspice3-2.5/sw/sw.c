/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Gordon M. Jacobs
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "swdefs.h"
#include "uflags.h"

static IFparm SWpTable[] = { /* parameters */ 
 IP(  "on",         SW_IC_ON,   IF_FLAG,     "Switch initially closed"),
 IP(  "off",        SW_IC_OFF,  IF_FLAG,     "Switch initially open"),
 IOPU("pos_node",   SW_POS_NODE,IF_INTEGER,  "Positive node of switch"),
 IOPU("neg_node",   SW_NEG_NODE,IF_INTEGER,  "Negative node of switch"),
 IOP( "control",    SW_CONTROL, IF_INSTANCE, "Name of controlling source"),
 OPU( "cont_p_node",SW_POS_CONT_NODE,IF_INTEGER,"Positive cont node of switch"),
 OPU( "cont_n_node",SW_NEG_CONT_NODE,IF_INTEGER,"Positive cont node of switch"),
 OP(  "i",          SW_CURRENT, IF_REAL,     "Switch current"),
 OP(  "p",          SW_POWER,   IF_REAL,     "Switch power")
};

static IFparm SWmPTable[] = { /* model parameters */
 IOPU("sw",   SW_MOD_SW,   IF_FLAG,"Switch model"),
 IOPU("vt",   SW_MOD_VTH,  IF_REAL,"Threshold voltage"),
 IOPU("vh",   SW_MOD_VHYS, IF_REAL,"Hysteresis voltage"),
 IOPU("it",   SW_MOD_ITH,  IF_REAL,"Threshold current"),
 IOPU("ih",   SW_MOD_IHYS, IF_REAL,"Hysterisis current"),
 IOPU("ron",  SW_MOD_RON,  IF_REAL,"Resistance when closed"),
 IOPU("roff", SW_MOD_ROFF, IF_REAL,"Resistance when open"),
 OPU( "gon",  SW_MOD_GON,  IF_REAL,"Conductance when closed"),
 OPU( "goff", SW_MOD_GOFF, IF_REAL,"Conductance when open")
};

static char *SWnames1[] = {
    "S+",
    "S-",
    "SC+",
    "SC-"
};

static char *SWnames2[] = {
    "S+",
    "S-"
};

static char *SWmodNames[] = {
    "sw",
    "csw",
    NULL
};

static IFkeys SWkeys[] = {
    { 's', NUMELEMS(SWnames1), SWnames1, 0, 0 },
    { 'w', NUMELEMS(SWnames2), SWnames2, 0, 1 },
};


static int SWkSize = NUMELEMS(SWkeys);
static int SWpTSize = NUMELEMS(SWpTable);
static int SWmPTSize = NUMELEMS(SWmPTable);
static int SWiSize = sizeof(SWinstance);
static int SWmSize = sizeof(SWmodel);


SPICEdev SWinfo = {
    {
        "Switch",
        "Ideal switch",

        &SWkSize,
        SWkeys,
        1,
        SWmodNames,
        SWparse,

        &SWpTSize,
        SWpTable,

        &SWmPTSize,
        SWmPTable,
    },

    SWparam,
    SWmParam,
    SWload,
    SWsetup,
    SWsetup,
    NULL,
    NULL,
    NULL,
    SWacLoad,   
    NULL,
    GENdestroy,
    GENmDelete,
    GENdelete,
    NULL,
    SWask,
    SWmAsk,
    SWpzLoad,
    NULL,
    NULL, /* DISTO */
    SWnoise,

    &SWiSize,
    &SWmSize
};
