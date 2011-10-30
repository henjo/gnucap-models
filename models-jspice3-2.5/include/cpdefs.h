/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Wayne A. Christopher, U. C. Berkeley CAD Group
**********/

/*
 * General stuff for the C-shell parser.
 */

/* Standard definitions */
#ifndef CPDEFS
#define CPDEFS

#include "cpstd.h"

#define MAXWORDS 512

/* Information about spice commands. */

struct comm {
    char *co_comname;   /* The name of the command. */
    void (*co_func) (); /* The function that handles the command. */
    bool co_stringargs; /* Collapse the arguments into a string. */
    bool co_spiceonly;  /* These can't be used from nutmeg. */
    bool co_major;      /* Is this a "major" command? */
    long co_cctypes[4]; /* Bitmasks for command completion. */
    unsigned int co_env;/* print help message on this environment mask */
    int co_minargs;     /* minimum number of arguments required */
    int co_maxargs;     /* maximum number of arguments allowed */
    int (*co_argfn) (); /* The fn that prompts the user. */
    char *co_help;      /* When these are printed, printf(string, av[0]) .. */
};

#define LOTS        1000

/* The history list. Things get put here after the first (basic) parse.
 * The word list will change later, so be sure to copy it.
 */

struct histent {
    int hi_event;
    wordlist *hi_wlist;
    struct histent *hi_next;
    struct histent *hi_prev;
};

/* The values returned by cp_userset(). */

#define US_OK           1   /* Either not relevant or nothing special. */
#define US_READONLY     2   /* Complain and don't set this var. */
#define US_DONTRECORD   3   /* Ok, but don't keep track of this one. */

/* Aliases. These will be expanded if the word is the first in an input
 * line. The substitution string may contain arg selectors.
 */

struct alias {
    char *al_name;        /* The word to be substituted for. */
    wordlist *al_text;    /* What to substitute for it. */
    struct alias *al_next;
    struct alias *al_prev;
} ;

/* The current record of what characters are special. */

#define CPC_BRR  004 /* Break word to right of character. */
#define CPC_BRL  010 /* Break word to left of character. */

/* For quoting individual characters. '' strings are all quoted, but `` and
 * "" strings are maintained as single words with the quotes around them.
 * Note that this won't work on non-ascii machines.
 */

#define quote(c)    ((c) | 0200)
#define strip(c)    ((c) & 0177)

#define CT_ALIASES  1
#define CT_LABEL    15

/* Get all the extern definitions... */

#include "cpextern.h"

#endif /*CPDEFS*/
