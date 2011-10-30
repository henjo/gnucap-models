/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

#ifndef DCTR
#define DCTR

#include "analysis.h"

    /*
     * structures used to describe D.C. transfer curve analyses to
     * be performed.
     */

typedef struct {
    int JOBtype;
    JOB *JOBnextJob;       /* pointer to next thing to do */
    char *JOBname;         /* name of this job */
    GENERIC *DCTplot;      /* pointer to plot */
    struct sDCTprms DC;    /* DC parameter storage */
} DCTAN;


#ifdef __STDC__
extern int DCTan(GENERIC*,int);
extern int DCTaskQuest(GENERIC*,GENERIC*,int,IFvalue*);
extern int DCTparse(GENERIC*,GENERIC*,int,GENERIC*,char**,GENERIC*,GENERIC*);
extern int DCTsetParm(GENERIC*,GENERIC*,int,IFvalue*);
#else
extern int DCTan();
extern int DCTaskQuest();
extern int DCTparse();
extern int DCTsetParm();
#endif


#endif /* DCTR */
