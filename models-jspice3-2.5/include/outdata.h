/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1992 Stephen R. Whiteley
****************************************************************************/

/*
 * Data structure passed to the OUT init routines.
 */

struct sOUTdata {

    GENERIC *circuitPtr;
    GENERIC *analysisPtr;
    IFuid analName;
    IFuid refName;
    int refType;
    int numNames;
    IFuid *dataNames;
    int dataType;
    GENERIC **plotPtr;
    int numPts;
    int count;
    double initValue;
    double finalValue;
    double step;
};



/* This structure allows external control over the outputting of data
 * for use with operating range analysis, threshold analysis, and
 * optimization routines.
 */

struct sOUTcontrol {
    int     out_check;
    int     out_usecurplot;
    int     out_keepplot;
    double  *out_points;
    int     out_index;
    int     out_max;
    int     out_fail;
    char    *out_rundesc;
#ifdef __STDC__
    int     (*out_evaluate)(void);
    void    (*out_end)(void);
    void    (*out_destroy)(void);
#else
    int     (*out_evaluate)();
    void    (*out_end)();
    void    (*out_destroy)();
#endif
};


/*
 * int  out_check      Set when under control of analysis driver by
 *                     the driver, evaluate pass/fail for margin analysis.
 *                     Keep only the latest output.
 *
 * int  out_usecurplot Set when under control of analysis driver by
 *                     the driver, use current plot rather than starting
 *                     a new one during init.
 *
 * int  out_keepplot   Set when under control of analysis driver by
 *                     the driver, keep the old plot and rundesc for
 *                     reuse.
 *
 * double *out_points  Pointer to an array of running variable (e.g,
 *                     time) points at which the output conditionals
 *                     are to be evaluated, loaded by driver. Only
 *                     used if out_check is true.
 *
 * int out_index       The current index into the array, incremented
 *                     by simulator when the running variable exceeds
 *                     a check point. Only used if out_check is true.
 *
 * int out_max         The size of the check point array, set by driver.
 *                     Only used if out_check is true;
 *
 * int  out_fail       Set if the conditionals failed, in the evaluate
 *                     function or globally to stop analysis. Only used if
 *                     out_check is true.
 *
 * out_evaluate();     Evaluation function, returns true on failure,
 *                     supplied by driver. Only used if out_check is true.
 *
 * out_end();          Called upon completion of each margin anlysis point,
 *                     supplied by driver.  Only used if out_check is true.
 *
 * out_destroy();      Called when analysis is done, performs cleanup,
 *                     supplied by simulator.
 */
