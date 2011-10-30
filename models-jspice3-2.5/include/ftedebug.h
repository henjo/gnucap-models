/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Wayne A. Christopher, U. C. Berkeley CAD Group
**********/

/*
 *
 * Stuff for debugging a spice run. Debugging information will be printed
 * on the standard output.
 */

#define DB_SAVE      1   /* Save a node. */
#define DB_TRACE     2   /* Print the value of a node every iteration. */
#define DB_STOPAFTER 3   /* Break after this many iterations. */
#define DB_STOPWHEN  4   /* Break when a node reaches this value. */
#define DB_IPLOT     5   /* Incrementally plot the stuff. */
#define DB_IPLOTALL  6   /* Incrementally plot everything. */
#define DB_DEADIPLOT 7   /* Incrementally plot the stuff. */

struct dbcomm {
    int db_number;          /* The number of this debugging command. */
    char db_type;           /* One of the above. */
    char *db_nodename1;     /* What node. */
    char *db_nodename2;     /* What node. */
    int db_iteration;       /* For the DB_STOPAFTER command. */
    int db_graphid;         /* If iplot, id of graph. */
    struct dbcomm *db_also; /* Link for conjunctions. */
    struct dbcomm *db_next; /* List of active debugging commands. */
} ;

#ifdef __STDC__
extern void dbfree(struct dbcomm*);
#else
extern void dbfree( );
#endif
