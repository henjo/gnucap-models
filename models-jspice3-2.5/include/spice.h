/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
****************************************************************************/

/*
 *  Operating System
 */

#if __MSDOS__ || __msdos__ || defined (MSDOS)
#ifndef MSDOS
#define MSDOS
#endif
#endif

#include "config.h"

#ifdef WANT_X11
#ifdef AVAIL_X11
#define HAVE_X11
#endif
#endif

#ifdef WANT_MFB
#ifdef AVAIL_MFB
#define HAVE_MFB
#endif
#endif

#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif

#include <math.h>

#ifdef HAVE_FLOAT_H
#include <float.h>
#endif

#ifndef HAVE_ERFC
#ifdef __STDC__
extern double erfc(double);
#else
extern double erfc();
#endif
#endif


#ifdef vax
/*
 *  VAX Floating Point
 */              

#define HAVE_VAX_FPERRORS
#define MAX_EXP_ARG    87.0        /* Approximate largest arg to exp() */

#ifndef DBL_EPSILON
#define DBL_EPSILON 6.93889e-18
#endif

#ifndef DBL_MAX
#define DBL_MAX     1.70141e+38
#endif

#ifndef DBL_MIN
#define DBL_MIN     2.938743e-39
#endif

#ifndef SHRT_MAX
#define SHRT_MAX    32766
#endif

#ifndef INT_MAX
#define INT_MAX     2147483646
#endif

#ifndef LONG_MAX
#define LONG_MAX    2147483646
#endif

#define MAXPOSINT   INT_MAX

#ifndef HUGE_VAL
#define HUGE_VAL    1.701411834604692293e+38 
#endif

#else /* !vax */
/*
 *  IEEE Floating point
 */

#define MAX_EXP_ARG  709.0

#ifndef DBL_EPSILON
#define DBL_EPSILON 8.9e-15
#endif

#ifndef DBL_MAX
#define DBL_MAX     1.79769313486231e+308
#endif

#ifndef DBL_MIN
#define DBL_MIN     2.22507385850721e-308
#endif

#ifndef SHRT_MAX
#define SHRT_MAX    32766
#endif

#ifndef INT_MAX
#define INT_MAX     2147483646
#endif

#ifndef LONG_MAX
#define LONG_MAX    2147483646
#endif

#define MAXPOSINT   INT_MAX

#ifndef HUGE_VAL
#define HUGE_VAL    1.8e+308
#endif

#endif /* vax? */


#ifndef HUGE
#define HUGE        HUGE_VAL
#endif

#ifndef M_PI
#define M_PI        3.14159265358979323846
#endif

#ifndef M_E
#define M_E         2.7182818284590452354
#endif

#ifndef M_LOG10E
#define M_LOG10E    0.43429448190325182765
#endif


/*
 *  #define-s that are always on
 */

#define NEWBYPASS
#define CAPZEROBYPASS
#define NEWCONV
/* #define CAPBYPASS    Internal use only */
#ifdef HAVE_NOVM
#define NOBYPASS
#endif


#define    NUMELEMS(ARRAY)    (sizeof(ARRAY)/sizeof(*ARRAY))

extern char *Spice_Exec_Dir;
extern char *Spice_Lib_Dir;
extern char Spice_OptChar;
extern char *Def_Editor;
extern char *Bug_Addr;
extern int  AsciiRawFile;
extern char *Spice_Host;
extern char *Spiced_Log;

extern char Spice_Notice[ ];
extern char Spice_Version[ ];
extern char Spice_Build_Date[ ];

extern char *News_File;
extern char *Default_MFB_Cap;
extern char *Spice_Path;
extern char *Help_Path;
extern char *Lib_Path;
extern char *Sced_Path;
