/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1986 Wayne A. Christopher
         1992 Stephen R. Whiteley
****************************************************************************/

/*
 * Definitions for external symbols in spice specific FTE.
 */

#ifndef spFTEext_h
#define spFTEext_h

/* bin/xeditor.c */
#ifdef __STDC__
extern int xeditor(char*);
extern void com_xeditor(wordlist*);
#else
extern int xeditor();
extern void com_xeditor();
#endif

/* breakp.c */
#ifdef __STDC__
extern void com_stop(wordlist*);
extern void com_trce(wordlist*);
extern void com_iplot(wordlist*);
extern void com_save(wordlist*);
extern void com_step(wordlist*);
extern void com_sttus(wordlist*);
extern void com_delete(wordlist*);
extern bool ft_bpcheck(struct plot*,int);
extern void ft_trquery(void);
extern int  ft_getSaves(char***);
#else
extern void com_stop();
extern void com_trce();
extern void com_iplot();
extern void com_save();
extern void com_step();
extern void com_sttus();
extern void com_delete();
extern bool ft_bpcheck();
extern void ft_trquery();
extern int  ft_getSaves();
#endif

/* check.c */
#ifdef __STDC__
extern void com_check(wordlist*);
extern void com_loop(wordlist*);
extern void ft_check(wordlist*,FILE*);
#else
extern void com_check();
extern void com_loop();
extern void ft_check();
#endif

/* circuits.c */
extern struct circ *ft_curckt;
extern struct circ *ft_circuits;
extern struct subcirc *ft_subcircuits;
#ifdef __STDC__
extern void ft_setccirc(char*);
extern void ft_newcirc(struct circ*);
extern void ft_newsubcirc(struct subcirc*);
#else
extern void ft_setccirc();
extern void ft_newcirc();
extern void ft_newsubcirc();
#endif

/* spcmdtab.c or nutctab.c */
extern struct comm *cp_coms;

/* debugcoms.c */
#ifdef __STDC__
extern void com_state(wordlist*);
extern void com_dump(wordlist*);
#else
extern void com_state();
extern void com_dump();
#endif

/* device.c */
#ifdef __STDC__
extern void com_show(wordlist*);
extern void com_alter(wordlist*);
#else
extern void com_show();
extern void com_alter();
#endif

/* dotcards.c */
#ifdef __STDC__
extern void ft_dotsaves(void);
extern void ft_savedotargs(void);
extern void ft_cktcoms(bool);
#else
extern void ft_dotsaves();
extern void ft_savedotargs();
extern void ft_cktcoms();
#endif

/* inp.c */
#ifdef __STDC__
extern void com_listing(wordlist*);
extern void inp_list(FILE*,struct line*,struct line*,int);
extern void inp_spsource(FILE*,bool,char*);
extern void inp_decksource(struct line*,bool,char*);
extern void inp_dodeck(struct line*,char*,wordlist*,bool,struct line*,char*);
extern void com_edit(wordlist*);
extern void inp_srcedit(char*,bool,bool);
extern void com_sced(wordlist*);
extern void com_source(wordlist*);
extern void inp_source(char*);
#else
extern void com_listing();
extern void inp_list();
extern void inp_spsource();
extern void inp_decksource();
extern void inp_dodeck();
extern void com_edit();
extern void inp_srcedit();
extern void com_sced();
extern void com_source();
extern void inp_source();
#endif

/* linear.c */
#ifdef __STDC__
extern void com_linearize(wordlist*);
#else
extern void com_linearize();
#endif

/* outitf.c */
extern bool OUTendit;
#ifdef __STDC__
extern char *OUTcntrlInit(void);
extern int  OUTbeginPlot(GENERIC*);
extern int  OUTdata(GENERIC*,IFvalue*,IFvalue*);
extern int  OUTsetDims(GENERIC*,int*,int);
extern int  OUTendPlot(GENERIC*);
extern int  OUTstopnow(void);
extern int  OUTerror(int,char*,IFuid*);
#else
extern char *OUTcntrlInit();
extern int  OUTbeginPlot();
extern int  OUTdata();
extern int  OUTsetDims();
extern int  OUTendPlot();
extern int  OUTstopnow();
extern int  OUTerror();
#endif

/* runcoms.c */
#ifdef __STDC__
extern void com_scirc(wordlist*);
extern void com_pz(wordlist*);
extern void com_op(wordlist*);
extern void com_dc(wordlist*);
extern void com_ac(wordlist*);
extern void com_tf(wordlist*);
extern void com_tran(wordlist*);
extern void com_sens(wordlist*);
extern void com_disto(wordlist*);
extern void com_noise(wordlist*);
extern void ft_dosim(char*,wordlist*);
extern void com_resume(wordlist*);
extern void com_run(wordlist*);
extern void ft_dorun(char*);
extern void com_rset(wordlist*);
extern void com_free(wordlist*);
#else
extern void com_scirc();
extern void com_pz();
extern void com_op();
extern void com_dc();
extern void com_ac();
extern void com_tf();
extern void com_tran();
extern void com_sens();
extern void com_disto();
extern void com_noise();
extern void ft_dosim();
extern void com_resume();
extern void com_run();
extern void ft_dorun();
extern void com_rset();
extern void com_free();
#endif

/* shyu.c */
#ifdef __STDC__
extern int if_sens_run(char*,wordlist*,char*);
#else
extern int if_sens_run();
#endif

/* spiceif.c or nutmegif.c */
#ifdef __STDC__
extern char *if_inpdeck(struct line*,char**);
extern int  if_run(char*,wordlist*);
extern void if_option(char*,char*,int,char*);
extern void if_dump(char*,FILE*);
extern void if_cktfree(char*,char*);
extern void if_cktclear(void);
extern char *if_errstring(int);
extern struct variable *spif_getparam(char*,char**,char*,int,wordlist**);
extern void if_setparam(char*,char*,char*,struct variable*);
extern void if_setndnames(char*);
extern bool if_tranparams(struct circ*,double*,double*,double*);
extern struct variable *if_getstat(char*,char*,wordlist**);
extern struct variable *nutif_getparam(char*,char**,char*,int,wordlist**);
extern wordlist *GetAnalysisFromDeck(void);
extern int  InProgress(void);
extern int  IsIplot(void);
#else
extern char *if_inpdeck();
extern int  if_run();
extern void if_option();
extern void if_dump();
extern void if_cktfree();
extern void if_cktclear();
extern char *if_errstring();
extern struct variable *spif_getparam();
extern void if_setparam();
extern void if_setndnames();
extern void if_setndnames();
extern bool if_tranparams();
extern struct variable *if_getstat();
extern struct variable *nutif_getparam();
extern wordlist *GetAnalysisFromDeck();
extern int  InProgress();
extern int  IsIplot();
#endif

/* subckt.c */
#ifdef __STDC__
extern struct line *inp_subcktexpand(struct line*);
#else
extern struct line *inp_subcktexpand();
#endif

/* where.c */
#ifdef __STDC__
extern void com_where();
#else
extern void com_where();
#endif

#endif /* spFTEext_h */
