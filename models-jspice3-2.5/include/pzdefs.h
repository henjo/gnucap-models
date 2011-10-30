/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: UCB CAD Group
         1992 Stephen R. Whiteley
****************************************************************************/

#ifndef PZDEFS
#define PZDEFS

    /* structure used to describe an PZ analysis to be performed */

typedef struct strial {
    SPcomplex s, f_raw, f_def;
    struct strial *next, *prev;
    int  mag_raw, mag_def;
    int  multiplicity;
    int  flags;
    int  seq_num;
    int  count;
} PZtrial;

typedef struct {
    int JOBtype;
    JOB *JOBnextJob;
    IFuid JOBname;
    int PZin_pos;
    int PZin_neg;
    int PZout_pos;
    int PZout_neg;
    int PZinput_type;
    int PZwhich;
    int PZnumswaps;
    int PZbalance_col;
    int PZsolution_col;
    PZtrial *PZpoleList;
    PZtrial *PZzeroList;
    int PZnPoles;
    int PZnZeros;
    double *PZdrive_pptr;
    double *PZdrive_nptr;
} PZAN;

#define PZ_DO_POLES 0x1
#define PZ_DO_ZEROS 0x2
#define PZ_IN_VOL 1
#define PZ_IN_CUR 2

#define PZ_NODEI 1
#define PZ_NODEG 2
#define PZ_NODEJ 3
#define PZ_NODEK 4
#define PZ_V 5
#define PZ_I 6
#define PZ_POL 7
#define PZ_ZER 8
#define PZ_PZ 9


#ifdef __STDC__
extern int PZan(GENERIC*,int);
extern int PZaskQuest(GENERIC*,GENERIC*,int,IFvalue*);
extern int PZparse(GENERIC*,GENERIC*,int,GENERIC*,char**,GENERIC*,GENERIC*);
extern int PZsetParm(GENERIC*,GENERIC*,int,IFvalue*);
#else
extern int PZan();
extern int PZaskQuest();
extern int PZparse();
extern int PZsetParm();
#endif


#endif /* PZDEFS */
