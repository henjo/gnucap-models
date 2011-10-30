/*************************************************************************
 MFB graphics and miscellaneous library
 Copyright (c) Stephen R. Whiteley 1992
 Author: Stephen R. Whiteley
 *************************************************************************/

/* Private include file for graphics and miscellaneous routines */

#include <math.h>

/* define for Paradise card, otherwise support Tseng Labs */
/* #define PARADISE */

/* where the mfb.rc file is located */
extern char *MFBRCD;

typedef char * vidmptr;

#if __GNUC__
#define inp inportb
#define outp outportb
#define outpw outportw
#endif

/* This takes the char read from the adapter.  Some compilers
 * optimize out a local variable.
 */
extern char mfb_trash;

extern unsigned short mfb_8X14font[];
extern char dp_pointermap[];
extern unsigned short mfb_chartab[];

/* structure used to store stuff */
/* Don't change this unless you want to fix the assembly code too. */
struct mfbpc {

    vidmptr base;                      /* video memory base address    */
    int bytpline;                      /* bytes per line               */
    int video_mode;                    /* video VGA mode               */
    int mfbMODE;                       /* display mode control         */
    int scan_height;                   /* scan lines per char in text  */

    int curx;                          /* current x coordinate         */
    int cury;                          /* current y coordinate         */
    int curcolor;                      /* current color                */
    int curstyle;                      /* current line style           */
    int curfillpatt;                   /* current fillpattern          */

    int cursor_color;                  /* current cursor color         */
    int cursx;                         /* cursor x coordinate          */
    int cursy;                         /* cursor y coordinate          */
    int xsize;                         /* screen width in pixels       */
    int ysize;                         /* screen height in pixels      */

    int alumode;                       /* current ALU mode             */
    int curcolormap;                   /* current colormap (1 of 16)   */
    int curstep;                       /* cursor movement, arrow key   */
    int curleap;                       /* cursor movement, alt arrow   */
    int pointertype;                   /* graphic pointing device type */

    int pointerdelay;                  /* pointer debounce time msec   */
    unsigned short *chartab;           /* pointer to current font      */
    int charheight;                    /* character height and width   */
    int charwidth;
    int functkey_yband;                /* Y value retrn from MFBPoint  */
                                       /* when function keys pressed   */
    unsigned char
    linestyles[NSTYLES];               /* linestyle storage            */
    int stipples[10][8];               /* stipple pattern storage      */
    unsigned char colormap[256][3];    /* vga colormap                 */
    /* ghost mode - XOR some drawing, attach to cursor */
    int refx, refy;                    /* anchor for ghost mode        */
#ifdef __STDC__
    void (*drawghost)(int,int,int,int);/* function to draw ghost image */
    /* args are (new x, new y, ref x, ref y) */
#else
    void (*drawghost)();
#endif
};
extern struct mfbpc pc;

struct vga_cursor {
    unsigned char map[64];
    unsigned char pbs[256];
};
extern struct vga_cursor vgacur;        /* marker shape & bkgrnd storage*/

/* values for pc.pointertype  */
#define KBRD    1    /* keyboard arrow keys */
#define MSMOUSE 2    /* Microsoft mouse */
#define MMTAB   4    /* Seiko DT-3503 tablet in Summagraphics MM mode */


#ifdef __STDC__

/* Microsoft mouse interface routines */
extern void ms_start(void);             /* start mouse interface           */
extern void ms_end(void);               /* quit mouse                      */
extern int  ms_hreset(void);            /* hard reset                      */
extern void ms_pointer_on(void);        /* display pointer                 */
extern void ms_pointer_off(void);       /* hide pointer                    */
extern void ms_status(int*,int*,int*);  /* return coordinates, buttons     */
extern void ms_point(int*,int*,int*,int*); /* point loop                   */

/* digitizing pad driver routines */
extern void dp_start(int,int);          /* enable interface                */
extern void dp_end(void);               /* disable interface               */
extern void dp_pointer_on(void);        /* turn cursor on                  */
extern void dp_pointer_off(void);       /* turn cursor off                 */
extern void dp_status(int*,int*,int*);  /* return coords, buttons          */
extern void dp_point(int*,int*,int*,int*); /* point loop                   */

/* Hard copy support */
extern Bool makehpplot(void);           /* screen dump driver              */
extern Bool rasterize(FILE*);           /* rasterize for laserjet          */
extern void PrinterErrmsg(char*,char*); /* graphics print message          */
extern Bool PrinterPortStatus(int);     /* check port status               */
extern void SetPrinterDevice(char*);    /* store printer port              */

/* VGA supporting routines */
extern void vga_get_color(int,int*,int*,int*);  /* get DAC register        */
extern void vga_set_color(int,int,int,int);     /* set DAC register        */
extern void vga_get_cblock(char (*)[3],int,int);/* get DAC register block  */
extern void vga_set_cblock(char (*)[3],int,int);/* set DAC register block  */
extern void vga_get_cpblock(char*);      /* get palette register block     */
extern void vga_set_cpblock(char*);      /* set palette register block     */

/* routines in mfbmark.c used by dpinterf.c */
extern void mode_1_mark(int,int);
extern void mode_2_mark(int,int);
extern void mode_1_clear(int,int);
extern void mode_2_clear(int,int);

#else

/* Microsoft mouse interface routines */
extern void ms_start();             /* start mouse interface           */
extern void ms_end();               /* quit mouse                      */
extern int  ms_hreset();            /* hard reset                      */
extern void ms_pointer_on();        /* display pointer                 */
extern void ms_pointer_off();       /* hide pointer                    */
extern void ms_status();            /* return coordinates, buttons     */
extern void ms_point();             /* point loop                      */

/* digitizing pad driver routines */
extern void dp_start();             /* enable interface                */
extern void dp_end();               /* disable interface               */
extern void dp_pointer_on();        /* turn cursor on                  */
extern void dp_pointer_off();       /* turn cursor off                 */
extern void dp_status();            /* return coords, buttons          */
extern void dp_point();             /* point loop                      */

/* Hard copy support */
extern Bool makehpplot();           /* screen dump driver              */
extern Bool rasterize();            /* rasterize for laserjet          */
extern void PrinterErrmsg();        /* graphics print message          */
extern Bool PrinterPortStatus();    /* check port status               */
extern void SetPrinterDevice();     /* store printer port              */

/* VGA supporting routines */
extern void vga_get_color();        /* get DAC register                */
extern void vga_set_color();        /* set DAC register                */
extern void vga_get_cblock();       /* get DAC register block          */
extern void vga_set_cblock();       /* set DAC register block          */
extern void vga_get_cpblock();      /* get palette register block      */
extern void vga_set_cpblock();      /* set palette register block      */

/* routines in mfbmark.c used by dpinterf.c */
extern void mode_1_mark();
extern void mode_2_mark();
extern void mode_1_clear();
extern void mode_2_clear();

#endif
