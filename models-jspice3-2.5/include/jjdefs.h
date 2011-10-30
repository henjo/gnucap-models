/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Author: 1992 Stephen R. Whiteley
****************************************************************************/

#ifndef JJ
#define JJ

#include "devdefs.h"

/* data structures used to describe Jopsephson junctions */

/* information needed per instance */

typedef struct sJJinstance {
    struct sJJmodel *JJmodPtr;     /* backpointer to model */
    struct sJJinstance *JJnextInstance;   /* pointer to next instance of 
                                           * current model */
    IFuid JJname;                  /* pointer to character string naming
                                    * this instance */
    int JJstate;                   /* pointer to start of state vector for
                                    * junction */
    int JJposNode;                 /* number of positive node of junction */
    int JJnegNode;                 /* number of negative node of junction */
    int JJphsNode;                 /* number of phase node of junction */
    int JJbranch;                  /* number of control current branch */
    IFuid JJcontrol;               /* name of controlling device */
    double JJarea;                 /* area factor for the junction */

    double JJinitCnd[2];           /* initial condition vector */
#define JJinitVoltage JJinitCnd[0]
#define JJinitPhase JJinitCnd[1]

    double JJinitControl;          /* initial control current */

    double JJdelVdelT;             /* dvdt storage */

    /* These parameters scale with area */
    double JJcriti;                /* junction critical current */
    double JJcap;                  /* junction capacitance */
    double JJg0;                   /* junction subgap conductance */
    double JJgn;                   /* junction normal conductance */
    double JJgs;                   /* junction step conductance */
    double JJg1;                   /* NbN model parameter */
    double JJg2;                   /* NbN model parameter */

    double *JJposNegPtr;           /* pointer to sparse matrix at 
                                    * (positive, negative) */
    double *JJnegPosPtr;           /* pointer to sparse matrix at 
                                    * (negative, positive) */
    double *JJposPosPtr;           /* pointer to sparse matrix at 
                                    * (positive, positive) */
    double *JJnegNegPtr;           /* pointer to sparse matrix at 
                                    * (negative, negative) */
    double *JJphsPhsPtr;           /* pointer to sparse matrix at 
                                    * (phase, phase) */
    double *JJposIbrPtr;           /* pointer to sparse matrix at 
                                    * (positive, branch equation) */
    double *JJnegIbrPtr;           /* pointer to sparse matrix at 
                                    * (negative, branch equation) */
    int JJtopcod;                  /* connection topology code */
    unsigned JJareaGiven : 1;      /* flag to indicate area was specified */
    unsigned JJinitVoltGiven : 1;  /* flag to indicate ic was specified   */
    unsigned JJinitPhaseGiven : 1; /* flag to indicate ic was specified   */
    unsigned JJcontrolGiven : 1;   /* control ind or vsource specified    */

} JJinstance;


#define JJvoltage JJstate
#define JJdvdt    JJstate + 1
#define JJphase   JJstate + 2
#define JJconI    JJstate + 3
#define JJphsInt  JJstate + 4
#define JJxtra1   JJstate + 5
#define JJxtra2   JJstate + 6

/* per model data */

typedef struct sJJmodel {          /* model structure for a junction */
    int JJmodType;                 /* type index of this device */      
    struct sJJmodel *JJnextModel;  /* pointer to next possible model in 
                                    * linked list */
    JJinstance * JJinstances;      /* pointer to list of instances 
                                    * that have this model */
    IFuid JJmodName;               /* pointer to character string naming
                                    * this model */
    int JJrtype;
    int JJictype;
    double JJvg;
    double JJdelv;
    double JJcriti;
    double JJcap;
    double JJr0;
    double JJrn;
    double JJccsens;
    double JJvless;
    double JJvmore;
    double JJvdpbak;
    unsigned JJrtypeGiven : 1;
    unsigned JJictypeGiven : 1;
    unsigned JJvgGiven : 1;
    unsigned JJdelvGiven : 1;
    unsigned JJccsensGiven : 1;
    unsigned JJr0Given : 1;
    unsigned JJrnGiven : 1;
    unsigned JJcritiGiven : 1;
    unsigned JJcapGiven : 1;

} JJmodel;


/* from jspice2
#define JJcphi 3.291090738e-16
*/
/* from jsim */
#define JJcphi 3.291086546e-16

extern double JJdphi;        /* max phase change between time points */

/* device parameters */
#define JJ_AREA 1 
#define JJ_IC   2
#define JJ_ICP  3
#define JJ_ICV  4
#define JJ_CON  5

/* device questions */

#define JJ_QUEST_CRT 6
#define JJ_QUEST_CAP 7
#define JJ_QUEST_G0  8
#define JJ_QUEST_GN  9
#define JJ_QUEST_GS  10
#define JJ_QUEST_G1  11
#define JJ_QUEST_G2  12

/* model parameters */

#define JJ_MOD_RT  101
#define JJ_MOD_IC  102
#define JJ_MOD_VG  103
#define JJ_MOD_DV  104
#define JJ_MOD_CRT 105
#define JJ_MOD_CAP 106
#define JJ_MOD_R0  107
#define JJ_MOD_RN  108
#define JJ_MOD_CCS 109
#define JJ_MOD_JJ  113

/* model questions */

#define JJ_MQUEST_VL  110
#define JJ_MQUEST_VM  111
#define JJ_MQUEST_VDP 112

#ifdef __STDC__

extern int  JJaccept(CKTcircuit*,GENmodel*);
extern int  JJask(CKTcircuit*,GENinstance*,int,IFvalue*,IFvalue*);
extern int  JJgetic(GENmodel*,CKTcircuit*);
extern int  JJload(GENmodel*,CKTcircuit*);
extern int  JJmAsk(CKTcircuit*,GENmodel*,int,IFvalue*);
extern int  JJmParam(int,IFvalue*,GENmodel*);
extern int  JJparam(CKTcircuit*,int,IFvalue*,GENinstance*,IFvalue*);
extern void JJparse(int,GENERIC*,GENERIC*,GENERIC*);
extern int  JJsetup(SMPmatrix*,GENmodel*,CKTcircuit*,int*);
extern int  JJtrunc(GENmodel*,CKTcircuit*,double*);

#else /* stdc */

extern int  JJaccept();
extern int  JJask();
extern int  JJgetic();
extern int  JJload();
extern int  JJmAsk();
extern int  JJmParam();
extern int  JJparam();
extern void JJparse();
extern int  JJsetup();
extern int  JJtrunc();

#endif /* stdc */

#endif /*JJ*/
