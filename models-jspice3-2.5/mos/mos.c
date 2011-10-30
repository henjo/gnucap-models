/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1989 Takayasu Sakurai
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "mosdefs.h"
#include "uflags.h"


static IFparm MOSpTable[] = { /* parameters */ 
 IOPU( "temp",  MOS_TEMP,      IF_REAL, "Instance operating temperature"),
 IOPU( "l",     MOS_L,         IF_REAL, "Length"),
 IOPU( "w",     MOS_W,         IF_REAL, "Width"),
 IOPU( "ad",    MOS_AD,        IF_REAL, "Drain area"),
 IOPU( "as",    MOS_AS,        IF_REAL, "Source area"),
 IOPU( "pd",    MOS_PD,        IF_REAL, "Drain perimeter"),
 IOPU( "ps",    MOS_PS,        IF_REAL, "Source perimeter"),
 IOPU( "nrd",   MOS_NRD,       IF_REAL, "Drain squares"),
 IOPU( "nrs",   MOS_NRS,       IF_REAL, "Source squares"),
 IP(   "off",   MOS_OFF,       IF_FLAG, "Device initially off"),
 IOPAU("icvds", MOS_IC_VDS,    IF_REAL, "Initial D-S voltage"),
 IOPAU("icvgs", MOS_IC_VGS,    IF_REAL, "Initial G-S voltage"),
 IOPAU("icvbs", MOS_IC_VBS,    IF_REAL, "Initial B-S voltage"),
 IP(   "ic",    MOS_IC,        IF_REALVEC, "Vector of D-S, G-S, B-S voltages"),
 OP(   "cgd",   MOS_CGD,       IF_REAL, "Gate-Drain capacitance"),
 OP(   "cgs",   MOS_CGS,       IF_REAL, "Gate-Source capacitance"),
 OPU(  "dnode", MOS_DNODE,     IF_INTEGER, "Number of drain node"),
 OPU(  "gnode", MOS_GNODE,     IF_INTEGER, "Number of gate node"),
 OPU(  "snode", MOS_SNODE,     IF_INTEGER, "Number of source node"),
 OPU(  "bnode", MOS_BNODE,     IF_INTEGER, "Number of bulk node"),
 OPU(  "dnodeprime", MOS_DNODEPRIME,  IF_INTEGER, 
                                     "Number of internal drain node"),
 OPU(  "snodeprime", MOS_SNODEPRIME,  IF_INTEGER, 
                                     "Number of internal source node"),
 OPU(  "drainconductance",  MOS_DRAINCONDUCT,  IF_REAL, "Drain conductance"),
 OPU(  "sourceconductance", MOS_SOURCECONDUCT, IF_REAL, "Source conductance"),
 OP(   "von",   MOS_VON,       IF_REAL, " "),
 OP(   "vdsat", MOS_VDSAT,     IF_REAL, "Saturation drain voltage"),
 OPU(  "drainvcrit",  MOS_DRAINVCRIT,  IF_REAL, "Critical drain voltage"),
 OPU(  "sourcevcrit", MOS_SOURCEVCRIT, IF_REAL, "Critical source voltage"),
 OP(   "id",    MOS_CD,        IF_REAL, "Drain current"),
 OPR(  "cd",    MOS_CD,        IF_REAL, "Drain current"),
 OPU(  "ibd",   MOS_CBD,       IF_REAL, "B-D junction capacitance"),
 OPU(  "ibs",   MOS_CBS,       IF_REAL, "B-S junction capacitance"),
 OP(   "gmb",   MOS_GMBS,      IF_REAL, "Bulk-Source transconductance"),
 OPR(  "gmbs",  MOS_GMBS,      IF_REAL, "Bulk-Source transconductance"),
 OP(   "gm",    MOS_GM,        IF_REAL, "Transconductance"),
 OP(   "gds",   MOS_GDS,       IF_REAL, "Drain-Source conductance"),
 OPU(  "gbd",   MOS_GBD,       IF_REAL, "Bulk-Drain conductance"),
 OPU(  "gbs",   MOS_GBS,       IF_REAL, "Bulk-Source conductance"),
 OP(   "capbd", MOS_CAPBD,     IF_REAL, "Bulk-Drain capacitance"),
 OP(   "capbs", MOS_CAPBS,     IF_REAL, "Bulk-Source capacitance"),
 OPU(  "cbd0",  MOS_CAPZEROBIASBD,  IF_REAL,
                                    "Zero-Bias B-D junction capacitance"),
 OPU(  "cbdsw0",MOS_CAPZEROBIASBDSW,IF_REAL, " "),
 OPU(  "cbs0",  MOS_CAPZEROBIASBS,  IF_REAL,
                                    "Zero-Bias B-S junction capacitance"),
 OPU(  "cbssw0",MOS_CAPZEROBIASBSSW,IF_REAL," "),
 OPU(  "vbd",   MOS_VBD,       IF_REAL, "Bulk-Drain voltage"),
 OP(   "vbs",   MOS_VBS,       IF_REAL, "Bulk-Source voltage"),
 OP(   "vgs",   MOS_VGS,       IF_REAL, "Gate-Source voltage"),
 OP(   "vds",   MOS_VDS,       IF_REAL, "Drain-Source voltage"),
 OP(   "capgd", MOS_CAPGD,     IF_REAL, "Gate-Drain capacitance"),
 OPU(  "qgd",   MOS_QGD,       IF_REAL, "Gate-Drain charge storage"),
 OPU(  "cqgd",  MOS_CQGD,      IF_REAL,
                           "Capacitance due to gate-drain charge storage"),
 OP(   "capgs", MOS_CAPGS,     IF_REAL, "Gate-Source capacitance"),
 OPU(  "qgs",   MOS_QGS,       IF_REAL, "Gate-Source charge storage"),
 OPU(  "cqgs",  MOS_CQGS,      IF_REAL,
                           "Capacitance due to gate-source charge storage"),
 OP(   "capgb", MOS_CAPGB,     IF_REAL, "Gate-Bulk capacitance"),
 OPU(  "qgb",   MOS_QGB,       IF_REAL, "Gate-Bulk charge storage"),
 OPU(  "cqgb",  MOS_CQGB,      IF_REAL,
                           "Capacitance due to gate-bulk charge storage"),
 OPU(  "qbd",   MOS_QBD,       IF_REAL, "Bulk-Drain charge storage"),
 OPU(  "cqbd",  MOS_CQBD,      IF_REAL,
                           "Capacitance due to bulk-drain charge storage"),
 OPU(  "qbs",   MOS_QBS,       IF_REAL, "Bulk-Source charge storage"),
 OPU(  "cqbs",  MOS_CQBS,      IF_REAL,
                           "Capacitance due to bulk-source charge storage"),
 OPU(  "ig",    MOS_CG,        IF_REAL, "Gate current "),
 OPU(  "is",    MOS_CS,        IF_REAL, "Source current "),
 OPU(  "ib",    MOS_CB,        IF_REAL, "Bulk current "),
 OPU(  "p",     MOS_POWER,     IF_REAL, "Instantaneous power "),
 OP(   "rd",    MOS_DRAINRESIST,  IF_REAL, "Drain resistance"),
 OP(   "rs",    MOS_SOURCERESIST, IF_REAL, "Source resistance")
};

static IFparm MOSmPTable[] = { /* model parameters */
 OP(  "type",   MOS_MOD_TYPE,  IF_STRING, "N-channel or P-channel MOS"),
 IOP( "level",  MOS_MOD_LEVEL, IF_INTEGER,"Berkeley model level"),
 IOPU("tnom",   MOS_MOD_TNOM,  IF_REAL,   "Parameter measurement temperature"),
 IOP( "vto",    MOS_MOD_VTO,   IF_REAL,   "Threshold voltage"),
 IOPR("vt0",    MOS_MOD_VTO,   IF_REAL,   "Threshold voltage"),
 IOP( "kp",     MOS_MOD_KP,    IF_REAL,   "Transconductance parameter"),
 IOP( "gamma",  MOS_MOD_GAMMA, IF_REAL,   "Bulk threshold parameter"),
 IOP( "phi",    MOS_MOD_PHI,   IF_REAL,   "Surface potential"),
 IOP( "rd",     MOS_MOD_RD,    IF_REAL,   "Drain ohmic resistance"),
 IOP( "rs",     MOS_MOD_RS,    IF_REAL,   "Source ohmic resistance"),
 IOPA("cbd",    MOS_MOD_CBD,   IF_REAL,   "B-D junction capacitance"),
 IOPA("cbs",    MOS_MOD_CBS,   IF_REAL,   "B-S junction capacitance"),
 IOP( "is",     MOS_MOD_IS,    IF_REAL,   "Bulk junction sat. current"),
 IOP( "pb",     MOS_MOD_PB,    IF_REAL,   "Bulk junction potential"),
 IOPA("cgso",   MOS_MOD_CGSO,  IF_REAL,   "Gate-source overlap cap."),
 IOPA("cgdo",   MOS_MOD_CGDO,  IF_REAL,   "Gate-drain overlap cap."),
 IOPA("cgbo",   MOS_MOD_CGBO,  IF_REAL,   "Gate-bulk overlap cap."),
 IOPA("cj",     MOS_MOD_CJ,    IF_REAL,   "Bottom junction cap per area"),
 IOP( "mj",     MOS_MOD_MJ,    IF_REAL,   "Bottom grading coefficient"),
 IOPA("cjsw",   MOS_MOD_CJSW,  IF_REAL,   "Side junction cap per area"),
 IOP( "mjsw",   MOS_MOD_MJSW,  IF_REAL,   "Side grading coefficient"),
 IOPU("js",     MOS_MOD_JS,    IF_REAL,   "Bulk jct. sat. current density"),
 IOP( "tox",    MOS_MOD_TOX,   IF_REAL,   "Oxide thickness"),
 IOP( "ld",     MOS_MOD_LD,    IF_REAL,   "Lateral diffusion"),
 IOP( "rsh",    MOS_MOD_RSH,   IF_REAL,   "Sheet resistance"),
 IOP( "u0",     MOS_MOD_U0,    IF_REAL,   "Surface mobility"),
 IOPR("uo",     MOS_MOD_U0,    IF_REAL,   "Surface mobility"),
 IOP( "fc",     MOS_MOD_FC,    IF_REAL,   "Forward bias jct. fit parm."),
 IOP( "nss",    MOS_MOD_NSS,   IF_REAL,   "Surface state density"),
 IOP( "nsub",   MOS_MOD_NSUB,  IF_REAL,   "Substrate doping"),
 IOP( "tpg",    MOS_MOD_TPG,   IF_INTEGER,"Gate type"),
 IP(  "nmos",   MOS_MOD_NMOS,  IF_FLAG,   "N type MOSfet model"),
 IP(  "pmos",   MOS_MOD_PMOS,  IF_FLAG,   "P type MOSfet model"),
 IP(  "kf",     MOS_MOD_KF,    IF_REAL,   "Flicker noise coefficient"),
 IP(  "af",     MOS_MOD_AF,    IF_REAL,   "Flicker noise exponent"),
 IOP( "lambda", MOS_MOD_LAMBDA,IF_REAL,   "Channel length modulation"),
 IOP( "uexp",   MOS_MOD_UEXP,  IF_REAL,   "Crit. field exp for mob. deg."),
 IOP( "neff",   MOS_MOD_NEFF,  IF_REAL,   "Total channel charge coeff."),
 IOP( "ucrit",  MOS_MOD_UCRIT, IF_REAL,   "Crit. field for mob. degradation"),
 IOP( "nfs",    MOS_MOD_NFS,   IF_REAL,   "Fast surface state density"),
 IOP( "delta",  MOS_MOD_DELTA, IF_REAL,   "Width effect on threshold"),
 IOP( "vmax",   MOS_MOD_VMAX,  IF_REAL,   "Maximum carrier drift velocity"),
 IOP( "xj",     MOS_MOD_XJ,    IF_REAL,   "Junction depth"),
 IOP( "eta",    MOS_MOD_ETA,   IF_REAL,   ""),
 IOP( "theta",  MOS_MOD_THETA, IF_REAL,   "Theta"),
 IOP( "alpha",  MOS_MOD_ALPHA, IF_REAL,   "Alpha"),
 IOP( "kappa",  MOS_MOD_KAPPA, IF_REAL,   "Kappa"),
 IOP( "xd",     MOS_MOD_XD,    IF_REAL,   "Depletion layer width"),
 OP("input_delta",MOS_DELTA,   IF_REAL,   "Input delta"),
 IOP( "kv",     MOS_MOD_KV,    IF_REAL,   "Saturation voltage factor"),
 IOP( "nv",     MOS_MOD_NV,    IF_REAL,   "Saturation voltage coeff."),
 IOP( "kc",     MOS_MOD_KC,    IF_REAL,   "Saturation current factor"),
 IOP( "nc",     MOS_MOD_NC,    IF_REAL,   "Saturation current coeff."),
 IOP( "gamma1", MOS_MOD_GAMMA1,IF_REAL,   "Bulk threshold parameter 1"),
 IOP( "sigma",  MOS_MOD_SIGMA, IF_REAL,   "Static feedback effect par."),
 IOP( "lambda0",MOS_MOD_LAMDA0,IF_REAL,"Channel length modulation param. 0"),
 IOP( "lambda1",MOS_MOD_LAMDA1,IF_REAL,"Channel length modulation param. 1"),
/* CryoMOS by Luong 1/19/95 */
 IOP( "a1",MOS_MOD_A1,IF_REAL,"Cryo CMOS mobility elec. heating param. 1"),
 IOP( "a2",MOS_MOD_A2,IF_REAL,"Cryo CMOS mobility elec. heating param. 2"),
 IOP( "k1",MOS_MOD_K1,IF_REAL,"Cryo CMOS mobility param. 1"),
 IOP( "k2",MOS_MOD_K2,IF_REAL,"Cryo CMOS mobility param. 2"),
 IOP( "k3",MOS_MOD_K3,IF_REAL,"Cryo CMOS mobility param. 3"),
 IOP( "k4",MOS_MOD_K4,IF_REAL,"Cryo CMOS mobility param. 4"),
 IOP( "pclm",MOS_MOD_PCLM,IF_REAL,"Channel length modulation"),
 IOP( "em",MOS_MOD_EM,IF_REAL,"Critical field")
/**/
};                                       
                                         
static char *MOSnames[] = {                    
    "Drain",                             
    "Gate",                              
    "Source",                            
    "Bulk"                               
};

static char *MOSmodNames[] = {
    "nmos",
    "pmos",
    NULL
};

static IFkeys MOSkeys[] = {
    { 'm', NUMELEMS(MOSnames), MOSnames, 0, 0 },
};


static int MOSkSize = NUMELEMS(MOSkeys);
static int MOSpTSize = NUMELEMS(MOSpTable);
static int MOSmPTSize = NUMELEMS(MOSmPTable);
static int MOSiSize = sizeof(MOSinstance);
static int MOSmSize = sizeof(MOSmodel);


SPICEdev MOSinfo = {
    {
        "MOS",
        "Mosfet model (Berkeley levels 1 - 3 and 6)",

        &MOSkSize,
        MOSkeys,
        0xA7,    /* levels 1,2,3, and 6; and 8 for cryo MOS*/
        MOSmodNames,
        GENmosParse,

        &MOSpTSize,
        MOSpTable,

        &MOSmPTSize,
        MOSmPTable,
    },

    MOSparam,
    MOSmParam,
    MOSload,
    MOSsetup,
    MOSsetup,
    MOStemp,
    MOStrunc,
    NULL,
    MOSacLoad,
    NULL,
    GENdestroy,
    GENmDelete,
    GENdelete,
    MOSgetic,
    MOSask,
    MOSmAsk,
    MOSpzLoad,
    MOSconvTest,
    MOSdisto,
    MOSnoise,

    &MOSiSize,
    &MOSmSize
};
