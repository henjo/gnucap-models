/***************************************************************************
SCED - Schematic Capture Editor
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1981 Giles C. Billingsley  (parts of KIC layout editor)
         1992 Stephen R. Whiteley
****************************************************************************/
 
/*
 * Fast CIF parser's data structures.
 * 
 */

/*
 * 24*80+1=1921 characters of string.
 */
#define PSTRINGSIZE 1921

/*
 * Status of parse when parser returns.
 */
extern char PStatus[80*3+1];

/*
 * CIF file desc.
 */
extern FILE *PCIFFileDesc;

/*
 * string for input line buffering
 */
extern char PString[PSTRINGSIZE];

/*
 * Global token variables.
 */
extern int  PChar;
extern long PInt;
extern int  PReturned;

#ifdef __STDC__
extern void PCIF(char*,char**,int*);
#else
extern void PCIF();
#endif

/*
 * Values routines return.
 */
#define PSUCCEEDED         1    /* successful return */
#define PFAILED            2    /* parser failed */
#define PNOTAPPLICABLE     3    /* parser failed due to syntax */

/*
 * Arguments to handle an EOF in PCharacter and PInteger
 */
#define PFAILONEOF         1
#define PDONTFAILONEOF     2

/*
 * Arguments to specify characters to be ignored by PWhiteSpace
 */
#define PSTRIPWHITESPACE1  1    /* strip blanks, tabs, commas, or    */
                                /* new lines.                        */
#define PSTRIPWHITESPACE2  2    /* strip everything but upper case,  */
                                /* hyphens, digits, parens, and ;'s. */
#define PSTRIPWHITESPACE3  3    /* strip everything but digits,      */
                                /* hyphens, parens, and ;'s.         */
#define PLEAVEWHITESPACE   4
