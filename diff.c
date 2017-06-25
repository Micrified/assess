/*
 *******************************************************************************
 *                                  Micrified
 * FILENAME:    diff.c
 * PROGRAMMER:  Randolph, Charles
 * CREATED:     21/07/2017
 * DESCRIPTION: Implementation file for buffer analysis functions.
 *******************************************************************************
 */
#include "diff.h"

#define MAXBUF 1000

/*
 ********************************************************************************
 *                           FUNCTION IMPLEMENTATIONS
 ********************************************************************************
 */

/* Prints the provided buffer, outputs whitespace in character representation. */
static void printline (const char *buf, FILE *ofp) {
    int c;
    do {
        c = *(buf++);
        if (isspace(c)) {
            switch (c) {
                case ' ':
                    fprintf(ofp, C_TAF(DIM, BLK, "_"));
                    break;
                case '\t':
                    fprintf(ofp, C_TAF(DIM, BLK, "\\t"));
                    break;
                case '\n':
                    fprintf(ofp, C_TAF(DIM, BLK, "\\n"));
                    break;
                case '\v':
                    fprintf(ofp, C_TAF(DIM, BLK, "\\v"));
                    break;
                case '\f':
                    fprintf(ofp, C_TAF(DIM, BLK, "\\f"));
                    break;
                case '\r':
                    fprintf(ofp, C_TAF(DIM, BLK, "\\r"));
                    break;
            }
        } else {
            putc(c, ofp);
        }
        
    } while (c != '\n');
    fflush(ofp);
}

/* Returns 0 if lines are identical. Else returns column number of first differing 
 character */
static int diffline (const char *abuf, const char *bbuf) {
    int ncol = 1;
    while (*abuf != '\0' && *bbuf != '\0' && *abuf == *bbuf) {
        abuf++; bbuf++; ncol++;
    }
    return ((*abuf == '\0' && *bbuf == '\0') ? 0 : ncol);
}

/* Returns 0 if no difference exists between files. Else returns the last 
 differing line number. If output file is non-null, all differing lines are 
 written */
int diff (const char *af, const char *bf, FILE *afp, FILE *bfp, FILE *ofp) {
    size_t len_a, len_b, nline, last, ncol;
    char abuf[MAXBUF], bbuf[MAXBUF], *a_state, *b_state;
    
    len_a = len_b = nline = last = ncol = 0;
    
    do {
        a_state = fgets(abuf, MAXBUF, afp);
        b_state = fgets(bbuf, MAXBUF, bfp);
        nline++;
        if ((ncol = diffline(abuf, bbuf))) {
            last = nline;
            if (ofp != NULL) {
                fprintf(ofp, "difference on line %zu, column %zu\n(%s): ", nline, ncol, af);
                printline(abuf, ofp);
                fprintf(ofp, "\n(%s): ", bf);
                printline(bbuf, ofp);
                putchar('\n');
            }
        }
    } while (a_state != NULL && b_state != NULL);
    
    return last;
}
