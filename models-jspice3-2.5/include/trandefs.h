/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

#ifndef TRAN
#define TRAN

#include "analysis.h"

    /* TRANdefs.h - defs for transient analyses */


/* structure to hold private parameters for transient analysis */
struct sTRANint {
    double tcheck;      /* running variable for plot points */
    double tend;        /* final time minus tolerance */
    double startdTime;  /* statistics */
    double startsTime;
    double startTime;
    double delmax;      /* maximum time delta */
    int  startIters;    /* statistics */
    int  polydegree;    /* interpolation degree */
    int  nointerp;      /* don't interpolate */
    int  hitusertp;     /* hit plot points like breakpoints */
    int  dumpit;        /* output the next point */
    int  firsttime;     /* first time through */
    int  nojjtp;        /* use truncation error for jj timestep */
    int  count;         /* output point count */
};


typedef struct {
    int JOBtype;
    JOB *JOBnextJob;      /* pointer to next thing to do */
    char *JOBname;        /* name of this job */
    double TRANfinalTime; /* final analysis point */
    double TRANstep;      /* user timestep */
    double TRANmaxStep;   /* maximum internal timestep */
    double TRANinitTime;  /* initial user output point */
    long TRANmode;        /* mode UIC? */
    GENERIC *TRANplot;    /* pointer to plot */
    struct sTRANint TS;   /* pass this to subroutines */
    struct sDCTprms DC;   /* DC parameter storage */
} TRANAN;

#define TRAN_TSTART 1
#define TRAN_TSTOP  2
#define TRAN_TSTEP  3
#define TRAN_TMAX   4
#define TRAN_UIC    5  


#ifdef __STDC__
extern int TRANan(GENERIC*,int);
extern int TRANaskQuest(GENERIC*,GENERIC*,int,IFvalue*);
extern int TRANinit(GENERIC*,GENERIC*);
extern int TRANparse(GENERIC*,GENERIC*,int,GENERIC*,char**,GENERIC*,GENERIC*);
extern int TRANsetParm(GENERIC*,GENERIC*,int,IFvalue*);
#else
extern int TRANan();
extern int TRANaskQuest();
extern int TRANinit();
extern int TRANparse();
extern int TRANsetParm();
#endif


#endif /* TRAN */
