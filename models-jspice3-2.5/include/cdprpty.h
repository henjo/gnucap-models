/***************************************************************************
SCED - Schematic Capture Editor
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1981 Giles C. Billingsley, Ken Keller  (parts of KIC layout editor)
         1992 Stephen R. Whiteley
****************************************************************************/
 
/*
 * Property values
 */
#define P_NODE   10
#define P_NAME   11
#define P_MUT    13
#define P_BRANCH 15
#define P_MODEL  1
#define P_VALUE  2
#define P_INITC  3
#define P_OTHER  4

/* Union used in the prpty_Data field of the CD property desc */

struct prp_node {
    int enode;
    int inode;
    int x;
    int y;
};

struct prp_mut {
    int x1;
    int y1;
    int x2;
    int y2;
    double coeff;
};

struct prp_branch {
    int x;
    int y;
    int r1;
    int r2;
};

struct prp_name {
    char *name;
    int num;
    char *subname;
};

union prp_data {
    struct prp_node p_node;
    struct prp_mut p_mut;
    struct prp_branch p_branch;
    struct prp_name p_name;
};


/*
 * Propety List desc.
 */
struct prpty {
    char *prpty_String;
    char *prpty_Info;
    union prp_data *prpty_Data;
    struct prpty *prpty_Succ;
    int prpty_Value;
};
