/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1986 Wayne A. Christopher
         1994 Stephen R. Whiteley
****************************************************************************/

/*
 * Definitions for the help system.
 */

#include "cpstd.h"

#ifdef HAVE_X11
#include <X11/Intrinsic.h>
#include "x11util.h"
#endif

/* name of help database file */
#define DBFILE "spicehlp.txt"

/* keywords used in the database file */
#define HLP_KEYWORD   "!!KEYWORD"
#define HLP_TITLE     "!!TITLE"
#define HLP_TEXT      "!!TEXT"
#define HLP_SEEALSO   "!!SEEALSO"
#define HLP_SUBTOPICS "!!SUBTOPICS"

struct sHlpEnt {
    long offset;
    char *keyword;
    char *title;
    struct sHlpEnt *next;
};

typedef struct toplink {
    char *description;          /* really the subject */
    char *buttontext;
    char *keyword;
    struct toplink *next;
    struct topic *top;
#ifdef HAVE_X11
    Widget button;
    struct topic *newtop;
#endif
} toplink;

typedef struct topic {
    char *title;
    char *keyword;
    wordlist *text;
    char *chartext;
    toplink *subtopics;
    toplink *seealso;
    int xposition;
    int yposition;
    struct topic *parent;
    struct topic *lastborn;
    struct topic *sibling;
    int numlines;
    int maxcols;
    int curtopline;
#ifdef HAVE_X11
    widget_bag widgets;
#endif
} topic;

#define X_INCR      20
#define Y_INCR      20
#define START_XPOS  100
#define START_YPOS  100


/* External symbols. */

/* provide.c */
extern bool hlp_usex;
#ifdef __STDC__
extern void hlp_provide(topic*);
extern void hlp_fixchildren(topic*);
extern void hlp_killfamily(topic*);
#else
extern void hlp_provide();
extern void hlp_fixchildren();
extern void hlp_killfamily();
#endif

/* readhelp.c */
extern char *hlp_directory;
extern int hlp_initxpos;
extern int hlp_initypos;
#ifdef __STDC__
extern void hlp_main(char*,wordlist*);
extern topic *hlp_read(char*);
extern void hlp_free(topic*);
#else
extern void hlp_main();
extern topic *hlp_read();
extern void hlp_free();
#endif

/* textdisplay.c */
extern int hlp_width;
#ifdef __STDC__
extern bool hlp_tdisplay(topic*);
extern toplink *hlp_thandle(topic**);
extern void hlp_tkillwin(topic*);
#else
extern bool hlp_tdisplay();
extern toplink *hlp_thandle();
extern void hlp_tkillwin();
#endif

/* X11disp.c */
extern bool hlp_killx;
#ifdef __STDC__
extern bool hlp_xdisplay(topic*);
extern void hlp_xclosedisplay(void);
extern toplink *hlp_xhandle(topic**);
extern void hlp_xkillwin(topic*);
extern void hlp_xwait(topic*,bool);
#else
extern bool hlp_xdisplay();
extern void hlp_xclosedisplay();
extern toplink *hlp_xhandle();
extern void hlp_xkillwin();
extern void hlp_xwait();
#endif
