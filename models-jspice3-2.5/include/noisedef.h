/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1987 Gary W. Ng
         1992 Stephen R. Whiteley
****************************************************************************/

#ifndef NOISEDEFS
#define NOISEDEFS

#include "analysis.h"


/* structure used to carry information between subparts
 * of the noise analysis code
 */

typedef struct {
    double freq;
    double lstFreq;
    double delFreq;
    double outNoiz;   /* integrated output noise as of last frequency point */
    double inNoise;   /* integrated input noise as of last frequency point */
    double GainSqInv;
    double lnGainInv;
    double lnFreq;
    double lnLastFreq;
    double delLnFreq;
    int outNumber;    /* keeps track of the current output variable */
    int numPlots;     /* keeps track of the number of active plots so we can
                         close them in loop. */
    unsigned int prtSummary;
    double *outpVector;  /* pointer to our array of noise outputs */
    GENERIC *NplotPtr;   /* the plot pointer */
    IFuid *namelist;     /* list of plot names */
} Ndata;


    /* structure used to describe an noise analysis */

typedef struct {
    int JOBtype;
    JOB *JOBnextJob;   /* pointer to next thing to do */
    char *JOBname;     /* name of this job */
    char *Noutput;     /* name of the noise output summation node */
    char *NoutputRef;  /* name of the noise output reference node */
    char *Ninput;      /* name of AC source used to input ref. the noise */
    double NsavOnoise; /* integrated output noise when we left last time */
    double NsavInoise; /* integrated input noise when we left last time */
    int NStpsSm; /* number of steps before we output a noise summary report */
    Ndata *NdataPtr;   /* pointer to substructure for subroutines */
    int NposOutNode;   /* node numbers for output */
    int NnegOutNode;
    int Nstep;          /* step count, for summary print */
    struct sACprms AC;  /* AC parameter storage */
    struct sDCTprms DC; /* DC parameter storage */
} NOISEAN;


/* codes for saving and retrieving integrated noise data */

#define LNLSTDENS 0 /* array loc that log of the last noise density stored */
#define OUTNOIZ   1 /* array loc that integrated output noise is stored */
#define INNOIZ    2 /* array loc that integrated input noise is stored */

#ifndef NSTATVARS
#define NSTATVARS 3 /* number of "state" variables that must be stored for
                       each noise generator.
                       in this case it is three: LNLSTDENS, OUTNOIZ
                       and INNOIZ */
#endif

/* noise analysis parameters */

#define N_OUTPUT     1
#define N_OUTREF     2
#define N_INPUT      3
#define N_PTSPERSUM  4


/* noise routine operations/modes */

#define N_DENS       1
#define INT_NOIZ     2
#define N_OPEN       1
#define N_CALC       2
#define N_CLOSE      3
#define SHOTNOISE    1
#define THERMNOISE   2
#define N_GAIN       3


/* tolerances and limits to make numerical analysis more robust */

#define N_MINLOG   1E-38
/*    the smallest number we can take the log of */


#define N_MINGAIN  1E-20
/*    the smallest input-output gain we can tolerate
 *    (to calculate input-referred noise we divide
 *    the output noise by the gain)
 */

#define N_INTFTHRESH 1E-10
/*    the largest slope (of a log-log noise spectral
 *    density vs. freq plot) at which the noise
 *    spectum is still considered flat. (no need for
 *    log curve fitting)
 */

#define N_INTUSELOG 1E-10
/*    decides which expression to use for the integral of
 *    x**k.  If k is -1, then we must use a 'ln' form.
 *    Otherwise, we use a 'power' form.  This
 *    parameter is the region around (k=) -1 for which
 *    use the 'ln' form.
 */


/* misc constants */

/* maximum length for noise output variables we will generate */
#define N_MXVLNTH  64


#ifdef __STDC__
extern int NOISEan(GENERIC *, int);
extern int NOISEaskQuest(GENERIC*,GENERIC*,int,IFvalue*);
extern int NOISEparse(GENERIC*,GENERIC*,int,GENERIC*,char**,GENERIC*,GENERIC*);
extern int NOISEsetParm(GENERIC*,GENERIC*,int,IFvalue*);
extern void   NevalSrc(double*,double*,CKTcircuit*,int,int,int,double);
extern double Nintegrate(double,double,double,GENERIC*);
#else
extern int NOISEan();
extern int NOISEaskQuest();
extern int NOISEparse();
extern int NOISEsetParm();
extern void   NevalSrc();
extern double Nintegrate();
#endif


#endif /* NOISEDEFS */
