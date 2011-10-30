/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

#ifndef JOBdef
#define JOBdef

typedef struct {
    IFanalysis public;
    int size;
    int domain;
#ifdef __STDC__
    int (*(parse))(GENERIC*,GENERIC*,int,GENERIC*,char**,GENERIC*,GENERIC*);
    int (*(setParm))(GENERIC*,GENERIC*,int,IFvalue*);
    int (*(askQuest))(GENERIC*,GENERIC*,int,IFvalue*);
    int (*an_func)(GENERIC*,int);
#else
    int (*(parse))( );
    int (*(setParm))( );
    int (*(askQuest))( );
    int (*an_func)( );
#endif

} SPICEanalysis;

extern SPICEanalysis *analInfo[];


#define NODOMAIN        0
#define TIMEDOMAIN      1
#define FREQUENCYDOMAIN 2
#define SWEEPDOMAIN     3

#endif /*JOBdef*/
