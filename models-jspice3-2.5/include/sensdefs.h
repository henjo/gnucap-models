/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: UCB CAD Group
         1993 Stephen R. Whiteley
****************************************************************************/

#ifndef SENS_DEFS
#define SENS_DEFS

#include "analysis.h"

    /* structures used to describe an Adjoint Sensitivity analysis */


/* internal data */
struct sSENSint {
    SMPmatrix *dY;
    double *dIr, *dIi, *dIdYr, *dIdYi;
    int size;
    IFcomplex *o_cvalues;
    double *o_values;
};


typedef struct st_output Output;
typedef struct st_nodes Nodes;
typedef struct st_paramsenlist ParamSenList;
typedef struct st_modsenlist ModSenList;
typedef struct st_devsenlist DevSenList;

typedef struct {
    int     JOBtype;
    JOB     *JOBnextJob;   /* pointer to next thing to do */
    char    *JOBname;      /* name of this job */

    DevSenList *first;

    CKTnode *SENSoutPos;   /* output positive node */
    CKTnode *SENSoutNeg;   /* output reference node */
    IFuid   SENSoutSrc;    /* output source UID */
    GENERIC *SENSoutSrcDev;/* pointer to output device */
    char    *SENSoutName;  /* name of output, e.g. v(1,2) */
    double  SENSdefTol;    /* default tolerance */
    double  SENSdefPert;   /* default perturbation */
    unsigned SENSpct :1;   /* flag to indicate pct change */
    GENERIC *SENSplot;     /* pointer to plot */
    struct sSENSint ST;    /* internal variables, pass to subroutines */
    struct sACprms AC;     /* AC parameter storage */
    struct sDCTprms DC;    /* DC parameter storage */

} SENSAN;

struct st_output {
    int     type;
    int     pos, neg;
};

struct st_nodes {
    int     pos, neg;
};

struct st_paramsenlist {
    ParamSenList *next;
    int     param_no;
    double  delta, tol;
};

struct st_modsenlist {
    ModSenList *next;
    int     mod_no;
    ParamSenList *first;
};

struct st_devsenlist {
    DevSenList *next;
    int     dev_no;
    ModSenList *first;
};


#define    SENS_POS         2
#define    SENS_NEG         3
#define    SENS_SRC         4
#define    SENS_NAME        5
#define    SENS_DEFTOL      6
#define    SENS_DEFPERT     7
#define    SENS_DEFPERTURB  8
#define    SENS_DEVDEFTOL   9
#define    SENS_DEVDEFPERT  10
#define    SENS_TYPE        11
#define    SENS_DEVICE      12
#define    SENS_PARAM       13
#define    SENS_TOL         14
#define    SENS_PERT        15


#ifdef __STDC__
extern int SENSan(GENERIC*,int);
extern int SENSaskQuest(GENERIC*,GENERIC*,int,IFvalue*);
extern int SENSparse(GENERIC*,GENERIC*,int,GENERIC*,char**,GENERIC*,GENERIC*);
extern int SENSsetParm(GENERIC*,GENERIC*,int,IFvalue*);
#else
extern int SENSan();
extern int SENSaskQuest();
extern int SENSparse();
extern int SENSsetParm();
#endif


#endif /* DEFS */

