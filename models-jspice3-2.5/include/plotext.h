/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1986 Wayne A. Christopher
         1992 Stephen R. Whiteley
****************************************************************************/

/*
 * Definitions for external symbols in FTE for output graphics.
 */

#ifndef PLOText_h
#define PLOText_h

extern bool ft_nopage;
extern char ErrorMessage[];


/* agraf.c */
#ifdef __STDC__
extern void ft_agraf(struct dvlist*,char*);
#else
extern void ft_agraf();
#endif

/* clip.c */
#ifdef __STDC__
extern bool clip_line(int*,int*,int*,int*,int,int,int,int);
extern bool clip_to_circle(int*,int*,int*,int*,int,int,int);
#else
extern bool clip_line();
extern bool clip_to_circle();
#endif

/* doplot.c */
#ifdef __STDC__
extern void com_asciiplot(wordlist*);
extern void com_xgraph(wordlist*);
extern void com_hardcopy(wordlist*);
extern void ft_hardcopy(wordlist*,GRAPH*,bool,char*,bool);
extern void com_plot(wordlist*);
extern void plot_extend(struct dvec*,int);
extern void ft_xgraph(struct dvlist*,char*);
#else
extern void com_asciiplot();
extern void com_xgraph();
extern void com_hardcopy();
extern void ft_hardcopy();
extern void com_plot();
extern void plot_extend();
extern void ft_xgraph();
#endif

/* fourier.c */
#ifdef __STDC__
extern void com_fourier(wordlist*);
#else
extern void com_fourier();
#endif

/* graf.c */
#ifdef __STDC__
extern GRAPH *gr_init(struct dvlist*,char*);
extern void gr_point(struct dvec*,double,double,double,double,int);
extern void gr_end(void);
#else
extern GRAPH *gr_init();
extern void gr_point();
extern void gr_end();
#endif

/* grid.c */
#ifdef __STDC__
extern void gr_fixgrid(GRAPH*,double,double,int,int);
extern void gr_redrawgrid(GRAPH*);
#else
extern void gr_fixgrid();
extern void gr_redrawgrid();
#endif

/* hcopy.c */
#ifdef __STDC__
extern int ft_makehardcopy(GRAPH*,char*,char*,char*,char*);
#else
extern int ft_makehardcopy();
#endif

/* iplot.c */
#ifdef __STDC__
extern void gr_iplot(struct plot*);
extern void gr_end_iplot(void);
#else
extern void gr_iplot();
extern void gr_end_iplot();
#endif

/* newgraf.c */
#ifdef __STDC__
extern void com_graf(wordlist*);
extern int  iplot_begin(struct dvlist*,struct plot*);
extern int  iplot_point(void);
extern void iplot_end(void);
extern void graf_newdisplay(GRAPH*,int,int);
extern void graf_slopelocation(GRAPH*,int,int,int,int);
extern void com_mplot(wordlist*);
extern int  mp_init(int,int,double,double,double,double);
extern int  mp_where(int,int,int);
extern int  mp_mark(int,int);
extern int  mp_done(int);
extern void com_echo(wordlist*);
extern void WARNmsg(char*);
extern char *KbEdit(char*,int,int,int,int,int);
extern void KbRepaint(int,int);
extern void KbCursor(int,int);
#else
extern void com_graf();
extern int  iplot_begin();
extern int  iplot_point();
extern void iplot_end();
extern void graf_newdisplay();
extern void graf_slopelocation();
extern void com_mplot();
extern int  mp_init();
extern int  mp_where();
extern int  mp_mark();
extern int  mp_done();
extern void com_echo();
extern void WARNmsg();
extern char *KbEdit();
extern void KbRepaint();
extern void KbCursor();
#endif

/* points.c */
#ifdef __STDC__
extern double *ft_minmax(struct dvec*,bool);
extern int ft_findpoint(double,double*,int,int,bool);
extern double *ft_SMITHminmax(struct dvec*,bool);
#else
extern double *ft_minmax();
extern int ft_findpoint();
extern double *ft_SMITHminmax();
#endif

/* x11.c */
#ifdef __STDC__
extern void com_setrdb(wordlist*);
extern int  PopUpErrMessage(FILE*);
extern void ScedESC(void);
#else
extern void com_setrdb();
extern int  PopUpErrMessage();
extern void ScedESC();
#endif

#endif /* PLOText_h */
