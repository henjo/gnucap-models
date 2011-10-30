/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
**********/

#ifndef UTIL
#define UTIL

#define MALLOC(x) calloc(1,(unsigned)(x))
#define FREE(x) {if (x) {free((char *)(x));(x) = 0;}}

#ifdef STDC_HEADERS
#ifndef STDLIB_IS_INCLUDED
#define STDLIB_IS_INCLUDED
#include <stdlib.h>
#endif
#else
extern char *malloc();
extern char *calloc();
extern char *realloc();
extern void free();
#endif

extern char *trealloc();
extern char *tmalloc();

#define TRUE 1
#define FALSE 0
#define REALLOC(x,y) trealloc((char *)(x),(unsigned)(y))

#ifdef DEBUG
#define DEBUGMSG(textargs) printf(textargs)
#else
#define DEBUGMSG(testargs) 
#endif

#ifdef HAVE_NOINLINE
#define FABS(a) fabs(a)
double fabs();
#else
#define FABS(a) ( ((a)<0) ? -(a) : (a) )
#endif

/* XXX Move these into the above ifdef someday */
#ifdef MIN
#undef MIN
#endif
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#ifdef MAX
#undef MAX
#endif
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#ifdef SIGN
#undef SIGN
#endif
#define SIGN(a,b) ( b >= 0 ? (a >= 0 ? a : - a) : (a >= 0 ? - a : a))

#ifdef ABORT
#undef ABORT
#endif
#define ABORT() fflush(stderr);fflush(stdout);abort();

#ifdef ERROR
#undef ERROR
#endif
#define ERROR(CODE,MESSAGE) {  \
  errMsg = MALLOC(strlen(MESSAGE) + 1); \
  strcpy(errMsg, (MESSAGE));  \
  return (CODE); }

#ifdef NEW
#undef NEW
#endif
#define NEW(TYPE) ((TYPE *) MALLOC(sizeof(TYPE)))

#endif /*UTIL*/

#define R_NORM(A,B) { \
  if ((A) == 0.0) { \
    (B) = 0; \
  } else { \
    while (FABS(A) > 1.0) { \
      (B) += 1; \
      (A) /= 2.0; \
    } \
    while (FABS(A) < 0.5) { \
      (B) -= 1; \
      (A) *= 2.0; \
    } \
  } \
}
