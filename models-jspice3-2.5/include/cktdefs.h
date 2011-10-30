/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

#ifndef CKT
#define CKT

#define MAXNUMDEVS 32
extern int DEVmaxnum;
#define MAXNUMDEVNODES 4

#include "ifsim.h"
#include "gendefs.h"
#include "smpdefs.h"
#include "tabdefs.h"

/* number of noise state variables stored for each generator */
#define NSTATVARS 3

typedef struct sJOB{
    int JOBtype;                /* type of job */
    struct sJOB *JOBnextJob;    /* next job in list */
    IFuid JOBname;              /* name of this job */

} JOB;

typedef struct sCKTnode {
    IFuid name;
    int type;

#define SP_VOLTAGE 3
#define SP_CURRENT 4
#define NODE_VOLTAGE SP_VOLTAGE
#define NODE_CURRENT SP_CURRENT

    int number;
    double ic;
    double nodeset;
    double *ptr;
    struct sCKTnode *next;
    unsigned int icGiven:1;
    unsigned int nsGiven:1;
} CKTnode;

/* defines for node parameters */
#define PARM_NS 1
#define PARM_IC 2
#define PARM_NODETYPE 3


/* SRW - Keep track of models actually used in linked list. */
struct sCKTmodHead {
    int type;                /* device type number */
    GENmodel *head;          /* model list head */
    struct sCKTmodHead *next;
};


typedef struct {
    struct sCKTmodHead *CKTheadList;
    GENmodel *CKThead[MAXNUMDEVS];
    /* SRW - for .table */
    struct sCKTtable *CKTtableHead;
    GENERIC *CKTstat; /* STATistics */
    double *(CKTstates[8]);
#define CKTstate0 CKTstates[0]
#define CKTstate1 CKTstates[1]
#define CKTstate2 CKTstates[2]
#define CKTstate3 CKTstates[3]
#define CKTstate4 CKTstates[4]
#define CKTstate5 CKTstates[5]
#define CKTstate6 CKTstates[6]
#define CKTstate7 CKTstates[7]
    double CKTtime;
    double CKTdelta;
    double CKTdeltaOld[7];
    double CKTtemp;
    double CKTnomTemp;
    double CKTvt;
    double CKTag[7];        /* the gear variable coefficient matrix */
#ifdef PREDICTOR
    double CKTagp[7];       /* the gear predictor variable coefficient matrix */
#endif /*PREDICTOR*/
    int CKTorder;           /* the integration method order */
    int CKTmaxOrder;        /* maximum integration method order */
    int CKTintegrateMethod; /* the integration method to be used */

/* known integration methods */
#define TRAPEZOIDAL 1
#define GEAR 2

    SMPmatrix *CKTmatrix;   /* pointer to sparse matrix */
    int CKTniState;         /* internal state */
    double *CKTrhs;         /* current rhs value - being loaded */
    double *CKTrhsOld;      /* previous rhs value for convergence testing */
    double *CKTrhsSpare;    /* spare rhs value for reordering */
    double *CKTirhs;        /* current rhs value - being loaded (imag) */
    double *CKTirhsOld;     /* previous rhs value (imaginary)*/
    double *CKTirhsSpare;   /* spare rhs value (imaginary)*/

    /* SRW - my predictor */
    double CKTpred[4];      /* factors used to predict new values */
    double *CKTsols[8];     /* previous 8 rhs solutions */

#ifdef PREDICTOR
    double *CKTpred;        /* predicted solution vector */
    double *CKTsols[8];     /* previous 8 solutions */
#endif /* PREDICTOR */

    double *CKTrhsOp;       /* opearating point values */

/*
 *  symbolic constants for CKTniState
 *      Note that they are bitwise disjoint
 */

#define NISHOULDREORDER   0x1
#define NIREORDERED       0x2
#define NIUNINITIALIZED   0x4
#define NIACSHOULDREORDER 0x10
#define NIACREORDERED     0x20
#define NIACUNINITIALIZED 0x40
#define NIDIDPREORDER     0x100
#define NIPZSHOULDREORDER 0x200

    int CKTmaxEqNum;
    int CKTcurrentAnalysis; /* the analysis in progress (if any) */

/* defines for the value of  CKTcurrentAnalysis */
/* are in TSKdefs.h */

    CKTnode *CKTnodes;
    CKTnode *CKTlastNode;
#define NODENAME(ckt,nodenum) CKTnodName(ckt,nodenum)
    int CKTnumStates;
    long CKTmode;

/* defines for CKTmode */

/* old 'mode' parameters */
#define MODE     0x3
#define MODETRAN 0x1
#define MODEAC   0x2

/* old 'modedc' parameters */
#define MODEDC          0x70
#define MODEDCOP        0x10
#define MODETRANOP      0x20
#define MODEDCTRANCURVE 0x40

/* old 'initf' parameters */
#define INITF         0x7f00
#define MODEINITFLOAT 0x100
#define MODEINITJCT   0x200
#define MODEINITFIX   0x400
#define MODEINITSMSIG 0x800
#define MODEINITTRAN  0x1000
#define MODEINITPRED  0x2000
/* new parameter */
#define MODETRANUPD   0x4000

/* old 'nosolv' paramater */
#define MODEUIC 0x10000l

    int CKTjjPresent;       /* Josephson junctions are in circuit */
    int CKTbypass;          /* use bypass */
    int CKTpreload;         /* preload constants in matrix during setup */
    int CKTdcMaxIter;       /* iteration limit for dc op.  (itl1) */
    int CKTdcTrcvMaxIter;   /* iteration limit for dc tran. curv (itl2) */
    int CKTtranMaxIter;     /* iteration limit for each timepoint for tran */
                            /* (itl4) */
    int CKTbreakSize;
    int CKTbreak;
    double CKTsaveDelta;
    double CKTminBreak;
    double *CKTbreaks;
    double CKTabstol;
    double CKTpivotAbsTol;
    double CKTpivotRelTol;
    double CKTreltol;
    double CKTchgtol;
    double CKTvoltTol;
#ifdef NEWTRUNC
    double CKTlteReltol;
    double CKTlteAbstol;
#endif /* NEWTRUNC */
    double CKTgmin;
    double CKTdelmin;
    double CKTtrtol;
    double CKTfinalTime;
    double CKTstep;
    double CKTmaxStep;
    double CKTinitTime;
    double CKTinitDelta;        /* SRW - initial timestep */
    double CKTomega;
    double CKTsrcFact;
    double CKTdiagGmin;
    int CKTnumSrcSteps;
    int CKTnumGminSteps;
    int CKTnoncon;
    double CKTdefaultMosL;
    double CKTdefaultMosW;
    double CKTdefaultMosAD;
    double CKTdefaultMosAS;
    unsigned int CKThadNodeset:1;
    unsigned int CKTfixLimit:1; /* flag to indicate that the limiting of 
                                 * MOSFETs should be done as in SPICE2 */
    unsigned int CKTnoOpIter:1; /* flag to indicate not to try the operating
                                 * point brute force, but to use gmin stepping
                                 * first */
    unsigned int CKTkeepOpInfo:1; /* flag for small signal analyses */
    unsigned int CKTisSetup:1;  /* flag to indicate if CKTsetup done */
    JOB *CKTcurJob;

    double *CKTtimePoints;      /* list of all accepted timepoints in the
                                   current transient simulation */
    double *CKTdeltaList;       /* list of all timesteps in the current
                                   transient simulation */
    int CKTtimeListSize;        /* size of above lists */
    int CKTtimeIndex;           /* current position in above lists */
    int CKTsizeIncr;            /* amount to increment size of above arrays
                                   when you run out of space */
    unsigned int CKTtryToCompact:1; /* try to compact past history for LTRA
                                       lines */
    int CKTtroubleNode;         /* Non-convergent node number */
    GENinstance *CKTtroubleElt; /* Non-convergent device instance */

} CKTcircuit;


extern IFfrontEnd *SPfrontEnd;

#endif /*CKT*/
