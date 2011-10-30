/***************************************************************************
SCED - Schematic Capture Editor
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1981 Giles C. Billingsley  (parts of KIC layout editor)
         1992 Stephen R. Whiteley
****************************************************************************/

/*
 * SCED data structures.
 * 
 */

#include <setjmp.h>
#include "cddefs.h"
#include "fteinp.h"

/*
 * Environmental data
 * (this should go somewhere else)
 */

#define DEFAULT_EDIT_FILE  "noname"
#define DEFAULT_TERMINAL   NULL
#define INIT_FILE          "sced.rc"
#define DEFAULT_PATH       "."

/* name of device library */
#define DeviceLibrary "device.lib"
#define ModelLibrary "model.lib"


/*
 * There are 100 data base units per micron
 */
#define RESOLUTION        100
#define HALFRESOLUTION    50

/*
 * Size of the storage for polygon vertices
 */
#define POLYGONBUFSIZE        600
#define MAXPOLYGONVERTICES    300

/*
 * Viewport control flags
 */
#define SPLITSCREEN           'b'
#define FULLSCREEN            'o'
#define FINEVIEWPORTONLY      'f'
#define COARSEVIEWPORTONLY    'c'

/*
 * Menu names
 */
#define BASICMENU             'B'
#define AMBIGUITYMENU         'A'

/* 
 * Returns from PointLoop() function.
 */
#define PL_ESC 1
#define PL_UND 2
#define PL_CMD 3
#define PL_PCW 4

/* colormap indices */
/* Oh, bother.  X likes the highlighting color
 * to be 1, but Microsoft mouse insists on
 * color 15 for the cursor, which also
 * is highlighted.
 */
#define BackgroundColor          0
#define HighlightingColor        1
#define MenuTextColor            2      
#define MenuTextPrefixColor      3
#define MenuSelectedColor        4
#define MenuSelectedPrefixColor  5
#define MenuHighlightingColor    6
#define PromptTextColor          7
#define MoreTextColor            8
#define CoarseGridColor          9
#define FineGridColor            10
#define InstanceBBColor          11
#define InstanceNameColor        12
#define ExtraColor1              13
#define DrawingColor             14
#define ExtraColor2              15

/* character that terminates most commands */
#define ESCAPE 27


struct kl {
      int klR,klG,klB;    /* RGB color */
};

/*
 * Cursor desc.
 */
struct kc {
    long kcPredX,kcPredY,kcX,kcY;
    long kcRawX,kcRawY;
    long kcDX,kcDY;
    int  kcInFine;
};

/*
 * Area structure.
 */
struct ka {
    long kaLeft,kaBottom,kaRight,kaTop;
    long kaX,kaY;
    double kaWidth,kaHeight;
};

/*
 * Structure to keep current area of interest.
 */
struct a {
    long aLF,aBF,aRF,aTF;
    long aLC,aBC,aRC,aTC;
    int  aInFine;
    int  aInCoarse;
};

/*
 * Structure used to save windows in window stack.
 */
struct kw {
    long kwLastWindowX;
    long kwLastWindowY;
    long kwLastWindowWidth;
    long kwLastFineWindowX;
    long kwLastFineWindowY;
    long kwLastFineWindowWidth;
    struct kw *kwNext;
    char kwExpand;
    char kwExpandFineOnly;
    char kwName[8];
};

/*
 * Layout windows and the viewports they map to.
 */
struct kv {
    struct ka *kvFineWindow;
    struct ka *kvFineViewport;
    struct ka *kvCoarseWindow;
    struct ka *kvCoarseViewport;
    struct ka *kvLargeCoarseViewport;
    struct ka *kvSmallCoarseViewport;
    /* viewport/window */
    double kvFineRatio;
    double kvCoarseRatio;
    int kvFineViewportOnBottom;
    char kvControl; 
};


/* definitions for hypertext */

#define HY_END  0
#define HY_NODE 1
#define HY_BRAN 2
#define HY_DEVN 4
#define HY_ALL  7
#define HY_TEXT 8
#define HY_BOGUS 16

/* structures for keeping track of coordinate reference */
struct parent {
    struct o *pPointer;      /* parent of reference */
    struct o *pOldPointer;   /* for undo */
    struct parent *pNext;    /* link */
};

struct hyEnt {
    long hyX;                /* coordinate */
    long hyY;
    long hyOldX;             /* previous coord for undo */
    long hyOldY;
    struct o *hyPointer;     /* device referenced */
    struct o *hyOldPointer;  /* previous reference for undo */
    struct parent *hyParent; /* list of parent devices for subckt ref */
    int hyOrient;            /* orientation of branch current */
    int hyOldOrient;         /* previous orient for undo */
    int hyRefType;           /* HY_... NODE, BRAN, DEVN, BOGUS */
    int hyUndo;              /* true if undone */
};

/* hypertext string list */
struct hprlist {
    int hlRefType;           /* HY_... NODE, BRAN, DEVN, TEXT */
    char *hlText;            /* text, if HY_TEXT */
    struct hyEnt *hlEnt;     /* hyEnt struct if not HY_TEXT */
    struct hprlist *hlNext;  /* link */
};


/*
 * Parameters that control SCED.
 */
struct kp {

    /* Symbol desc for current cell */
    struct s *kpCellDesc;
      
    /* Symbol name for current cell */
    char *kpCellName;

    /* Symbol desc for top level cell */
    struct s *kpTopDesc;

    /* Symbol name for top level cell */
    char *kpTopName;

    /* Command selected if any from command menu */
    char *kpCommand;

    /* Linked list of saved views */
    struct kw *kpWindowStack;

    /* current instance array parameters */
    int kpNumX;
    int kpNumY;
    double kpDX;
    double kpDY;

    /* True if subcircuits should be expanded */
    int kpExpandInstances;

    /* Number of RESOLUTION*lambda between grid points. */
    long kpGrid;

    /* grid line pattern */
    int kpGridLineStyle;

    /* If True, display grid */
    int kpGridDisplayed;

    /* If True, put grid below layout geometries */
    int kpGridOnTop;

    /* If True, grid will be shown in large viewport */
    int kpShowGridInLargeViewport;

    /* If False then the SelectQ is never redisplayed (for speed) */
    int kpEnableSelectQRedisplay;

    /* Current colormap index */
    int kpColor;

    /* If true, display subcircuit terminals */
    int kpShowTerminals;

    /* If True, user pointed inside coarse viewport and Command[0] == EOS */
    int kpPointCoarseWindow;

    /* True if current cell has been modified */
    int kpModified;

    /* Debug parameters */
    int kpNumGeometries;

    /* If True, user has just pressed the interrupt key */
    int kpSIGINTERRUPT;

    /* If true, show connection points */
    int kpShowDots;

    /* Which types of object will be selected */
    char kpSelectTypes[8];

    /*
     * PointingThreshold is the minimum value of ViewportWidth/WindowWidth
     * such that it is still comfortable to point with lambda precision.
     */
    int kpPointingThreshold;

    /* Current transform defined in Selection menu */
    int kpRotationAngle;    /* 0, 90, 180, or 270 */
    int kpMX;
    int kpMY;

    /* True if making hard copy */
    int kpDoingHardcopy;

    /* Factor for scaling text labels for hard copy */
    int kpHardcopyTextScale;

    /* Keep track of cursor location */
    int kpLastCursorColumn;

    /* Type of cursor chosen */
    int kpCursorShape;

    /* True if full screen cursor in use */
    int kpFullScreenCursor;

    /* Beep volume */
    int kpPointBeepVolume;

    /* Name of X font used */
    char kpFontName[81];

    /*
     * == COARSEVIEWPORTONLY if coarse window-viewport to be redisplayed
     * == FINEVIEWPORTONLY if fine window-viewport should be redisplayed
     * == SPLITSCREEN if both should be redisplayed
     */
    char kpRedisplayControl;

    /*
     * Current command menu
     *     == BASICMENU denotes basic menu
     *     == AMBIGUITYMENU denotes ambiguity menu
     */
    char kpMenu;
};

/*
 * Selection queue.
 *
 *
 * The select Q is a single-linked list of select Q descriptors.
 * Each desc points to the object descriptor for a selected object. 
 * SelectQHead is the head of the list.
 * SelectQBB is the BB of ALL of the selected objects.
 * Move and delete are easily implemented as operations on the Q.
 */

struct ks {
    struct ks *ksSucc;
    struct o *ksPointer;
};

/* Values of Info parameter used to manage objects in SelectQ */

/* Previous object not in Q */
#define SQ_OLD     0

/* Previous object in Q, selected */
#define SQ_OLDSEL  1

/* Conditionally deleted object */
#define SQ_GONE    2

/* Newly created object, not selected */
#define SQ_NEW     3

/* Newly created object, selected */
#define SQ_NEWSEL  4

/* Partially created object */
#define SQ_INCMPLT 5


/*
 * menus.
 */

struct menu {
    char *mEntry;
    short mActive;
    char mPrefix[6];
};
typedef struct menu MENU;


/* coordinate transformation macros */

#define CoarseLToP(X,Y,XT,YT) \
    XT = .5+(X-View->kvCoarseWindow->kaLeft)*View->kvCoarseRatio; \
    XT += View->kvCoarseViewport->kaLeft; \
    YT = .5+(Y-View->kvCoarseWindow->kaBottom)*View->kvCoarseRatio; \
    YT += View->kvCoarseViewport->kaBottom;

#define FineLToP(X,Y,XT,YT) \
    XT = .5+(X-View->kvFineWindow->kaLeft)*View->kvFineRatio; \
    XT += View->kvFineViewport->kaLeft; \
    YT = .5+(Y-View->kvFineWindow->kaBottom)*View->kvFineRatio; \
    YT += View->kvFineViewport->kaBottom;

#define ClipVP(Viewport,X,Y) \
    if(X < Viewport->kaLeft) X = Viewport->kaLeft; \
    else if(X > Viewport->kaRight) X = Viewport->kaRight; \
    if(Y < Viewport->kaBottom) Y = Viewport->kaBottom; \
    else if(Y > Viewport->kaTop) Y = Viewport->kaTop;

#include "scedext.h"
