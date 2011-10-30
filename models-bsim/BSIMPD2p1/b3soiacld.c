/* $Id: b3soiacld.c,v 2.1 99/9/27 Pin Su Release $  */
/*
$Log:   b3soiacld.c,v $
 * Revision 2.1  99/9/27  Pin Su
 * BSIMPD2.1 release
 *
*/
static char rcsid[] = "$Id: b3soiacld.c,v 2.1 99/9/27 Pin Su Release $";

/*************************************/

/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1998 Samuel Fung, Dennis Sinitsky and Stephen Tang
File: b3soiacld.c          98/5/01
Modified by Pin Su    99/4/30
Modified by Pin Su    99/9/27
**********/

#include "spice.h"
#include <stdio.h>
#include "cktdefs.h"
#include "b3soidef.h"
#include "sperror.h"
#include "suffix.h"


int
B3SOIacLoad(inModel,ckt)
GENmodel *inModel;
register CKTcircuit *ckt;
{
register B3SOImodel *model = (B3SOImodel*)inModel;
register B3SOIinstance *here;
register int selfheat;
double xcggb, xcgdb, xcgsb, xcgeb, xcgT;
double xcdgb, xcddb, xcdsb, xcdeb, xcdT;
double xcsgb, xcsdb, xcssb, xcseb, xcsT;
double xcbgb, xcbdb, xcbsb, xcbeb, xcbT;
double xcegb, xceeb, xceT;
double gdpr, gspr, gds;
double cggb, cgdb, cgsb, cgT;
double cdgb, cddb, cdsb, cdeb, cdT;
double cbgb, cbdb, cbsb, cbeb, cbT;
double ceeb, ceT;
double GSoverlapCap, GDoverlapCap, GEoverlapCap, FwdSum, RevSum, Gm, Gmbs, GmT;
double omega;
double dxpart, sxpart;
double gbbg, gbbdp, gbbb, gbbp, gbbsp, gbbT;
double gddpg, gddpdp, gddpsp, gddpb, gddpT;
double gsspg, gsspdp, gsspsp, gsspb, gsspT;
double gppdp, gppb, gppp, gppT;
double xcTt, cTt, gcTt, gTtt, gTtg, gTtb, gTtdp, gTtsp;
double EDextrinsicCap, ESextrinsicCap;
double xcedb, xcesb;


    omega = ckt->CKTomega;
    for (; model != NULL; model = model->B3SOInextModel) 
    {    

      for (here = model->B3SOIinstances; here!= NULL;
              here = here->B3SOInextInstance) 
	 {    
              selfheat = (model->B3SOIshMod == 1) && (here->B3SOIrth0 != 0.0);
              if (here->B3SOImode >= 0) 
	      {   Gm = here->B3SOIgm;
		  Gmbs = here->B3SOIgmbs;
                  GmT = model->B3SOItype * here->B3SOIgmT;
		  FwdSum = Gm + Gmbs;
		  RevSum = 0.0;

                  cbgb = here->B3SOIcbgb;
                  cbsb = here->B3SOIcbsb;
                  cbdb = here->B3SOIcbdb;
                  cbeb = here->B3SOIcbeb;
                  cbT  = model->B3SOItype * here->B3SOIcbT;

                  ceeb = here->B3SOIceeb;
                  ceT  = model->B3SOItype * here->B3SOIceT;

                  cggb = here->B3SOIcggb;
                  cgsb = here->B3SOIcgsb;
                  cgdb = here->B3SOIcgdb;
                  cgT  = model->B3SOItype * here->B3SOIcgT;

                  cdgb = here->B3SOIcdgb;
                  cdsb = here->B3SOIcdsb;
                  cddb = here->B3SOIcddb;
                  cdeb = here->B3SOIcdeb;
                  cdT  = model->B3SOItype * here->B3SOIcdT;

                  cTt = here->pParam->B3SOIcth;

                  gbbg  = -here->B3SOIgbgs;
                  gbbdp = -here->B3SOIgbds;
                  gbbb  = -here->B3SOIgbbs;
                  gbbp  = -here->B3SOIgbps;
                  gbbT  = -model->B3SOItype * here->B3SOIgbT;
                  gbbsp = - ( gbbg + gbbdp + gbbb + gbbp);

                  gddpg  = -here->B3SOIgjdg;
                  gddpdp = -here->B3SOIgjdd;
                  gddpb  = -here->B3SOIgjdb;
                  gddpT  = -model->B3SOItype * here->B3SOIgjdT;
                  gddpsp = - ( gddpg + gddpdp + gddpb );

                  gsspg  = -here->B3SOIgjsg;
                  gsspdp = -here->B3SOIgjsd;
                  gsspb  = -here->B3SOIgjsb;
                  gsspT  = -model->B3SOItype * here->B3SOIgjsT;
                  gsspsp = - (gsspg + gsspdp + gsspb );

                  gppdp = 0;
             	  gppb = -here->B3SOIgbpbs;
                  gppp = -here->B3SOIgbpps;
                  gppT = -model->B3SOItype * here->B3SOIgbpT;

                  gTtg  = here->B3SOIgtempg;
                  gTtb  = here->B3SOIgtempb;
                  gTtdp = here->B3SOIgtempd;
                  gTtt  = here->B3SOIgtempT;
                  gTtsp = - (gTtg + gTtb + gTtdp);

                  sxpart = 0.6;
                  dxpart = 0.4;

              } 
	      else
	      {   Gm = -here->B3SOIgm;
		  Gmbs = -here->B3SOIgmbs;
                  GmT = -model->B3SOItype * here->B3SOIgmT;
		  FwdSum = 0.0;
		  RevSum = -Gm - Gmbs ;

                  cdgb = - (here->B3SOIcdgb + here->B3SOIcggb + here->B3SOIcbgb);
                  cdsb = - (here->B3SOIcddb + here->B3SOIcgdb + here->B3SOIcbdb);
                  cddb = - (here->B3SOIcdsb + here->B3SOIcgsb + here->B3SOIcbsb);
                  cdeb = - (here->B3SOIcdeb + here->B3SOIcbeb + here->B3SOIceeb);
                  cdT  = - model->B3SOItype * (here->B3SOIcgT + here->B3SOIcbT
                         + here->B3SOIcdT + here->B3SOIceT);

                  ceeb = here->B3SOIceeb;
                  ceT  = model->B3SOItype * here->B3SOIceT;

                  cggb = here->B3SOIcggb;
                  cgsb = here->B3SOIcgdb;
                  cgdb = here->B3SOIcgsb;
                  cgT  = model->B3SOItype * here->B3SOIcgT;

                  cbgb = here->B3SOIcbgb;
                  cbsb = here->B3SOIcbdb;
                  cbdb = here->B3SOIcbsb;
                  cbeb = here->B3SOIcbeb;
                  cbT  = model->B3SOItype * here->B3SOIcbT;

                  cTt = here->pParam->B3SOIcth;

                  gbbg  = -here->B3SOIgbgs;
                  gbbb  = -here->B3SOIgbbs;
                  gbbp  = -here->B3SOIgbps;
                  gbbsp = -here->B3SOIgbds;
                  gbbT  = -model->B3SOItype * here->B3SOIgbT;
                  gbbdp = - ( gbbg + gbbsp + gbbb + gbbp);

                  gddpg  = -here->B3SOIgjsg;
                  gddpsp = -here->B3SOIgjsd;
                  gddpb  = -here->B3SOIgjsb;
                  gddpT  = -model->B3SOItype * here->B3SOIgjsT;
                  gddpdp = - (gddpg + gddpsp + gddpb );

                  gsspg  = -here->B3SOIgjdg;
                  gsspsp = -here->B3SOIgjdd;
                  gsspb  = -here->B3SOIgjdb;
                  gsspT  = -model->B3SOItype * here->B3SOIgjdT;
                  gsspdp = - ( gsspg + gsspsp + gsspb );

                  gppb = -here->B3SOIgbpbs;
                  gppp = -here->B3SOIgbpps;
                  gppT = -model->B3SOItype * here->B3SOIgbpT;
                  gppdp = - (gppb + gppp);

                  gTtt = here->B3SOIgtempT;
                  gTtg = here->B3SOIgtempg;
                  gTtb = here->B3SOIgtempb;
                  gTtdp = here->B3SOIgtempd;
                  gTtsp = - (gTtt + gTtg + gTtb + gTtdp);

                  gTtg  = here->B3SOIgtempg;
                  gTtb  = here->B3SOIgtempb;
                  gTtsp = here->B3SOIgtempd;
                  gTtt  = here->B3SOIgtempT;
                  gTtdp = - (gTtg + gTtb + gTtsp);

                  sxpart = 0.6;
                  sxpart = 0.4;
                  dxpart = 0.6;
              }

              gdpr=here->B3SOIdrainConductance;
              gspr=here->B3SOIsourceConductance;
              gds= here->B3SOIgds;

	      GSoverlapCap = here->B3SOIcgso;
	      GDoverlapCap = here->B3SOIcgdo;
	      GEoverlapCap = here->pParam->B3SOIcgeo;

              EDextrinsicCap = here->B3SOIgcde;
              ESextrinsicCap = here->B3SOIgcse;
              xcedb = -EDextrinsicCap * omega;
              xcdeb = (cdeb - EDextrinsicCap) * omega;
              xcddb = (cddb + GDoverlapCap + EDextrinsicCap) * omega;
              xceeb = (ceeb + GEoverlapCap + EDextrinsicCap + ESextrinsicCap) * omega;
              xcesb = -ESextrinsicCap * omega;
              xcssb = (GSoverlapCap + ESextrinsicCap - (cgsb + cbsb + cdsb)) * omega;

              xcseb = -(cbeb + cdeb + ceeb + ESextrinsicCap) * omega;

              xcegb = (- GEoverlapCap) * omega;
              xceT  =  ceT * omega;
              xcggb = (cggb + GDoverlapCap + GSoverlapCap + GEoverlapCap)
		    * omega;
              xcgdb = (cgdb - GDoverlapCap ) * omega;
              xcgsb = (cgsb - GSoverlapCap) * omega;
              xcgeb = (- GEoverlapCap) * omega;
              xcgT  = cgT * omega;

              xcdgb = (cdgb - GDoverlapCap) * omega;
              xcdsb = cdsb * omega;
              xcdT  = cdT * omega;

              xcsgb = -(cggb + cbgb + cdgb + GSoverlapCap) * omega;
              xcsdb = -(cgdb + cbdb + cddb) * omega;
              xcsT  = -(cgT + cbT + cdT + ceT) * omega;

              xcbgb = cbgb * omega;
              xcbdb = cbdb * omega;
              xcbsb = cbsb * omega;
              xcbeb = cbeb * omega;
              xcbT  = cbT * omega;

              xcTt = cTt * omega;

              *(here->B3SOIEdpPtr +1) += xcedb;
              *(here->B3SOIEspPtr +1) += xcesb;
              *(here->B3SOIDPePtr +1) += xcdeb;
              *(here->B3SOISPePtr +1) += xcseb;
              *(here->B3SOIEgPtr  +1) += xcegb;
              *(here->B3SOIGePtr  +1) += xcgeb;

              *(here->B3SOIEePtr  +1) += xceeb;

              *(here->B3SOIGgPtr  +1) += xcggb;
              *(here->B3SOIGdpPtr +1) += xcgdb;
              *(here->B3SOIGspPtr +1) += xcgsb;

              *(here->B3SOIDPgPtr +1) += xcdgb;
              *(here->B3SOIDPdpPtr +1) += xcddb;
              *(here->B3SOIDPspPtr +1) += xcdsb;

              *(here->B3SOISPgPtr +1) += xcsgb;
              *(here->B3SOISPdpPtr +1) += xcsdb;
              *(here->B3SOISPspPtr +1) += xcssb;

              *(here->B3SOIBePtr +1) += xcbeb;
              *(here->B3SOIBgPtr +1) += xcbgb;
              *(here->B3SOIBdpPtr +1) += xcbdb;
              *(here->B3SOIBspPtr +1) += xcbsb;

              *(here->B3SOIEbPtr  +1) -= xcegb + xceeb + xcedb + xcesb;

              *(here->B3SOIGbPtr +1) -= xcggb + xcgdb + xcgsb + xcgeb;
              *(here->B3SOIDPbPtr +1) -= xcdgb + xcddb + xcdsb + xcdeb;
              *(here->B3SOISPbPtr +1) -= xcsgb + xcsdb + xcssb + xcseb;
              *(here->B3SOIBbPtr +1) -= xcbgb + xcbdb + xcbsb + xcbeb;

              if (selfheat)
              {
                 *(here->B3SOITemptempPtr + 1) += xcTt;
                 *(here->B3SOIDPtempPtr + 1) += xcdT;
                 *(here->B3SOISPtempPtr + 1) += xcsT;
                 *(here->B3SOIBtempPtr + 1) += xcbT;
                 *(here->B3SOIEtempPtr + 1) += xceT;
                 *(here->B3SOIGtempPtr + 1) += xcgT;
              }
                                                               
 

              *(here->B3SOIEePtr) += 0.0;

              *(here->B3SOIDPgPtr) += Gm + gddpg;
              *(here->B3SOIDPdpPtr) += gdpr + gds + gddpdp + RevSum ;
              *(here->B3SOIDPspPtr) -= gds + FwdSum - gddpsp;
              *(here->B3SOIDPdPtr) -= gdpr;

              *(here->B3SOISPgPtr) -= Gm - gsspg;
              *(here->B3SOISPdpPtr) -= gds + RevSum - gsspdp;
              *(here->B3SOISPspPtr) += gspr + gds + FwdSum + gsspsp;
              *(here->B3SOISPsPtr) -= gspr;

              *(here->B3SOIBePtr) += 0;
              *(here->B3SOIBgPtr)  += gbbg;
              *(here->B3SOIBdpPtr) += gbbdp;
              *(here->B3SOIBspPtr) += gbbsp;
	      *(here->B3SOIBbPtr) += gbbb;
              *(here->B3SOIEbPtr) += 0.0;
              *(here->B3SOISPbPtr) -= Gmbs - gsspb; 
              *(here->B3SOIDPbPtr) -= (-gddpb - Gmbs);

              if (selfheat)
              {
                 *(here->B3SOIDPtempPtr) += GmT + gddpT;
                 *(here->B3SOISPtempPtr) += -GmT + gsspT;
                 *(here->B3SOIBtempPtr) += gbbT;

                 *(here->B3SOITemptempPtr) += gTtt + 1/here->pParam->B3SOIrth;
                 *(here->B3SOITempgPtr) += gTtg;
                 *(here->B3SOITempbPtr) += gTtb;
                 *(here->B3SOITempdpPtr) += gTtdp;
                 *(here->B3SOITempspPtr) += gTtsp;
              }


              *(here->B3SOIDdPtr) += gdpr;
              *(here->B3SOIDdpPtr) -= gdpr;
              *(here->B3SOISsPtr) += gspr;
              *(here->B3SOISspPtr) -= gspr;


              if (here->B3SOIbodyMod == 1) {
                 (*(here->B3SOIBpPtr) -= gppp);
                 (*(here->B3SOIPbPtr) += gppb);
                 (*(here->B3SOIPpPtr) += gppp);
              }
              if (here->B3SOIdebugMod != 0)
              {
                      *(here->B3SOIVbsPtr) += 1;
                      *(here->B3SOIIdsPtr) += 1;
                      *(here->B3SOIIcPtr) += 1;
                      *(here->B3SOIIbsPtr) += 1;
                      *(here->B3SOIIbdPtr) += 1;
                      *(here->B3SOIIiiPtr) += 1;
                      *(here->B3SOIIgidlPtr) += 1;
                      *(here->B3SOIItunPtr) += 1;
                      *(here->B3SOIIbpPtr) += 1;
                      *(here->B3SOICbgPtr) += 1;
                      *(here->B3SOICbbPtr) += 1;
                      *(here->B3SOICbdPtr) += 1;
                      *(here->B3SOIQbfPtr) += 1;
                      *(here->B3SOIQjsPtr) += 1;
                      *(here->B3SOIQjdPtr) += 1;

              }

        }
    }
    return(OK);
}

