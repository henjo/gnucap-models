/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1992 Stephen R. Whiteley
****************************************************************************/

/*
 * Input/output passed through SCED.
 */


enum Active {SCED_inactive=0, SCED_active, SCED_inhelp};

#ifdef __STDC__
extern int  SCEDline(char*);
extern void SCEDfflush(void);
extern char *SCEDfgets(char*,int,FILE*,char*);
extern enum Active SCEDactive(void);
#else
extern int  SCEDline();
extern void SCEDfflush();
extern char *SCEDfgets();
extern enum Active SCEDactive();
#endif


#ifndef SHORT_SCEDIO

/* structure for spice communication - entries are
 * NULL in nutmeg
 */

struct sSCEDitf {

#ifdef __STDC__
    int  (*sc_inprogress)(void);
    void (*sc_dosim)(char*,wordlist*);
    void (*sc_source)(struct line*,bool,char*);
    void (*sc_cktclear)(void);
    void (*sc_resume)(wordlist*);
    int  (*sc_isiplot)(void);
    void (*sc_setiplot)(wordlist*);
    void (*sc_clriplot)(wordlist*);
#else
    int  (*sc_inprogress)();
    void (*sc_dosim)();
    void (*sc_source)();
    void (*sc_cktclear)();
    void (*sc_resume)();
    int  (*sc_isiplot)();
    void (*sc_setiplot)();
    void (*sc_clriplot)();
#endif

};
extern struct sSCEDitf *ft_sced;


#endif
