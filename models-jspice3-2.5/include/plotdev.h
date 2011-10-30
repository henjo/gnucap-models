/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1987 Jeffrey M. Hsu
         1992 Stephen R. Whiteley
****************************************************************************/

/*
 * The display device structure.
 */

#include "ftegraph.h"

struct polygn {
    int nvertices;
    long *xy;
};
typedef struct polygn POLYGON;

#ifdef __STDC__

typedef struct {
    char *name;
    bool windows;       /* true if multiple windows, asynchronous redraw,
                         * such as X.  False if single window, synchronous
                         * redraw, such as MFB */
    bool hardcopy;      /* true if hard copy device */
    int minx, miny;
    int width, height;            /* in screen coordinate system */
    int numlinestyles, numcolors; /* number supported */
    int (*Init)(void);
    int (*NewViewport)(GRAPH*);
    int (*Close)(void);
    int (*Halt)(void);
    int (*Clear)(void);
    int (*Title)(char*,char*);
    int (*Pixel)(int,int);
    int (*Line)(int,int,int,int);
    int (*Box)(int,int,int,int);
    int (*Arc)(int,int,int,double,double);
    int (*Polygon)(POLYGON*);
    int (*Text)(char*,int,int);
    int (*SetGhost)(void(*)(int,int,int,int),int,int);
    int (*DefineColor)(int,double,double,double);
    int (*DefineLinestyle)(int,int);
    int (*SetLinestyle)(int);
    int (*SetColor)(int);
    int (*Update)(void);
/*  int (*NDCtoScreen)(); */
    int (*Track)(void);
    int (*MakeMenu)(void);
    int (*MakeDialog)(void);
    int (*Input)(REQUEST*,RESPONSE*);
    int (*DatatoScreen)(GRAPH*,double,double,int*,int*);
} DISPDEVICE;

#else

typedef struct {
    char *name;
    bool windows;       /* true if multiple windows, asynchronous redraw,
                         * such as X.  False if single window, synchronous
                         * redraw, such as MFB */
    bool hardcopy;      /* true if hard copy device */
    int minx, miny;
    int width, height;            /* in screen coordinate system */
    int numlinestyles, numcolors; /* number supported */
    int (*Init)();
    int (*NewViewport)();
    int (*Close)();
    int (*Halt)();
    int (*Clear)();
    int (*Title)();
    int (*Pixel)();
    int (*Line)();
    int (*Box)();
    int (*Arc)();
    int (*Polygon)();
    int (*Text)();
    int (*SetGhost)();
    int (*DefineColor)();
    int (*DefineLinestyle)();
    int (*SetLinestyle)();
    int (*SetColor)();
    int (*Update)();
/*  int (*NDCtoScreen)(); */
    int (*Track)();
    int (*MakeMenu)();
    int (*MakeDialog)();
    int (*Input)();
    int (*DatatoScreen)();
} DISPDEVICE;

#endif

/* display.c */
extern DISPDEVICE device[];
extern DISPDEVICE *dispdev;
#ifdef __STDC__
extern DISPDEVICE *FindDev(char*);
extern int  DevXterm(char*);
extern void DevGrHalt(void);
extern void SaveText(GRAPH*,char*,int,int);
extern void DevInit(void);
extern int  DevNewViewport(GRAPH*);
extern void DevClose(void);
extern void DevHalt(void);
extern void DevClear(void);
extern void DevTitle(char*,char*);
extern void DevPixel(int,int);
extern void DevLine(int,int,int,int);
extern void DevBox(int,int,int,int);
extern void DevArc(int,int,int,double,double);
extern void DevPolygon(POLYGON*);
extern void DevText(char*,int,int);
extern void DevSetGhost(void(*)(int,int,int,int),int,int);
extern void DevDefineColor(int,double,double,double);
extern void DevDefineLinestyle(int,int);
extern void DevSetLinestyle(int);
extern void DevSetColor(int);
extern void DevUpdate(void);
extern void DevDatatoScreen(GRAPH*,double,double,int*,int*);
extern void DevInput(REQUEST*,RESPONSE*);
extern int  DevSwitch(char*);
extern int  DevGetchar(FILE*);
#else
extern DISPDEVICE *FindDev();
extern int  DevXterm();
extern void DevGrHalt();
extern void SaveText();
extern void DevInit();
extern int  DevNewViewport();
extern void DevClose();
extern void DevHalt();
extern void DevClear();
extern void DevTitle();
extern void DevPixel();
extern void DevLine();
extern void DevBox();
extern void DevArc();
extern void DevPolygon();
extern void DevText();
extern void DevSetGhost();
extern void DevDefineColor();
extern void DevDefineLinestyle();
extern void DevSetLinestyle();
extern void DevSetColor();
extern void DevUpdate();
extern void DevDatatoScreen();
extern void DevInput();
extern int  DevSwitch();
extern int  DevGetchar();
#endif

#ifdef HAVE_X11
/* x11.c */
#ifdef __STDC__
extern int X11_Init(void);
extern int X11_NewViewport(GRAPH*);
extern int X11_Close(void);
extern int X11_Halt(void);
extern int X11_Title(char*,char*);
extern int X11_Pixel(int,int);
extern int X11_Line(int,int,int,int);
extern int X11_Box(int,int,int,int);
extern int X11_Arc(int,int,int,double,double);
extern int X11_Polygon(POLYGON*);
extern int X11_Text(char*,int,int);
extern int X11_SetGhost(void(*)(int,int,int,int),int,int);
extern int X11_DefineColor(int,double,double,double);
extern int X11_DefineLinestyle(int,int);
extern int X11_SetLinestyle(int);
extern int X11_SetColor(int);
extern int X11_Update(void);
extern int X11_Clear(void);
extern int X11_Input(REQUEST*,RESPONSE*);
#else
extern int X11_Init();
extern int X11_NewViewport();
extern int X11_Close();
extern int X11_Halt();
extern int X11_Title();
extern int X11_Pixel();
extern int X11_Line();
extern int X11_Box();
extern int X11_Arc();
extern int X11_Polygon();
extern int X11_Text();
extern int X11_SetGhost();
extern int X11_DefineColor();
extern int X11_DefineLinestyle();
extern int X11_SetLinestyle();
extern int X11_SetColor();
extern int X11_Update();
extern int X11_Clear();
extern int X11_Input();
#endif
#endif

#ifdef HAVE_MFB
/* mfb.c */
#ifdef __STDC__
extern int Mfb_Init(void);
extern int Mfb_NewViewport(GRAPH*);
extern int Mfb_Close(void);
extern int Mfb_Halt(void);
extern int Mfb_Pixel(int,int);
extern int Mfb_Line(int,int,int,int);
extern int Mfb_Box(int,int,int,int);
extern int Mfb_Arc(int,int,int,double,double);
extern int Mfb_Polygon(POLYGON*);
extern int Mfb_Text(char*,int,int);
extern int Mfb_SetGhost(void(*)(int,int,int,int),int,int);
extern int Mfb_DefineColor(int,double,double,double);
extern int Mfb_DefineLinestyle(int,int);
extern int Mfb_SetLinestyle(int);
extern int Mfb_SetColor(int);
extern int Mfb_Update(void);
extern int Mfb_Clear(void);
extern int Mfb_MakeMenu(void);
extern int Mfb_MakeDialog(void);
extern int Mfb_Input(REQUEST*,RESPONSE*);
#else
extern int Mfb_Init();
extern int Mfb_NewViewport();
extern int Mfb_Close();
extern int Mfb_Halt();
extern int Mfb_Pixel();
extern int Mfb_Line();
extern int Mfb_Box();
extern int Mfb_Arc();
extern int Mfb_Polygon();
extern int Mfb_Text();
extern int Mfb_SetGhost();
extern int Mfb_DefineColor();
extern int Mfb_DefineLinestyle();
extern int Mfb_SetLinestyle();
extern int Mfb_SetColor();
extern int Mfb_Update();
extern int Mfb_Clear();
extern int Mfb_MakeMenu();
extern int Mfb_MakeDialog();
extern int Mfb_Input();
#endif
#endif

/* plot5.c */
#ifdef __STDC__
extern int Plt5_Init(void);
extern int Plt5_NewViewport(GRAPH*);
extern int Plt5_Close(void);
extern int Plt5_Halt(void);
extern int Plt5_Pixel(int,int);
extern int Plt5_Line(int,int,int,int);
extern int Plt5_Box(int,int,int,int);
extern int Plt5_Arc(int,int,int,double,double);
extern int Plt5_Polygon(POLYGON*);
extern int Plt5_Text(char*,int,int);
extern int Plt5_SetLinestyle(int);
extern int Plt5_SetColor(int);
extern int Plt5_Update(void);
#else
extern int Plt5_Init();
extern int Plt5_NewViewport();
extern int Plt5_Close();
extern int Plt5_Halt();
extern int Plt5_Pixel();
extern int Plt5_Line();
extern int Plt5_Box();
extern int Plt5_Arc();
extern int Plt5_Polygon();
extern int Plt5_Text();
extern int Plt5_SetLinestyle();
extern int Plt5_SetColor();
extern int Plt5_Update();
#endif

/* postsc.c */
#ifdef __STDC__
extern int PS_Init(void);
extern int PS_NewViewport(GRAPH*);
extern int PS_Close(void);
extern int PS_Halt(void);
extern int PS_Pixel(int,int);
extern int PS_Line(int,int,int,int);
extern int PS_Box(int,int,int,int);
extern int PS_Arc(int,int,int,double,double);
extern int PS_Polygon(POLYGON*);
extern int PS_Text(char*,int,int);
extern int PS_SetLinestyle(int);
extern int PS_SetColor(int);
extern int PS_Update(void);
extern int PS_ScaledText(char*,int,int,int);
#else
extern int PS_Init();
extern int PS_NewViewport();
extern int PS_Close();
extern int PS_Halt();
extern int PS_Pixel();
extern int PS_Line();
extern int PS_Box();
extern int PS_Arc();
extern int PS_Polygon();
extern int PS_Text();
extern int PS_SetLinestyle();
extern int PS_SetColor();
extern int PS_Update();
extern int PS_ScaledText();
#endif

/* hplaser.c */
#ifdef __STDC__
extern int HP_Init(void);
extern int HP_NewViewport(GRAPH*);
extern int HP_Close(void);
extern int HP_Halt(void);
extern int HP_Pixel(int,int);
extern int HP_Line(int,int,int,int);
extern int HP_Box(int,int,int,int);
extern int HP_Arc(int,int,int,double,double);
extern int HP_Polygon(POLYGON*);
extern int HP_Text(char*,int,int);
extern int HP_DefineLinestyle(int,int);
extern int HP_SetLinestyle(int);
#else
extern int HP_Init();
extern int HP_NewViewport();
extern int HP_Close();
extern int HP_Halt();
extern int HP_Pixel();
extern int HP_Line();
extern int HP_Box();
extern int HP_Arc();
extern int HP_Polygon();
extern int HP_Text();
extern int HP_DefineLinestyle();
extern int HP_SetLinestyle();
#endif

/* wp.c */
#ifdef __STDC__
extern int WP_Init(void);
extern int WP_NewViewport(GRAPH*);
extern int WP_Close(void);
extern int WP_Halt(void);
extern int WP_Pixel(int,int);
extern int WP_Line(int,int,int,int);
extern int WP_Box(int,int,int,int);
extern int WP_Arc(int,int,int,double,double);
extern int WP_Polygon(POLYGON*);
extern int WP_Text(char*,int,int);
extern int WP_DefineLinestyle(int,int);
extern int WP_SetLinestyle(int);
#else
extern int WP_Init();
extern int WP_NewViewport();
extern int WP_Close();
extern int WP_Halt();
extern int WP_Pixel();
extern int WP_Line();
extern int WP_Box();
extern int WP_Arc();
extern int WP_Polygon();
extern int WP_Text();
extern int WP_DefineLinestyle();
extern int WP_SetLinestyle();
#endif
