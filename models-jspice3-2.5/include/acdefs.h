/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

#ifndef ACDEFS
#define ACDEFS

#include "analysis.h"

    /* structure used to describe an AC analysis to be performed */

typedef struct {
    int JOBtype;
    JOB *JOBnextJob;    /* pointer to next job to do */
    char *JOBname;      /* name of this job */
    GENERIC *ACplot;    /* pointer to plot */
    struct sACprms AC;  /* AC parameter storage */
    struct sDCTprms DC; /* DC parameter storage */
} ACAN;


#ifdef __STDC__
extern int ACan(GENERIC*,int);
extern int ACaskQuest(GENERIC*,GENERIC*,int,IFvalue*);
extern int ACparse(GENERIC*,GENERIC*,int,GENERIC*,char**,GENERIC*,GENERIC*);
extern int ACsetParm(GENERIC*,GENERIC*,int,IFvalue*);
#else
extern int ACan();
extern int ACaskQuest();
extern int ACparse();
extern int ACsetParm();
#endif


#endif /* ACDEFS */
