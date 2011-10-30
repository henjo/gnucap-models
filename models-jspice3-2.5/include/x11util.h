/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1995 Stephen R. Whiteley
****************************************************************************/

typedef enum {NOGO, NO_EXIST, READ_OK, WRITE_OK} check_type;

typedef struct {
    Widget shell;
    Widget form;
    Widget view;
    Widget text;
    Widget title;
    Widget butbox;
    Widget popup;
    Widget popup_text;
    Widget message;
    Atom wm_delete;
    String *popup_sens_list;
    XtPointer client_data;
#ifdef XEDITOR
    Bool TextChanged;
    event_type Last;
    String saved_as;
#endif
} widget_bag;

extern Display *Xdisplay;

#ifdef __STDC__
extern void PopUpInput(String, String, void(*)(), widget_bag*);
extern void PopDownInput(Widget, XtPointer, XtPointer);
extern void PopUpMessage(String, widget_bag*);
extern void PopDownMessage(Widget, XtPointer, XtPointer);
extern void PopUpErr(String, widget_bag*);
extern void PopDownErr(Widget, XtPointer, XtPointer);
extern void CenterWidgetOnWidget(Widget, Widget);
extern check_type CheckFile(char*, int, widget_bag*);
#else
extern void PopUpInput();
extern void PopDownInput();
extern void PopUpMessage();
extern void PopDownMessage();
extern void PopUpErr();
extern void PopDownErr();
extern void CenterWidgetOnWidget();
extern check_type CheckFile();
#endif
