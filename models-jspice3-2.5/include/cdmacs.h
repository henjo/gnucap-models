/***************************************************************************
SCED - Schematic Capture Editor
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1981 Giles C. Billingsley  (parts of KIC layout editor)
         1992 Stephen R. Whiteley
****************************************************************************/
 
#define EOS '\0'
#define elif else if
#define loop for(;;)
#define True 1
#define False 0
#define And &&
#define Or ||
#define Not !
#define Max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define Min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))
#define Abs(Dragon) ((Dragon) >= 0 ? (Dragon) : (-(Dragon)))
#define SwapInts(Dragon,Eagle) {long ShakingCrane; ShakingCrane = Dragon; Dragon = Eagle; Eagle = ShakingCrane;}
#define alloc(type) (struct type *) tmalloc(sizeof(struct type))
#define tfree(x) (txfree((char*)x), x = 0)
