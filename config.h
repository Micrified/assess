/*
 *******************************************************************************
 *                                  Micrified
 * FILENAME:    config.h
 * PROGRAMMER:  Randolph, Charles
 * CREATED:     24/07/2017
 * DESCRIPTION: Header file for testfile configuration program.
 *******************************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include "colors.h"

#define DEF_EXT     ".tsf"

#if !defined(config_h)
#define config_h

/*
 *******************************************************************************
 *                                  PROTOTYPES
 *******************************************************************************
 */

/* Creates a testfile if none exists. Returns zero if successful, else nonzero. */
int init (void);

/* Removes a testfile if one exists. Returns zero if successful, else nonzero. */
int delete (void);

/* Attempts to uninstall the assess symlink from /usr/local/bin. Returns zero
 if successful, else nonzero. */
int uninstall (void);



#endif
