/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1988 Jaijeet S Roychowdhury
         1992 Stephen R. Whiteley
****************************************************************************/

#ifndef DISTODEF
#define DISTODEF

#ifdef D_DBG_ALLTIMES
#define D_DBG_BLOCKTIMES
#define D_DBG_SMALLTIMES
#endif

/* structure for passing a large number of values */
typedef struct {
    
    double cxx;
    double cyy;
    double czz;
    double cxy;
    double cyz;
    double cxz;
    double cxxx;
    double cyyy;
    double czzz;
    double cxxy;
    double cxxz;
    double cxyy;
    double cyyz;
    double cxzz;
    double cyzz;
    double cxyz;
    double r1h1x;
    double i1h1x;
    double r1h1y;
    double i1h1y;
    double r1h1z;
    double i1h1z;
    double r1h2x;
    double i1h2x;
    double r1h2y;
    double i1h2y;
    double r1h2z;
    double i1h2z;
    double r2h11x;
    double i2h11x;
    double r2h11y;
    double i2h11y;
    double r2h11z;
    double i2h11z;
    double h2f1f2x;
    double ih2f1f2x;
    double h2f1f2y;
    double ih2f1f2y;
    double h2f1f2z;
    double ih2f1f2z;

} DpassStr;


/* structure to keep derivatives of upto 3rd order w.r.t 3 variables */
typedef struct {

    double value;
    double d1_p;
    double d1_q;
    double d1_r;
    double d2_p2;
    double d2_q2;
    double d2_r2;
    double d2_pq;
    double d2_qr;
    double d2_pr;
    double d3_p3;
    double d3_q3;
    double d3_r3;
    double d3_p2q;
    double d3_p2r;
    double d3_pq2;
    double d3_q2r;
    double d3_pr2;
    double d3_qr2;
    double d3_pqr;

} Dderivs;

    /* structure used to describe an DISTO analysis to be performed */
typedef struct {
    int JOBtype;
    JOB *JOBnextJob;   /* pointer to next thing to do */
    char *JOBname;     /* name of this job */
    double DstartF1;   /* the start value of the higher frequency for
                          distortion analysis */
    double DstopF1;    /* the stop value ove above */
    double DfreqDelta; /* multiplier for decade/octave stepping, */
                       /* step for linear steps. */
    double DsaveF1;    /* frequency at which we left off last time*/
    int DstepType;     /* values described below */
    int DnumSteps;
    int Df2wanted;     /* set if f2overf1 is given in the disto command */
    int Df2given;      /* set if at least 1 source has an f2 input */
    double Df2ovrF1;   /* ratio of f2 over f1 if 2 frequencies given
                          should be < 1 */
    double Domega1;    /* current omega1 */
    double Domega2;    /* current omega2 */

    double* r1H1ptr;
    double* i1H1ptr;
    double* r2H11ptr;
    double* i2H11ptr;
    double* r3H11ptr;
    double* i3H11ptr;
    double* r1H2ptr;   /* distortion analysis Volterra transforms */
    double* i1H2ptr;
    double* r2H12ptr;
    double* i2H12ptr;
    double* r2H1m2ptr;
    double* i2H1m2ptr;
    double* r3H1m2ptr;
    double* i3H1m2ptr;

    double** r1H1stor;
    double** i1H1stor;
    double** r2H11stor;
    double** i2H11stor;
    double** r3H11stor;
    double** i3H11stor; /*these store computed values*/
    double** r1H2stor;        /* for the plots */
    double** i1H2stor;
    double** r2H12stor;
    double** i2H12stor;
    double** r2H1m2stor;
    double** i2H1m2stor;
    double** r3H1m2stor;
    double** i3H1m2stor;
} DISTOAN;

/* available step types: */

#define DECADE 1
#define OCTAVE 2
#define LINEAR 3

/* defns. used in DsetParm */

#define D_DEC     1
#define D_OCT     2
#define D_LIN     3
#define D_START   4
#define D_STOP    5
#define D_STEPS   6
#define D_F2OVRF1 7

/* defns. used by CKTdisto for calling different functions */

#define D_SETUP     1
#define D_F1        2
#define D_F2        3
#define D_TWOF1     4
#define D_THRF1     5
#define D_F1PF2     6
#define D_F1MF2     7
#define D_2F1MF2    8
#define D_RHSF1     9
#define D_RHSF2    10

#endif /* DISTODEFS */
