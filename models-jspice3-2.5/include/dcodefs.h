/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

#ifndef DC_OP
#define DC_OP

    /*
     * structures used to describe D.C. operationg point analyses to
     * be performed.
     */

typedef struct {
    int JOBtype;
    JOB *JOBnextJob;
    char *JOBname;
} DCOAN;


#ifdef __STDC__
extern int DCOan(GENERIC*,int);
extern int DCOaskQuest(GENERIC*,GENERIC*,int,IFvalue*);
extern int DCOparse(GENERIC*,GENERIC*,int,GENERIC*,char**,GENERIC*,GENERIC*);
extern int DCOsetParm(GENERIC*,GENERIC*,int,IFvalue*);
#else
extern int DCOan();
extern int DCOaskQuest();
extern int DCOparse();
extern int DCOsetParm();
#endif


#endif /* DC_OP */
