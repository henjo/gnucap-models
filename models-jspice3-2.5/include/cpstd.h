/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1986 Wayne A. Christopher
         1992 Stephen R. Whiteley
****************************************************************************/

/*
 * Standard definitions. This file serves as the header file for std.c and
 * wlist.c
 */

#ifndef _STD_H_
#define _STD_H_

typedef int bool;

#include "misc.h"

/* Doubly linked lists of words. */
struct wordlist {
    char *wl_word;
    struct wordlist *wl_next;
    struct wordlist *wl_prev;
} ;
typedef struct wordlist wordlist;

/* Complex numbers. */
struct _complex {   /* IBM portability... */
    double cx_real;
    double cx_imag;
} ;
typedef struct _complex complex;

#define realpart(cval)  ((struct _complex *) (cval))->cx_real
#define imagpart(cval)  ((struct _complex *) (cval))->cx_imag

/* Variables that are accessible to the parser via $varname expansions.
 * If the type is VT_LIST the value is a pointer to a list of the elements.
 */
struct variable {
    char va_type;
    char *va_name;
    union {
        bool vV_bool;
        int vV_num;
        double vV_real;
        char *vV_string;
        struct variable *vV_list;
    } va_V;
    struct variable *va_next;      /* Link. */
} ;

#define va_bool   va_V.vV_bool
#define va_num    va_V.vV_num
#define va_real   va_V.vV_real
#define va_string va_V.vV_string
#define va_vlist  va_V.vV_list

#define VT_BOOL   1
#define VT_NUM    2
#define VT_REAL   3
#define VT_STRING 4
#define VT_LIST   5

/* Misc externs */

extern int cp_numdgt;
#ifdef __STDC__
extern void fatal(void);
#else
extern void fatal();
#endif

/* Externs from wlist.c */

#ifdef __STDC__
extern int wl_length(wordlist*);
extern void wl_free(wordlist*);
extern wordlist *wl_copy(wordlist*);
extern wordlist *wl_splice(wordlist*,wordlist*);
extern void wl_print(wordlist*,FILE*);
extern wordlist *wl_build(char**);
extern char **wl_mkvec(wordlist*);
extern wordlist *wl_append(wordlist*,wordlist*);
extern wordlist *wl_reverse(wordlist*);
extern char *wl_flatten(wordlist*);
extern wordlist *wl_nthelem(int,wordlist*);
extern void wl_sort(wordlist*);
extern wordlist *wl_range(wordlist*,int,int);
#else
extern int wl_length();
extern void wl_free();
extern wordlist *wl_copy();
extern wordlist *wl_splice();
extern void wl_print();
extern wordlist *wl_build();
extern char **wl_mkvec();
extern wordlist *wl_append();
extern wordlist *wl_reverse();
extern char *wl_flatten();
extern wordlist *wl_nthelem();
extern void wl_sort();
extern wordlist *wl_range();
#endif

#endif /* _STD_H_*/
