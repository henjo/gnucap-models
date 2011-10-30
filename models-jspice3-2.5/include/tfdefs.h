/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

#ifndef TF
#define TF

#include "analysis.h"

    /* TFdefs.h - defs for transfer function analyses */

typedef struct {
    int JOBtype;
    JOB *JOBnextJob;
    IFuid JOBname;
    CKTnode *TFoutPos;    /* output nodes */
    CKTnode *TFoutNeg;
    IFuid TFoutSrc;       /* device names */
    IFuid TFinSrc;
    GENERIC *TFinSrcDev;  /* pointers to devices */
    GENERIC *TFoutSrcDev;
    GENERIC *TFplot;      /* pointer to plot */
    char *TFoutName;      /* a printable name for an output v(x,y) */
    unsigned int TFoutIsV :1;
    unsigned int TFoutIsI :1;
    unsigned int TFinIsV :1;
    unsigned int TFinIsI :1;
    struct sACprms AC;    /* AC parameter storage */
    struct sDCTprms DC;   /* DC parameter storage */
} TFAN;

#define TF_OUTPOS  1
#define TF_OUTNEG  2
#define TF_OUTSRC  3
#define TF_INSRC   4
#define TF_OUTNAME 5


#ifdef __STDC__
extern int TFan(GENERIC*,int);
extern int TFaskQuest(GENERIC*,GENERIC*,int,IFvalue*);
extern int TFparse(GENERIC*,GENERIC*,int,GENERIC*,char**,GENERIC*,GENERIC*);
extern int TFsetParm(GENERIC*,GENERIC*,int,IFvalue*);
#else
extern int TFan();
extern int TFaskQuest();
extern int TFparse();
extern int TFsetParm();
#endif


#endif /* TF */
