/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

#ifndef IND
#define IND


/* turn on mutual inductor code */
#define MUTUAL

#include "devdefs.h"


/* information needed for each instance */

/* structures used to describe inductors */
typedef struct sINDinstance {

    /* backpointer to model */
    struct sINDmodel *INDmodPtr;

    /* pointer to next instance of current model*/
    struct sINDinstance *INDnextInstance;

    /* pointer to character string naming this instance */
    IFuid INDname;

    /* pointer to beginning of state vector for inductor */
    int INDstate;

    /* number of positive node of inductor */
    int INDposNode;

    /* number of negative node of inductor */
    int INDnegNode;

    /* number of the branch equation added for current */
    int INDbrEq;

    /* flux in the inductor */
#define INDflux INDstate

    /* voltage - save an entry in table */
#define INDvolt INDstate+1

    /* inductance */
    double INDinduct;

    /* initial inductor voltage if specified */
    double INDinitCond;

    /* storage for veq */
    double INDveq;

    /* storage for req */
    double INDreq;

    /* previous total flux */
    double INDprevFlux;

    /* pointer to sparse matrix diagonal at (positive,branch eq) */
    double *INDposIbrptr;

    /* pointer to sparse matrix diagonal at (negative,branch eq) */
    double *INDnegIbrptr;

    /* pointer to sparse matrix offdiagonal at (branch eq,negative) */
    double *INDibrNegptr;

    /* pointer to sparse matrix offdiagonal at (branch eq,positive) */
    double *INDibrPosptr;

    /* pointer to sparse matrix offdiagonal at (branch eq,branch eq) */
    double *INDibrIbrptr;

    /* flags to indicate: */
    unsigned INDindGiven : 1;   /* inductance was specified */
    unsigned INDicGiven  : 1;   /* init. cond. was specified */

} INDinstance ;


/* per model data */

/* model structure for an inductor */
typedef struct sINDmodel {

    /* type index of this device type */
    int INDmodType;

    /* pointer to next possible model in linked list */
    struct sINDmodel *INDnextModel;

    /* pointer to list of instances that have this model */
    INDinstance * INDinstances;

    /* pointer to character string naming this model */
    IFuid INDmodName;

#ifdef MUTUAL
    /* pointer to mutual inductor models */
    GENmodel *INDmutModels;
#endif

} INDmodel;

/* device parameters */
#define IND_IND      1
#define IND_IC       2
#define IND_FLUX     3
#define IND_VOLT     4
#define IND_CURRENT  5
#define IND_POWER    6

#ifdef __STDC__

extern int  INDacLoad(GENmodel*,CKTcircuit*);
extern int  INDask(CKTcircuit*,GENinstance*,int,IFvalue*,IFvalue*);
extern int  INDfindBr(CKTcircuit*,GENmodel*,IFuid);
extern int  INDgetic(GENmodel*,CKTcircuit*);
extern int  INDload(GENmodel*,CKTcircuit*);
extern int  INDparam(CKTcircuit*,int,IFvalue*,GENinstance*,IFvalue*);
extern void INDparse(int,GENERIC*,GENERIC*,GENERIC*);
extern int  INDpzLoad(GENmodel*,CKTcircuit*,SPcomplex*);
extern int  INDsetup(SMPmatrix*,GENmodel*,CKTcircuit*,int*);
extern int  INDtrunc(GENmodel*,CKTcircuit*,double*);

#else /* stdc */

extern int  INDacLoad();
extern int  INDask();
extern int  INDfindBr();
extern int  INDgetic();
extern int  INDload();
extern int  INDparam();
extern void INDparse();
extern int  INDpzLoad();
extern int  INDsetup();
extern int  INDtrunc();

#endif /* stdc */


#ifdef MUTUAL

/* information needed for each instance */

/* structures used to describe mutual inductors */
typedef struct sMUTinstance {

    /* backpointer to model */
    struct sMUTmodel *MUTmodPtr;

    /* pointer to next instance of current model*/
    struct sMUTinstance *MUTnextInstance;

    /* pointer to character string naming this instance */
    IFuid MUTname;

    /* mutual inductance input by user */
    double MUTcoupling;

    /* mutual inductance scaled for internal use */
    double MUTfactor;

    /* name of coupled inductor 1 */
    IFuid MUTindName1;

    /* name of coupled inductor 2 */
    IFuid MUTindName2;

    /* pointer to coupled inductor 1 */
    INDinstance *MUTind1;

    /* pointer to coupled inductor 2 */
    INDinstance *MUTind2;

    double *MUTbr1br2;  /* pointers to off-diagonal intersections of */
    double *MUTbr2br1;  /* current branch equations in matrix */

    /* flag to indicate inductance was specified */
    unsigned MUTindGiven : 1;

} MUTinstance ;


/* per model data */

/* model structure for a mutual inductor */
typedef struct sMUTmodel {

    /* type index of this device type */
    int MUTmodType;

    /* pointer to next possible model in linked list */
    struct sMUTmodel *MUTnextModel;

    /* pointer to list of instances that have this model */
    MUTinstance * MUTinstances;

    /* pointer to character string naming this model */
    IFuid MUTmodName;

} MUTmodel;

/* device parameters */
#define MUT_COEFF    401
#define MUT_IND1     402
#define MUT_IND2     403

#ifdef __STDC__

extern int  MUTask(CKTcircuit*,GENinstance*,int,IFvalue*,IFvalue*);
extern int  MUTparam(CKTcircuit*,int,IFvalue*,GENinstance*,IFvalue*);
extern void MUTparse(int,GENERIC*,GENERIC*,GENERIC*);

#else /* stdc */

extern int  MUTask();
extern int  MUTparam();
extern void MUTparse();

#endif /* stdc */

#endif /*MUTUAL*/
#endif /*IND*/
