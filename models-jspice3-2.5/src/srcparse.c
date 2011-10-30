/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1987 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

#include "spice.h"
#include <stdio.h>
#include "inpdefs.h"
#include "inpmacs.h"
#include "iferrmsg.h"

#ifdef __STDC__
static char *src_parse(char**,GENERIC*,int,GENERIC*,double*,int*,INPtables*);
#else
static char *src_parse();
#endif

void
SRCparse(type,ckt,tabp,currentp)

int  type;
GENERIC *ckt;
GENERIC *tabp;
GENERIC *currentp;
{
    /* parser for all voltage/current sources */

    /*
     *   'arbitrary' source:
     *   Aname <node> <node> [{"v" | "i"}=<expr>]
     *           [[DC] <val>] [AC [<val> [<val>]]]
     *
     *   'independent' voltage source:
     *   Vname <node> <node> [<expr>] [[DC] <val>] [AC [<val> [<val>]]]
     *
     *   'independent' current source:
     *   Iname <node> <node> [<expr>] [[DC] <val>] [AC [<val> [<val>]]]
     *
     *   'ccvs'
     *   Gname <node> <node> {"function" | <node> <node>} <expr>
     *           [[DC] <val>] [AC [<val> [<val>]]]
     *
     *   'vcvs'
     *   Ename <node> <node> {"function" | <node> <node>} <expr>
     *           [[DC] <val>] [AC [<val> [<val>]]]
     *
     *   'ccvs'
     *   Fname <node> <node> {"function" | <vname>} <expr>
     *           [[DC] <val>] [AC [<val> [<val>]]]
     *
     *   'cccs'
     *   Hname <node> <node> {"function" | <vname>} <expr>
     *           [[DC] <val>] [AC [<val> [<val>]]]
     *
     * General rules:
     *   For G,E,F,H the DC/AC values must appear after the expr (which
     *   is a number for linear dependent sources).
     *   Otherwise, the DC/AC specs can appear before or after the expr.
     *   If before, the numerical parameters must be given explicitly
     *   unless the following token prefix is manifestly not a number.
     *   The AC spec must be followed by something that is numerically
     *   not a number (vector parse not smart enough to stop).
     *   The expr must appear immediately after "v", "i", or "function".
     *   If "DC" is omitted, and an expr is given, the dc value must
     *   follow the expr.
     */

    INPtables *tab = (INPtables*)tabp;
    card *current = (card*)currentp;

    char *line;     /* the part of the current line left to parse */
    char *name;     /* the device's name */
    char *nname1;   /* the first node's name */
    char *nname2;   /* the second node's name */
    char *nname3;   /* the third node's name */
    char *nname4;   /* the fourth node's name */
    GENERIC *node1; /* the first node's node pointer */
    GENERIC *node2; /* the second node's node pointer */
    GENERIC *node3; /* the third node's node pointer */
    GENERIC *node4; /* the fourth node's node pointer */
    int error;      /* error code temporary */
    GENERIC *fast;  /* pointer to the actual instance */
    IFvalue ptemp;  /* a value structure to package resistance into */
    IFvalue *parm;  /* a pointer to value structure to pick things up into */
    int waslead;    /* flag to indicate that unlabeled number was found */
    double leadval; /* actual value of unlabeled number */
    IFuid uid;      /* uid of default model to be created */
    char *tl;       /* temporary */
    char *nm;
    char key;
    char *leadname = NULL;
    char *groundname = "0";
    char buf[128];

    line = current->line;
    key = *line;
    if (islower(key))
        key = toupper(key);
    INPgetTok(&line,&name,1);
    INPinsert(&name,tab);

    INPgetTok(&line,&nname1,1);
    INPtermInsert(ckt,&nname1,tab,&node1);

    INPgetTok(&line,&nname2,1);
    INPtermInsert(ckt,&nname2,tab,&node2);

    if (!tab->defAmod) {
        /* create default source model */
        IFnewUid(ckt,&uid,(IFuid)NULL,"A",UID_MODEL,(GENERIC**)NULL);
        IFC(newModel,(ckt,type,&(tab->defAmod),uid))
    }
    IFC(newInstance,(ckt,tab->defAmod,&fast,name))
    IFC(bindNode,(ckt,fast,1,node1))
    IFC(bindNode,(ckt,fast,2,node2))

    tfree(ft_sim->ptXalias);

    switch (key) {
        case 'A':
            break;
        case 'I':
        case 'V':
            /* set alias for "x" in function */
            ft_sim->ptXalias = copy("time");
            leadname = "dc";
            break;
        case 'E':
        case 'G':
            /* control nodes optional with function */
            tl = line;
            INPgetTok(&line,&nname3,1);
            if (strcmp(nname3,"function")) {
                INPtermInsert(ckt,&nname3,tab,&node3);
                INPgetTok(&line,&nname4,1);
                INPtermInsert(ckt,&nname4,tab,&node4);
                IFC(bindNode,(ckt,fast,3,node3))
                IFC(bindNode,(ckt,fast,4,node4))

                /* set alias for "x" in function */
                *buf = '\0';
                if (strcmp(nname3,groundname) && strcmp(nname4,groundname))
                    sprintf(buf,"v(%s,%s)",nname3,nname4);
                else if (strcmp(nname3,groundname))
                    sprintf(buf,"v(%s)",nname3);
                else if (strcmp(nname4,groundname))
                    sprintf(buf,"(-v(%s))",nname4);

                if (strlen(buf)) {
                    ft_sim->ptXalias = copy(buf);
                }
            }
            else {
                line = tl;
                txfree(nname3);
            }
            leadname = "gain";
            break;
        case 'F':
        case 'H':
            /* control device is optional with function */
            tl = line;
            INPgetTok(&tl,&nm,1);
            if (strcmp(nm,"function")) {
                parm = INPgetValue(ckt,&line,IF_INSTANCE,tab);
                GCA(INPpName,("control",parm,ckt,type,fast))

                /* set alias for "x" in function */
                sprintf(buf,"i(%s)",parm->uValue);
                ft_sim->ptXalias = copy(buf);
            }
            txfree(nm);
            leadname = "gain";
            break;
    }
    ptemp.iValue = key;
    GCA(INPpName,("type",&ptemp,ckt,type,fast))
    current->error = INPerrCat(current->error,
        src_parse(&line,ckt,type,fast,&leadval,&waslead,tab));
    if (waslead && leadname != NULL) {
        ptemp.rValue = leadval;
        GCA(INPpName,(leadname,&ptemp,ckt,type,fast))
    }
}


static char *
src_parse(line,ckt,dev,fast,leading,waslead,tab)

/* A special parser for source specifications.  The strategy is to
 * first extract the recognized keywords and variables from the string,
 * then examine what is left.  If only a number remains, the leadval
 * is set.  Otherwise, if anything remains, a parse tree is built.
 */

char **line;     /* the line to parse */
GENERIC *ckt;    /* the circuit this device is a member of */
int dev;         /* the device type code to the device being parsed */
GENERIC *fast;   /* direct pointer to device being parsed */
double *leading; /* the optional leading numeric parameter */
int *waslead;    /* flag - 1 if leading double given, 0 otherwise */
INPtables *tab;
{
    int error;  /* int to store evaluate error return codes in */
    char *parm;
    char *errbuf;
    char *place;
    int i;
    IFvalue *val;
    IFdevice *device = ft_sim->devices[dev];
    char *nline, *nend, *start, *end;

    nline = tmalloc(strlen(*line)+1);
    nend = nline;
    start = end = *line;
    *waslead = 0;

    while (**line != (char) 0) {

        /* get token, make sure we move past ')' */
        start = *line;
        error = INPgetTok(line,&parm,1);
        if (error)
            return (INPerror(error));
        while (**line == ')')
            (*line)++;
        end = *line;

        if (!isalpha(*parm)) {
            /* a keyword always starts with letter */
            txfree(parm);
            while (start < end)
                *nend++ = *start++;
            continue;
        }

        /* Have to be careful here.  If the token is "v" or "i", it
         * could be a parameter for asrc, or it could be part of the
         * function specification.
         */

        if ((*parm == 'v' || *parm == 'i') && strlen(parm) == 1) {
            char *s = start;
            while (*s != *parm) s++;
            s++;
            if (*s == '(' || *s == ')') {
                /* v(something) or i(something), or i(v) (source named
                 * "v"). Not a parameter.
                 */
                txfree(parm);
                while (start < end)
                    *nend++ = *start++;
                continue;
            }
        }

        for (i = 0; i < *device->numInstanceParms; i++) {
            if (!strcmp(parm,device->instanceParms[i].keyword)
                && (device->instanceParms[i].dataType & IF_SET)) {

                txfree(parm);
                val = INPgetValue(ckt,line,
                        device->instanceParms[i].dataType,tab);
                if (!val) {
                    return (INPerror(E_PARMVAL));
                }
                error = (*(ft_sim->setInstanceParm))(ckt,fast,
                        device->instanceParms[i].id,
                        val,(IFvalue*)NULL);
                if (error) {
                    return (INPerror(error));
                }
                break;
            }
        }
        if (i == *device->numInstanceParms) {
            txfree(parm);
            while (start < end)
                *nend++ = *start++;
        }
    }
    *nend = '\0';

    /* nline contains function spec and/or leadval */
    nend = nline;

    /* count tokens */
    i = 0;
    while (*nend != (char) 0) {
        error = INPgetTok(&nend,&parm,1);
        if (error)
            return (INPerror(error));
        if (*parm)
            i++;
        txfree(parm);
    }
    nend = nline;

    if (i == 0) {
        /* nothing left */
        txfree(nline);
        return ((char *)NULL);
    }

    /* check for leading value */
    *leading = INPevaluate(&nend,&error,0);
    if (error == 0) {
        /* found a good leading number */
        *waslead = 1;
        if (i == 1) {
            txfree(nline);
            return ((char *)NULL);
        }
        while (isalpha(*nend))
            nend++;
        while (isspace(*nend) || *nend == ',')
            nend++;
    }
    else {
        *leading = 0.0;
        nend = nline;
    }

    /* something parseable left */

    for (i = 0; i < *device->numInstanceParms; i++) {
        if (!strcmp("function",device->instanceParms[i].keyword)) {

            place = nend;
            val = INPgetValue(ckt,&nend,
                    device->instanceParms[i].dataType,tab);
            if (!val) {
                /* leading value was part of function, perhaps */
                if (*waslead == 1) {
                    errbuf = errMsg;
                    errMsg = NULL;
                    nend = nline;
                    val = INPgetValue(ckt,&nend,
                            device->instanceParms[i].dataType,tab);
                    if (!val || nend <= place) {
                        /* leadval asumed ok, later tokens bad */
                        if (val) {
                            IFfree(val->tValue);
                        }
                        txfree(errMsg);
                        errMsg = errbuf;
                        return (INPerror(E_SYNTAX));
                    }
                    /* leadval was part of function */
                    *waslead = 0;
                    *leading = 0.0;
                    txfree(errbuf);
                }
                else {
                    return (INPerror(E_SYNTAX));
                }
            }
            error = (*(ft_sim->setInstanceParm))(ckt,fast,
                    device->instanceParms[i].id,
                    val,(IFvalue*)NULL);
            if (error) {
                return (INPerror(error));
            }
            break;
        }
    }

    /* anything still there? */
    while (*nend) {
        if (!*waslead) {
            /* check for leading value after function */
            *leading = INPevaluate(&nend,&error,1);

            if (error == 0) {
                /* found a good leading number */
                *waslead = 1;
                continue;
            }
        }
        (void)INPgetTok(&nend,&parm,1);
        if (*parm) {
            errbuf = tmalloc(strlen(parm)+25);
            (void)sprintf(errbuf,"Unknown parameter (%s).",parm);
            txfree(parm);
            txfree(nline);
            return (errbuf);
        }
    }
    txfree(nline);
    return ((char *)NULL);
}
