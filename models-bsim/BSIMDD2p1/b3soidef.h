/**********
Copyright 1999 Regents of the University of California.  All rights reserved.
Author: Weidong Liu and Pin Su       Feb 1999
Author: 1998 Samuel Fung
Modified by Pin Su, Wei Jin 99/9/27
File: b3soidef.h
**********/

#ifndef B3SOI
#define B3SOI

#define SOICODE
/*  #define BULKCODE  */

#include "ifsim.h"
#include "gendefs.h"
#include "cktdefs.h"
#include "complex.h"
#include "noisedef.h"         

typedef struct sB3SOIinstance
{
    struct sB3SOImodel *B3SOImodPtr;
    struct sB3SOIinstance *B3SOInextInstance;
    IFuid B3SOIname;
    int B3SOIstates;     /* index into state table for this device */

    int B3SOIdNode;
    int B3SOIgNode;
    int B3SOIsNode;
    int B3SOIeNode;
    int B3SOIbNode;
    int B3SOItempNode;
    int B3SOIpNode;  
    int B3SOIdNodePrime;
    int B3SOIsNodePrime;

    int B3SOIvbsNode;
    /* for Debug */
    int B3SOIidsNode;
    int B3SOIicNode;
    int B3SOIibsNode;
    int B3SOIibdNode;
    int B3SOIiiiNode;
    int B3SOIigidlNode;
    int B3SOIitunNode;
    int B3SOIibpNode;
    int B3SOIabeffNode;
    int B3SOIvbs0effNode;
    int B3SOIvbseffNode;
    int B3SOIxcNode;
    int B3SOIcbbNode;
    int B3SOIcbdNode;
    int B3SOIcbeNode;
    int B3SOIcbgNode;
    int B3SOIqbNode;
    int B3SOIqbfNode;
    int B3SOIqjsNode;
    int B3SOIqjdNode;

/* clean up last */
    int B3SOIgmNode;
    int B3SOIgmbsNode;
    int B3SOIgdsNode;
    int B3SOIgmeNode;
    int B3SOIqgNode;
    int B3SOIqdNode;
    int B3SOIqeNode;
    int B3SOIiterations;
    int B3SOIvbs0teffNode;
    int B3SOIvthNode;
    int B3SOIvgsteffNode;
    int B3SOIxcsatNode;
    int B3SOIqaccNode;
    int B3SOIqsub0Node;
    int B3SOIqsubs1Node;
    int B3SOIqsubs2Node;
    int B3SOIvcscvNode;
    int B3SOIvdscvNode;
    int B3SOIdum1Node;
    int B3SOIdum2Node;
    int B3SOIdum3Node;
    int B3SOIdum4Node;
    int B3SOIdum5Node;
/* end clean up last */

    double B3SOIphi;
    double B3SOIvtm;
    double B3SOIni;
    double B3SOIueff;
    double B3SOIthetavth; 
    double B3SOIvon;
    double B3SOIvbsdio;
    double B3SOIvdsat;
    double B3SOIcgdo;
    double B3SOIcgso;
    double B3SOIcgeo;

    double B3SOIids;
    double B3SOIic;
    double B3SOIibs;
    double B3SOIibd;
    double B3SOIiii;
    double B3SOIigidl;
    double B3SOIitun;
    double B3SOIibp;
    double B3SOIabeff;
    double B3SOIvbs0eff;
    double B3SOIvbseff;
    double B3SOIxc;
    double B3SOIcbg;
    double B3SOIcbb;
    double B3SOIcbd;
    double B3SOIqb;
    double B3SOIqbf;
    double B3SOIqjs;
    double B3SOIqjd;
    double B3SOIminIsub;
    int    B3SOIfloat;

/* clean up last */
    double B3SOIdum1;
    double B3SOIdum2;
    double B3SOIdum3;
    double B3SOIdum4;
    double B3SOIdum5;
/* end clean up last */

    double B3SOIl;
    double B3SOIw;
    double B3SOIdrainArea;
    double B3SOIsourceArea;
    double B3SOIdrainSquares;
    double B3SOIsourceSquares;
    double B3SOIdrainPerimeter;
    double B3SOIsourcePerimeter;
    double B3SOIsourceConductance;
    double B3SOIdrainConductance;

    double B3SOIicVBS;
    double B3SOIicVDS;
    double B3SOIicVGS;
    double B3SOIicVES;
    double B3SOIicVPS;
    int B3SOIbjtoff;
    int B3SOIbodyMod;
    int B3SOIdebugMod;
    double B3SOIrth0;
    double B3SOIcth0;
    double B3SOIbodySquares;
    double B3SOIrbodyext;

    double B3SOIcsbox;
    double B3SOIcdbox;
    double B3SOIcsmin;
    double B3SOIcdmin;
    double B3SOIst4;
    double B3SOIdt4;

    int B3SOIoff;
    int B3SOImode;

    /* OP point */
    double B3SOIqinv;
    double B3SOIcd;
    double B3SOIcjs;
    double B3SOIcjd;
    double B3SOIcbody;
    double B3SOIcbodcon;
    double B3SOIcth;
    double B3SOIcsubstrate;

    double B3SOIgm;
    double B3SOIgme;
    double B3SOIcb;
    double B3SOIcdrain;
    double B3SOIgds;
    double B3SOIgmbs;
    double B3SOIgmT;

    double B3SOIgbbs;
    double B3SOIgbgs;
    double B3SOIgbds;
    double B3SOIgbes;
    double B3SOIgbps;
    double B3SOIgbT;

    double B3SOIgjsd;
    double B3SOIgjsb;
    double B3SOIgjsg;
    double B3SOIgjsT;

    double B3SOIgjdb;
    double B3SOIgjdd;
    double B3SOIgjdg;
    double B3SOIgjde;
    double B3SOIgjdT;

    double B3SOIgbpbs;
    double B3SOIgbpgs;
    double B3SOIgbpds;
    double B3SOIgbpes;
    double B3SOIgbpps;
    double B3SOIgbpT;

    double B3SOIgtempb;
    double B3SOIgtempg;
    double B3SOIgtempd;
    double B3SOIgtempe;
    double B3SOIgtempT;

    double B3SOIcggb;
    double B3SOIcgdb;
    double B3SOIcgsb;
    double B3SOIcgeb;
    double B3SOIcgT;

    double B3SOIcbgb;
    double B3SOIcbdb;
    double B3SOIcbsb;
    double B3SOIcbeb;
    double B3SOIcbT;

    double B3SOIcdgb;
    double B3SOIcddb;
    double B3SOIcdsb;
    double B3SOIcdeb;
    double B3SOIcdT;

    double B3SOIcegb;
    double B3SOIcedb;
    double B3SOIcesb;
    double B3SOIceeb;
    double B3SOIceT;

    double B3SOIqse;
    double B3SOIgcse;
    double B3SOIqde;
    double B3SOIgcde;

    struct b3soiSizeDependParam  *pParam;

    unsigned B3SOIlGiven :1;
    unsigned B3SOIwGiven :1;
    unsigned B3SOIdrainAreaGiven :1;
    unsigned B3SOIsourceAreaGiven    :1;
    unsigned B3SOIdrainSquaresGiven  :1;
    unsigned B3SOIsourceSquaresGiven :1;
    unsigned B3SOIdrainPerimeterGiven    :1;
    unsigned B3SOIsourcePerimeterGiven   :1;
    unsigned B3SOIdNodePrimeSet  :1;
    unsigned B3SOIsNodePrimeSet  :1;
    unsigned B3SOIicVBSGiven :1;
    unsigned B3SOIicVDSGiven :1;
    unsigned B3SOIicVGSGiven :1;
    unsigned B3SOIicVESGiven :1;
    unsigned B3SOIicVPSGiven :1;
    unsigned B3SOIbjtoffGiven :1;
    unsigned B3SOIdebugModGiven :1;
    unsigned B3SOIrth0Given :1;
    unsigned B3SOIcth0Given :1;
    unsigned B3SOIbodySquaresGiven :1;
    unsigned B3SOIoffGiven :1;

    double *B3SOIEePtr;
    double *B3SOIEbPtr;
    double *B3SOIBePtr;
    double *B3SOIEgPtr;
    double *B3SOIEdpPtr;
    double *B3SOIEspPtr;
    double *B3SOITemptempPtr;
    double *B3SOITempdpPtr;
    double *B3SOITempspPtr;
    double *B3SOITempgPtr;
    double *B3SOITempbPtr;
    double *B3SOITempePtr;
    double *B3SOIGtempPtr;
    double *B3SOIDPtempPtr;
    double *B3SOISPtempPtr;
    double *B3SOIEtempPtr;
    double *B3SOIBtempPtr;
    double *B3SOIPtempPtr;
    double *B3SOIBpPtr;
    double *B3SOIPbPtr;
    double *B3SOIPpPtr;
    double *B3SOIPgPtr;
    double *B3SOIPdpPtr;
    double *B3SOIPspPtr;
    double *B3SOIPePtr;
    double *B3SOIDPePtr;
    double *B3SOISPePtr;
    double *B3SOIGePtr;
    double *B3SOIDdPtr;
    double *B3SOIGgPtr;
    double *B3SOISsPtr;
    double *B3SOIBbPtr;
    double *B3SOIDPdpPtr;
    double *B3SOISPspPtr;
    double *B3SOIDdpPtr;
    double *B3SOIGbPtr;
    double *B3SOIGdpPtr;
    double *B3SOIGspPtr;
    double *B3SOISspPtr;
    double *B3SOIBdpPtr;
    double *B3SOIBspPtr;
    double *B3SOIDPspPtr;
    double *B3SOIDPdPtr;
    double *B3SOIBgPtr;
    double *B3SOIDPgPtr;
    double *B3SOISPgPtr;
    double *B3SOISPsPtr;
    double *B3SOIDPbPtr;
    double *B3SOISPbPtr;
    double *B3SOISPdpPtr;

    double *B3SOIVbsPtr;
    /* Debug */
    double *B3SOIIdsPtr;
    double *B3SOIIcPtr;
    double *B3SOIIbsPtr;
    double *B3SOIIbdPtr;
    double *B3SOIIiiPtr;
    double *B3SOIIgidlPtr;
    double *B3SOIItunPtr;
    double *B3SOIIbpPtr;
    double *B3SOIAbeffPtr;
    double *B3SOIVbs0effPtr;
    double *B3SOIVbseffPtr;
    double *B3SOIXcPtr;
    double *B3SOICbbPtr;
    double *B3SOICbdPtr;
    double *B3SOICbgPtr;
    double *B3SOIqbPtr;
    double *B3SOIQbfPtr;
    double *B3SOIQjsPtr;
    double *B3SOIQjdPtr;

    /* clean up last */
    double *B3SOIGmPtr;
    double *B3SOIGmbsPtr;
    double *B3SOIGdsPtr;
    double *B3SOIGmePtr;
    double *B3SOIVbs0teffPtr;
    double *B3SOIVthPtr;
    double *B3SOIVgsteffPtr;
    double *B3SOIXcsatPtr;
    double *B3SOIQaccPtr;
    double *B3SOIQsub0Ptr;
    double *B3SOIQsubs1Ptr;
    double *B3SOIQsubs2Ptr;
    double *B3SOIVdscvPtr;
    double *B3SOIVcscvPtr;
    double *B3SOICbePtr;
    double *B3SOIqgPtr;
    double *B3SOIqdPtr;
    double *B3SOIqePtr;
    double *B3SOIDum1Ptr;
    double *B3SOIDum2Ptr;
    double *B3SOIDum3Ptr;
    double *B3SOIDum4Ptr;
    double *B3SOIDum5Ptr;
    /* End clean up last */

#define B3SOIvbd B3SOIstates+ 0
#define B3SOIvbs B3SOIstates+ 1
#define B3SOIvgs B3SOIstates+ 2
#define B3SOIvds B3SOIstates+ 3
#define B3SOIves B3SOIstates+ 4
#define B3SOIvps B3SOIstates+ 5

#define B3SOIvg B3SOIstates+ 6
#define B3SOIvd B3SOIstates+ 7
#define B3SOIvs B3SOIstates+ 8
#define B3SOIvp B3SOIstates+ 9
#define B3SOIve B3SOIstates+ 10
#define B3SOIdeltemp B3SOIstates+ 11

#define B3SOIqb B3SOIstates+ 12
#define B3SOIcqb B3SOIstates+ 13
#define B3SOIqg B3SOIstates+ 14
#define B3SOIcqg B3SOIstates+ 15
#define B3SOIqd B3SOIstates+ 16
#define B3SOIcqd B3SOIstates+ 17
#define B3SOIqe B3SOIstates+ 18
#define B3SOIcqe B3SOIstates+ 19

#define B3SOIqbs  B3SOIstates+ 20
#define B3SOIqbd  B3SOIstates+ 21
#define B3SOIqbe  B3SOIstates+ 22

#define B3SOIqth B3SOIstates+ 23
#define B3SOIcqth B3SOIstates+ 24

#define B3SOInumStates 25


/* indices to the array of B3SOI NOISE SOURCES */

#define B3SOIRDNOIZ       0
#define B3SOIRSNOIZ       1
#define B3SOIIDNOIZ       2
#define B3SOIFLNOIZ       3
#define B3SOIFBNOIZ       4
#define B3SOITOTNOIZ      5

#define B3SOINSRCS        6     /* the number of MOSFET(3) noise sources */

#ifndef NONOISE
    double B3SOInVar[NSTATVARS][B3SOINSRCS];
#else /* NONOISE */
        double **B3SOInVar;
#endif /* NONOISE */

} B3SOIinstance ;

struct b3soiSizeDependParam
{
    double Width;
    double Length;
    double Rth0;
    double Cth0;

    double B3SOIcdsc;           
    double B3SOIcdscb;    
    double B3SOIcdscd;       
    double B3SOIcit;           
    double B3SOInfactor;      
    double B3SOIvsat;         
    double B3SOIat;         
    double B3SOIa0;   
    double B3SOIags;      
    double B3SOIa1;         
    double B3SOIa2;         
    double B3SOIketa;     
    double B3SOInpeak;        
    double B3SOInsub;
    double B3SOIngate;        
    double B3SOIgamma1;      
    double B3SOIgamma2;     
    double B3SOIvbx;      
    double B3SOIvbi;       
    double B3SOIvbm;       
    double B3SOIvbsc;       
    double B3SOIxt;       
    double B3SOIphi;
    double B3SOIlitl;
    double B3SOIk1;
    double B3SOIkt1;
    double B3SOIkt1l;
    double B3SOIkt2;
    double B3SOIk2;
    double B3SOIk3;
    double B3SOIk3b;
    double B3SOIw0;
    double B3SOInlx;
    double B3SOIdvt0;      
    double B3SOIdvt1;      
    double B3SOIdvt2;      
    double B3SOIdvt0w;      
    double B3SOIdvt1w;      
    double B3SOIdvt2w;      
    double B3SOIdrout;      
    double B3SOIdsub;      
    double B3SOIvth0;
    double B3SOIua;
    double B3SOIua1;
    double B3SOIub;
    double B3SOIub1;
    double B3SOIuc;
    double B3SOIuc1;
    double B3SOIu0;
    double B3SOIute;
    double B3SOIvoff;
    double B3SOIvfb;
    double B3SOIuatemp;
    double B3SOIubtemp;
    double B3SOIuctemp;
    double B3SOIrbody;
    double B3SOIrth;
    double B3SOIcth;
    double B3SOIrds0denom;
    double B3SOIvfbb;
    double B3SOIjbjt;
    double B3SOIjdif;
    double B3SOIjrec;
    double B3SOIjtun;
    double B3SOIcsesw;
    double B3SOIcdesw;

   /* Added */
    double B3SOIsdt1;
    double B3SOIst2;
    double B3SOIst3;
    double B3SOIdt2;
    double B3SOIdt3;
  /* Added */

    double B3SOIdelta;
    double B3SOIrdsw;       
    double B3SOIrds0;       
    double B3SOIprwg;       
    double B3SOIprwb;       
    double B3SOIprt;       
    double B3SOIeta0;         
    double B3SOIetab;         
    double B3SOIpclm;      
    double B3SOIpdibl1;      
    double B3SOIpdibl2;      
    double B3SOIpdiblb;      
    double B3SOIpvag;       
    double B3SOIwr;
    double B3SOIdwg;
    double B3SOIdwb;
    double B3SOIb0;
    double B3SOIb1;
    double B3SOIalpha0;
    double B3SOIalpha1;
    double B3SOIbeta0;


    /* CV model */
    double B3SOIcgsl;
    double B3SOIcgdl;
    double B3SOIckappa;
    double B3SOIcf;
    double B3SOIclc;
    double B3SOIcle;

/* Added for binning - START0 */
    double B3SOIvbsa;
    double B3SOIdelp;
    double B3SOIkb1;
    double B3SOIkb3;
    double B3SOIdvbd0;
    double B3SOIdvbd1;
    double B3SOIabp;
    double B3SOImxc;
    double B3SOIadice0;
    double B3SOIaii;
    double B3SOIbii;
    double B3SOIcii;
    double B3SOIdii;
    double B3SOIagidl;
    double B3SOIbgidl;
    double B3SOIngidl;
    double B3SOIntun;
    double B3SOIndiode;
    double B3SOIisbjt;
    double B3SOIisdif;
    double B3SOIisrec;
    double B3SOIistun;
    double B3SOIedl;
    double B3SOIkbjt1;
    double B3SOIvsdfb;
    double B3SOIvsdth;
/* Added for binning - END0 */

/* Pre-calculated constants */

    double B3SOIdw;
    double B3SOIdl;
    double B3SOIleff;
    double B3SOIweff;

    double B3SOIdwc;
    double B3SOIdlc;
    double B3SOIleffCV;
    double B3SOIweffCV;
    double B3SOIabulkCVfactor;
    double B3SOIcgso;
    double B3SOIcgdo;
    double B3SOIcgeo;

    double B3SOIu0temp;       
    double B3SOIvsattemp;   
    double B3SOIsqrtPhi;   
    double B3SOIphis3;   
    double B3SOIXdep0;          
    double B3SOIsqrtXdep0;          
    double B3SOItheta0vb0;
    double B3SOIthetaRout; 

    double B3SOIcof1;
    double B3SOIcof2;
    double B3SOIcof3;
    double B3SOIcof4;
    double B3SOIcdep0;
    struct b3soiSizeDependParam  *pNext;
};


typedef struct sB3SOImodel 
{
    int B3SOImodType;
    struct sB3SOImodel *B3SOInextModel;
    B3SOIinstance *B3SOIinstances;
    IFuid B3SOImodName; 
    int B3SOItype;

    int    B3SOImobMod;
    int    B3SOIcapMod;
    int    B3SOInoiMod;
    int    B3SOIshMod;
    int    B3SOIbinUnit;
    int    B3SOIparamChk;
    double B3SOIversion;             
    double B3SOItox;             
    double B3SOIcdsc;           
    double B3SOIcdscb; 
    double B3SOIcdscd;          
    double B3SOIcit;           
    double B3SOInfactor;      
    double B3SOIvsat;         
    double B3SOIat;         
    double B3SOIa0;   
    double B3SOIags;      
    double B3SOIa1;         
    double B3SOIa2;         
    double B3SOIketa;     
    double B3SOInsub;
    double B3SOInpeak;        
    double B3SOIngate;        
    double B3SOIgamma1;      
    double B3SOIgamma2;     
    double B3SOIvbx;      
    double B3SOIvbm;       
    double B3SOIxt;       
    double B3SOIk1;
    double B3SOIkt1;
    double B3SOIkt1l;
    double B3SOIkt2;
    double B3SOIk2;
    double B3SOIk3;
    double B3SOIk3b;
    double B3SOIw0;
    double B3SOInlx;
    double B3SOIdvt0;      
    double B3SOIdvt1;      
    double B3SOIdvt2;      
    double B3SOIdvt0w;      
    double B3SOIdvt1w;      
    double B3SOIdvt2w;      
    double B3SOIdrout;      
    double B3SOIdsub;      
    double B3SOIvth0;
    double B3SOIua;
    double B3SOIua1;
    double B3SOIub;
    double B3SOIub1;
    double B3SOIuc;
    double B3SOIuc1;
    double B3SOIu0;
    double B3SOIute;
    double B3SOIvoff;
    double B3SOIdelta;
    double B3SOIrdsw;       
    double B3SOIprwg;
    double B3SOIprwb;
    double B3SOIprt;       
    double B3SOIeta0;         
    double B3SOIetab;         
    double B3SOIpclm;      
    double B3SOIpdibl1;      
    double B3SOIpdibl2;      
    double B3SOIpdiblb;
    double B3SOIpvag;       
    double B3SOIwr;
    double B3SOIdwg;
    double B3SOIdwb;
    double B3SOIb0;
    double B3SOIb1;
    double B3SOIalpha0;
    double B3SOIalpha1;
    double B3SOIbeta0;
    double B3SOItbox;
    double B3SOItsi;
    double B3SOIxj;
    double B3SOIkb1;
    double B3SOIkb3;
    double B3SOIdvbd0;
    double B3SOIdvbd1;
    double B3SOIvbsa;
    double B3SOIdelp;
    double B3SOIrbody;
    double B3SOIrbsh;
    double B3SOIadice0;
    double B3SOIabp;
    double B3SOImxc;
    double B3SOIrth0;
    double B3SOIcth0;
    double B3SOIaii;
    double B3SOIbii;
    double B3SOIcii;
    double B3SOIdii;
    double B3SOIngidl;
    double B3SOIagidl;
    double B3SOIbgidl;
    double B3SOIndiode;
    double B3SOIntun;
    double B3SOIisbjt;
    double B3SOIisdif;
    double B3SOIisrec;
    double B3SOIistun;
    double B3SOIxbjt;
    double B3SOIxdif;
    double B3SOIxrec;
    double B3SOIxtun;
    double B3SOIedl;
    double B3SOIkbjt1;
    double B3SOItt;
    double B3SOIvsdfb;
    double B3SOIvsdth;
    double B3SOIcsdmin;
    double B3SOIasd;

    /* CV model */
    double B3SOIcgsl;
    double B3SOIcgdl;
    double B3SOIckappa;
    double B3SOIcf;
    double B3SOIclc;
    double B3SOIcle;
    double B3SOIdwc;
    double B3SOIdlc;

    double B3SOItnom;
    double B3SOIcgso;
    double B3SOIcgdo;
    double B3SOIcgeo;

    double B3SOIxpart;
    double B3SOIcFringOut;
    double B3SOIcFringMax;

    double B3SOIsheetResistance;
    double B3SOIbodyJctGateSideGradingCoeff;
    double B3SOIGatesidewallJctPotential;
    double B3SOIunitLengthGateSidewallJctCap;
    double B3SOIcsdesw;

    double B3SOILint;
    double B3SOILl;
    double B3SOILln;
    double B3SOILw;
    double B3SOILwn;
    double B3SOILwl;
    double B3SOILmin;
    double B3SOILmax;

    double B3SOIWint;
    double B3SOIWl;
    double B3SOIWln;
    double B3SOIWw;
    double B3SOIWwn;
    double B3SOIWwl;
    double B3SOIWmin;
    double B3SOIWmax;

/* Added for binning - START1 */
    /* Length Dependence */
    double B3SOIlnpeak;        
    double B3SOIlnsub;
    double B3SOIlngate;        
    double B3SOIlvth0;
    double B3SOIlk1;
    double B3SOIlk2;
    double B3SOIlk3;
    double B3SOIlk3b;
    double B3SOIlvbsa;
    double B3SOIldelp;
    double B3SOIlkb1;
    double B3SOIlkb3;
    double B3SOIldvbd0;
    double B3SOIldvbd1;
    double B3SOIlw0;
    double B3SOIlnlx;
    double B3SOIldvt0;      
    double B3SOIldvt1;      
    double B3SOIldvt2;      
    double B3SOIldvt0w;      
    double B3SOIldvt1w;      
    double B3SOIldvt2w;      
    double B3SOIlu0;
    double B3SOIlua;
    double B3SOIlub;
    double B3SOIluc;
    double B3SOIlvsat;         
    double B3SOIla0;   
    double B3SOIlags;      
    double B3SOIlb0;
    double B3SOIlb1;
    double B3SOIlketa;
    double B3SOIlabp;
    double B3SOIlmxc;
    double B3SOIladice0;
    double B3SOIla1;         
    double B3SOIla2;         
    double B3SOIlrdsw;       
    double B3SOIlprwb;
    double B3SOIlprwg;
    double B3SOIlwr;
    double B3SOIlnfactor;      
    double B3SOIldwg;
    double B3SOIldwb;
    double B3SOIlvoff;
    double B3SOIleta0;         
    double B3SOIletab;         
    double B3SOIldsub;      
    double B3SOIlcit;           
    double B3SOIlcdsc;           
    double B3SOIlcdscb; 
    double B3SOIlcdscd;          
    double B3SOIlpclm;      
    double B3SOIlpdibl1;      
    double B3SOIlpdibl2;      
    double B3SOIlpdiblb;      
    double B3SOIldrout;      
    double B3SOIlpvag;       
    double B3SOIldelta;
    double B3SOIlaii;
    double B3SOIlbii;
    double B3SOIlcii;
    double B3SOIldii;
    double B3SOIlalpha0;
    double B3SOIlalpha1;
    double B3SOIlbeta0;
    double B3SOIlagidl;
    double B3SOIlbgidl;
    double B3SOIlngidl;
    double B3SOIlntun;
    double B3SOIlndiode;
    double B3SOIlisbjt;       
    double B3SOIlisdif;
    double B3SOIlisrec;       
    double B3SOIlistun;       
    double B3SOIledl;       
    double B3SOIlkbjt1;       
    /* CV model */
    double B3SOIlvsdfb;
    double B3SOIlvsdth;

    /* Width Dependence */
    double B3SOIwnpeak;        
    double B3SOIwnsub;
    double B3SOIwngate;        
    double B3SOIwvth0;
    double B3SOIwk1;
    double B3SOIwk2;
    double B3SOIwk3;
    double B3SOIwk3b;
    double B3SOIwvbsa;
    double B3SOIwdelp;
    double B3SOIwkb1;
    double B3SOIwkb3;
    double B3SOIwdvbd0;
    double B3SOIwdvbd1;
    double B3SOIww0;
    double B3SOIwnlx;
    double B3SOIwdvt0;      
    double B3SOIwdvt1;      
    double B3SOIwdvt2;      
    double B3SOIwdvt0w;      
    double B3SOIwdvt1w;      
    double B3SOIwdvt2w;      
    double B3SOIwu0;
    double B3SOIwua;
    double B3SOIwub;
    double B3SOIwuc;
    double B3SOIwvsat;         
    double B3SOIwa0;   
    double B3SOIwags;      
    double B3SOIwb0;
    double B3SOIwb1;
    double B3SOIwketa;
    double B3SOIwabp;
    double B3SOIwmxc;
    double B3SOIwadice0;
    double B3SOIwa1;         
    double B3SOIwa2;         
    double B3SOIwrdsw;       
    double B3SOIwprwb;
    double B3SOIwprwg;
    double B3SOIwwr;
    double B3SOIwnfactor;      
    double B3SOIwdwg;
    double B3SOIwdwb;
    double B3SOIwvoff;
    double B3SOIweta0;         
    double B3SOIwetab;         
    double B3SOIwdsub;      
    double B3SOIwcit;           
    double B3SOIwcdsc;           
    double B3SOIwcdscb; 
    double B3SOIwcdscd;          
    double B3SOIwpclm;      
    double B3SOIwpdibl1;      
    double B3SOIwpdibl2;      
    double B3SOIwpdiblb;      
    double B3SOIwdrout;      
    double B3SOIwpvag;       
    double B3SOIwdelta;
    double B3SOIwaii;
    double B3SOIwbii;
    double B3SOIwcii;
    double B3SOIwdii;
    double B3SOIwalpha0;
    double B3SOIwalpha1;
    double B3SOIwbeta0;
    double B3SOIwagidl;
    double B3SOIwbgidl;
    double B3SOIwngidl;
    double B3SOIwntun;
    double B3SOIwndiode;
    double B3SOIwisbjt;       
    double B3SOIwisdif;
    double B3SOIwisrec;       
    double B3SOIwistun;       
    double B3SOIwedl;       
    double B3SOIwkbjt1;       
    /* CV model */
    double B3SOIwvsdfb;
    double B3SOIwvsdth;

    /* Cross-term Dependence */
    double B3SOIpnpeak;        
    double B3SOIpnsub;
    double B3SOIpngate;        
    double B3SOIpvth0;
    double B3SOIpk1;
    double B3SOIpk2;
    double B3SOIpk3;
    double B3SOIpk3b;
    double B3SOIpvbsa;
    double B3SOIpdelp;
    double B3SOIpkb1;
    double B3SOIpkb3;
    double B3SOIpdvbd0;
    double B3SOIpdvbd1;
    double B3SOIpw0;
    double B3SOIpnlx;
    double B3SOIpdvt0;      
    double B3SOIpdvt1;      
    double B3SOIpdvt2;      
    double B3SOIpdvt0w;      
    double B3SOIpdvt1w;      
    double B3SOIpdvt2w;      
    double B3SOIpu0;
    double B3SOIpua;
    double B3SOIpub;
    double B3SOIpuc;
    double B3SOIpvsat;         
    double B3SOIpa0;   
    double B3SOIpags;      
    double B3SOIpb0;
    double B3SOIpb1;
    double B3SOIpketa;
    double B3SOIpabp;
    double B3SOIpmxc;
    double B3SOIpadice0;
    double B3SOIpa1;         
    double B3SOIpa2;         
    double B3SOIprdsw;       
    double B3SOIpprwb;
    double B3SOIpprwg;
    double B3SOIpwr;
    double B3SOIpnfactor;      
    double B3SOIpdwg;
    double B3SOIpdwb;
    double B3SOIpvoff;
    double B3SOIpeta0;         
    double B3SOIpetab;         
    double B3SOIpdsub;      
    double B3SOIpcit;           
    double B3SOIpcdsc;           
    double B3SOIpcdscb; 
    double B3SOIpcdscd;          
    double B3SOIppclm;      
    double B3SOIppdibl1;      
    double B3SOIppdibl2;      
    double B3SOIppdiblb;      
    double B3SOIpdrout;      
    double B3SOIppvag;       
    double B3SOIpdelta;
    double B3SOIpaii;
    double B3SOIpbii;
    double B3SOIpcii;
    double B3SOIpdii;
    double B3SOIpalpha0;
    double B3SOIpalpha1;
    double B3SOIpbeta0;
    double B3SOIpagidl;
    double B3SOIpbgidl;
    double B3SOIpngidl;
    double B3SOIpntun;
    double B3SOIpndiode;
    double B3SOIpisbjt;       
    double B3SOIpisdif;
    double B3SOIpisrec;       
    double B3SOIpistun;       
    double B3SOIpedl;       
    double B3SOIpkbjt1;       
    /* CV model */
    double B3SOIpvsdfb;
    double B3SOIpvsdth;
/* Added for binning - END1 */

/* Pre-calculated constants */
    double B3SOIcbox;
    double B3SOIcsi;
    double B3SOIcsieff;
    double B3SOIcoxt;
    double B3SOIcboxt;
    double B3SOIcsit;
    double B3SOInfb;
    double B3SOIadice;
    double B3SOIqsi;
    double B3SOIqsieff;
    double B3SOIeg0;

    /* MCJ: move to size-dependent param. */
    double B3SOIvtm;   
    double B3SOIcox;
    double B3SOIcof1;
    double B3SOIcof2;
    double B3SOIcof3;
    double B3SOIcof4;
    double B3SOIvcrit;
    double B3SOIfactor1;

    double B3SOIoxideTrapDensityA;      
    double B3SOIoxideTrapDensityB;     
    double B3SOIoxideTrapDensityC;  
    double B3SOIem;  
    double B3SOIef;  
    double B3SOIaf;  
    double B3SOIkf;  
    double B3SOInoif;  

    struct b3soiSizeDependParam *pSizeDependParamKnot;

    /* Flags */

    unsigned B3SOItboxGiven:1;
    unsigned B3SOItsiGiven :1;
    unsigned B3SOIxjGiven :1;
    unsigned B3SOIkb1Given :1;
    unsigned B3SOIkb3Given :1;
    unsigned B3SOIdvbd0Given :1;
    unsigned B3SOIdvbd1Given :1;
    unsigned B3SOIvbsaGiven :1;
    unsigned B3SOIdelpGiven :1;
    unsigned B3SOIrbodyGiven :1;
    unsigned B3SOIrbshGiven :1;
    unsigned B3SOIadice0Given :1;
    unsigned B3SOIabpGiven :1;
    unsigned B3SOImxcGiven :1;
    unsigned B3SOIrth0Given :1;
    unsigned B3SOIcth0Given :1;
    unsigned B3SOIaiiGiven :1;
    unsigned B3SOIbiiGiven :1;
    unsigned B3SOIciiGiven :1;
    unsigned B3SOIdiiGiven :1;
    unsigned B3SOIngidlGiven :1;
    unsigned B3SOIagidlGiven :1;
    unsigned B3SOIbgidlGiven :1;
    unsigned B3SOIndiodeGiven :1;
    unsigned B3SOIntunGiven :1;
    unsigned B3SOIisbjtGiven :1;
    unsigned B3SOIisdifGiven :1;
    unsigned B3SOIisrecGiven :1;
    unsigned B3SOIistunGiven :1;
    unsigned B3SOIxbjtGiven :1;
    unsigned B3SOIxdifGiven :1;
    unsigned B3SOIxrecGiven :1;
    unsigned B3SOIxtunGiven :1;
    unsigned B3SOIedlGiven :1;
    unsigned B3SOIkbjt1Given :1;
    unsigned B3SOIttGiven :1;
    unsigned B3SOIvsdfbGiven :1;
    unsigned B3SOIvsdthGiven :1;
    unsigned B3SOIasdGiven :1;
    unsigned B3SOIcsdminGiven :1;

    unsigned  B3SOImobModGiven :1;
    unsigned  B3SOIbinUnitGiven :1;
    unsigned  B3SOIcapModGiven :1;
    unsigned  B3SOIparamChkGiven :1;
    unsigned  B3SOInoiModGiven :1;
    unsigned  B3SOIshModGiven :1;
    unsigned  B3SOItypeGiven   :1;
    unsigned  B3SOItoxGiven   :1;
    unsigned  B3SOIversionGiven   :1;

    unsigned  B3SOIcdscGiven   :1;
    unsigned  B3SOIcdscbGiven   :1;
    unsigned  B3SOIcdscdGiven   :1;
    unsigned  B3SOIcitGiven   :1;
    unsigned  B3SOInfactorGiven   :1;
    unsigned  B3SOIvsatGiven   :1;
    unsigned  B3SOIatGiven   :1;
    unsigned  B3SOIa0Given   :1;
    unsigned  B3SOIagsGiven   :1;
    unsigned  B3SOIa1Given   :1;
    unsigned  B3SOIa2Given   :1;
    unsigned  B3SOIketaGiven   :1;    
    unsigned  B3SOInsubGiven   :1;
    unsigned  B3SOInpeakGiven   :1;
    unsigned  B3SOIngateGiven   :1;
    unsigned  B3SOIgamma1Given   :1;
    unsigned  B3SOIgamma2Given   :1;
    unsigned  B3SOIvbxGiven   :1;
    unsigned  B3SOIvbmGiven   :1;
    unsigned  B3SOIxtGiven   :1;
    unsigned  B3SOIk1Given   :1;
    unsigned  B3SOIkt1Given   :1;
    unsigned  B3SOIkt1lGiven   :1;
    unsigned  B3SOIkt2Given   :1;
    unsigned  B3SOIk2Given   :1;
    unsigned  B3SOIk3Given   :1;
    unsigned  B3SOIk3bGiven   :1;
    unsigned  B3SOIw0Given   :1;
    unsigned  B3SOInlxGiven   :1;
    unsigned  B3SOIdvt0Given   :1;   
    unsigned  B3SOIdvt1Given   :1;     
    unsigned  B3SOIdvt2Given   :1;     
    unsigned  B3SOIdvt0wGiven   :1;   
    unsigned  B3SOIdvt1wGiven   :1;     
    unsigned  B3SOIdvt2wGiven   :1;     
    unsigned  B3SOIdroutGiven   :1;     
    unsigned  B3SOIdsubGiven   :1;     
    unsigned  B3SOIvth0Given   :1;
    unsigned  B3SOIuaGiven   :1;
    unsigned  B3SOIua1Given   :1;
    unsigned  B3SOIubGiven   :1;
    unsigned  B3SOIub1Given   :1;
    unsigned  B3SOIucGiven   :1;
    unsigned  B3SOIuc1Given   :1;
    unsigned  B3SOIu0Given   :1;
    unsigned  B3SOIuteGiven   :1;
    unsigned  B3SOIvoffGiven   :1;
    unsigned  B3SOIrdswGiven   :1;      
    unsigned  B3SOIprwgGiven   :1;      
    unsigned  B3SOIprwbGiven   :1;      
    unsigned  B3SOIprtGiven   :1;      
    unsigned  B3SOIeta0Given   :1;    
    unsigned  B3SOIetabGiven   :1;    
    unsigned  B3SOIpclmGiven   :1;   
    unsigned  B3SOIpdibl1Given   :1;   
    unsigned  B3SOIpdibl2Given   :1;  
    unsigned  B3SOIpdiblbGiven   :1;  
    unsigned  B3SOIpvagGiven   :1;    
    unsigned  B3SOIdeltaGiven  :1;     
    unsigned  B3SOIwrGiven   :1;
    unsigned  B3SOIdwgGiven   :1;
    unsigned  B3SOIdwbGiven   :1;
    unsigned  B3SOIb0Given   :1;
    unsigned  B3SOIb1Given   :1;
    unsigned  B3SOIalpha0Given   :1;
    unsigned  B3SOIalpha1Given   :1;
    unsigned  B3SOIbeta0Given   :1;

    /* CV model */
    unsigned  B3SOIcgslGiven   :1;
    unsigned  B3SOIcgdlGiven   :1;
    unsigned  B3SOIckappaGiven   :1;
    unsigned  B3SOIcfGiven   :1;
    unsigned  B3SOIclcGiven   :1;
    unsigned  B3SOIcleGiven   :1;
    unsigned  B3SOIdwcGiven   :1;
    unsigned  B3SOIdlcGiven   :1;

/* Added for binning - START2 */
    /* Length Dependence */
    unsigned  B3SOIlnpeakGiven   :1;        
    unsigned  B3SOIlnsubGiven   :1;
    unsigned  B3SOIlngateGiven   :1;        
    unsigned  B3SOIlvth0Given   :1;
    unsigned  B3SOIlk1Given   :1;
    unsigned  B3SOIlk2Given   :1;
    unsigned  B3SOIlk3Given   :1;
    unsigned  B3SOIlk3bGiven   :1;
    unsigned  B3SOIlvbsaGiven   :1;
    unsigned  B3SOIldelpGiven   :1;
    unsigned  B3SOIlkb1Given   :1;
    unsigned  B3SOIlkb3Given   :1;
    unsigned  B3SOIldvbd0Given   :1;
    unsigned  B3SOIldvbd1Given   :1;
    unsigned  B3SOIlw0Given   :1;
    unsigned  B3SOIlnlxGiven   :1;
    unsigned  B3SOIldvt0Given   :1;      
    unsigned  B3SOIldvt1Given   :1;      
    unsigned  B3SOIldvt2Given   :1;      
    unsigned  B3SOIldvt0wGiven   :1;      
    unsigned  B3SOIldvt1wGiven   :1;      
    unsigned  B3SOIldvt2wGiven   :1;      
    unsigned  B3SOIlu0Given   :1;
    unsigned  B3SOIluaGiven   :1;
    unsigned  B3SOIlubGiven   :1;
    unsigned  B3SOIlucGiven   :1;
    unsigned  B3SOIlvsatGiven   :1;         
    unsigned  B3SOIla0Given   :1;   
    unsigned  B3SOIlagsGiven   :1;      
    unsigned  B3SOIlb0Given   :1;
    unsigned  B3SOIlb1Given   :1;
    unsigned  B3SOIlketaGiven   :1;
    unsigned  B3SOIlabpGiven   :1;
    unsigned  B3SOIlmxcGiven   :1;
    unsigned  B3SOIladice0Given   :1;
    unsigned  B3SOIla1Given   :1;         
    unsigned  B3SOIla2Given   :1;         
    unsigned  B3SOIlrdswGiven   :1;       
    unsigned  B3SOIlprwbGiven   :1;
    unsigned  B3SOIlprwgGiven   :1;
    unsigned  B3SOIlwrGiven   :1;
    unsigned  B3SOIlnfactorGiven   :1;      
    unsigned  B3SOIldwgGiven   :1;
    unsigned  B3SOIldwbGiven   :1;
    unsigned  B3SOIlvoffGiven   :1;
    unsigned  B3SOIleta0Given   :1;         
    unsigned  B3SOIletabGiven   :1;         
    unsigned  B3SOIldsubGiven   :1;      
    unsigned  B3SOIlcitGiven   :1;           
    unsigned  B3SOIlcdscGiven   :1;           
    unsigned  B3SOIlcdscbGiven   :1; 
    unsigned  B3SOIlcdscdGiven   :1;          
    unsigned  B3SOIlpclmGiven   :1;      
    unsigned  B3SOIlpdibl1Given   :1;      
    unsigned  B3SOIlpdibl2Given   :1;      
    unsigned  B3SOIlpdiblbGiven   :1;      
    unsigned  B3SOIldroutGiven   :1;      
    unsigned  B3SOIlpvagGiven   :1;       
    unsigned  B3SOIldeltaGiven   :1;
    unsigned  B3SOIlaiiGiven   :1;
    unsigned  B3SOIlbiiGiven   :1;
    unsigned  B3SOIlciiGiven   :1;
    unsigned  B3SOIldiiGiven   :1;
    unsigned  B3SOIlalpha0Given   :1;
    unsigned  B3SOIlalpha1Given   :1;
    unsigned  B3SOIlbeta0Given   :1;
    unsigned  B3SOIlagidlGiven   :1;
    unsigned  B3SOIlbgidlGiven   :1;
    unsigned  B3SOIlngidlGiven   :1;
    unsigned  B3SOIlntunGiven   :1;
    unsigned  B3SOIlndiodeGiven   :1;
    unsigned  B3SOIlisbjtGiven   :1;       
    unsigned  B3SOIlisdifGiven   :1;
    unsigned  B3SOIlisrecGiven   :1;       
    unsigned  B3SOIlistunGiven   :1;       
    unsigned  B3SOIledlGiven   :1;       
    unsigned  B3SOIlkbjt1Given   :1;       
    /* CV model */
    unsigned  B3SOIlvsdfbGiven   :1;
    unsigned  B3SOIlvsdthGiven   :1;

    /* Width Dependence */
    unsigned  B3SOIwnpeakGiven   :1;        
    unsigned  B3SOIwnsubGiven   :1;
    unsigned  B3SOIwngateGiven   :1;        
    unsigned  B3SOIwvth0Given   :1;
    unsigned  B3SOIwk1Given   :1;
    unsigned  B3SOIwk2Given   :1;
    unsigned  B3SOIwk3Given   :1;
    unsigned  B3SOIwk3bGiven   :1;
    unsigned  B3SOIwvbsaGiven   :1;
    unsigned  B3SOIwdelpGiven   :1;
    unsigned  B3SOIwkb1Given   :1;
    unsigned  B3SOIwkb3Given   :1;
    unsigned  B3SOIwdvbd0Given   :1;
    unsigned  B3SOIwdvbd1Given   :1;
    unsigned  B3SOIww0Given   :1;
    unsigned  B3SOIwnlxGiven   :1;
    unsigned  B3SOIwdvt0Given   :1;      
    unsigned  B3SOIwdvt1Given   :1;      
    unsigned  B3SOIwdvt2Given   :1;      
    unsigned  B3SOIwdvt0wGiven   :1;      
    unsigned  B3SOIwdvt1wGiven   :1;      
    unsigned  B3SOIwdvt2wGiven   :1;      
    unsigned  B3SOIwu0Given   :1;
    unsigned  B3SOIwuaGiven   :1;
    unsigned  B3SOIwubGiven   :1;
    unsigned  B3SOIwucGiven   :1;
    unsigned  B3SOIwvsatGiven   :1;         
    unsigned  B3SOIwa0Given   :1;   
    unsigned  B3SOIwagsGiven   :1;      
    unsigned  B3SOIwb0Given   :1;
    unsigned  B3SOIwb1Given   :1;
    unsigned  B3SOIwketaGiven   :1;
    unsigned  B3SOIwabpGiven   :1;
    unsigned  B3SOIwmxcGiven   :1;
    unsigned  B3SOIwadice0Given   :1;
    unsigned  B3SOIwa1Given   :1;         
    unsigned  B3SOIwa2Given   :1;         
    unsigned  B3SOIwrdswGiven   :1;       
    unsigned  B3SOIwprwbGiven   :1;
    unsigned  B3SOIwprwgGiven   :1;
    unsigned  B3SOIwwrGiven   :1;
    unsigned  B3SOIwnfactorGiven   :1;      
    unsigned  B3SOIwdwgGiven   :1;
    unsigned  B3SOIwdwbGiven   :1;
    unsigned  B3SOIwvoffGiven   :1;
    unsigned  B3SOIweta0Given   :1;         
    unsigned  B3SOIwetabGiven   :1;         
    unsigned  B3SOIwdsubGiven   :1;      
    unsigned  B3SOIwcitGiven   :1;           
    unsigned  B3SOIwcdscGiven   :1;           
    unsigned  B3SOIwcdscbGiven   :1; 
    unsigned  B3SOIwcdscdGiven   :1;          
    unsigned  B3SOIwpclmGiven   :1;      
    unsigned  B3SOIwpdibl1Given   :1;      
    unsigned  B3SOIwpdibl2Given   :1;      
    unsigned  B3SOIwpdiblbGiven   :1;      
    unsigned  B3SOIwdroutGiven   :1;      
    unsigned  B3SOIwpvagGiven   :1;       
    unsigned  B3SOIwdeltaGiven   :1;
    unsigned  B3SOIwaiiGiven   :1;
    unsigned  B3SOIwbiiGiven   :1;
    unsigned  B3SOIwciiGiven   :1;
    unsigned  B3SOIwdiiGiven   :1;
    unsigned  B3SOIwalpha0Given   :1;
    unsigned  B3SOIwalpha1Given   :1;
    unsigned  B3SOIwbeta0Given   :1;
    unsigned  B3SOIwagidlGiven   :1;
    unsigned  B3SOIwbgidlGiven   :1;
    unsigned  B3SOIwngidlGiven   :1;
    unsigned  B3SOIwntunGiven   :1;
    unsigned  B3SOIwndiodeGiven   :1;
    unsigned  B3SOIwisbjtGiven   :1;       
    unsigned  B3SOIwisdifGiven   :1;
    unsigned  B3SOIwisrecGiven   :1;       
    unsigned  B3SOIwistunGiven   :1;       
    unsigned  B3SOIwedlGiven   :1;       
    unsigned  B3SOIwkbjt1Given   :1;       
    /* CV model */
    unsigned  B3SOIwvsdfbGiven   :1;
    unsigned  B3SOIwvsdthGiven   :1;

    /* Cross-term Dependence */
    unsigned  B3SOIpnpeakGiven   :1;        
    unsigned  B3SOIpnsubGiven   :1;
    unsigned  B3SOIpngateGiven   :1;        
    unsigned  B3SOIpvth0Given   :1;
    unsigned  B3SOIpk1Given   :1;
    unsigned  B3SOIpk2Given   :1;
    unsigned  B3SOIpk3Given   :1;
    unsigned  B3SOIpk3bGiven   :1;
    unsigned  B3SOIpvbsaGiven   :1;
    unsigned  B3SOIpdelpGiven   :1;
    unsigned  B3SOIpkb1Given   :1;
    unsigned  B3SOIpkb3Given   :1;
    unsigned  B3SOIpdvbd0Given   :1;
    unsigned  B3SOIpdvbd1Given   :1;
    unsigned  B3SOIpw0Given   :1;
    unsigned  B3SOIpnlxGiven   :1;
    unsigned  B3SOIpdvt0Given   :1;      
    unsigned  B3SOIpdvt1Given   :1;      
    unsigned  B3SOIpdvt2Given   :1;      
    unsigned  B3SOIpdvt0wGiven   :1;      
    unsigned  B3SOIpdvt1wGiven   :1;      
    unsigned  B3SOIpdvt2wGiven   :1;      
    unsigned  B3SOIpu0Given   :1;
    unsigned  B3SOIpuaGiven   :1;
    unsigned  B3SOIpubGiven   :1;
    unsigned  B3SOIpucGiven   :1;
    unsigned  B3SOIpvsatGiven   :1;         
    unsigned  B3SOIpa0Given   :1;   
    unsigned  B3SOIpagsGiven   :1;      
    unsigned  B3SOIpb0Given   :1;
    unsigned  B3SOIpb1Given   :1;
    unsigned  B3SOIpketaGiven   :1;
    unsigned  B3SOIpabpGiven   :1;
    unsigned  B3SOIpmxcGiven   :1;
    unsigned  B3SOIpadice0Given   :1;
    unsigned  B3SOIpa1Given   :1;         
    unsigned  B3SOIpa2Given   :1;         
    unsigned  B3SOIprdswGiven   :1;       
    unsigned  B3SOIpprwbGiven   :1;
    unsigned  B3SOIpprwgGiven   :1;
    unsigned  B3SOIpwrGiven   :1;
    unsigned  B3SOIpnfactorGiven   :1;      
    unsigned  B3SOIpdwgGiven   :1;
    unsigned  B3SOIpdwbGiven   :1;
    unsigned  B3SOIpvoffGiven   :1;
    unsigned  B3SOIpeta0Given   :1;         
    unsigned  B3SOIpetabGiven   :1;         
    unsigned  B3SOIpdsubGiven   :1;      
    unsigned  B3SOIpcitGiven   :1;           
    unsigned  B3SOIpcdscGiven   :1;           
    unsigned  B3SOIpcdscbGiven   :1; 
    unsigned  B3SOIpcdscdGiven   :1;          
    unsigned  B3SOIppclmGiven   :1;      
    unsigned  B3SOIppdibl1Given   :1;      
    unsigned  B3SOIppdibl2Given   :1;      
    unsigned  B3SOIppdiblbGiven   :1;      
    unsigned  B3SOIpdroutGiven   :1;      
    unsigned  B3SOIppvagGiven   :1;       
    unsigned  B3SOIpdeltaGiven   :1;
    unsigned  B3SOIpaiiGiven   :1;
    unsigned  B3SOIpbiiGiven   :1;
    unsigned  B3SOIpciiGiven   :1;
    unsigned  B3SOIpdiiGiven   :1;
    unsigned  B3SOIpalpha0Given   :1;
    unsigned  B3SOIpalpha1Given   :1;
    unsigned  B3SOIpbeta0Given   :1;
    unsigned  B3SOIpagidlGiven   :1;
    unsigned  B3SOIpbgidlGiven   :1;
    unsigned  B3SOIpngidlGiven   :1;
    unsigned  B3SOIpntunGiven   :1;
    unsigned  B3SOIpndiodeGiven   :1;
    unsigned  B3SOIpisbjtGiven   :1;       
    unsigned  B3SOIpisdifGiven   :1;
    unsigned  B3SOIpisrecGiven   :1;       
    unsigned  B3SOIpistunGiven   :1;       
    unsigned  B3SOIpedlGiven   :1;       
    unsigned  B3SOIpkbjt1Given   :1;       
    /* CV model */
    unsigned  B3SOIpvsdfbGiven   :1;
    unsigned  B3SOIpvsdthGiven   :1;
/* Added for binning - END2 */

    unsigned  B3SOIuseFringeGiven   :1;

    unsigned  B3SOItnomGiven   :1;
    unsigned  B3SOIcgsoGiven   :1;
    unsigned  B3SOIcgdoGiven   :1;
    unsigned  B3SOIcgeoGiven   :1;
    unsigned  B3SOIxpartGiven   :1;
    unsigned  B3SOIsheetResistanceGiven   :1;
    unsigned  B3SOIGatesidewallJctPotentialGiven   :1;
    unsigned  B3SOIbodyJctGateSideGradingCoeffGiven   :1;
    unsigned  B3SOIunitLengthGateSidewallJctCapGiven   :1;
    unsigned  B3SOIcsdeswGiven :1;

    unsigned  B3SOIoxideTrapDensityAGiven  :1;         
    unsigned  B3SOIoxideTrapDensityBGiven  :1;        
    unsigned  B3SOIoxideTrapDensityCGiven  :1;     
    unsigned  B3SOIemGiven  :1;     
    unsigned  B3SOIefGiven  :1;     
    unsigned  B3SOIafGiven  :1;     
    unsigned  B3SOIkfGiven  :1;     
    unsigned  B3SOInoifGiven  :1;     

    unsigned  B3SOILintGiven   :1;
    unsigned  B3SOILlGiven   :1;
    unsigned  B3SOILlnGiven   :1;
    unsigned  B3SOILwGiven   :1;
    unsigned  B3SOILwnGiven   :1;
    unsigned  B3SOILwlGiven   :1;
    unsigned  B3SOILminGiven   :1;
    unsigned  B3SOILmaxGiven   :1;

    unsigned  B3SOIWintGiven   :1;
    unsigned  B3SOIWlGiven   :1;
    unsigned  B3SOIWlnGiven   :1;
    unsigned  B3SOIWwGiven   :1;
    unsigned  B3SOIWwnGiven   :1;
    unsigned  B3SOIWwlGiven   :1;
    unsigned  B3SOIWminGiven   :1;
    unsigned  B3SOIWmaxGiven   :1;

} B3SOImodel;


#ifndef NMOS
#define NMOS 1
#define PMOS -1
#endif /*NMOS*/


/* device parameters */
#define B3SOI_W 1
#define B3SOI_L 2
#define B3SOI_AS 3
#define B3SOI_AD 4
#define B3SOI_PS 5
#define B3SOI_PD 6
#define B3SOI_NRS 7
#define B3SOI_NRD 8
#define B3SOI_OFF 9
#define B3SOI_IC_VBS 10
#define B3SOI_IC_VDS 11
#define B3SOI_IC_VGS 12
#define B3SOI_IC_VES 13
#define B3SOI_IC_VPS 14
#define B3SOI_BJTOFF 15
#define B3SOI_RTH0 16
#define B3SOI_CTH0 17
#define B3SOI_NRB 18
#define B3SOI_IC 19
#define B3SOI_NQSMOD 20
#define B3SOI_DEBUG  21

/* model parameters */
#define B3SOI_MOD_CAPMOD          101
#define B3SOI_MOD_NQSMOD          102
#define B3SOI_MOD_MOBMOD          103    
#define B3SOI_MOD_NOIMOD          104    
#define B3SOI_MOD_SHMOD           105
#define B3SOI_MOD_DDMOD           106

#define B3SOI_MOD_TOX             107



#define B3SOI_MOD_CDSC            108
#define B3SOI_MOD_CDSCB           109
#define B3SOI_MOD_CIT             110
#define B3SOI_MOD_NFACTOR         111
#define B3SOI_MOD_XJ              112
#define B3SOI_MOD_VSAT            113
#define B3SOI_MOD_AT              114
#define B3SOI_MOD_A0              115
#define B3SOI_MOD_A1              116
#define B3SOI_MOD_A2              117
#define B3SOI_MOD_KETA            118   
#define B3SOI_MOD_NSUB            119
#define B3SOI_MOD_NPEAK           120
#define B3SOI_MOD_NGATE           121
#define B3SOI_MOD_GAMMA1          122
#define B3SOI_MOD_GAMMA2          123
#define B3SOI_MOD_VBX             124
#define B3SOI_MOD_BINUNIT         125    

#define B3SOI_MOD_VBM             126

#define B3SOI_MOD_XT              127
#define B3SOI_MOD_K1              129
#define B3SOI_MOD_KT1             130
#define B3SOI_MOD_KT1L            131
#define B3SOI_MOD_K2              132
#define B3SOI_MOD_KT2             133
#define B3SOI_MOD_K3              134
#define B3SOI_MOD_K3B             135
#define B3SOI_MOD_W0              136
#define B3SOI_MOD_NLX             137

#define B3SOI_MOD_DVT0            138
#define B3SOI_MOD_DVT1            139
#define B3SOI_MOD_DVT2            140

#define B3SOI_MOD_DVT0W           141
#define B3SOI_MOD_DVT1W           142
#define B3SOI_MOD_DVT2W           143

#define B3SOI_MOD_DROUT           144
#define B3SOI_MOD_DSUB            145
#define B3SOI_MOD_VTH0            146
#define B3SOI_MOD_UA              147
#define B3SOI_MOD_UA1             148
#define B3SOI_MOD_UB              149
#define B3SOI_MOD_UB1             150
#define B3SOI_MOD_UC              151
#define B3SOI_MOD_UC1             152
#define B3SOI_MOD_U0              153
#define B3SOI_MOD_UTE             154
#define B3SOI_MOD_VOFF            155
#define B3SOI_MOD_DELTA           156
#define B3SOI_MOD_RDSW            157
#define B3SOI_MOD_PRT             158
#define B3SOI_MOD_LDD             159
#define B3SOI_MOD_ETA             160
#define B3SOI_MOD_ETA0            161
#define B3SOI_MOD_ETAB            162
#define B3SOI_MOD_PCLM            163
#define B3SOI_MOD_PDIBL1          164
#define B3SOI_MOD_PDIBL2          165
#define B3SOI_MOD_PSCBE1          166
#define B3SOI_MOD_PSCBE2          167
#define B3SOI_MOD_PVAG            168
#define B3SOI_MOD_WR              169
#define B3SOI_MOD_DWG             170
#define B3SOI_MOD_DWB             171
#define B3SOI_MOD_B0              172
#define B3SOI_MOD_B1              173
#define B3SOI_MOD_ALPHA0          174
#define B3SOI_MOD_BETA0           175
#define B3SOI_MOD_PDIBLB          178

#define B3SOI_MOD_PRWG            179
#define B3SOI_MOD_PRWB            180

#define B3SOI_MOD_CDSCD           181
#define B3SOI_MOD_AGS             182

#define B3SOI_MOD_FRINGE          184
#define B3SOI_MOD_CGSL            186
#define B3SOI_MOD_CGDL            187
#define B3SOI_MOD_CKAPPA          188
#define B3SOI_MOD_CF              189
#define B3SOI_MOD_CLC             190
#define B3SOI_MOD_CLE             191
#define B3SOI_MOD_PARAMCHK        192
#define B3SOI_MOD_VERSION         193

#define B3SOI_MOD_TBOX            195
#define B3SOI_MOD_TSI             196
#define B3SOI_MOD_KB1             197
#define B3SOI_MOD_KB3             198
#define B3SOI_MOD_DVBD0           199
#define B3SOI_MOD_DVBD1           200
#define B3SOI_MOD_DELP            201
#define B3SOI_MOD_VBSA            202
#define B3SOI_MOD_RBODY           204
#define B3SOI_MOD_ADICE0          205
#define B3SOI_MOD_ABP             206
#define B3SOI_MOD_MXC             207
#define B3SOI_MOD_RTH0            208
#define B3SOI_MOD_CTH0            209
#define B3SOI_MOD_AII             210
#define B3SOI_MOD_BII             211
#define B3SOI_MOD_CII             212
#define B3SOI_MOD_DII             213
#define B3SOI_MOD_ALPHA1	  214
#define B3SOI_MOD_NGIDL           215
#define B3SOI_MOD_AGIDL           216
#define B3SOI_MOD_BGIDL           217
#define B3SOI_MOD_NDIODE          218
#define B3SOI_MOD_LDIOF           219
#define B3SOI_MOD_LDIOR           220
#define B3SOI_MOD_NTUN            221
#define B3SOI_MOD_ISBJT           222
#define B3SOI_MOD_ISDIF           223
#define B3SOI_MOD_ISREC           224
#define B3SOI_MOD_ISTUN           225
#define B3SOI_MOD_XBJT            226
#define B3SOI_MOD_XDIF            227
#define B3SOI_MOD_XREC            228
#define B3SOI_MOD_XTUN            229
#define B3SOI_MOD_EDL             230
#define B3SOI_MOD_KBJT1           231
#define B3SOI_MOD_TT              232
#define B3SOI_MOD_VSDTH           233
#define B3SOI_MOD_VSDFB           234
#define B3SOI_MOD_ASD             235
#define B3SOI_MOD_CSDMIN          236
#define B3SOI_MOD_RBSH            237

/* Added for binning - START3 */
/* Length dependence */
#define B3SOI_MOD_LNPEAK           301
#define B3SOI_MOD_LNSUB            302
#define B3SOI_MOD_LNGATE           303
#define B3SOI_MOD_LVTH0            304
#define B3SOI_MOD_LK1              305     
#define B3SOI_MOD_LK2              306
#define B3SOI_MOD_LK3              307
#define B3SOI_MOD_LK3B             308
#define B3SOI_MOD_LVBSA            309
#define B3SOI_MOD_LDELP            310
#define B3SOI_MOD_LKB1             311
#define B3SOI_MOD_LKB3             312
#define B3SOI_MOD_LDVBD0           313
#define B3SOI_MOD_LDVBD1           314
#define B3SOI_MOD_LW0              315
#define B3SOI_MOD_LNLX             316
#define B3SOI_MOD_LDVT0            317
#define B3SOI_MOD_LDVT1            318
#define B3SOI_MOD_LDVT2            319
#define B3SOI_MOD_LDVT0W           320
#define B3SOI_MOD_LDVT1W           321
#define B3SOI_MOD_LDVT2W           322
#define B3SOI_MOD_LU0              323
#define B3SOI_MOD_LUA              324
#define B3SOI_MOD_LUB              325
#define B3SOI_MOD_LUC              326
#define B3SOI_MOD_LVSAT            327
#define B3SOI_MOD_LA0              328
#define B3SOI_MOD_LAGS             329
#define B3SOI_MOD_LB0              330
#define B3SOI_MOD_LB1              331
#define B3SOI_MOD_LKETA            332   
#define B3SOI_MOD_LABP             333   
#define B3SOI_MOD_LMXC             334   
#define B3SOI_MOD_LADICE0          335   
#define B3SOI_MOD_LA1              336
#define B3SOI_MOD_LA2              337
#define B3SOI_MOD_LRDSW            338
#define B3SOI_MOD_LPRWB            339
#define B3SOI_MOD_LPRWG            340
#define B3SOI_MOD_LWR              341
#define B3SOI_MOD_LNFACTOR         342
#define B3SOI_MOD_LDWG             343
#define B3SOI_MOD_LDWB             344
#define B3SOI_MOD_LVOFF            345
#define B3SOI_MOD_LETA0            346
#define B3SOI_MOD_LETAB            347
#define B3SOI_MOD_LDSUB            348
#define B3SOI_MOD_LCIT             349
#define B3SOI_MOD_LCDSC            350
#define B3SOI_MOD_LCDSCB           351
#define B3SOI_MOD_LCDSCD           352
#define B3SOI_MOD_LPCLM            353
#define B3SOI_MOD_LPDIBL1          354
#define B3SOI_MOD_LPDIBL2          355
#define B3SOI_MOD_LPDIBLB          356
#define B3SOI_MOD_LDROUT           357
#define B3SOI_MOD_LPVAG            358
#define B3SOI_MOD_LDELTA           359
#define B3SOI_MOD_LAII             360
#define B3SOI_MOD_LBII             361
#define B3SOI_MOD_LCII             362
#define B3SOI_MOD_LDII             363
#define B3SOI_MOD_LALPHA0          364
#define B3SOI_MOD_LALPHA1          365
#define B3SOI_MOD_LBETA0           366
#define B3SOI_MOD_LAGIDL           367
#define B3SOI_MOD_LBGIDL           368
#define B3SOI_MOD_LNGIDL           369
#define B3SOI_MOD_LNTUN            370
#define B3SOI_MOD_LNDIODE          371
#define B3SOI_MOD_LISBJT           372
#define B3SOI_MOD_LISDIF           373
#define B3SOI_MOD_LISREC           374
#define B3SOI_MOD_LISTUN           375
#define B3SOI_MOD_LEDL             376
#define B3SOI_MOD_LKBJT1           377
#define B3SOI_MOD_LVSDFB           378
#define B3SOI_MOD_LVSDTH           379

/* Width dependence */
#define B3SOI_MOD_WNPEAK           401
#define B3SOI_MOD_WNSUB            402
#define B3SOI_MOD_WNGATE           403
#define B3SOI_MOD_WVTH0            404
#define B3SOI_MOD_WK1              405     
#define B3SOI_MOD_WK2              406
#define B3SOI_MOD_WK3              407
#define B3SOI_MOD_WK3B             408
#define B3SOI_MOD_WVBSA            409
#define B3SOI_MOD_WDELP            410
#define B3SOI_MOD_WKB1             411
#define B3SOI_MOD_WKB3             412
#define B3SOI_MOD_WDVBD0           413
#define B3SOI_MOD_WDVBD1           414
#define B3SOI_MOD_WW0              415
#define B3SOI_MOD_WNLX             416
#define B3SOI_MOD_WDVT0            417
#define B3SOI_MOD_WDVT1            418
#define B3SOI_MOD_WDVT2            419
#define B3SOI_MOD_WDVT0W           420
#define B3SOI_MOD_WDVT1W           421
#define B3SOI_MOD_WDVT2W           422
#define B3SOI_MOD_WU0              423
#define B3SOI_MOD_WUA              424
#define B3SOI_MOD_WUB              425
#define B3SOI_MOD_WUC              426
#define B3SOI_MOD_WVSAT            427
#define B3SOI_MOD_WA0              428
#define B3SOI_MOD_WAGS             429
#define B3SOI_MOD_WB0              430
#define B3SOI_MOD_WB1              431
#define B3SOI_MOD_WKETA            432   
#define B3SOI_MOD_WABP             433   
#define B3SOI_MOD_WMXC             434   
#define B3SOI_MOD_WADICE0          435   
#define B3SOI_MOD_WA1              436
#define B3SOI_MOD_WA2              437
#define B3SOI_MOD_WRDSW            438
#define B3SOI_MOD_WPRWB            439
#define B3SOI_MOD_WPRWG            440
#define B3SOI_MOD_WWR              441
#define B3SOI_MOD_WNFACTOR         442
#define B3SOI_MOD_WDWG             443
#define B3SOI_MOD_WDWB             444
#define B3SOI_MOD_WVOFF            445
#define B3SOI_MOD_WETA0            446
#define B3SOI_MOD_WETAB            447
#define B3SOI_MOD_WDSUB            448
#define B3SOI_MOD_WCIT             449
#define B3SOI_MOD_WCDSC            450
#define B3SOI_MOD_WCDSCB           451
#define B3SOI_MOD_WCDSCD           452
#define B3SOI_MOD_WPCLM            453
#define B3SOI_MOD_WPDIBL1          454
#define B3SOI_MOD_WPDIBL2          455
#define B3SOI_MOD_WPDIBLB          456
#define B3SOI_MOD_WDROUT           457
#define B3SOI_MOD_WPVAG            458
#define B3SOI_MOD_WDELTA           459
#define B3SOI_MOD_WAII             460
#define B3SOI_MOD_WBII             461
#define B3SOI_MOD_WCII             462
#define B3SOI_MOD_WDII             463
#define B3SOI_MOD_WALPHA0          464
#define B3SOI_MOD_WALPHA1          465
#define B3SOI_MOD_WBETA0           466
#define B3SOI_MOD_WAGIDL           467
#define B3SOI_MOD_WBGIDL           468
#define B3SOI_MOD_WNGIDL           469
#define B3SOI_MOD_WNTUN            470
#define B3SOI_MOD_WNDIODE          471
#define B3SOI_MOD_WISBJT           472
#define B3SOI_MOD_WISDIF           473
#define B3SOI_MOD_WISREC           474
#define B3SOI_MOD_WISTUN           475
#define B3SOI_MOD_WEDL             476
#define B3SOI_MOD_WKBJT1           477
#define B3SOI_MOD_WVSDFB           478
#define B3SOI_MOD_WVSDTH           479

/* Cross-term dependence */
#define B3SOI_MOD_PNPEAK           501
#define B3SOI_MOD_PNSUB            502
#define B3SOI_MOD_PNGATE           503
#define B3SOI_MOD_PVTH0            504
#define B3SOI_MOD_PK1              505     
#define B3SOI_MOD_PK2              506
#define B3SOI_MOD_PK3              507
#define B3SOI_MOD_PK3B             508
#define B3SOI_MOD_PVBSA            509
#define B3SOI_MOD_PDELP            510
#define B3SOI_MOD_PKB1             511
#define B3SOI_MOD_PKB3             512
#define B3SOI_MOD_PDVBD0           513
#define B3SOI_MOD_PDVBD1           514
#define B3SOI_MOD_PW0              515
#define B3SOI_MOD_PNLX             516
#define B3SOI_MOD_PDVT0            517
#define B3SOI_MOD_PDVT1            518
#define B3SOI_MOD_PDVT2            519
#define B3SOI_MOD_PDVT0W           520
#define B3SOI_MOD_PDVT1W           521
#define B3SOI_MOD_PDVT2W           522
#define B3SOI_MOD_PU0              523
#define B3SOI_MOD_PUA              524
#define B3SOI_MOD_PUB              525
#define B3SOI_MOD_PUC              526
#define B3SOI_MOD_PVSAT            527
#define B3SOI_MOD_PA0              528
#define B3SOI_MOD_PAGS             529
#define B3SOI_MOD_PB0              530
#define B3SOI_MOD_PB1              531
#define B3SOI_MOD_PKETA            532   
#define B3SOI_MOD_PABP             533   
#define B3SOI_MOD_PMXC             534   
#define B3SOI_MOD_PADICE0          535   
#define B3SOI_MOD_PA1              536
#define B3SOI_MOD_PA2              537
#define B3SOI_MOD_PRDSW            538
#define B3SOI_MOD_PPRWB            539
#define B3SOI_MOD_PPRWG            540
#define B3SOI_MOD_PWR              541
#define B3SOI_MOD_PNFACTOR         542
#define B3SOI_MOD_PDWG             543
#define B3SOI_MOD_PDWB             544
#define B3SOI_MOD_PVOFF            545
#define B3SOI_MOD_PETA0            546
#define B3SOI_MOD_PETAB            547
#define B3SOI_MOD_PDSUB            548
#define B3SOI_MOD_PCIT             549
#define B3SOI_MOD_PCDSC            550
#define B3SOI_MOD_PCDSCB           551
#define B3SOI_MOD_PCDSCD           552
#define B3SOI_MOD_PPCLM            553
#define B3SOI_MOD_PPDIBL1          554
#define B3SOI_MOD_PPDIBL2          555
#define B3SOI_MOD_PPDIBLB          556
#define B3SOI_MOD_PDROUT           557
#define B3SOI_MOD_PPVAG            558
#define B3SOI_MOD_PDELTA           559
#define B3SOI_MOD_PAII             560
#define B3SOI_MOD_PBII             561
#define B3SOI_MOD_PCII             562
#define B3SOI_MOD_PDII             563
#define B3SOI_MOD_PALPHA0          564
#define B3SOI_MOD_PALPHA1          565
#define B3SOI_MOD_PBETA0           566
#define B3SOI_MOD_PAGIDL           567
#define B3SOI_MOD_PBGIDL           568
#define B3SOI_MOD_PNGIDL           569
#define B3SOI_MOD_PNTUN            570
#define B3SOI_MOD_PNDIODE          571
#define B3SOI_MOD_PISBJT           572
#define B3SOI_MOD_PISDIF           573
#define B3SOI_MOD_PISREC           574
#define B3SOI_MOD_PISTUN           575
#define B3SOI_MOD_PEDL             576
#define B3SOI_MOD_PKBJT1           577
#define B3SOI_MOD_PVSDFB           578
#define B3SOI_MOD_PVSDTH           579
/* Added for binning - END3 */

#define B3SOI_MOD_TNOM             701
#define B3SOI_MOD_CGSO             702
#define B3SOI_MOD_CGDO             703
#define B3SOI_MOD_CGEO             704
#define B3SOI_MOD_XPART            705

#define B3SOI_MOD_RSH              706
#define B3SOI_MOD_NMOS             814
#define B3SOI_MOD_PMOS             815

#define B3SOI_MOD_NOIA             816
#define B3SOI_MOD_NOIB             817
#define B3SOI_MOD_NOIC             818

#define B3SOI_MOD_LINT             819
#define B3SOI_MOD_LL               820
#define B3SOI_MOD_LLN              821
#define B3SOI_MOD_LW               822
#define B3SOI_MOD_LWN              823
#define B3SOI_MOD_LWL              824

#define B3SOI_MOD_WINT             827
#define B3SOI_MOD_WL               828
#define B3SOI_MOD_WLN              829
#define B3SOI_MOD_WW               830
#define B3SOI_MOD_WWN              831
#define B3SOI_MOD_WWL              832

#define B3SOI_MOD_DWC              835
#define B3SOI_MOD_DLC              836

#define B3SOI_MOD_EM               837
#define B3SOI_MOD_EF               838
#define B3SOI_MOD_AF               839
#define B3SOI_MOD_KF               840
#define B3SOI_MOD_NOIF             841


#define B3SOI_MOD_PBSWG            843
#define B3SOI_MOD_MJSWG            844
#define B3SOI_MOD_CJSWG            845
#define B3SOI_MOD_CSDESW           846

/* device questions */
#define B3SOI_DNODE                901
#define B3SOI_GNODE                902
#define B3SOI_SNODE                903
#define B3SOI_BNODE                904
#define B3SOI_ENODE                905
#define B3SOI_DNODEPRIME           906
#define B3SOI_SNODEPRIME           907
#define B3SOI_VBD                  908
#define B3SOI_VBS                  909
#define B3SOI_VGS                  910
#define B3SOI_VES                  911
#define B3SOI_VDS                  912
#define B3SOI_CD                   913
#define B3SOI_CBS                  914
#define B3SOI_CBD                  915
#define B3SOI_GM                   916
#define B3SOI_GDS                  917
#define B3SOI_GMBS                 918
#define B3SOI_GBD                  919
#define B3SOI_GBS                  920
#define B3SOI_QB                   921
#define B3SOI_CQB                  922
#define B3SOI_QG                   923
#define B3SOI_CQG                  924
#define B3SOI_QD                   925
#define B3SOI_CQD                  926
#define B3SOI_CGG                  927
#define B3SOI_CGD                  928
#define B3SOI_CGS                  929
#define B3SOI_CBG                  930
#define B3SOI_CAPBD                931
#define B3SOI_CQBD                 932
#define B3SOI_CAPBS                933
#define B3SOI_CQBS                 934
#define B3SOI_CDG                  935
#define B3SOI_CDD                  936
#define B3SOI_CDS                  937
#define B3SOI_VON                  938
#define B3SOI_VDSAT                939
#define B3SOI_QBS                  940
#define B3SOI_QBD                  941
#define B3SOI_SOURCECONDUCT        942
#define B3SOI_DRAINCONDUCT         943
#define B3SOI_CBDB                 944
#define B3SOI_CBSB                 945
#define B3SOI_GMID                 946


#include "b3soiext.h"

#ifdef __STDC__
extern void B3SOIevaluate(double,double,double,B3SOIinstance*,B3SOImodel*,
        double*,double*,double*, double*, double*, double*, double*, 
        double*, double*, double*, double*, double*, double*, double*, 
        double*, double*, double*, double*, CKTcircuit*);
extern int B3SOIdebug(B3SOImodel*, B3SOIinstance*, CKTcircuit*, int);
extern int B3SOIcheckModel(B3SOImodel*, B3SOIinstance*, CKTcircuit*);
#else /* stdc */
extern void B3SOIevaluate();
extern int B3SOIdebug();
extern int B3SOIcheckModel();
#endif /* stdc */

#endif /*B3SOI*/

