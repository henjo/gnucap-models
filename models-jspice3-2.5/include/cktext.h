/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1992 Stephen R. Whiteley
****************************************************************************/

#ifndef CKTEXT
#define CKTEXT

#ifdef __STDC__
extern int  CKTacDump(CKTcircuit*,double,GENERIC*);
extern int  CKTacLoad(CKTcircuit*);
extern int  CKTaccept(CKTcircuit*);
extern int  CKTask(GENERIC*,GENERIC*,int,IFvalue*,IFvalue*);
extern int  CKTaskAnalQ(GENERIC*,GENERIC*,int,IFvalue*,IFvalue*);
extern int  CKTaskNodQst(GENERIC*,GENERIC*,int,IFvalue*,IFvalue*);
extern int  CKTbindNode(GENERIC*,GENERIC*,int,GENERIC*);
extern void CKTbreakDump(CKTcircuit*);
extern int  CKTclrBreak(CKTcircuit*);
extern int  CKTconvTest(CKTcircuit*);
extern int  CKTcrtElt(GENERIC*,GENERIC*,GENERIC**,IFuid);
extern int  CKTdelTask(GENERIC*,GENERIC*);
extern int  CKTdestroy(GENERIC*);
extern int  CKTdltAnal(GENERIC*,GENERIC*,GENERIC*);
extern int  CKTdltInst(GENERIC*,GENERIC*);
extern int  CKTdltMod(GENERIC*,GENERIC*);
extern int  CKTdltNod(GENERIC*,GENERIC*);
extern int  CKTdoJob(GENERIC*,int,GENERIC*);
extern void CKTdump(CKTcircuit*,double,GENERIC*);
extern int  CKTfndAnal(GENERIC*,int*,GENERIC**,IFuid,GENERIC*,IFuid);
extern int  CKTfndBranch(CKTcircuit*,IFuid);
extern int  CKTfndDev(GENERIC*,int*,GENERIC**,IFuid,GENERIC*,IFuid);
extern int  CKTfndMod(GENERIC*,int*,GENERIC**,IFuid);
extern int  CKTfndNode(GENERIC*,GENERIC**,IFuid);
extern int  CKTfndTask(GENERIC*,GENERIC**,IFuid );
extern int  CKTground(GENERIC*,GENERIC**,IFuid);
extern int  CKTic(CKTcircuit*);
extern int  CKTinit(GENERIC**);
extern int  CKTinst2Node(GENERIC*,GENERIC*,int,GENERIC**,IFuid*);
extern int  CKTlinkEq(CKTcircuit*,CKTnode*);
extern int  CKTload(CKTcircuit*);
extern int  CKTmapNode(GENERIC*,GENERIC**,IFuid);
extern int  CKTmkCur(CKTcircuit *,CKTnode**,IFuid,char*);
extern int  CKTmkNode(CKTcircuit*,CKTnode**);
extern int  CKTmkVolt(CKTcircuit *,CKTnode**,IFuid,char*);
extern int  CKTmodAsk(GENERIC*,GENERIC*,int,IFvalue*,IFvalue*);
extern int  CKTmodCrt(GENERIC*,int,GENERIC**,IFuid);
extern int  CKTmodParam(GENERIC*,GENERIC*,int,IFvalue*,IFvalue*);
extern int  CKTnames(CKTcircuit*,int*,IFuid**);
extern int  CKTnewAnal(GENERIC*,int,IFuid,GENERIC**,GENERIC*);
extern int  CKTnewEq(GENERIC*,GENERIC**,IFuid);
extern int  CKTnewNode(GENERIC*,GENERIC**,IFuid);
extern int  CKTnewTask(GENERIC*,GENERIC**,IFuid);
extern IFuid CKTnodName(CKTcircuit*,int);
extern void CKTnodOut(CKTcircuit*);
extern CKTnode *CKTnum2nod(CKTcircuit*,int);
extern int  CKTop(CKTcircuit*,long,long,int);
extern int  CKTpModName(char*,IFvalue*,CKTcircuit*,int,IFuid,GENmodel**);
extern int  CKTpName(char*,IFvalue*,CKTcircuit*,int,char*,GENinstance**);
extern int  CKTparam(GENERIC*,GENERIC*,int,IFvalue*,IFvalue*);
extern int  CKTpzFindZeros(CKTcircuit*,GENERIC**,int*);
extern int  CKTpzLoad(CKTcircuit*,SPcomplex*);
extern int  CKTpzSetup(CKTcircuit*,int);
extern int  CKTsenAC(CKTcircuit*);
extern int  CKTsenComp(CKTcircuit*);
extern int  CKTsenDCtran(CKTcircuit*);
extern int  CKTsenLoad(CKTcircuit*);
extern void CKTsenPrint(CKTcircuit*);
extern int  CKTsenSetup(CKTcircuit*);
extern int  CKTsenUpdate(CKTcircuit*);
extern int  CKTsetAnalPm(GENERIC*,GENERIC*,int,IFvalue*,IFvalue*);
extern int  CKTsetBreak(CKTcircuit*,double);
extern int  CKTsetNodPm(GENERIC*,GENERIC*,int,IFvalue*,IFvalue*);
extern int  CKTsetic(CKTcircuit*);
extern int  CKTsetup(CKTcircuit*);
extern int  CKTtemp(CKTcircuit*);
extern char *CKTtrouble(GENERIC*,char*);
extern void CKTterr(int,CKTcircuit*,double*);
extern int  CKTtrunc(CKTcircuit*,double*);
extern int  CKTtypelook(char*);
extern int  SPIinit(IFfrontEnd*,IFsimulator**);
extern char *SPerror(int);

#else /* stdc*/

extern int  CKTacDump();
extern int  CKTacLoad();
extern int  CKTaccept();
extern int  CKTask();
extern int  CKTaskAnalQ();
extern int  CKTaskNodQst();
extern int  CKTbindNode();
extern void CKTbreakDump();
extern int  CKTclrBreak();
extern int  CKTcrtElt();
extern int  CKTdelTask();
extern int  CKTdestroy();
extern int  CKTdltAnal();
extern int  CKTdltInst();
extern int  CKTdltMod();
extern int  CKTdltNod();
extern int  CKTdoJob();
extern void CKTdump();
extern int  CKTfndAnal();
extern int  CKTfndBranch();
extern int  CKTfndDev();
extern int  CKTfndMod();
extern int  CKTfndNode();
extern int  CKTfndTask();
extern int  CKTground();
extern int  CKTic();
extern int  CKTinit();
extern int  CKTinst2Node();
extern int  CKTlinkEq();
extern int  CKTload();
extern int  CKTmapNode();
extern int  CKTmkCur();
extern int  CKTmkNode();
extern int  CKTmkVolt();
extern int  CKTmodAsk();
extern int  CKTmodCrt();
extern int  CKTmodParam();
extern int  CKTnames();
extern int  CKTnewAnal();
extern int  CKTnewEq();
extern int  CKTnewNode();
extern int  CKTnewTask();
extern IFuid CKTnodName();
extern void CKTnodOut();
extern CKTnode *CKTnum2nod();
extern int  CKTop();
extern int  CKTpModName();
extern int  CKTpName();
extern int  CKTparam();
extern int  CKTpzLoad();
extern int  CKTpzSetup();
extern int  CKTsenAC();
extern int  CKTsenComp();
extern int  CKTsenDCtran();
extern int  CKTsenLoad();
extern void CKTsenPrint();
extern int  CKTsenSetup();
extern int  CKTsenUpdate();
extern int  CKTsetAnalPm();
extern int  CKTsetBreak();
extern int  CKTsetNodPm();
extern int  CKTsetic();
extern int  CKTsetup();
extern int  CKTpzSetup();
extern int  CKTtemp();
extern char *CKTtrouble();
extern void CKTterr();
extern int  CKTtrunc();
extern int  CKTtypelook();
extern int  SPIinit();
extern char *SPerror();
#endif /* stdc*/

#endif
