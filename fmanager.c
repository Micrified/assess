/*
 *******************************************************************************
 *                                  Micrified
 * FILENAME:    fmanager.c
 * PROGRAMMER:  Randolph, Charles
 * CREATED:     21/07/2017
 * DESCRIPTION: Contains file parsing functions to offload work from assess.c
 *******************************************************************************
 */

#include "fmanager.h"

#define MAXLINE  5000

/*
 ********************************************************************************
 *                              PRIVATE VARIABLES
 ********************************************************************************
 */

/* File being read */
static FILE *fp;

/* Last line read */
static int line;

/* Local buffer */
static char b[MAXLINE + 1];
static int bp;

/*
 ********************************************************************************
 *                           FUNCTION IMPLEMENTATIONS
 ********************************************************************************
 */

/* Sets the file to be read. Returns zero if successful, nonzero otherwise */
int openFile (const char *f) {
    if (((fp = fopen(f, "r")) == NULL)) {
        return 1;
    }
    line = 0;
    return 0;
}

/* Closes any open file. Returns zero if successful, nonzero otherwise */
int closeFile (void) {
    return (fclose(fp) == EOF);
}

/* Returns nonzero if a new line could be loaded, else returns zero */
int getNext (void) {
    line++;
    return (fgets(b, MAXLINE, fp) != NULL);
}

/* Returns the current line number in the buffer */
int getLine (void) {
    return line;
}

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
static int getarg (const char **);
static int getLong (long *);

int parseNext (const char **infile, const char **outfile, const char **time, const char **compileInstruction, const char **executable) {
    int c, argc = 0;
    const char *sp;
    bp = 0;

    while ((c = getarg(&sp)) != '\0') {
        switch (c) {
            case 'c':
                *compileInstruction = sp;
                argc++;
                break;
            case 'i':
                *infile = sp;
                argc++;
                break;
            case 'o':
                *outfile = sp;
                argc++;
                break;
            case 't':
                *time = sp;
                break;
            case 'x':
                *executable = sp;
                argc++;
                break;
            default:
                return c;
                break;
        }
    }
    return !(argc >= 3); /* compile, outfile, and executable mandatory. Time and input files are not */
}

/* Finds next argument in buffer, appends null and assigns head to string pointer. Increments buffer pointer. Returns argument character */
static int getarg (const char **sp) {
    int c, valid;
    while (bp < MAXLINE && isspace(b[bp])) {
        bp++;
    }

    if (bp >= MAXLINE || !isalpha((c = b[bp++])) || b[bp++] != '=') {
        return '\0';
    }

    *sp = b + bp;
    while (bp < MAXLINE && b[bp] != ';') {
        bp++;
    }

    valid = (b[bp] == ';');
    b[bp++] = '\0'; /* Buffer extended 1 past MAX to account for this */

    return (valid ? c : '\0');
}
