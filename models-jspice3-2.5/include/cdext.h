/***************************************************************************
SCED - Schematic Capture Editor
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1981 Giles C. Billingsley  (parts of KIC layout editor)
         1992 Stephen R. Whiteley
****************************************************************************/

/* debugging... */

/* print trace from actions.c
#define TRACEPARSER
*/

/* print trace from parser.c
#define TRACE
*/

/* debugging from cd.c
#define DEBUGREFLECT
#define DEBUGGEN
#define DEBUG CD_UNMARK
*/

/* for CDIntersect()
#define FLOAT
*/

/* imports */
extern char *ctime();
extern char *tmalloc();
extern void txfree();
extern char *strcpy();
extern double sin(), cos();

#ifdef __STDC__
extern void CDLabelBB(struct o*,long*,long*,long*,long*); 
extern FILE *OpenDevice(char*);
extern void UpdateProperties(struct o*);
#else
extern void CDLabelBB(); 
extern FILE *OpenDevice();
extern void UpdateProperties();
#endif

/* actions.c */
#ifdef __STDC__
extern void AEnd(void);
extern int  ABeginSymbol(int,long,long);
extern void AEndSymbol(void);
extern void ADeleteSymbol(int);
extern int  AEndCall(void);
extern int  AT(int,long,long);
extern int  ABeginCall(int);
extern int  APolygon(struct p*);
extern int  AWire(long,struct p*);
extern int  ABox(long,long,long,long,int,int);
extern int  ARoundFlash(long,long,long);
extern int  ALayer(int,char*);
extern int  AUserExtension(int,char*);
extern void AComment(char*);
#else
extern void AEnd();
extern int  ABeginSymbol();
extern void AEndSymbol();
extern void ADeleteSymbol();
extern int  AEndCall();
extern int  AT();
extern int  ABeginCall();
extern int  APolygon();
extern int  AWire();
extern int  ABox();
extern int  ARoundFlash();
extern int  ALayer();
extern int  AUserExtension();
extern void AComment();
#endif

/* cd.c */
extern struct bu *CDSymbolTable[CDNUMLAYERS+1];
extern struct d  CDDesc;
extern struct l  CDLayer[CDNUMLAYERS+1];
extern char *CDStatusString;
extern int  CDStatusInt;
#ifdef __STDC__
extern int  CDInit(void);
extern int  CDPath(char*);
extern void CDSetLayer(int,int,char*);
extern void CDDebug(int);
extern int  CDOpen(char*,struct s**,int);
extern void CDSymbol(char*,struct s**);
extern int  CDClose(struct s*);
extern int  CDReflect(struct s*);
extern int  CDPatchInstances(struct s*,char*);
extern int  CDMakeBox(struct s*,int,long,long,long,long,struct o**);
extern int  CDMakeLabel(struct s*,int,char*,long,long,int,struct o**);
extern int  CDMakePolygon(struct s*,int,struct p*,struct o**);
extern int  CDMakeWire(struct s*,int,long,struct p*,struct o**);
extern int  CDMakeRoundFlash(struct s*,int,long,long,long,struct o**);
extern int  CDBeginMakeCall(struct s*,char*,int,long,int,long,struct o**);
extern int  CDT(struct o*,int,long,long);
extern int  CDEndMakeCall(struct s*,struct o*);
extern void CDCheckPath(struct p*);
extern int  CDInsertObject(struct s*,struct o*);
extern void CDDeleteObjectDesc(struct s*,struct o*);
extern void CDCall(struct o*,char**,int*,long*,int*,long*);
extern void CDBox(struct o*,int*,long*,long*,long*,long*);
extern void CDLabel(struct o*,int*,char**,long*,long*,char*);
extern void CDPolygon(struct o*,int*,struct p**);
extern void CDWire(struct o*,int*,long*,struct p**);
extern void CDRoundFlash(struct o*,int*,long*,long*,long*);
extern void CDInfo(struct s*,struct o*,int*);
extern void CDSetInfo(struct s*,struct o*,int);
extern void CDType(struct o*,char*);
extern int  CDBB(struct s*,struct o*,long*,long*,long*,long*);
extern void CDIntersect(long,long,long,long,long*,long*,long*,long*);
extern int  CDInitGen(struct s*,int,long,long,long,long,struct g**);
extern void CDGen(struct s*,struct g*,struct o**);
extern void CDInitTGen(struct o*,struct t**);
extern void CDTGen(struct t**,char*,long*,long*);
extern int  CDUpdate(struct s*,char*);
extern int  CDGenCIF(FILE*,struct s*,int*,long,long,int);
extern int  CDTo(char*,char*,long,long,int);
extern int  CDFrom(char*,char*,long,long,int*,int,int);
extern int  CDParseCIF(char*,char*,int);
extern int  CDUnmark(struct s*);
extern int  CDError(int);
#else
extern int  CDInit();
extern int  CDPath();
extern void CDSetLayer();
extern void CDDebug();
extern int  CDOpen();
extern void CDSymbol();
extern int  CDClose();
extern int  CDReflect();
extern int  CDPatchInstances();
extern int  CDMakeBox();
extern int  CDMakeLabel();
extern int  CDMakePolygon();
extern int  CDMakeWire();
extern int  CDMakeRoundFlash();
extern int  CDBeginMakeCall();
extern int  CDT();
extern int  CDEndMakeCall();
extern void CDCheckPath();
extern int  CDInsertObject();
extern void CDDeleteObjectDesc();
extern void CDCall();
extern void CDBox();
extern void CDLabel();
extern void CDPolygon();
extern void CDWire();
extern void CDRoundFlash();
extern void CDInfo();
extern void CDSetInfo();
extern void CDType();
extern int  CDBB();
extern void CDIntersect();
extern int  CDInitGen();
extern void CDGen();
extern void CDInitTGen();
extern void CDTGen();
extern int  CDUpdate();
extern int  CDGenCIF();
extern int  CDTo();
extern int  CDFrom();
extern int  CDParseCIF();
extern int  CDUnmark();
extern int  CDError();
#endif

/* cdprpty.c */
#ifdef __STDC__
extern void CDProperty(struct s*,struct o*,struct prpty**);
extern int  CDCopyProperty(struct s*,struct o*,struct prpty*);
extern int  CDAddProperty(struct s*,struct o*,int,char*);
extern int  CDRemoveProperty(struct s*,struct o*,int);
extern void CDPrptyListFree(struct prpty*);
extern void CDPrptyListPrint(FILE*,struct prpty*);
#else
extern void CDProperty();
extern int  CDCopyProperty();
extern int  CDAddProperty();
extern int  CDRemoveProperty();
extern void CDPrptyListFree();
extern void CDPrptyListPrint();
#endif

/* gencif.c */
#ifdef __STDC__
extern void GenEnd(FILE*);
extern void GenBeginSymbol(FILE*,int,long,long);
extern void GenEndSymbol(FILE*);
extern void GenBeginCall(FILE*,int);
extern void GenEndCall(FILE*);
extern void GenTranslation(FILE*,long,long);
extern void GenRotation(FILE*,long,long);
extern void GenMirrorX(FILE*);
extern void GenMirrorY(FILE*);
extern void GenPolygon(FILE*,struct p*);
extern void GenPolygonOffset(FILE*,struct p*,long,long);
extern void GenWire(FILE*,long,struct p*);
extern void GenWireOffset(FILE*,long,struct p*,long,long);
extern void GenBox(FILE*,long,long,long,long,int,int);
extern void GenLayer(FILE*,int,char*);
extern void GenUserExtension(FILE*,int,char*);
extern void GenComment(FILE*,char*);
#else
extern void GenEnd();
extern void GenBeginSymbol();
extern void GenEndSymbol();
extern void GenBeginCall();
extern void GenEndCall();
extern void GenTranslation();
extern void GenRotation();
extern void GenMirrorX();
extern void GenMirrorY();
extern void GenPolygon();
extern void GenPolygonOffset();
extern void GenWire();
extern void GenWireOffset();
extern void GenBox();
extern void GenLayer();
extern void GenUserExtension();
extern void GenComment();
#endif

/* parser.c */
#ifdef __STDC__
extern void PCIF(char*,char**,int*);
extern int PCharacter(int,int);
#else
extern void PCIF();
extern int PCharacter();
#endif

/* paths.c */
#ifdef __STDC__
extern int  PSetPath(char*);
extern int  PAppendPath(char*);
extern char *PGetPath(void);
extern FILE *POpen(char*,char*,char**);
#else
extern int  PSetPath();
extern int  PAppendPath();
extern char *PGetPath();
extern FILE *POpen();
#endif

/* xforms.c */
#ifdef __STDC__
extern void TInit(void);
extern int  TEmpty(void);
extern int  TFull(void);
extern void TPush(void);
extern void TPop(void);
extern void TCurrent(long*);
extern void TLoadCurrent(long*);
extern void TTranslate(long,long);
extern void TMY(void);
extern void TMX(void);
extern void TRotate(long,long);
extern void TIdentity(void);
extern void TPoint(long*,long*);
extern void TPremultiply(void);
extern void TInverse(void);
extern void TInversePoint(long*,long*);
extern void TStore(void);
extern void TLoad(void);
extern void TLoadInverse(void);
#else
extern void TInit();
extern int  TEmpty();
extern int  TFull();
extern void TPush();
extern void TPop();
extern void TCurrent();
extern void TLoadCurrent();
extern void TTranslate();
extern void TMY();
extern void TMX();
extern void TRotate();
extern void TIdentity();
extern void TPoint();
extern void TPremultiply();
extern void TInverse();
extern void TInversePoint();
extern void TStore();
extern void TLoad();
extern void TLoadInverse();
#endif
