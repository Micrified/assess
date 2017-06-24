/*
 *******************************************************************************
 *                                  Micrified
 * FILENAME:    assess.h
 * PROGRAMMER:  Randolph, Charles
 * CREATED:     21/07/2017
 * DESCRIPTION: Header file for program assessment file.
 *******************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include "config.h"
#include "fmanager.h"
#include "diff.h"
#include "colors.h"
#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

#if !defined(assess_h)
#define assess_h

/*
 *******************************************************************************
 *                                  PROTOTYPES
 *******************************************************************************
 */

/* Parses argument vector, sets global flag object. Returns NULL if successful, 
 else a pointer to illegal option in argv */
static const char *getflags (int argc, const char *argv[]);

/* Performs an assessment of the given file and writes the result to stdout. 
 Returns zero if successful. Else returns nonzero. */
int assessProgram (const char *infile, const char *outfile, const char *time,
                   const char *compileInstruction, const char *executable);

/* Returns zero if a test file 'f' could be successfully assessed. Else returns 
 nonzero */
int assessTestFile (const char *f);

#endif
