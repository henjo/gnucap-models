/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1992 Stephen R. Whiteley
****************************************************************************/

/* 
 * Header for new graphics package.
 */

/* max number of displayed curves */
#define PNUM 8

/* number of characters in trace name field */
#define FIELD 12

/* option mask defines */
#define OPT_n 0x1
#define OPT_N 0x2
#define OPT_x 0x4
#define OPT_X 0x8
#define OPT_b 0x10
#define OPT_t 0x20
#define OPT_p 0x40
#define OPT_Xx 0x80
#define OPT_help 0x100
#define OPT_quit 0x200
#define OPT_mark 0x400
#define OPT_add  0x800
#define OPT_hcpy 0x1000
#define OPT_step 0x2000

/* type of trace: voltage, current, other */
#define GRAF_V 0
#define GRAF_I 1
#define GRAF_O 2


struct gframe {
    double beg;   /* initial data value */
    double end;   /* final data value */
    double min;   /* scale initial value */
    double max;   /* scale final value */
};

/* scale factors for plot */
struct gscale {
    int ncells;            /* number of x axis cells */
    int numpts;            /* number of x axis points to plot */
    int strtpt;            /* starting x axis point offset */
    int lflg;              /* nonzero if log x axis */
    /*
     * The following two arrays store min/max values used for
     * building special y scales.  The last four elements are
     * the values for all traces (global min/max), just the "v"
     * traces, just the "i" traces, and other traces.  The
     * first (up to PNUM elements) store min/max values derived
     * from data for expanded (x option) plots.
     */
    double ymin[PNUM+4];   /* y scale min values: plots,all,v,i,other */
    double ymax[PNUM+4];   /* y scale max values: plots,all,v,i,other */
    struct gframe xfull;   /* full x scale values */
    struct gframe xcurr;   /* current x scale */
};

/* trace data */
struct gtrace {
    int cycles;            /* cycles of multidimensional plot */ 
    int extra;             /* extra points of incomplete multi plot */
    char name[FIELD+2];    /* name of trace */
    int type;              /* GRAF_V, GRAF_I, GRAF_O */
    double scale[2];       /* default y scale for trace */
    int lasty;             /* last point plotted (screen coord) */
    float *data;           /* interpolated data for trace */
};

/* viewport description */
struct screen {
    int X;         /* plot viewport width       */
    int Y;         /* plot viewport height      */
    int W;         /* font width                */
    int H;         /* font height               */
    int W2;        /* half font width           */
    int H2;        /* half font height          */
    int H3;        /* vertical space and a half */
    int XL;        /* left margin               */
    int XU;        /* right      "              */
    int YL;        /* lower      "              */
    int YU;        /* upper      "              */
    int XS;        /* screen width              */
    int YS;        /* screen height             */
    int XC;        /* X center of plot area     */
    int YC;        /* Y center of plot area     */
};

/* stored reference */
struct gref {
    int x;         /* X reference               */
    bool set;      /* using offsets             */
};

/* plot data storage */
struct gplot {
    int numtr;                 /* number of traces */
    int numpt;                 /* number of points in traces */
    int opt;                   /* option mask, see defines */
    int lastx;                 /* last x point (screen coords) */
    bool iniplot;              /* set when doing iplot */
    float *xdata;              /* interpolated x scale data */
    struct gscale *scale;      /* scaling parameters for x axis */
    struct plot *plot;         /* originating plot structure */
    struct dvlist *dlist;      /* list of dvecs being plotted, for iplot */
    struct gtrace tdata[PNUM]; /* trace data */
    struct screen scr;         /* viewport description */
    struct gref ref;           /* storage of marker offset */
};

/*
 * Structures for operating range analysis plotting.
 */

struct chkpts {   /* linked list of x-y file points to plot */
    char *v1;     /* value 1 steps (as int) */
    char *v2;     /* value 2 steps (as int) */
    char *pf;     /* boolean pass/file values */
    char delta1;  /* number of steps 1 */
    char delta2;  /* number of steps 2 */
    int size;     /* length of list */
    int rsize;    /* allocated length */
    int xc;       /* screen center x */
    int yc;       /* screen center y */
    int d;        /* screen cell size */
    int d1;       /* current step 1 */
    int d2;       /* current step 2 */
    double minv1; /* min/max data values */
    double maxv1;
    double minv2;
    double maxv2;
    struct chkpts *next;
};

struct mdata {
    struct chkpts *pts;
    struct screen scr;
};
