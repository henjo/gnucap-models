/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1993 Stephen R. Whiteley
****************************************************************************/

#ifndef ANALH
#define ANALH

#define IFP IF_SET|IF_ASK


/* general DC analysis defines */

#define DCTNESTLEVEL 2 /* depth of nesting of curves - 2 for spice2 */

#define DC_NAME1  101
#define DC_START1 102
#define DC_STOP1  103
#define DC_STEP1  104
#define DC_NAME2  105
#define DC_START2 106
#define DC_STOP2  107
#define DC_STEP2  108

struct sDCTprms {
    double vstart[DCTNESTLEVEL];     /* starting voltage/current */
    double vstop[DCTNESTLEVEL];      /* ending voltage/current */
    double vstep[DCTNESTLEVEL];      /* voltage/current step */
    double vsave[DCTNESTLEVEL];      /* voltage of this source BEFORE
                                      * analysis - to restore when done */
    double vstate[DCTNESTLEVEL];     /* internal values saved during pause */
    GENinstance *elt[DCTNESTLEVEL];  /* pointer to source */
    IFuid eltName[DCTNESTLEVEL];     /* source being varied */
    int nestLevel;                   /* number of levels of nesting */
    int nestSave;                    /* iteration state during pause */
    int dims[DCTNESTLEVEL+1];        /* dimensions of output vector */
    int skip;                        /* restart subanalysis */
};


/* general AC analysis defines */

/* available step types: */

#define DCSTEP 0
#define DECADE 1
#define OCTAVE 2
#define LINEAR 3


#define AC_DEC   201
#define AC_OCT   202
#define AC_LIN   203
#define AC_START 204
#define AC_STOP  205
#define AC_STEPS 206

struct sACprms {
    double fstart;
    double fstop;
    double fsave;
    int numSteps;
    int stepType;
};

#ifdef __STDC__
extern int ACquery(struct sACprms*,int,IFvalue*);
extern int ACsetp(struct sACprms*,int,IFvalue*);
extern int DCTquery(struct sDCTprms*,int,IFvalue*);
extern int DCTsetp(struct sDCTprms*,int,IFvalue*);
#else
extern int ACquery();
extern int ACsetp();
extern int DCTquery();
extern int DCTsetp();
#endif

#ifdef ANAL_EXT
/* sOUTdata defined */

#ifdef __STDC__
extern int ACloop(int(*)(),CKTcircuit*,int,struct sACprms*,
            struct sOUTdata*);
extern int DCTloop(int(*)(),CKTcircuit*,int,struct sDCTprms*,
            struct sOUTdata*);
#else
extern int ACloop();
extern int DCTloop();
#endif

#endif


#endif
