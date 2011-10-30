/***  B4SOI 11/30/2005 Xuemei (Jane) Xi Release   ***/

static char rcsid[] = "$Id: b4soipzld.c  11/30/2005 Jane Xi Release $";

/**********
 * Copyright 2005 Regents of the University of California.  All rights reserved.
 * Authors: 1998 Samuel Fung, Dennis Sinitsky and Stephen Tang
 * Authors: 1999-2004 Pin Su, Hui Wan, Wei Jin, b3soipzld.c
 * Authors: 2005- Hui Wan, Xuemei Xi, Ali Niknejad, Chenming Hu.
 * File: b4soipzld.c
 * Modified by Hui Wan, Xuemei Xi 11/30/2005
 **********/

#include "spice.h"
#include <stdio.h>
#include "cktdefs.h"
#include "complex.h"
#include "sperror.h"
#include "b4soidef.h"
#include "suffix.h"

int
B4SOIpzLoad(inModel,ckt,s)
GENmodel *inModel;
register CKTcircuit *ckt;
register SPcomplex *s;
{
register B4SOImodel *model = (B4SOImodel*)inModel;
register B4SOIinstance *here;
double xcggb, xcgdb, xcgsb, xcbgb, xcbdb, xcbsb, xcddb, xcssb, xcdgb;
double gdpr, gspr, gds, gbd, gbs, capbd, capbs, xcsgb, xcdsb, xcsdb;
double cggb, cgdb, cgsb, cbgb, cbdb, cbsb, cddb, cdgb, cdsb;
double GSoverlapCap, GDoverlapCap, GBoverlapCap;
double FwdSum, RevSum, Gm, Gmbs;

    for (; model != NULL; model = model->B4SOInextModel) 
    {    for (here = model->B4SOIinstances; here!= NULL;
              here = here->B4SOInextInstance) 
	 {
            if (here->B4SOImode >= 0) 
	    {   Gm = here->B4SOIgm;
		Gmbs = here->B4SOIgmbs;
		FwdSum = Gm + Gmbs;
		RevSum = 0.0;
                cggb = here->B4SOIcggb;
                cgsb = here->B4SOIcgsb;
                cgdb = here->B4SOIcgdb;

                cbgb = here->B4SOIcbgb;
                cbsb = here->B4SOIcbsb;
                cbdb = here->B4SOIcbdb;

                cdgb = here->B4SOIcdgb;
                cdsb = here->B4SOIcdsb;
                cddb = here->B4SOIcddb;
            }
	    else
	    {   Gm = -here->B4SOIgm;
		Gmbs = -here->B4SOIgmbs;
		FwdSum = 0.0;
		RevSum = -Gm - Gmbs;
                cggb = here->B4SOIcggb;
                cgsb = here->B4SOIcgdb;
                cgdb = here->B4SOIcgsb;

                cbgb = here->B4SOIcbgb;
                cbsb = here->B4SOIcbdb;
                cbdb = here->B4SOIcbsb;

                cdgb = -(here->B4SOIcdgb + cggb + cbgb);
                cdsb = -(here->B4SOIcddb + cgsb + cbsb);
                cddb = -(here->B4SOIcdsb + cgdb + cbdb);
            }
            gdpr=here->B4SOIdrainConductance;
            gspr=here->B4SOIsourceConductance;
            gds= here->B4SOIgds;
            gbd= here->B4SOIgjdb;
            gbs= here->B4SOIgjsb;
#ifdef BULKCODE
            capbd= here->B4SOIcapbd;
            capbs= here->B4SOIcapbs;
#endif
	    GSoverlapCap = here->B4SOIcgso;
	    GDoverlapCap = here->B4SOIcgdo;
#ifdef BULKCODE
	    GBoverlapCap = here->pParam->B4SOIcgbo;
#endif

            xcdgb = (cdgb - GDoverlapCap);
            xcddb = (cddb + capbd + GDoverlapCap);
            xcdsb = cdsb;
            xcsgb = -(cggb + cbgb + cdgb + GSoverlapCap);
            xcsdb = -(cgdb + cbdb + cddb);
            xcssb = (capbs + GSoverlapCap - (cgsb+cbsb+cdsb));
            xcggb = (cggb + GDoverlapCap + GSoverlapCap + GBoverlapCap);
            xcgdb = (cgdb - GDoverlapCap);
            xcgsb = (cgsb - GSoverlapCap);
            xcbgb = (cbgb - GBoverlapCap);
            xcbdb = (cbdb - capbd);
            xcbsb = (cbsb - capbs);


            *(here->B4SOIGgPtr ) += xcggb * s->real;
            *(here->B4SOIGgPtr +1) += xcggb * s->imag;
            *(here->B4SOIBbPtr ) += (-xcbgb-xcbdb-xcbsb) * s->real;
            *(here->B4SOIBbPtr +1) += (-xcbgb-xcbdb-xcbsb) * s->imag;
            *(here->B4SOIDPdpPtr ) += xcddb * s->real;
            *(here->B4SOIDPdpPtr +1) += xcddb * s->imag;
            *(here->B4SOISPspPtr ) += xcssb * s->real;
            *(here->B4SOISPspPtr +1) += xcssb * s->imag;
            *(here->B4SOIGbPtr ) += (-xcggb-xcgdb-xcgsb) * s->real;
            *(here->B4SOIGbPtr +1) += (-xcggb-xcgdb-xcgsb) * s->imag;
            *(here->B4SOIGdpPtr ) += xcgdb * s->real;
            *(here->B4SOIGdpPtr +1) += xcgdb * s->imag;
            *(here->B4SOIGspPtr ) += xcgsb * s->real;
            *(here->B4SOIGspPtr +1) += xcgsb * s->imag;
            *(here->B4SOIBgPtr ) += xcbgb * s->real;
            *(here->B4SOIBgPtr +1) += xcbgb * s->imag;
            *(here->B4SOIBdpPtr ) += xcbdb * s->real;
            *(here->B4SOIBdpPtr +1) += xcbdb * s->imag;
            *(here->B4SOIBspPtr ) += xcbsb * s->real;
            *(here->B4SOIBspPtr +1) += xcbsb * s->imag;
            *(here->B4SOIDPgPtr ) += xcdgb * s->real;
            *(here->B4SOIDPgPtr +1) += xcdgb * s->imag;
            *(here->B4SOIDPbPtr ) += (-xcdgb-xcddb-xcdsb) * s->real;
            *(here->B4SOIDPbPtr +1) += (-xcdgb-xcddb-xcdsb) * s->imag;
            *(here->B4SOIDPspPtr ) += xcdsb * s->real;
            *(here->B4SOIDPspPtr +1) += xcdsb * s->imag;
            *(here->B4SOISPgPtr ) += xcsgb * s->real;
            *(here->B4SOISPgPtr +1) += xcsgb * s->imag;
            *(here->B4SOISPbPtr ) += (-xcsgb-xcsdb-xcssb) * s->real;
            *(here->B4SOISPbPtr +1) += (-xcsgb-xcsdb-xcssb) * s->imag;
            *(here->B4SOISPdpPtr ) += xcsdb * s->real;
            *(here->B4SOISPdpPtr +1) += xcsdb * s->imag;
            *(here->B4SOIDdPtr) += gdpr;
            *(here->B4SOISsPtr) += gspr;
            *(here->B4SOIBbPtr) += gbd+gbs;
            *(here->B4SOIDPdpPtr) += gdpr+gds+gbd+RevSum;
            *(here->B4SOISPspPtr) += gspr+gds+gbs+FwdSum;
            *(here->B4SOIDdpPtr) -= gdpr;
            *(here->B4SOISspPtr) -= gspr;
            *(here->B4SOIBdpPtr) -= gbd;
            *(here->B4SOIBspPtr) -= gbs;
            *(here->B4SOIDPdPtr) -= gdpr;
            *(here->B4SOIDPgPtr) += Gm;
            *(here->B4SOIDPbPtr) -= gbd - Gmbs;
            *(here->B4SOIDPspPtr) -= gds + FwdSum;
            *(here->B4SOISPgPtr) -= Gm;
            *(here->B4SOISPsPtr) -= gspr;
            *(here->B4SOISPbPtr) -= gbs + Gmbs;
            *(here->B4SOISPdpPtr) -= gds + RevSum;

        }
    }
    return(OK);
}


