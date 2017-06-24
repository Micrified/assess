/*
 *******************************************************************************
 *                                  Micrified
 * FILENAME:    config.c
 * PROGRAMMER:  Randolph, Charles
 * CREATED:     24/07/2017
 * DESCRIPTION: Implementation file for testfile configuration program.
 *******************************************************************************
 */

#include "config.h"

#define SYMPATH     "/usr/local/bin"

FILE *fp = NULL;
int exitcode;

/*
 *******************************************************************************
 *                           FUNCTION IMPLEMENTATIONS
 *******************************************************************************
 */

/* Creates a testfile if none exists. Returns zero if successful, else nonzero. */
int init (void) {

    if ((fp = fopen(DEF_EXT, "r")) != NULL) {
        fprintf(stderr, C_TAF(BOL, MAG, "Note:") " a testfile already exists.\n");
        fclose(fp);
        return 0;
    } else {
        fclose(fp);
    }
    
    fp = fopen(DEF_EXT, "w");
    fclose(fp);
    if (ferror(fp)) {
        fprintf(stderr, C_TAF(BOL, RED, "error:") " problem creating testfile. Check permissions\n");
    } else {
        fprintf(stderr, C_TAF(BOL, MAG, "Note:") " testfile created.\n");
    }
    return ferror(fp);
}

/* Removes a testfile if one exists. Returns zero if successful, else nonzero. */
int delete (void) {
    if ((exitcode = system("rm " DEF_EXT))) {
        fprintf(stderr, C_TAF(BOL, RED, "error:") " nonzero exit code for rm .tsf.\n");
    } else {
        fprintf(stderr, C_TAF(BOL, MAG, "Note:") " Removal complete.\n");
    }
    return exitcode;
}

/* Attempts to uninstall the assess symlink from /usr/local/bin. Returns zero
 if successful, else nonzero. */
int uninstall (void) {
    if ((exitcode = system("rm " SYMPATH "/assess"))) {
        fprintf(stderr, C_TAF(BOL, RED, "error:") " nonzero exit code for rm " SYMPATH "/assess\n");
    } else {
        fprintf(stderr, C_TAF(BOL, MAG, "Note:") " Program uninstalled from " SYMPATH "\n");
    }
    return exitcode;
}
