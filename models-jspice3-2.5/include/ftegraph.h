/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1988 Jeffery M. Hsu
         1992 Stephen R. Whiteley
****************************************************************************/

/*
 * This file contains the graph structure.
 */

#ifndef _GRAPH_H_
#define _GRAPH_H_

#include "fteconst.h"
#include "ftedata.h"      /* for struct dvec */

#define NUMCOLORS 32

#define GR_PLOT 0
#define GR_GRAF 1
#define GR_MPLT 2
#define GR_SCED 3

/* generic pointer to type specific data structure */
typedef char * GRDATA;

/* Device-independent data structure for plots. */
typedef struct graph {
    int graphid;
    int graphtype;     /* type of graph: GR_? */

    GRDATA plotdata;   /* normalized data */

    char *plotname;    /* name of plot this graph is in */
    int onevalue;      /* boolean variable,
                          true if plotting one value
                          against itself (real vs imaginary) */
    int degree;        /* degree of polynomial interpretation */

    int currentcolor;
    int linestyle;

    struct {
        int height, width;
    } viewport;
    int viewportxoff;  /* x offset of viewport w/in graph */
    int viewportyoff;  /* y offset of viewport w/in graph */

    struct {
        int xpos;      /* x position of graph in screen coord */
        int ypos;      /* y position of graph in screen coord */
        int width;     /* width of window on screen */
        int height;    /* height of window on screen */
    } absolute;

    struct {
        double xmin, ymin, xmax, ymax;
        /* cache:  width = xmax - xmin  height = ymax - ymin */
        double width, height;
    } datawindow;

    /* callbacks:
     */
#ifdef __STDC__
    void (*redraw)(struct graph*);  /* redraw procedure */
    void (*destroy)(GRDATA);        /* free data procedure */
    GRDATA (*copydata)(GRDATA);     /* copy data procedure */
#else
    void (*redraw)();
    void (*destroy)();
    GRDATA (*copydata)();
#endif

    /* note: this int is device dependent */
    int colors[NUMCOLORS];

    /* cache (datawindow size) / (viewport size) */
    double aspectratiox, aspectratioy;

    int ticmarks;               /* mark every ticmark'th point */
    int fontwidth, fontheight;  /* for use in grid */

    PLOTTYPE plottype;      /* defined in FTEconstant.h */
    struct {
        GRIDTYPE gridtype;    /* defined in FTEconstant.h */
        int circular;         /* TRUE if circular plot area */
        union {
            struct {
                char units[16]; /* unit labels */
                int spacing, numspace, distance, lowlimit, highlimit, mult;
                int onedec;     /* a boolean */
                int hacked;     /* true if hi - lo already hacked up */
            } lin;
            struct {
                int hmt, lmt, decsp, subs, pp;
            } log;
            struct {
                int radius, center;
                double mrad;
                int lmt;
            } circular;     /* bogus, rework when write polar grids, etc */
        } xaxis, yaxis;
        int xdatatype, ydatatype;
        double xdelta, ydelta; /* if non-zero, user-specified deltas */
        char *xlabel, *ylabel;
    } grid;

    int numbuttons;     /* number of buttons */
    struct {
        int id;
        char *message;
    } *buttons;
    int buttonsxoff;    /* viewportxoff + x size of viewport */
    int buttonsyoff;

    struct {
        int width, height;
        char message[161];        /* two lines of text */
    } messagebox;
    int messagexoff;
    int messageyoff;

    /* characters the user typed on graph */
    struct _keyed {
        char *text;
        int x, y;
        int colorindex;       /* index into colors array */
        struct _keyed *next;
    } *keyed;

    /* for zoomin */
    char *commandline;

    /* Need to have device dependent info here because
     * otherwise every device implementation would need to handle
     * its own device dependent analog of GRAPH.  I want a
     * language with generics.
     * Use pointer so we don't have to include device dependent
     * header files here.
     * Space here is allocated by NewViewport
     * and de-allocated by DestroyGraph.
     */
    char *devdep;

} GRAPH;

extern GRAPH *currentgraph;
extern int numgraphcxsw;

#define NEWGRAPH (GRAPH *) calloc(1, sizeof(GRAPH))



/***************************************************************************
Defs for the Input routine.

char_option: Used by the lexer and elsewhere.  response.reply.ch contains the
    character, obtained from the graphics package if request.fp is 0,
    otherwise from the standard input.

button_option: Used by the menu system and the help package,
    response.reply.button contains the button number.

click_option: Used in the X10 version for the hardcopy command,
    response.reply.graph is the associated graph structure.

point_option: Used in the SCED schematic capture editor.  Returns the window
    coords of the pointer in response.x,response.y.  If there was a button
    press, thr reply option is button_option and response.reply.button
    contains the button number.  If the keyboard was used, the character
    is returned in response.reply.ch, and the response option is char_option.

checkup_option: For handling pending asynchonous events.
****************************************************************************/

typedef enum {
    error_option,       /* a reply option only */
    button_option,      /* get a button press */
    char_option,        /* get a char */
    click_option,       /* point at a widget */
    point_option,       /* get a char or button, return coords */
    checkup_option      /* see if events in queue */
} OPTION;

typedef struct request {
    OPTION option;
    FILE *fp;
} REQUEST;

typedef struct response {
    OPTION option;
    int x;
    int y;
    union {
      int ch;
      GRAPH *graph;
      int button;
    } reply;
} RESPONSE;


#define rnd(x)  (int) ((x)+0.5)

/* graphdb.c */
#ifdef __STDC__
extern GRAPH *NewGraph(void);
extern GRAPH *FindGraph(int);
extern GRAPH *CopyGraph(GRAPH*);
extern int   DestroyGraph(int);
extern void  FreeGraphs(void);
extern void  SetGraphContext(int);
extern void  PushGraphContext(GRAPH*);
extern void  PopGraphContext(void);
#else
extern GRAPH *NewGraph();
extern GRAPH *FindGraph();
extern GRAPH *CopyGraph();
extern int   DestroyGraph();
extern void  FreeGraphs();
extern void  SetGraphContext();
extern void  PushGraphContext();
extern void  PopGraphContext();
#endif


#endif  /* notdef _GRAPH_H_ */
