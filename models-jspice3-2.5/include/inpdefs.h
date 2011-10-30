/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1987 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

#ifndef INP
#define INP

    /* structure declarations used by either/both input package */

#include "ifsim.h"
#include "gendefs.h"
#include "inpptree.h"

extern IFsimulator *ft_sim;

struct INPtab {
    char *t_ent;
    struct INPtab *t_next;
};

struct INPnTab {
    char *t_ent;
    GENERIC* t_node;
    struct INPnTab *t_next;
};

typedef struct sINPtables{
    struct INPtab **INPsymtab;
    struct INPnTab **INPtermsymtab;
    int INPsize;
    int INPtermsize;
    GENERIC *defAmod;
    GENERIC *defBmod;
    GENERIC *defCmod;
    GENERIC *defDmod;
    GENERIC *defEmod;
    GENERIC *defFmod;
    GENERIC *defGmod;
    GENERIC *defHmod;
    GENERIC *defImod;
    GENERIC *defJmod;
    GENERIC *defKmod;
    GENERIC *defLmod;
    GENERIC *defMmod;
    GENERIC *defNmod;
    GENERIC *defOmod;
    GENERIC *defPmod;
    GENERIC *defQmod;
    GENERIC *defRmod;
    GENERIC *defSmod;
    GENERIC *defTmod;
    GENERIC *defUmod;
    GENERIC *defVmod;
    GENERIC *defWmod;
    GENERIC *defYmod;
    GENERIC *defZmod;
} INPtables;

typedef struct card {
    int linenum;
    char *line;
    char *error;
    struct card *nextcard;
    struct card *actualLine;
} card;

/* structure used to save models in after they are read during pass 1 */
typedef struct sINPmodel{
    IFuid INPmodName;   /* uid of model */
    int INPmodType;     /* type index of device type */
    struct sINPmodel *INPnextModel;  /* link to next model */
    int INPmodUsed;     /* flag to indicate it has already been used */
    card *INPmodLine;   /* pointer to line describing model */
    GENERIC *INPmodfast;   /* high speed pointer to model for access */
} INPmodel;

/* listing types - used for debug listings */
#define LOGICAL 1
#define PHYSICAL 2


/* from analysis routines */
#ifdef __STDC__
extern int ParseAC(GENERIC*,char**,card*,int,GENERIC*,INPtables*);
extern int ParseDC(GENERIC*,char**,card*,int,GENERIC*,INPtables*,int);
extern int ParseTRAN(GENERIC*,char**,card*,int,GENERIC*,INPtables*);
#else
extern int ParseAC();
extern int ParseDC();
extern int ParseTRAN();
#endif


/* inptabpa.c */
#ifdef __STDC__
extern char     *INPtablParse(char**,GENERIC*);
extern char     *INPtablCheck(char*,GENERIC*);
extern void     INPtablFix(GENERIC*);
extern int      INPtablFind(char*,GENERIC*,GENERIC**);
extern double   INPtablEval(GENERIC*,double);
extern void     INPtablFree(GENERIC*);
#else
extern char     *INPtablParse();
extern char     *INPtablCheck();
extern void     INPtablFix();
extern int      INPtablFind();
extern double   INPtablEval();
extern void     INPtablFree();
#endif


#ifdef __STDC__
extern int      IFnewUid(GENERIC*,IFuid*,IFuid,char*,int,GENERIC**);
extern int      INP2dot(GENERIC*,INPtables*,card*,GENERIC*,GENERIC*);
extern int      INPaName(char*,IFvalue*,GENERIC*,int*,char*,GENERIC**,
                    IFsimulator*,int*,IFvalue*);
extern int      INPapName(GENERIC*,int,GENERIC*,char*,IFvalue*);
extern char     *INPdevErr(char *);
extern char     *INPdevParse(char**,GENERIC*,int,GENERIC*,double*,int*,
                    INPtables*);
extern char     *INPdomodel(GENERIC *,card*, INPtables*);
extern char     *INPerrCat(char *, char *);
extern char     *INPerror(int);
extern double   INPevaluate(char**,int*,int);
extern char     *INPfindLev(char*,int*);
extern char     *INPgetMod(GENERIC*,char*,INPmodel**,INPtables*);
extern int      INPgetTok(char**,char**,int);
extern void     INPgetTree(char**,INPparseTree**,GENERIC*,INPtables*);
extern IFvalue  *INPgetValue(GENERIC*,char**,int,INPtables*);
extern int      INPgndInsert(GENERIC*,char**,INPtables*,GENERIC**);
extern int      INPinsert(char**,INPtables*);
extern void     INPkillMods(void);
extern int      INPlookMod(char*);
extern int      INPmakeMod(char*,int,card*);
extern int      INPmkTerm(GENERIC*,char**,INPtables*,GENERIC**);
extern int      INPnumRefs(int,int*,int*,int*,int*);
extern int      INPpName(char*,IFvalue*,GENERIC*,int,GENERIC*);
extern void     INPpas1(GENERIC*,card*,INPtables*);
extern void     INPpas2(GENERIC*,card*,INPtables*,GENERIC *);
extern void     INPptPrint(char*,IFparseTree*);
extern void     INPtabEnd(INPtables *);
extern INPtables *INPtabInit(int);
extern int      INPtermInsert(GENERIC*,char**,INPtables*,GENERIC**);

#else /* stdc */

extern int      IFnewUid();
extern int      INP2dot();
extern int      INPaName();
extern int      INPapName();
extern char     *INPdevErr();
extern char     *INPdevParse();
extern char     *INPdomodel();
extern char     *INPerrCat();
extern char     *INPerror();
extern double   INPevaluate();
extern char     *INPfindLev();
extern char     *INPgetMod();
extern int      INPgetTok();
extern void     INPgetTree();
extern IFvalue  *INPgetValue();
extern int      INPgndInsert();
extern int      INPinsert();
extern void     INPkillMods();
extern int      INPlookMod();
extern int      INPmakeMod();
extern int      INPmkTerm();
extern int      INPnumRefs();
extern int      INPpName();
extern void     INPpas1() ;
extern void     INPpas2() ;
extern void     INPptPrint();
extern void     INPtabEnd();
extern INPtables *INPtabInit();
extern int      INPtermInsert();

#endif /* stdc */

#endif /*INP*/
