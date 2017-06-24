/*
 *******************************************************************************
 *                                  Micrified
 * FILENAME:    fmanager.h
 * PROGRAMMER:  Randolph, Charles
 * CREATED:     21/07/2017
 * DESCRIPTION: Header for file manager file.
 *******************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#if !defined(fmanager_h)
#define fmanager_h

/*
 ********************************************************************************
 *                                   PROTOTYPES
 ********************************************************************************
 */

/* Sets the file to be read. Returns zero if successful, nonzero otherwise */
int openFile (const char *f);

/* Closes any open file. Returns zero if successful, nonzero otherwise */
int closeFile (void);

/* Returns nonzero if a new line could be loaded, else returns zero */
int getNext (void);

/* Returns the current line number in the buffer */
int getLine (void);

/*
 * Parse the next line of the file. Assigns supplied pointers to desired targets 
 * within the buffer. Arguments per line are delimited with a semicolon ';'.
 ********************************************************************************
 * infile: Assigned to point to the given input file argument (-i=).
 * outfile: Assigned to point to the given output file argument (-o=).
 * time: Assigned to point to the given time argument (-t=) (optional).
 * compileInstruction: Assigned to point to the given compile instruction argument (-c=).
 * executable: Assigned to point to the given executable argument (-x=).
 ********************************************************************************
 * Returns zero if successful, nonzero otherwise
 */
int parseNext (const char **infile, const char **outfile, const char **time, const char **compileInstruction, const char **executable);

#endif
