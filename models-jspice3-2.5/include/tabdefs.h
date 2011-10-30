/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1992 Stephen R. Whiteley
****************************************************************************/

/* Structures used for keeping track of tables input by the user
 * with .table lines.
 */

/* defines for struct sCKTentry e_type */
#define ENT_NUMBER   1
#define ENT_TABLE    2
#define ENT_OMITTED  4

struct sCKTentry {

    /* table entry ordinate */
    double ent_val;

    /* table value, if real entry */
    double ent_real;

    /* pointer to next entry */
    struct sCKTentry *ent_next;

    /* generic pointer to function, table (or NULL) */
    char *ent_string;

    /* entry type (ENT_???) */
    int ent_type;
};

struct sCKTtable {

    /* name of table */
    char *tab_name;

    /* pointer to entry list head */
    struct sCKTentry * tab_entry;

    /* pointer to next table, if any */
    struct sCKTtable * tab_next;
};

#ifdef __STDC__
extern struct sCKTtable **DEVtableHead(GENERIC*);
#else
extern struct sCKTtable **DEVtableHead();
#endif
