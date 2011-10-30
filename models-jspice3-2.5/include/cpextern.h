/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1986 Wayne A. Christopher
         1992 Stephen R. Whiteley
****************************************************************************/

/*
 * Definitions for external symbols in CP.
 */

#ifndef CPEXTERN_H
#define CPEXTERN_H

/* alias.c */
extern struct alias *cp_aliases;
#ifdef __STDC__
extern void cp_doalias(wordlist**);
extern void cp_setalias(char*,wordlist*);
extern void cp_unalias(char*);
extern void cp_paliases(char*);
extern void com_alias(wordlist*);
extern void com_unalias(wordlist*);
#else
extern void cp_doalias();
extern void cp_setalias();
extern void cp_unalias();
extern void cp_paliases();
extern void com_alias();
extern void com_unalias();
#endif

/* backq.c */
extern char cp_back;
#ifdef __STDC__
extern void cp_bquote(wordlist**);
#else
extern void cp_bquote();
#endif

/* complete.c */
extern bool cp_nocc;
#ifdef __STDC__
extern void cp_ccom(wordlist*,char*,bool);
extern wordlist *cp_cctowl(char*);
extern void cp_ccon(bool);
extern bool cp_comlook(char*);
extern void cp_addcomm(char*,long,long,long,long);
extern void cp_remcomm(char*);
extern void cp_addkword(int,char*);
extern void cp_remkword(int,char*);
extern char *cp_kwswitch(int,char*);
extern void cp_ccfreetrie(char*);
extern void cp_ccrestart(bool);
#else
extern void cp_ccom();
extern wordlist *cp_cctowl();
extern void cp_ccon();
extern bool cp_comlook();
extern void cp_addcomm();
extern void cp_remcomm();
extern void cp_addkword();
extern void cp_remkword();
extern char *cp_kwswitch();
extern void cp_ccfreetrie();
extern void cp_ccrestart();
#endif

/* cshpar.c */
extern FILE *cp_in;
extern FILE *cp_out;
extern FILE *cp_err;
extern FILE *cp_curin;
extern FILE *cp_curout;
extern FILE *cp_curerr;
extern bool cp_debug;
extern char cp_amp;
extern char cp_gt;
extern char cp_lt;
extern char cp_chars[];
#ifdef __STDC__
extern void cp_init(void);
extern wordlist *cp_parse(char*);
extern void com_echo(wordlist*);
extern void cp_redirect(wordlist**);
extern void cp_ioreset(void);
extern void cp_fixdescriptors(void);
extern void com_shell(wordlist*);
extern void com_rehash(wordlist*);
extern void com_chdir(wordlist*);
extern void com_strcmp(wordlist*);
#else
extern void cp_init();
extern wordlist *cp_parse();
extern void com_echo();
extern void cp_redirect();
extern void cp_ioreset();
extern void cp_fixdescriptors();
extern void com_shell();
extern void com_rehash();
extern void com_chdir();
extern void com_strcmp();
#endif

/* front.c */
extern bool cp_cwait;
extern bool cp_dounixcom;
extern char *cp_csep;
#ifdef __STDC__
extern void com_codeblock(wordlist*);
extern void cp_addblock(char*,wordlist*);
extern bool cp_isblockdef(char*);
extern void cp_freeblock(char*);
extern int  cp_evloop(char*);
extern void cp_execcontrol(char*);
extern void cp_resetcontrol(void);
extern void cp_popcontrol(void);
extern void cp_pushcontrol(void);
extern void cp_toplevel(void);
extern void com_cdump(wordlist*);
#else
extern void com_codeblock();
extern void cp_addblock();
extern bool cp_isblockdef();
extern void cp_freeblock();
extern int cp_evloop();
extern void cp_execcontrol();
extern void cp_resetcontrol();
extern void cp_popcontrol();
extern void cp_pushcontrol();
extern void cp_toplevel();
extern void com_cdump();
#endif

/* glob.c */
extern char cp_cbrac;
extern char cp_ccurl;
extern char cp_comma;
extern char cp_huh;
extern char cp_obrac;
extern char cp_ocurl;
extern char cp_star;
extern char cp_til;
#ifdef __STDC__
extern void cp_doglob(wordlist**);
extern char *cp_tildexpand(char*);
extern void cp_pathfix(char*);
extern bool cp_globmatch(char*,char*);
#else
extern void cp_doglob();
extern char *cp_tildexpand();
extern void cp_pathfix();
extern bool cp_globmatch();
#endif

/* history.c */
extern bool cp_didhsubst;
extern char cp_bang;
extern char cp_hat;
extern int cp_maxhistlength;
extern struct histent *cp_lastone;
#ifdef __STDC__
extern void cp_histsubst(wordlist**);
extern void cp_addhistent(int,wordlist*);
extern void cp_hprint(int,int,bool);
extern void com_history(wordlist*);
#else
extern void cp_histsubst();
extern void cp_addhistent();
extern void cp_hprint();
extern void com_history();
#endif

/* lexical.c */
extern FILE *cp_inp_cur;
extern bool cp_bqflag;
extern bool cp_interactive;
extern char *cp_altprompt;
extern char *cp_promptstring;
extern char cp_hash;
extern int cp_event;
#ifdef __STDC__
extern wordlist *cp_lexer(char*);
extern int cp_inchar(FILE*);
#else
extern wordlist *cp_lexer();
extern int cp_inchar();
#endif

/* output.c */
extern bool out_moremode;
extern bool out_isatty;
extern int out_width;
extern int out_height;
#ifdef __STDC__
extern void out_winsize(int*,int*);
extern void out_init(void);
extern void out_send(char*);
# ifndef out_printf
extern void out_printf(char*, ...);
# endif
extern void out_wlprint(wordlist*);
#else
extern void out_winsize();
extern void out_init();
extern void out_send();
# ifndef out_printf
extern void out_printf();
# endif
extern void out_wlprint();
#endif


/* quote.c */
#ifdef __STDC__
extern void cp_wstrip(char*);
extern void cp_quoteword(char*);
extern void cp_printword(char*,FILE*);
extern void cp_striplist(wordlist*);
extern void cp_unquote(char*);
#else
extern void cp_wstrip();
extern void cp_quoteword();
extern void cp_printword();
extern void cp_striplist();
extern void cp_unquote();
#endif

/* unixcom.c */
#ifdef __STDC__
extern void cp_rehash(char*,bool);
extern bool cp_unixcom(wordlist*);
extern void cp_hstat(void);
#else
extern void cp_rehash();
extern bool cp_unixcom();
extern void cp_hstat();
#endif

/* variable.c */
extern bool cp_ignoreeof;
extern bool cp_noclobber;
extern bool cp_noglob;
extern bool cp_nonomatch;
extern char cp_dol;
#ifdef __STDC__
extern void cp_variablesubst(wordlist**);
extern void cp_vprint(void);
extern wordlist *cp_varwl(struct variable*);
extern void cp_vset(char*,int,char*);
extern void cp_remvar(char*);
extern void com_set(wordlist*);
extern struct variable *cp_setparse(wordlist*);
extern void com_unset(wordlist*);
extern void com_shift(wordlist*);
extern bool cp_getvar(char*,int,char*);
extern struct variable *cp_getvlist(void);
extern void va_free(struct variable*);
extern struct variable *va_copy(struct variable*);
extern void cp_pusharg(wordlist*);
extern void cp_poparg(void);
#else
extern void cp_variablesubst();
extern void cp_vprint();
extern wordlist *cp_varwl();
extern void cp_vset();
extern void cp_remvar();
extern void com_set();
extern struct variable *cp_setparse();
extern void com_unset();
extern void com_shift();
extern bool cp_getvar();
extern struct variable *cp_getvlist();
extern void va_free();
extern struct variable *va_copy();
extern void cp_pusharg();
extern void cp_poparg();
#endif

/* cpitf.c etc -- stuff CP needs from FTE */
extern struct comm *cp_coms;
extern char *cp_program;
extern char *cp_display;
extern bool ft_nutmeg;
#ifdef __STDC__
extern void ft_cpinit(void);
extern bool cp_istrue(wordlist*);
extern void cp_periodic(void);
extern void cp_doquit(void);
extern bool cp_oddcomm(char*,wordlist*);
extern bool cp_block(char*);
extern void cp_bind(char*);
extern void cp_unbind(char*);
extern double *ft_numparse(char**,bool);
extern struct variable *cp_enqvar(char*);
extern void cp_usrvars(struct variable**,struct variable**);
extern int cp_usrset(struct variable*,bool);
extern void fatal(void);
#else
extern void ft_cpinit();
extern bool cp_istrue();
extern void cp_periodic();
extern void cp_doquit();
extern bool cp_oddcomm();
extern bool cp_block();
extern void cp_bind();
extern void cp_unbind();
extern double *ft_numparse();
extern struct variable *cp_enqvar();
extern void cp_usrvars();
extern int cp_usrset();
extern void fatal();
#endif

#endif
