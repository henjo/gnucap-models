/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

struct s_sgen {
    CKTcircuit *ckt;
    GENmodel   **devlist;
    GENmodel   *model, *next_model, *first_model;
    GENinstance *instance, *next_instance, *first_instance;
    IFparm     *ptable;
    double     value;
    int        dev;
    int        istate;
    int        param, max_param;
    int        is_dc;
    int        is_instparam;
    int        is_q;
    int        is_principle;
    int        is_zerook;
};
typedef struct s_sgen sgen;

#ifdef __STDC__
extern sgen *sgen_init(CKTcircuit*,int);
extern int sgen_next(sgen**);
extern int sens_getp(sgen*,CKTcircuit*,IFvalue*);
#else
extern sgen *sgen_init();
extern int sgen_next();
extern int sens_getp();
#endif
