/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1986 Wayne A. Christopher
         1992 Stephen R. Whiteley
****************************************************************************/

/*
 * Definitions for external symbols in FTE.
 */

#ifndef FTEext_h
#define FTEext_h


/* main.c (in bin) */
extern bool ft_nutmeg;
extern char *ft_rawfile;
extern char *enddata;
extern char *errRtn;
extern char *errMsg;
extern char *cp_program;
extern char *cp_display;
extern IFsimulator *ft_sim;
#ifdef __STDC__
extern struct variable *(*if_getparam)(char*,char**,char*,int,wordlist**);
#else
extern struct variable *(*if_getparam)();
#endif

extern struct circ *ft_curckt;
extern struct circ *ft_circuits;

extern bool ft_intrpt;
extern bool ft_setflag;
extern bool ft_servermode;
extern bool ft_batchmode;
extern bool ft_nospiceadd;
extern bool ft_nopage;
extern bool ft_debug;
extern bool ft_parsedb;
extern bool ft_evdb;
extern bool ft_vecdb;
extern bool ft_grdb;
extern bool ft_gidb;
extern bool ft_controldb;
extern bool ft_asyncdb;
extern bool ft_simdb;

extern FILE *cp_curin;
extern FILE *cp_curout;
extern FILE *cp_curerr;
extern FILE *cp_in;
extern FILE *cp_out;
extern FILE *cp_err;

extern bool ft_listprint;
extern bool ft_optsprint;
extern bool ft_nodesprint;
extern bool ft_acctprint;
extern struct plot *plot_list;

/* arg.c */
#ifdef __STDC__
extern int arg_print(wordlist*,struct comm*);
extern int arg_plot(wordlist*,struct comm*);
extern int arg_load(wordlist*,struct comm*);
extern int arg_let(wordlist*,struct comm*);
extern int arg_set(wordlist*,struct comm*);
extern int arg_display(void);
extern wordlist *outmenuprompt(char*);
#else
extern int arg_print();
extern int arg_plot();
extern int arg_load();
extern int arg_let();
extern int arg_set();
extern int arg_display();
extern wordlist *outmenuprompt();
#endif

/* aspice.c */
#ifdef __STDC__
extern void com_aspice(wordlist*);
extern void com_jobs(wordlist*);
extern void ft_checkkids(void);
extern void com_rspice(wordlist*);
#else
extern void com_aspice();
extern void com_jobs();
extern void ft_checkkids();
extern void com_rspice();
#endif

/* cmath1.c */
extern bool cx_degrees;
#ifdef __STDC__
extern char *cx_mag(char*,int,int,int*,short*);
extern char *cx_ph(char*,int,int,int*,short*);
extern char *cx_j(char*,int,int,int*,short*);
extern char *cx_real(char*,int,int,int*,short*);
extern char *cx_imag(char*,int,int,int*,short*);
extern char *cx_pos(char*,int,int,int*,short*);
extern char *cx_db(char*,int,int,int*,short*);
extern char *cx_log(char*,int,int,int*,short*);
extern char *cx_ln(char*,int,int,int*,short*);
extern char *cx_exp(char*,int,int,int*,short*);
extern char *cx_sqrt(char*,int,int,int*,short*);
extern char *cx_sin(char*,int,int,int*,short*);
extern char *cx_cos(char*,int,int,int*,short*);
#else
extern char *cx_mag();
extern char *cx_ph();
extern char *cx_j();
extern char *cx_real();
extern char *cx_imag();
extern char *cx_pos();
extern char *cx_db();
extern char *cx_log();
extern char *cx_ln();
extern char *cx_exp();
extern char *cx_sqrt();
extern char *cx_sin();
extern char *cx_cos();
#endif

/* cmath2.c */
#ifdef __STDC__
extern char *cx_tan(char*,int,int,int*,short*);
extern char *cx_atan(char*,int,int,int*,short*);
extern char *cx_norm(char*,int,int,int*,short*);
extern char *cx_uminus(char*,int,int,int*,short*);
extern char *cx_mean(char*,int,int,int*,short*);
extern char *cx_length(char*,int,int,int*,short*);
extern char *cx_vector(char*,int,int,int*,short*);
extern char *cx_unitvec(char*,int,int,int*,short*);
extern char *cx_plus(char*,char*,int,int,int);
extern char *cx_minus(char*,char*,int,int,int);
extern char *cx_times(char*,char*,int,int,int);
extern char *cx_mod(char*,char*,int,int,int);
extern char *cx_rnd(char*,int,int,int*,short*);
extern char *cx_gauss(char*,int,int,int*,short*);
#else
extern char *cx_tan();
extern char *cx_atan();
extern char *cx_norm();
extern char *cx_uminus();
extern char *cx_mean();
extern char *cx_length();
extern char *cx_vector();
extern char *cx_unitvec();
extern char *cx_plus();
extern char *cx_minus();
extern char *cx_times();
extern char *cx_mod();
extern char *cx_rnd();
extern char *cx_gauss();
#endif

/* cmath3.c */
#ifdef __STDC__
extern char *cx_divide(char*,char*,int,int,int);
extern char *cx_comma(char*,char*,int,int,int);
extern char *cx_power(char*,char*,int,int,int);
extern char *cx_eq(char*,char*,int,int,int);
extern char *cx_gt(char*,char*,int,int,int);
extern char *cx_lt(char*,char*,int,int,int);
extern char *cx_ge(char*,char*,int,int,int);
extern char *cx_le(char*,char*,int,int,int);
extern char *cx_ne(char*,char*,int,int,int);
#else
extern char *cx_divide();
extern char *cx_comma();
extern char *cx_power();
extern char *cx_eq();
extern char *cx_gt();
extern char *cx_lt();
extern char *cx_ge();
extern char *cx_le();
extern char *cx_ne();
#endif

/* cmath4.c */
#ifdef __STDC__
extern char *cx_and(char*,char*,int,int,int);
extern char *cx_or(char*,char*,int,int,int);
extern char *cx_not(char*,int,int,int*,short*);
extern char *cx_interpolate(char*,int,int,int*,short*,
            struct plot*,struct plot*);
extern char *cx_deriv(char*,int,int,int*,short*,
            struct plot*,struct plot*);
#else
extern char *cx_and();
extern char *cx_or();
extern char *cx_not();
extern char *cx_interpolate();
extern char *cx_deriv();
#endif

/* spcmdtab.c or nutctab.c */
extern struct comm *cp_coms;

/* compose.c */
#ifdef __STDC__
extern void com_compose(wordlist*);
extern double xrandom(void);
extern double xgauss(void);
#else
extern void com_compose();
extern double xrandom();
extern double xgauss();
#endif

/* cpitf.c symbols declared in CPextern.h */

/* define.c */
#ifdef __STDC__
extern void com_define(wordlist*);
extern struct pnode *ft_substdef(char*,struct pnode*);
extern void com_undefine(wordlist*);
#else
extern void com_define();
extern struct pnode *ft_substdef();
extern void com_undefine();
#endif

/* diff.c */
#ifdef __STDC__
extern void com_diff(wordlist*);
#else
extern void com_diff();
#endif

/* error.c */
extern char ErrorMessage[];
#ifdef __STDC__
extern void fperror(char*,int);
extern void ft_sperror(int,char*);
extern void fatal(void);
#else
extern void fperror();
extern void ft_sperror();
extern void fatal();
#endif

/* evaluate.c */
#ifdef __STDC__
extern struct dvec *ft_evaluate(struct pnode*);
extern struct dvlist *ft_dvlist(struct pnode*);
extern struct dvec *op_plus(struct pnode*,struct pnode*);
extern struct dvec *op_minus(struct pnode*,struct pnode*);
extern struct dvec *op_comma(struct pnode*,struct pnode*);
extern struct dvec *op_times(struct pnode*,struct pnode*);
extern struct dvec *op_mod(struct pnode*,struct pnode*);
extern struct dvec *op_divide(struct pnode*,struct pnode*);
extern struct dvec *op_power(struct pnode*,struct pnode*);
extern struct dvec *op_eq(struct pnode*,struct pnode*);
extern struct dvec *op_gt(struct pnode*,struct pnode*);
extern struct dvec *op_lt(struct pnode*,struct pnode*);
extern struct dvec *op_ge(struct pnode*,struct pnode*);
extern struct dvec *op_le(struct pnode*,struct pnode*);
extern struct dvec *op_ne(struct pnode*,struct pnode*);
extern struct dvec *op_and(struct pnode*,struct pnode*);
extern struct dvec *op_or(struct pnode*,struct pnode*);
extern struct dvec *op_range(struct pnode*,struct pnode*);
extern struct dvec *op_ind(struct pnode*,struct pnode*);
extern struct dvec *op_uminus(struct pnode*);
extern struct dvec *op_not(struct pnode*);
#else
extern struct dvec *ft_evaluate();
extern struct dvlist *ft_dvlist();
extern struct dvec *op_plus();
extern struct dvec *op_minus();
extern struct dvec *op_comma();
extern struct dvec *op_times();
extern struct dvec *op_mod();
extern struct dvec *op_divide();
extern struct dvec *op_power();
extern struct dvec *op_eq();
extern struct dvec *op_gt();
extern struct dvec *op_lt();
extern struct dvec *op_ge();
extern struct dvec *op_le();
extern struct dvec *op_ne();
extern struct dvec *op_and();
extern struct dvec *op_or();
extern struct dvec *op_range();
extern struct dvec *op_ind();
extern struct dvec *op_uminus();
extern struct dvec *op_not();
#endif

/* inpcom.c */
#ifdef __STDC__
extern char *readline(FILE*);
extern FILE *inp_pathopen(char*,char*);
extern void inp_readall(FILE*,struct line**,char*);
extern void inp_casefix(char*);
extern void inp_deckfree(struct line*);
extern struct line *inp_deckcopy(struct line*);
#else
extern char *readline();
extern FILE *inp_pathopen();
extern void inp_readall();
extern void inp_casefix();
extern void inp_deckfree();
extern struct line *inp_deckcopy();
#endif

/* interp.c */
#ifdef __STDC__
extern bool ft_interpolate(double*,double*,double*,int,double*,int,int);
extern bool ft_polyfit(double*,double*,double*,int,double*);
extern double ft_peval(double,double*,int);
#else
extern bool ft_interpolate();
extern bool ft_polyfit();
extern double ft_peval();
#endif

/* misccoms.c */
#ifdef __STDC__
extern void com_help(wordlist*);
extern void com_ahelp(wordlist*);
extern void com_ghelp(wordlist*);
extern void com_quit(wordlist*);
extern void com_bug(wordlist*);
extern void com_version(wordlist*);
#else
extern void com_help();
extern void com_ahelp();
extern void com_ghelp();
extern void com_quit();
extern void com_bug();
extern void com_version();
#endif

/* numparse.c */
extern bool ft_strictnumparse;
#ifdef __STDC__
extern double *ft_numparse(char**,bool);
#else
extern double *ft_numparse();
#endif

/* nutinp.c */
#ifdef __STDC__
extern void nutcom_edit(wordlist*);
extern void nutcom_sced(wordlist*);
extern bool inp_edit(char*);
extern void inp_nutsource(FILE*,bool,char*);
extern void nutcom_source(wordlist*);
#else
extern void nutcom_edit();
extern void nutcom_sced();
extern bool inp_edit();
extern void inp_nutsource();
extern void nutcom_source();
#endif

/* options.c */
#ifdef __STDC__
extern void cp_internal_init(void);
extern void com_usrset(wordlist*);
extern int  cp_internalset(struct variable*,bool);
extern struct variable *cp_enqvar(char*);
extern struct variable *cp_uservars(struct variable**,struct variable**);
extern struct line *inp_getopts(struct line*);
#else
extern void cp_internal_init();
extern void com_usrset();
extern int  cp_internalset();
extern struct variable *cp_enqvar();
extern struct variable *cp_uservars();
extern struct line *inp_getopts();
#endif

/* parse.c */
extern struct func ft_funcs[];
extern struct func func_not;
extern struct func func_uminus;
#ifdef __STDC__
extern struct pnode *ft_getpnames(wordlist*,bool);
extern void inp_pnfree(struct pnode*);
#else
extern struct pnode *ft_getpnames();
extern void inp_pnfree();
#endif

/* postcoms.c */
#ifdef __STDC__
extern void com_let(wordlist*);
extern void com_unlet(wordlist*);
extern void com_load(wordlist*);
extern void com_print(wordlist*);
extern void com_write(wordlist*);
extern void com_display(wordlist*);
extern void com_splot(wordlist*);
extern void com_cross(wordlist*);
extern void com_destroy(wordlist*);
#else
extern void com_let();
extern void com_unlet();
extern void com_load();
extern void com_print();
extern void com_write();
extern void com_display();
extern void com_splot();
extern void com_cross();
extern void com_destroy();
#endif

/* rawfile.c */
extern int raw_prec;
#ifdef __STDC__
extern void raw_write(char*,struct plot*,bool,bool);
extern struct plot *raw_read(char*);
#else
extern void raw_write();
extern struct plot *raw_read();
#endif

/* resource.c */
#ifdef __STDC__
extern void com_rusage(wordlist*);
extern void ft_ckspace(void);
extern void rusage_init(void);
#else
extern void com_rusage();
extern void ft_ckspace();
extern void rusage_init();
#endif

/* signal.c */
#ifdef __STDC__
extern RETSIGTYPE ft_sigintr(void);
extern RETSIGTYPE sigfloat(int,int);
extern RETSIGTYPE sigstop(void);
extern RETSIGTYPE sigcont(void);
extern RETSIGTYPE sigill(void);
extern RETSIGTYPE sigbus(void);
extern RETSIGTYPE sigsegv(void);
extern RETSIGTYPE sig_sys(void);
#else
extern RETSIGTYPE ft_sigintr();
extern RETSIGTYPE sigfloat();
extern RETSIGTYPE sigstop();
extern RETSIGTYPE sigcont();
extern RETSIGTYPE sigill();
extern RETSIGTYPE sigbus();
extern RETSIGTYPE sigsegv();
extern RETSIGTYPE sig_sys();
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
extern struct variable *if_getstat();
extern struct variable *nutif_getparam();
extern wordlist *GetAnalysisFromDeck();
extern int  InProgress();
extern int  IsIplot();
#endif

/* types.c */
#ifdef __STDC__
extern void com_dftype(wordlist*);
extern char *ft_typabbrev(int);
extern char *ft_typenames(int);
extern int ft_typnum(char*);
extern char *ft_plotabbrev(char*);
extern void com_stype(wordlist*);
#else
extern void com_dftype();
extern char *ft_typabbrev();
extern char *ft_typenames();
extern int ft_typnum();
extern char *ft_plotabbrev();
extern void com_stype();
#endif

/* vectors.c */
extern struct plot *plot_cur;
#ifdef __STDC__
extern void ft_loadfile(char*);
extern void plot_add(struct plot*);
extern void plot_new(struct plot*);
extern void plot_delete(struct plot*);
extern void plot_setcur(char*);
extern void plot_docoms(wordlist*);
extern struct plot *plot_alloc(char*);
extern bool plot_prefix(char*,char*);
extern void vec_newtemp(struct dvec*);
extern void vec_newperm(struct dvec*);
extern void vec_remove(char*);
extern void vec_dlfree(struct dvlist*);
extern struct dvec *vec_fromplot(char*,struct plot*);
extern struct dvec *vec_get(char*);
extern struct dvec *vec_copy(struct dvec*);
extern void vec_gc(void);
extern bool vec_eq(struct dvec*,struct dvec*);
extern char *vec_basename(struct dvec*);
extern void vec_sort(struct dvec*);
extern struct dvec *vec_mkfamily(struct dvec*);
#else
extern void ft_loadfile();
extern void plot_add();
extern void plot_new();
extern void plot_delete();
extern void plot_setcur();
extern struct plot *plot_alloc();
extern bool plot_prefix();
extern void vec_newtemp();
extern void vec_newperm();
extern void vec_remove();
extern void vec_dlfree();
extern struct dvec *vec_fromplot();
extern struct dvec *vec_get();
extern void plot_docoms();
extern struct dvec *vec_copy();
extern void vec_gc();
extern bool vec_eq();
extern char *vec_basename();
extern void vec_sort();
extern struct dvec *vec_mkfamily();
#endif

#endif /* FTEext_h */
