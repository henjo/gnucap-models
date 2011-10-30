/***************************************************************************
SCED - Schematic Capture Editor
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1981 Giles C. Billingsley  (parts of KIC layout editor)
         1992 Stephen R. Whiteley
****************************************************************************/

#include "plotdev.h"

#define ERASE                  'e'
#define DISPLAY                'd'
#define FILL                   'f'
#define OUTLINE                'o'
#define ROW_COLUMN             'r'
#define PIXEL_COORDINATE       'p'

#define MARK_CROSS 0
#define MARK_ARROW 1
#define MARK_GP1 2
#define MARK_GP2 3
#define MARK_GP3 4
#define MARK_GP4 5
#define MARK_GP5 6
#define MARK_GP6 7
#define MARK_GP7 8
#define MARK_GP8 9
#define MARK_NONE 0
#define MARK_DN 1
#define MARK_RT 2
#define MARK_UP 3
#define MARK_LT 4

#ifdef MSDOS
#define ARROW_RT 333
#define ARROW_LT 331
#define ARROW_UP 328
#define ARROW_DN 336
#else
/* X assumed here */
#define ARROW_RT 0xff53
#define ARROW_LT 0xff51
#define ARROW_UP 0xff52
#define ARROW_DN 0xff54
#endif

/* max intensity value used for color specs */
#define FBMAXINTENSITY 255

/* from cp */
extern FILE* cp_in;
extern FILE* cp_out;

extern GRAPH *currentgraph;

#define gi_fntwidth    (currentgraph->fontwidth)
#define gi_fntheight   (currentgraph->fontheight)
#define gi_maxx        (currentgraph->absolute.width-1)
#define gi_maxy        (currentgraph->absolute.height-1)
#define gi_numtextrows (currentgraph->absolute.height/currentgraph->fontheight)
#define gi_numtextcols (currentgraph->absolute.width/currentgraph->fontwidth)

#define Matching(string) !strcmp(Parameters.kpCommand,string)

#define FBEraseBox(L,B,R,T) { \
     DevSetColor(BackgroundColor); \
     DevBox((int)(L),(int)(B),(int)(R),(int)(T)); \
     } \

#define FBEmptyBox(Pixel,DisplayOrErase,StyleId,L,B,R,T) { \
     DevSetColor(Pixel); \
     DevLine((int)(L),(int)(B),(int)(R),(int)(B)); \
     DevLine((int)(R),(int)(B),(int)(R),(int)(T)); \
     DevLine((int)(R),(int)(T),(int)(L),(int)(T)); \
     DevLine((int)(L),(int)(T),(int)(L),(int)(B)); \
     } \

#define FBBox(Pixel,DisplayOrErase,Type,L,B,R,T) { \
     DevSetColor(Pixel); \
     if(Type == OUTLINE){ \
      DevLine((int)(L),(int)(B),(int)(R),(int)(B)); \
      DevLine((int)(R),(int)(B),(int)(R),(int)(T)); \
      DevLine((int)(R),(int)(T),(int)(L),(int)(T)); \
      DevLine((int)(L),(int)(T),(int)(L),(int)(B)); \
     } \
     else { \
      DevBox((int)(L),(int)(B),(int)(R),(int)(T)); \
     } \
    }

#ifdef __STDC__
extern void RedisplaySCED(GRAPH*);
extern void ResizeSCED(GRAPH*);
#else
extern void RedisplaySCED();
extern void ResizeSCED();
#endif
