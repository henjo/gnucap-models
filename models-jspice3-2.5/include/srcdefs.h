/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1987 Kanwar Jit Singh
         1993 Stephen R. Whiteley
****************************************************************************/

#ifndef SRC
#define SRC

#include "devdefs.h"

/*
 * structures to describe Arbitrary sources
 */

/* information to describe a single instance */

typedef struct sSRCinstance {

    /* backpointer to model */
    struct sSRCmodel *SRCmodPtr;

    /* pointer to next instance of current model */
    struct sSRCinstance *SRCnextInstance;

    /* pointer to character string naming this instance */
    IFuid SRCname;

    /* state info */
    int SRCstate;

    /* number of positive node of source */
    int SRCposNode;

    /* number of negative node of source */
    int SRCnegNode;

    /* number of positive node of linear controlling voltage source */
    int SRCcontPosNode;

    /* number of negative node of linear controlling voltage source */
    int SRCcontNegNode;

    /* number of branch equation added for voltage source */
    int SRCbranch;

    /* number of branch eq of controlling current source */
    int SRCcontBranch;

    /* pointer to pointers of the elements in the sparse matrix */
    double **SRCposptr;

    /* functions to call to obtain source output */
#ifdef __STDC__
    void (*SRCtranFunc)(CKTcircuit*,struct sSRCinstance*);
    void (*SRCdcFunc)(CKTcircuit*,struct sSRCinstance*);
#else
    void (*SRCtranFunc)();
    void (*SRCdcFunc)();
#endif

    /* whether source is voltage or current */
    int SRCtype;

    /* whether the source is a linear dependent source and what kind */
    int SRCdep;

    /* linear gain coefficient */
    double SRCcoeff;

    /* pointer to name of controlling instance */
    IFuid SRCcontName;

    /* DC and TRANSIENT value of source */
    double SRCdcValue;

    /* AC magnitude and phase */
    double SRCacVec[2];
#define SRCacMag SRCacVec[0]
#define SRCacPhase SRCacVec[1]

    /* AC real component */
    double SRCacReal;

    /* AC imaginary component */
    double SRCacImag;

    /* distortion f1 magnitude */
    double SRCdF1mag;

    /* distortion f2 magnitude */
    double SRCdF2mag;

    /* distortion f1 phase */
    double SRCdF1phase;

    /* distortion f2 phase */
    double SRCdF2phase;

    /* the parse tree */
    IFparseTree *SRCtree;

    /* store rhs and derivatives for ac anal */
    double *SRCacValues;

    /* store input values to function */
    double *SRCvalues;

    /* present source value */
    double SRCvalue;

    /* store partial derivatives */
    double *SRCderivs;

    /* store equation numbers of input values */
    int *SRCeqns;

    /* previous value for the convergence test */
    double SRCprev;

    /* pointer to sparse matrix element at 
      (positive node, control positive node) */
    double *SRCposContPosptr;

    /* pointer to sparse matrix element at 
      (positive node, control negative node) */
    double *SRCposContNegptr;

    /* pointer to sparse matrix element at 
      (negative node, control positive node) */
    double *SRCnegContPosptr;

    /* pointer to sparse matrix element at 
      (negative node, control negative node) */
    double *SRCnegContNegptr;

    /* pointer to sparse matrix element at 
      (positive node, control branch eq) */
    double *SRCposContBrptr;

    /* pointer to sparse matrix element at 
      (negative node, control branch eq) */
    double *SRCnegContBrptr;

    /* pointer to sparse matrix element at 
      (branch equation, control positive node) */
    double *SRCibrContPosptr;

    /* pointer to sparse matrix element at 
      (branch equation, control negative node) */
    double *SRCibrContNegptr;

    /* pointer to sparse matrix element at 
      (branch equation, control branch eq) */
    double *SRCibrContBrptr;

    /* pointer to sparse matrix element at
       (positive node, branch equation) */
    double *SRCposIbrptr;

    /* pointer to sparse matrix element at
       (negative node, branch equation) */
    double *SRCnegIbrptr;

    /* pointer to sparse matrix element at
       (branch equation, positive node) */
    double *SRCibrPosptr;

    /* pointer to sparse matrix element at
       (branch equation, negative node) */
    double *SRCibrNegptr;

    /* pointer to sparse matrix element at
       (branch equation, branch equation) */
    double *SRCibrIbrptr;

    /* flags to indicate: */
    unsigned SRCdcGiven      :1; /* dc value given */
    unsigned SRCacGiven      :1; /* ac keyword given */
    unsigned SRCacMGiven     :1; /* ac magnitude given */
    unsigned SRCacPGiven     :1; /* ac phase given */
    unsigned SRCdGiven       :1; /* source is a disto input */
    unsigned SRCdF1given     :1; /* source is an f1 dist input */
    unsigned SRCdF2given     :1; /* source is an f2 dist input */
    unsigned SRCccCoeffGiven :1; /* current controlled gain coeff given */
    unsigned SRCvcCoeffGiven :1; /* voltage controlled gain coeff given */

} SRCinstance;


/* per model data */

/* model structure for a source */
typedef struct sSRCmodel {

    /* type index of this device */
    int SRCmodType;

    /* pointer to next possible model in linked list */
    struct sSRCmodel *SRCnextModel;

    /* pointer to list of instances that have this model */
    SRCinstance *SRCinstances;

    /* pointer to character string naming this model */
    IFuid SRCmodName;

} SRCmodel;

#define SRC_CC 1
#define SRC_VC 2

/* device parameters */
#define SRC_I            1
#define SRC_V            2
#define SRC_DEP          3
#define SRC_DC           4
#define SRC_AC           5
#define SRC_AC_MAG       6
#define SRC_AC_PHASE     7
#define SRC_FUNC         8
#define SRC_D_F1         9
#define SRC_D_F2         10
#define SRC_GAIN         11
#define SRC_CONTROL      12
#define SRC_POS_NODE     13
#define SRC_NEG_NODE     14
#define SRC_AC_REAL      15
#define SRC_AC_IMAG      16
#define SRC_CURRENT      17
#define SRC_POWER        18
#define SRC_CONT_P_NODE  19
#define SRC_CONT_N_NODE  20

#ifdef __STDC__

extern int  SRCacLoad(GENmodel*,CKTcircuit*);
extern int  SRCask(CKTcircuit*,GENinstance *,int,IFvalue *,IFvalue*);
extern int  SRCconvTest(GENmodel *,CKTcircuit*);
extern int  SRCdelete(GENmodel *,IFuid,GENinstance **);
extern void SRCdestroy(GENmodel**);
extern int  SRCfindBr(CKTcircuit *,GENmodel *,IFuid);
extern int  SRCload(GENmodel *,CKTcircuit*);
extern int  SRCmDelete(GENmodel**,IFuid,GENmodel*);
extern int  SRCparam(CKTcircuit*,int,IFvalue*,GENinstance*,IFvalue*);
extern void SRCparse(int,GENERIC*,GENERIC*,GENERIC*);
extern int  SRCpzLoad(GENmodel*,CKTcircuit*,SPcomplex*);
extern int  SRCsetup(SMPmatrix*,GENmodel*,CKTcircuit*,int*);

#else /* stdc */

extern int  SRCacLoad();
extern int  SRCask();
extern int  SRCconvTest();
extern int  SRCdelete();
extern void SRCdestroy();
extern int  SRCfindBr();
extern int  SRCload();
extern int  SRCmDelete();
extern int  SRCparam();
extern void SRCparse();
extern int  SRCpzLoad();
extern int  SRCsetup();

#endif /* stdc */


#endif /* SRC */
