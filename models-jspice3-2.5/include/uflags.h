/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1993 Stephen R. Whiteley
****************************************************************************/

/* user specified flags for sensitivity analysis, etc. */

/* mask for the standard flags defined in ifsim.h */
#define IF_STDFLAGS 0xffff

/* Used by sensetivity to check if a parameter is not useful */
#define IF_REDUNDANT 0x0010000
#define IF_PRINCIPAL 0x0020000
#define IF_AC        0x0040000
#define IF_AC_ONLY   0x0080000
#define IF_NOISE     0x0100000
#define IF_NONSENSE  0x0200000

#define IF_SETQUERY  0x0400000
#define IF_ORQUERY   0x0800000
#define IF_CHKQUERY  0x1000000

/* For "show" command: do not print value in a table by default */
#define IF_UNINT 0x2000000


/* IOP()    Input/output parameter
 * IOPP()   IO parameter which the principle value of a device (used
 *           for naming output variables in sensetivity)
 * IOPA()   IO parameter significant for time-varying (non-dc) analyses
 * IOPAP()  Principle value is significant for time-varying analyses
 * IOPAA()  IO parameter significant for ac analyses only
 * IOPAAP() IO parameter principle value for ac analyses only
 * IOPN()   IO parameter significant for noise analyses only
 * IOPR()   Redundant IO parameter name (e.g. "vto" vs. "vt0")
 * IOPX()   IO parameter which is not used by sensetivity in any case
 *
 * IOPQ()   This (Q) parameter must be non-zero for sensetivity of
 *           following Z parameter (model params done first)
 * IOPZ()   Prev. 'Q' parameter must be non-zero for sensetivity
 * IOPQO()  Like Q, but or-ed with previous Q value
 * ....U()  Uninteresting for default "show" command output
 */

#ifndef HAVE_MINDATA

#define IOPU(a,b,c,d)   {a, b, c|IF_SET|IF_ASK|IF_UNINT, d}
#define IOPP(a,b,c,d)   {a, b, c|IF_SET|IF_ASK|IF_PRINCIPAL, d}
#define IOPA(a,b,c,d)   {a, b, c|IF_SET|IF_ASK|IF_AC, d}
#define IOPAU(a,b,c,d)  {a, b, c|IF_SET|IF_ASK|IF_AC|IF_UNINT, d}
#define IOPAP(a,b,c,d)  {a, b, c|IF_SET|IF_ASK|IF_AC|IF_PRINCIPAL, d}
#define IOPAA(a,b,c,d)  {a, b, c|IF_SET|IF_ASK|IF_AC_ONLY, d }
#define IOPAAU(a,b,c,d) {a, b, c|IF_SET|IF_ASK|IF_AC_ONLY|IF_UNINT,d}
#define IOPPA(a,b,c,d)  {a, b, c|IF_SET|IF_ASK|IF_AC_ONLY|IF_PRINCIPAL, d}
#define IOPN(a,b,c,d)   {a, b, c|IF_SET|IF_ASK|IF_NOISE, d}
#define IOPR(a,b,c,d)   {a, b, c|IF_SET|IF_ASK|IF_REDUNDANT, NULL}
#define IOPX(a,b,c,d)   {a, b, c|IF_SET|IF_ASK|IF_NONSENSE, d}
#define IOPXU(a,b,c,d)  {a, b, c|IF_SET|IF_ASK|IF_NONSENSE|IF_UNINT, d}
#define IOPQ(a,b,c,d)   {a, b, c|IF_SET|IF_ASK|IF_SETQUERY, d }
#define IOPQU(a,b,c,d)  {a, b, c|IF_SET|IF_ASK|IF_SETQUERY|IF_UNINT, d}
#define IOPZ(a,b,c,d)   {a, b, c|IF_SET|IF_ASK|IF_CHKQUERY, d }
#define IOPZU(a,b,c,d)  {a, b, c|IF_SET|IF_ASK|IF_CHKQUERY|IF_UNINT, d}
#define IOPQO(a,b,c,d)  {a, b, c|IF_SET|IF_ASK|IF_ORQUERY,d }
#define OPU(a,b,c,d)    {a, b, c|IF_ASK|IF_UNINT, d}
#define OPR(a,b,c,d)    {a, b, c|IF_ASK|IF_REDUNDANT, d}

#else /* HAVE_MINDATA */

/*
 * these are for the IBM PC which gets upset by the long description 
 * strings used in IFparm definitions because they add up to more than
 * 64k of static data
 */

#define IOPU(a,b,c,d)   {a, b, c|IF_SET|IF_ASK|IF_UNINT, NULL}
#define IOPP(a,b,c,d)   {a, b, c|IF_SET|IF_ASK|IF_PRINCIPAL, NULL}
#define IOPA(a,b,c,d)   {a, b, c|IF_SET|IF_ASK|IF_AC, NULL}
#define IOPAU(a,b,c,d)  {a, b, c|IF_SET|IF_ASK|IF_AC|IF_UNINT, NULL}
#define IOPAP(a,b,c,d)  {a, b, c|IF_SET|IF_ASK|IF_AC|IF_PRINCIPAL, NULL}
#define IOPAA(a,b,c,d)  {a, b, c|IF_SET|IF_ASK|IF_AC_ONLY, NULL}
#define IOPAAU(a,b,c,d) {a, b, c|IF_SET|IF_ASK|IF_AC_ONLY|IF_UNINT, NULL}
#define IOPPA(a,b,c,d)  {a, b, c|IF_SET|IF_ASK|IF_AC_ONLY|IF_PRINCIPAL, NULL}
#define IOPN(a,b,c,d)   {a, b, c|IF_SET|IF_ASK|IF_NOISE, NULL}
#define IOPR(a,b,c,d)   {a, b, c|IF_SET|IF_ASK|IF_REDUNDANT, NULL}
#define IOPX(a,b,c,d)   {a, b, c|IF_SET|IF_ASK|IF_NONSENSE, NULL}
#define IOPXU(a,b,c,d)  {a, b, c|IF_SET|IF_ASK|IF_NONSENSE|IF_UNINT, NULL}
#define IOPQ(a,b,c,d)   {a, b, c|IF_SET|IF_ASK|IF_SETQUERY, NULL}
#define IOPQU(a,b,c,d)  {a, b, c|IF_SET|IF_ASK|IF_SETQUERY|IF_UNINT, NULL}
#define IOPZ(a,b,c,d)   {a, b, c|IF_SET|IF_ASK|IF_CHKQUERY, NULL}
#define IOPZU(a,b,c,d)  {a, b, c|IF_SET|IF_ASK|IF_CHKQUERY|IF_UNINT, NULL}
#define IOPQO(a,b,c,d)  {a, b, c|IF_SET|IF_ASK|IF_ORQUERY, NULL}
#define OPU(a,b,c,d)    {a, b, c|IF_ASK|IF_UNINT, NULL}
#define OPR(a,b,c,d)    {a, b, c|IF_ASK|IF_REDUNDANT, NULL}

#endif
