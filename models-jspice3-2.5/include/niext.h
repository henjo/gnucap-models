/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

#ifndef NIEXT
#define NIEXT

#ifdef __STDC__
extern int  NIacIter(CKTcircuit*);
extern int  NIcomCof(CKTcircuit*); 
extern int  NIconvTest(CKTcircuit*);
extern void NIdestroy(CKTcircuit*);
extern int  NIdIter(CKTcircuit*);
extern int  NIinit(CKTcircuit *);
extern int  NIintegrate(CKTcircuit*,double*,double*,double,int);
extern int  NIiter(CKTcircuit*,int);
extern void NInzIter(CKTcircuit*,int,int);
extern int  NIpred(CKTcircuit*);
extern void NIprint(CKTcircuit*);

#ifdef PZDEFS
extern int  NIpzMuller(PZtrial**,PZtrial*);
extern int  NIpzComplex(PZtrial**,PZtrial*);
extern int  NIpzSym(PZtrial**,PZtrial*);
extern int  NIpzSym2(PZtrial**,PZtrial*);
#endif

extern int  NIreinit(CKTcircuit*);
extern int  NIsenReinit(CKTcircuit*);
extern int  NIsum(CKTcircuit*,double*,int);
#else
extern int  NIacIter();
extern int  NIcomCof(); 
extern int  NIconvTest();
extern void NIdestroy();
extern int  NIdIter();
extern int  NIinit();
extern int  NIintegrate();
extern int  NIiter();
extern void NInzIter();
extern int  NIpred();
extern void NIprint();
extern int  NIpzMuller();
extern int  NIpzSolve();
extern int  NIreinit();
extern int  NIsenReinit();
extern int  NIsum();
#endif

#endif
