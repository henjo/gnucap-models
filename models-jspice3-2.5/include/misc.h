/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Wayne A. Christopher
         1992 Stephen R. Whiteley
****************************************************************************/

#ifndef MISC_H
#define MISC_H

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif


#define BSIZE_SP  512

#ifdef HAVE_EXIT1
#define EXIT_NORMAL 1
#define EXIT_BAD    0
#else
#define EXIT_NORMAL 0
#define EXIT_BAD    1
#endif

#define eq(a,b)  (!strcmp((a), (b)))
#define isalphanum(c)   (isalpha(c) || isdigit(c))
#define hexnum(c) ((((c) >= '0') && ((c) <= '9')) ? ((c) - '0') : ((((c) >= \
        'a') && ((c) <= 'f')) ? ((c) - 'a' + 10) : ((((c) >= 'A') && \
        ((c) <= 'F')) ? ((c) - 'A' + 10) : 0)))

#ifndef tfree
#define tfree(x)  (txfree((char*)x), x = 0)
#endif

#ifndef alloc
#define alloc(TYPE)  ((TYPE *) tmalloc(sizeof(TYPE)))
#endif

/* alloc.c */
#ifdef __STDC__
extern char *tmalloc(int);
extern char *trealloc(char*,int);
extern void txfree(char*);
#else
extern char *tmalloc();
extern char *trealloc();
extern void txfree();
#endif

/* hash.c */
#ifdef __STDC__
extern void *htab_init(void);
extern void htab_add(char*,void*,void*);
extern void htab_delete(char*,void*);
extern void *htab_get(char*,void*);
extern void htab_free(void*,int);
extern void htab_print(void*,char*);
extern void *htab_wl(void*);
extern void *htab_list(void*);
extern int  htab_empty(void*);
#else
extern void *htab_init();
extern void htab_add();
extern void htab_delete();
extern void *htab_get();
extern void htab_free();
extern void htab_print();
extern void *htab_wl();
extern void *htab_list();
extern int  htab_empty();
#endif

/* libfuncs.c */
#ifdef __STDC__
extern char *smktemp(char*);
#else
extern char *smktemp();
#endif

/* printnum.c */
#ifdef __STDC__
extern char *fixxp2(char*);
extern char *ecvt_n(double,int);
extern char *printnum(double);
#else
extern char *fixxp2();
extern char *ecvt_n();
extern char *printnum();
#endif

/* string.c */
#ifdef __STDC__
extern int  prefix(char*,char*);
extern char *copy(char*);
extern int  substring(char*,char*);
extern void appendc(char*,int);
extern int  scannum(char*);
extern int  cieq(char*,char*);
extern int  ciprefix(char*,char*);
extern void strtolower(char*);
extern char *gettok(char**);
extern int  copytok(char*,char**);
extern int  copytok1(char*,char**);
extern void advtok(char**);
#else
extern int  prefix();
extern char *copy();
extern int  substring();
extern void appendc();
extern int  scannum();
extern int  cieq();
extern int  ciprefix();
extern void strtolower();
extern char *gettok();
extern int  copytok();
extern int  copytok();
extern void advtok();
#endif

/* time.c */
#ifdef __STDC__
extern char *datestring(void);
extern void timedeff();
extern double seconds(void);
#else
extern char *datestring();
extern void timedeff();
extern double seconds();
#endif


/* Externs from libc */

#ifdef STDC_HEADERS

#ifndef STDLIB_IS_INCLUDED
#define STDLIB_IS_INCLUDED
#include <stdlib.h>
#endif

#endif

#define false 0
#define true 1

#ifdef HAVE_DOSDIRS

typedef int  *DIR;
struct direct {
    int   d_reclen;
    short d_ino;
    short d_namelen;
    char  d_name[20];
};

#ifdef __STDC__
extern DIR *opendir(char *);
extern struct direct *readdir(DIR *);
#else
extern DIR *opendir( );
extern struct direct *readdir( );
#endif

#endif

#endif /* MISC_H */
