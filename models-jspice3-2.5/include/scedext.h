/***************************************************************************
SCED - Schematic Capture Editor
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1981 Giles C. Billingsley  (parts of KIC layout editor)
         1992 Stephen R. Whiteley
****************************************************************************/

extern char *tmalloc();
extern int  cieq();
extern int  ciprefix();
extern void strtolower();


/* bascmd.c */
#ifdef __STDC__
extern void Basic(int*);
extern void Undo(void);
extern void Color(int*);
extern void Bgrnd(int*);
extern void Hlite(int*);
extern void RGB(void);
extern void AlterColor(int,int);
extern void SetGrid(int*);
extern void MenuParams(int*);
extern void MenuPrompt(int*);
extern void MenuMore(int*);
extern void ShowRGB(void);
extern void DoAlloc(void);
extern void Edit(int,int,int);
extern void TitleWindow(void);
extern void Save(void);
extern void WriteCell(void);
extern int  FixCellName(char*);
extern void Rdraw(void);
extern void ShowFull(void);
extern void Expand(void);
#else
extern void Basic();
extern void Undo();
extern void Color();
extern void Bgrnd();
extern void Hlite();
extern void RGB();
extern void AlterColor();
extern void SetGrid();
extern void MenuParams();
extern void MenuPrompt();
extern void MenuMore();
extern int  GetColor();
extern void ShowRGB();
extern void DoAlloc();
extern void Edit();
extern void TitleWindow();
extern void Save();
extern void WriteCell();
extern int  FixCellName();
extern void Rdraw();
extern void ShowFull();
extern void Expand();
#endif

/* choose.c */
struct ks *SelectQHead;
struct ka SelectQBB;
#ifdef __STDC__
extern void MX(void);
extern void MY(void);
extern void Rotat0(void);
extern void Rotat90(void);
extern void Rotat180(void);
extern void Rotat270(void);
extern void Sel(int*);
extern void Area(int*);
extern void Desel(void);
extern void Selection(struct ka*);
extern int  AreTypesInQ(char*);
extern void SelectTypes(char*);
extern void SQInit(void);
extern void SQClear(void);
extern void SQInsert(struct o*);
extern void SQDelete(struct o*);
extern void SQComputeBB(void);
extern void SQRestore(int);
extern void SQDesel(char*);
extern void SQShow(void);
extern long *InPath(int,struct p*,long,long);
#else
extern void MX();
extern void MY();
extern void Rotat0();
extern void Rotat90();
extern void Rotat180();
extern void Rotat270();
extern void Sel();
extern void Area();
extern void Desel();
extern void Selection();
extern int  AreTypesInQ();
extern void SelectTypes();
extern void SQInit();
extern void SQClear();
extern void SQInsert();
extern void SQDelete();
extern void SQComputeBB();
extern void SQRestore();
extern void SQDesel();
extern void SQShow();
extern long *InPath();
#endif

/* copy.c */
#ifdef __STDC__
extern void Copy(int*);
extern void Move(int*);
extern void ShowGhost(long,long,long,long);
extern void CopyPathWithXForm(struct p**);
extern void ShowTransformed(struct s*,struct ka*,long*);
extern void SetNewTransform(long,long,long,long);
extern void AddResultingTransform(struct o*,long*);
#else
extern void Copy();
extern void Move();
extern void ShowGhost();
extern void CopyPathWithXForm();
extern void ShowTransformed();
extern void SetNewTransform();
extern void AddResultingTransform();
#endif

/* cutwire.c */
#ifdef __STDC__
extern void Break(int*);
extern struct p *CopyPath(struct p*);
#else
extern void Break();
extern struct p *CopyPath();
#endif

/* dir.h */
#ifdef __STDC__
extern void Dir(void);
extern char *CopyString(char*);
#else
extern void Dir();
extern char *CopyString();
#endif

/* display.c */
#ifdef __STDC__
extern void Pan(int*);
extern void Zoom(int*);
extern void Windo(int*);
extern void LastView(void);
extern void SaveLastView(void);
extern void SaveViewOnStack(void);
extern void Redisplay(struct ka*);
extern void RedisplayAfterInterrupt(void);
extern int  TCheck(void);
extern void SetTransform(struct o*);
#else
extern void Pan();
extern void Zoom();
extern void Windo();
extern void LastView();
extern void SaveLastView();
extern void SaveViewOnStack();
extern void Redisplay();
extern void RedisplayAfterInterrupt();
extern int  TCheck();
extern void SetTransform();
#endif

/* grafintr.c */
extern int IplotStatusChanged;
#ifdef __STDC__
extern void ShowOutput(int*);
extern void DoIplot(void);
extern void SetIplot(void);
extern void ClearIplot(void);
extern char *GPstring(int,struct hyEnt**);
extern char *GPgetString(struct hyEnt*);
extern int  GPdeleteGrafRef(struct hyEnt*);
extern void GPclearGraf(void);
#else
extern void ShowOutput();
extern void DoIplot();
extern void SetIplot();
extern void ClearIplot();
extern char *GPstring();
extern char *GPgetString();
extern int  GPdeleteGrafRef();
extern void GPclearGraf();
#endif

/* gridline.c */
#ifdef __STDC__
extern void ShowGrid(void);
extern void ShowLine(int,long,long,long,long);
extern void ShowManhattanLine(int,long,long,long,long);
#else
extern void ShowGrid();
extern void ShowLine();
extern void ShowManhattanLine();
#endif

/* hcopy.c */
#ifdef __STDC__
extern void Hcopy(void);
#else
extern void Hcopy();
#endif

/* hypertxt.c */
#ifdef __STDC__
extern char *HYtostr(struct hprlist *);
extern char *HYtoascii(struct hprlist *);
extern struct hprlist *HYfromascii(char*);
extern char *HYgetentry(struct hprlist*);
extern struct hprlist *HYcopy(struct hprlist*);
extern struct hyEnt *HYcopyEnt(struct hyEnt*);
extern void HYfree(struct hprlist*);
extern char *KbEdit(char*,int,int,int,int,int);
extern struct hprlist *HYedit(struct hprlist*,int,int,int,int,int,int);
extern void HYrepaint(int,int);
extern void KbCursor(int,int);
extern void HYinit(void);
extern void HYadd(struct hyEnt*);
extern void HYdel(struct hyEnt*);
extern void HYtransform(struct o*, struct o*);
extern void HYtransformStretch(struct o*,struct o*,long,long,long,long);
extern void HYundoTransform(void);
extern void HYclearUndoFlags(void);
extern void HYdeleteReference(struct o*);
#else
extern char *HYtostr();
extern char *HYtoascii();
extern struct hprlist *HYfromascii();
extern char *HYgetentry();
extern struct hprlist *HYcopy();
extern struct hyEnt *HYcopyEnt();
extern void HYfree();
extern char *KbEdit();
extern struct hprlist *HYedit();
extern void HYrepaint();
extern void KbCursor();
extern void HYinit();
extern void HYadd();
extern void HYdel();
extern void HYtransform();
extern void HYtransformStretch();
extern void HYundoTransform();
extern void HYclearUndoFlags();
extern void HYdeleteReference();
#endif

/* init.c */
extern struct kv *View;
extern struct ka MenuViewport;
extern struct kp Parameters;
extern struct kl *ColorTable;
extern struct kc SCursor;
extern struct a  CurrentAOI;
extern char TypeOut[200];
#ifdef __STDC__
extern char *Init(void);
extern void InitVLT(void);
extern void InitParameters(void);
extern void DefaultWindows(void);
extern void InitParameters(void);
extern void InitCoarseWindow(long,long,long);
extern void InitFineWindow(long,long);
extern void SetPositioning(void);
extern void InitViewport(void);
extern void SetCurrentAOI(struct ka*);
extern void CenterFullView(void);
extern void UpdateSCED(void);
extern void Readin(void);
extern short FBGetchar(int);
extern char *FBEdit(char*);
extern struct hprlist *FBHyEdit(struct hprlist*);
extern void FBText(int,int,int,char*);
extern void FBPolygon(int,int,long*,int);
extern void FBPolygonClip(long*,int*,struct ka*);
extern void FBSetRubberBanding(int);
#else
extern char *Init();
extern void InitVLT();
extern void InitParameters();
extern void DefaultWindows();
extern void InitCoarseWindow();
extern void InitFineWindow();
extern void SetPositioning();
extern void InitViewport();
extern void SetCurrentAOI();
extern void CenterFullView();
extern void UpdateSCED();
extern void Readin();
extern short FBGetchar();
extern char *FBEdit();
extern struct hprlist *FBHyEdit();
extern void FBText();
extern void FBPolygon();
extern void FBPolygonClip();
extern void FBSetRubberBanding();
#endif

/* labels.c */
#ifdef __STDC__
extern void Label(int*);
extern void BBLabel(struct ka*,struct o*,struct ka*);
extern void CDLabelBB(struct o*,long*,long*,long*,long*);
extern void ShowLabel(int,char*,long,long,int,int);
extern char SetXform(long*);
#else
extern void Label();
extern void BBLabel();
extern void CDLabelBB();
extern void ShowLabel();
extern char SetXform();
#endif

/* library.c */
#ifdef __STDC__
extern void LibraryOpen(char*);
extern FILE *OpenDevice(char*);
extern int  IsCellInLib(char*);
extern void SetLibraryChoices(void);
extern void ModelLibraryOpen(char*);
extern void QueueModel(char*);
extern struct line *PrintModels(void);
extern struct line *GetModelText(char*);
extern void DestroySCEDlibs(void);
extern void LibraryClose(void);
extern void ModelLibraryClose(void);
#else
extern void LibraryOpen();
extern FILE *OpenDevice();
extern int  IsCellInLib();
extern void SetLibraryChoices();
extern void ModelLibraryOpen();
extern void QueueModel();
extern struct line *PrintModels();
extern struct line *GetModelText();
extern void DestroySCEDlibs();
extern void LibraryClose();
extern void ModelLibraryClose();
#endif

/* moretext.c */
#ifdef __STDC__
extern void MoreText(int,int,int,int,FILE*,int);
extern int  MoreLine(char*);
extern int  EnableMore(int);
extern int  RepaintMore(void);
extern int  MorePageDisplay(void);
#else
extern void MoreText();
extern int  MoreLine();
extern int  EnableMore();
extern int  RepaintMore();
extern int  MorePageDisplay();
#endif

/* mutual.c */
#ifdef __STDC__
extern void ShowMutual(int*);
extern struct line *PrintMutual(struct s*);
extern char *GetName(struct prpty*);
#else
extern void ShowMutual();
extern struct line *PrintMutual();
extern char *GetName();
#endif

/* netlist.c */
#ifdef __STDC__
extern void UpdateProperties(struct o*);
extern void UpdateDeviceName(struct o*);
extern void AssignWireProperties(struct o*);
extern void DoSpiceList(void);
extern void DumpSpiceFile(FILE*);
extern struct line *MakeSpiceDeck(void);
extern struct line *SpiceList(struct s*);
extern void ConnectRecursive(struct s*);
extern void Connect(struct s*);
extern void ShowProperties(int*);
extern void UpdatePropertyString(struct prpty*);
extern void Subcircuit(int*);
extern int  SelectNode(long*,long*);
extern void ShowTerminals(void);
extern void DisplayTerminals(int);
extern void TransformReferences(struct o*);
extern void TransformReferencePoint(struct o*,long,long);
extern void UndoReferenceTransform(void);
extern void ClearReferenceUndoFlags(void);
#else
extern void UpdateProperties();
extern void UpdateDeviceName();
extern void AssignWireProperties();
extern void DoSpiceList();
extern void DumpSpiceFile();
extern struct line *MakeSpiceDeck();
extern struct line *SpiceList();
extern void ConnectRecursive();
extern void Connect();
extern void ShowProperties();
extern void UpdatePropertyString();
extern void Subcircuit();
extern int  SelectNode();
extern void ShowTerminals();
extern void DisplayTerminals();
extern void TransformReferences();
extern void TransformReferencePoint();
extern void UndoReferenceTransform();
extern void ClearReferenceUndoFlags();
#endif

/* objects.c */
#ifdef __STDC__
extern void Del(int*);
extern void Wires(int*);
extern void ShowWire(int,long,struct p*);
extern int  IsManhattan(long,long,long,long);
extern void ShowPath(int,struct p*,int);
extern void ShowPolygon(int,struct p*);
extern int  InBox(long,long,struct ka*);
extern void OversizeBox(struct ka*,int);
extern void OutlineBox(struct ka*);
extern void ShowEmptyBox(int,struct ka*);
extern void EraseBox(struct ka*);
extern void StretchPath(int*);
extern void ShowStretch(long,long,long,long);
extern void Dots(int*);
#else
extern void Del();
extern void Wires();
extern void ShowWire();
extern int  IsManhattan();
extern void ShowPath();
extern void ShowPolygon();
extern int  InBox();
extern void OversizeBox();
extern void OutlineBox();
extern void ShowEmptyBox();
extern void EraseBox();
extern void StretchPath();
extern void ShowStretch();
extern void Dots();
#endif

/* point.c */
#ifdef __STDC__
extern void Point(void);
extern int  ButtonPress(int,long,long);
extern int  PointLoop(int*);
extern int  PointLoopSafe(int*);
extern int  PointColor(int*);
extern void FinePosition(long,long,int);
extern void NotPointingAtLayout(void);
#else
extern void Point();
extern int  ButtonPress();
extern int  PointLoop();
extern int  PointLoopSafe();
extern int  PointColor();
extern void FinePosition();
extern void NotPointingAtLayout();
#endif

/* subckt.c */
#ifdef __STDC__
extern void InitDeviceMenu(void);
extern void NewSubckt(void);
extern void Place(int*);
extern void ShowNewInstance(long,long,long,long);
extern void NewSymbol(void);
extern int  MutSelected(struct prpty*);
extern void Push(int*);
extern void UpdateParent(char*);
extern void Pop(void);
extern int  CheckModified(void);
extern void ClearContext(void);
extern void Flatten(int*);
extern int  OpenCell(char*,struct s**);
#else
extern void InitDeviceMenu();
extern void NewSubckt();
extern void Place();
extern void ShowNewInstance();
extern void NewSymbol();
extern int  MutSelected();
extern void Push();
extern void UpdateParent();
extern void Pop();
extern int  CheckModified();
extern void ClearContext();
extern void Flatten();
extern int  OpenCell();
#endif

/* viewport.h */
#ifdef __STDC__
extern void PToL(struct ka*,long*,long*);
extern void ClipToGridPoint(long*,long*);
extern void ShowCommandMenu(void);
extern void ShowMenu(MENU*);
extern MENU *GetCurrentMenu(void);
extern void AlterMenuEntries(char*,char*);
extern void MenuSelect(char*);
extern void MenuDeselect(char*);
extern void MenuSelectDev(char*);
extern void MenuDeselectDev(char*);
extern void FixMenuPrefix(MENU*);
extern void ShowPrompt(char*);
extern void ShowPromptAndWait(char*);
extern void ShowPromptWithColor(char*,int);
extern void RedrawPrompt(void);
extern void AppendToOldPrompt(int);
extern void ErasePrompt(void);
extern void OutlineText(int,int,int,int,int,int,int);
extern void EraseLargeCoarseViewport(void);
extern void EraseFineViewport(void);
extern void ShowFineViewport(void);
extern void ShowCurrentObject(struct o*,int);
extern void ShowMarker(int,int,long,long,int,int,int);
#else
extern void PToL();
extern void ClipToGridPoint();
extern void ShowCommandMenu();
extern void ShowMenu();
extern MENU *GetCurrentMenu();
extern void AlterMenuEntries();
extern void MenuSelect();
extern void MenuDeselect();
extern void MenuSelectDev();
extern void MenuDeselectDev();
extern void FixMenuPrefix();
extern void ShowPrompt();
extern void ShowPromptAndWait();
extern void ShowPromptWithColor();
extern void RedrawPrompt();
extern void AppendToOldPrompt();
extern void ErasePrompt();
extern void OutlineText();
extern void EraseLargeCoarseViewport();
extern void EraseFineViewport();
extern void ShowFineViewport();
extern void ShowCurrentObject();
extern void ShowMarker();
#endif

/* scedintr.c */

#define DefMenuSize 80

extern MENU *BasicMenu;
extern MENU *DeviceMenu;
extern MENU AmbiguityMenu[DefMenuSize];
#ifdef __STDC__
extern void InitMenus(void);
extern void InitBasicMenu(void);
extern int  sced(char**);
extern int  SafeCmds(int*);
extern void RunSpice(void);
extern int  CheckSaved(void);
extern int  CheckError(void);
extern int  IsSymfile(FILE*);
extern void SetPathFromSpice(void);
extern void SetCDPath(char*);
extern char *GetHardCopyDevice(void);
extern void InitSignals(void);
extern char *NextCellName(void);
extern void MallocFailed(void);
extern struct line *AllocateLine(char*);
extern char *AllocateNewString(char*,char*);
extern void ShowProcess(char*);
extern void SCEDhelp(void);
extern void SCEDquitHelp(int);
#else
extern void InitMenus();
extern void InitBasicMenu();
extern int  sced();
extern int  SafeCmds();
extern void RunSpice();
extern int  CheckSaved();
extern int  CheckError();
extern int  IsSymfile();
extern void SetPathFromSpice();
extern void SetCDPath();
extern char *GetHardCopyDevice();
extern void InitSignals();
extern char *NextCellName();
extern void MallocFailed();
extern struct line *AllocateLine();
extern char *AllocateNewString();
extern void ShowProcess();
extern void SCEDhelp();
extern void SCEDquitHelp();
#endif
