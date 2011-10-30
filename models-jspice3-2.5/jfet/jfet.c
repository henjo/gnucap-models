/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/
/********** new in 3f2
Sydney University mods Copyright(c) 1989 Anthony E. Parker, David J. Skellern
    Laboratory for Communication Science Engineering
    Sydney University Department of Electrical Engineering, Australia
**********/

#include "spice.h"
#include <stdio.h>
#include "jfetdefs.h"
#include "uflags.h"

static IFparm JFETpTable[] = { /* device parameters */ 
 IOPU( "area",        JFET_AREA,      IF_REAL,   "Area factor"),
 IOPAU("ic-vds",      JFET_IC_VDS,    IF_REAL,   "Initial D-S voltage"),
 IOPAU("ic-vgs",      JFET_IC_VGS,    IF_REAL,   "Initial G-S volrage"),
 IP(   "ic",          JFET_IC,        IF_REALVEC,"Initial VDS,VGS vector"),
 IOPU( "off",         JFET_OFF,       IF_FLAG,   "Device initially off"),
 IOPU( "temp",        JFET_TEMP,      IF_REAL,   "Instance temperature"),
 OPU(  "drain-node",  JFET_DRAINNODE, IF_INTEGER,"Number of drain node"),
 OPU(  "gate-node",   JFET_GATENODE,  IF_INTEGER,"Number of gate node"),
 OPU(  "source-node", JFET_SOURCENODE,IF_INTEGER,"Number of source node"),
 OPU(  "drain-prime-node", JFET_DRAINPRIMENODE, IF_INTEGER,
                                                 "Internal drain node"),
 OPU(  "source-prime-node",JFET_SOURCEPRIMENODE,IF_INTEGER,
                                                 "Internal source node"),
 OP(   "vgs",   JFET_VGS,  IF_REAL,   "Voltage G-S"),
 OP(   "vgd",   JFET_VGD,  IF_REAL,   "Voltage G-D"),
 OP(   "ig",    JFET_CG,   IF_REAL,   "Current at gate node"),
 OP(   "id",    JFET_CD,   IF_REAL,   "Current at drain node"),
 OP(   "igd",   JFET_CGD,  IF_REAL,   "Current G-D"),
 OP(   "gm",    JFET_GM,   IF_REAL,   "Transconductance"),
 OP(   "gds",   JFET_GDS,  IF_REAL,   "Conductance D-S"),
 OP(   "ggs",   JFET_GGS,  IF_REAL,   "Conductance G-S"),
 OP(   "ggd",   JFET_GGD,  IF_REAL,   "Conductance G-D"),
 OPU(  "qgs",   JFET_QGS,  IF_REAL,   "Charge storage G-S junction"),
 OPU(  "cqgs",  JFET_CQGS, IF_REAL,
                        "Capacitance due to charge storage G-S junction"),
 OPU(  "qgd",   JFET_QGD,  IF_REAL,   "Charge storage G-D junction"),
 OPU(  "cqgd",  JFET_CQGD, IF_REAL,
                        "Capacitance due to charge storage G-D junction"),
 OP(   "is",    JFET_CS,   IF_REAL,   "Source current"),
 OPU(  "p",     JFET_POWER,IF_REAL,   "Power dissipated by the JFET")
};

static IFparm JFETmPTable[] = { /* model parameters */
 IOP("vt0",     JFET_MOD_VTO,   IF_REAL,"Threshold voltage"),
 IOPR("vto",    JFET_MOD_VTO,   IF_REAL,"Threshold voltage"),
 IOP("beta",    JFET_MOD_BETA,  IF_REAL,"Transconductance parameter"),
 IOP("lambda",  JFET_MOD_LAMBDA,IF_REAL,"Channel length modulation param."),
 IOP("rd",      JFET_MOD_RD,    IF_REAL,"Drain ohmic resistance"),
 IOP("rs",      JFET_MOD_RS,    IF_REAL,"Source ohmic resistance"),
 IOPA("cgs",    JFET_MOD_CGS,   IF_REAL,"G-S junction capactance"),
 IOPA("cgd",    JFET_MOD_CGD,   IF_REAL,"G-D junction cap"),
 IOP("pb",      JFET_MOD_PB,    IF_REAL,"Gate junction potential"),
 IOP("is",      JFET_MOD_IS,    IF_REAL,"Gate junction saturation current"),
 IOP("fc",      JFET_MOD_FC,    IF_REAL,"Forward bias junction fir parm."),
 IP ("njf",     JFET_MOD_NJF,   IF_FLAG,"N type JFET model"),
 IP ("pjf",     JFET_MOD_PJF,   IF_FLAG,"P type JFET model"),
 IOPU("tnom",   JFET_MOD_TNOM,  IF_REAL,"parameter measurement temperature"),
 IP("kf",       JFET_MOD_KF,    IF_REAL,"Flicker Noise Coefficient"),
 IP("af",       JFET_MOD_AF,    IF_REAL,"Flicker Noise Exponent"),
 IOP("b",       JFET_MOD_B,     IF_REAL,"Doping tail parameter"),
 OPU("gd",      JFET_MOD_DRAINCONDUCT, IF_REAL,"Drain conductance"),
 OPU("gs",      JFET_MOD_SOURCECONDUCT,IF_REAL,"Source conductance"),
 OP("type",     JFET_MOD_TYPE,  IF_STRING, "N-type or P-type JFET model")
};


static char *JFETnames[] = {
    "Drain",
    "Gate",
    "Source"
};

static char *JFETmodNames[] = {
    "njf",
    "pjf",
    NULL
};

static IFkeys JFETkeys[] = {
    { 'j', NUMELEMS(JFETnames), JFETnames, 0, 0 },
};


static int JFETkSize = NUMELEMS(JFETkeys);
static int JFETpTSize = NUMELEMS(JFETpTable);
static int JFETmPTSize = NUMELEMS(JFETmPTable);
static int JFETiSize = sizeof(JFETinstance);
static int JFETmSize = sizeof(JFETmodel);


SPICEdev JFETinfo = {
    {
        "JFET",
        "Junction Field effect transistor",

        &JFETkSize,
        JFETkeys,
        1,
        JFETmodNames,
        JFETparse,

        &JFETpTSize,
        JFETpTable,

        &JFETmPTSize,
        JFETmPTable,
    },

    JFETparam,
    JFETmParam,
    JFETload,
    JFETsetup,
    JFETsetup,
    JFETtemp,
    JFETtrunc,
    NULL,
    JFETacLoad,
    NULL,
    GENdestroy,
    GENmDelete,
    GENdelete,
    JFETgetic,
    JFETask,
    JFETmAsk,
    JFETpzLoad,
    NULL,
    JFETdisto,
    JFETnoise,

    &JFETiSize,
    &JFETmSize
};
