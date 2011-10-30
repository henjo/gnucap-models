/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Wayne A. Christopher
         1992 Stephen R. Whiteley
****************************************************************************/

/*
 * General front end stuff for output graphics.
 */

#ifndef PLOTdefs_h
#define PLOTdefs_h

#define DEF_WIDTH   80  /* Line printer width. */
#define DEF_HEIGHT  60  /* Line printer height. */
#define IPOINTMIN   20  /* When we start plotting incremental plots. */

#include "cpdefs.h"
#include "fteinp.h"
#include "plotdev.h"

#define mylog10(xx) (((xx) > 0.0) ? log10(xx) : (- log10(HUGE)))

extern struct plot *plot_cur;

/* structure to pass to gr_init(), etc */
struct grinit {
    double *xlims;        /* The size of the screen. */
    double *ylims;
    char *xname;          /* What to label things. */
    char *plotname;       
    char *hcopy;          /* The raster file. */
    int nplots;           /* How many plots there will be. */
    double xdelta;        /* Line increments for the scale. */
    double ydelta;        
    GRIDTYPE gridtype;    /* The grid type */
    PLOTTYPE plottype;    /*  and the plot type. */
    char *xlabel;         /* Labels for axes. */
    char *ylabel;         
    int xtype;            /* The types of the data graphed. */
    int ytype;            
    char *pname;          
    char *commandline;    /* For zoomdata() */
    bool nointerp;        /* skip interpolation */
};

#include "plotext.h"

#endif /* PLOTdefs_h */

