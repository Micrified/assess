/*
 *******************************************************************************
 *                                  Micrified
 * FILENAME:    diff.h
 * PROGRAMMER:  Randolph, Charles
 * CREATED:     21/07/2017
 * DESCRIPTION: Header file for buffer analysis functions.
 *******************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "colors.h"

#if !defined(diff_h)
#define diff_h

/*
 ********************************************************************************
 *                                   PROTOTYPES
 ********************************************************************************
 */

/* Returns 0 if lines are identical. Else returns column number of first 
 differing character */
static int diffline (const char *abuf, const char *bbuf);

/* Returns 0 if no difference exists between files. Else returns the last 
 differing line number. If output file is non-null, all differing lines are 
 written */
int diff (const char *af, const char *bf, FILE *afp, FILE *bfp, FILE *ofp);


#endif
