/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1988 Jaijeet S Roychowdhury
         1992 Stephen R. Whiteley
****************************************************************************/

#ifndef DISTOEXT
#define DISTOEXT

#ifdef __STDC__
#ifdef CKT
/* only if cktdefs.h has been included */
extern int DISTOan(GENERIC *, int);
extern int DISTOaskQuest(GENERIC*,GENERIC*,int,IFvalue*);
extern int DISTOparse(GENERIC*,GENERIC*,int,GENERIC*,char**,GENERIC*,GENERIC*);
extern int DISTOsetParm(GENERIC*,GENERIC*,int,IFvalue*);
#endif
extern double D1i2F1(double, double, double);
extern double D1i3F1(double, double, double, double, double, double);
extern double D1iF12(double, double, double, double, double);
extern double D1i2F12(double, double, double, double, double, double, double,
                double, double, double);
extern double D1n2F1(double, double, double);
extern double D1n3F1(double, double, double, double, double, double);
extern double D1nF12(double, double, double, double, double);
extern double D1n2F12(double, double, double, double, double, double, double,
                double, double, double);
extern double DFn2F1(double, double, double, double, double,
                double, double, double, double, double, double, double);
extern double DFi2F1(double, double, double, double, double,
                double, double, double, double, double, double, double);
extern double DFi3F1(double, double, double, double, 
                double, double, double, double, double, double, double,
                double, double, double, double, double, double, double,
                double, double, double, double, double, double, double,
                double, double, double);
extern double DFn3F1(double, double, double, double, 
                double, double, double, double, double, double, double,
                double, double, double, double, double, double, double,
                double, double, double, double, double, double, double,
                double, double, double);
extern double DFnF12(double, double, double, double,
                double, double, double, double, double, double, double,
                double, double, double, double, double, double, double);
extern double DFiF12(double, double, double, double, 
                double, double, double, double, double, double, double,
                double, double, double, double, double, double, double);
extern double DFn2F12(DpassStr*);
extern double DFi2F12(DpassStr*);

#else /* stdc */

extern int DISTOan();
extern int DISTOaskQuest();
extern int DISTOparse();
extern int DISTOsetParm();
extern double D1i2F1();
extern double D1i3F1();
extern double D1iF12();
extern double D1i2F12();
extern double D1n2F1();
extern double D1n3F1();
extern double D1nF12();
extern double D1n2F12();
extern double DFn2F1();
extern double DFi2F1();
extern double DFn3F1();
extern double DFi3F1();
extern double DFnF12();
extern double DFiF12();
extern double DFn2F12();
extern double DFi2F12();
#endif /* stdc */

#endif
