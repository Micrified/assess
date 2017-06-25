/*
 *******************************************************************************
 *                                  Micrified
 * FILENAME:    assess.c
 * PROGRAMMER:  Randolph, Charles
 * CREATED:     21/07/2017
 * DESCRIPTION: Core file assessment program.
 *******************************************************************************
 */

#include "assess.h"

#define MAXLINE             1000

/*
 *******************************************************************************
 *                           PRIVATE TYPE DECLARATIONS
 *******************************************************************************
 */

typedef enum {
    config_help = 1,
    config_init,
    config_delete,
    config_uninstall
} ConfigOption;

typedef struct {
    const char      *withFile;
    const char      *withTime;
    const char      *withIn;
    const char      *withOut;
    const char      *withComp;
    const char      *withEx;
    ConfigOption    withConfig;
    FILE            *withDiff;
} FlagSet;

/*
 *******************************************************************************
 *                              PRIVATE VARIABLES
 *******************************************************************************
 */

/* Output describes how to use assess */
const char *usage = "\nUsing the Assess program:\n\n \
" C_TAF(UND, MAG, "PROGRAM TOOLS:") "\n\n \
\t" C_TF(RED, "-help") "\t\t\t Displays current manual.\n \
\t" C_TF(RED, "-init") "\t\t\t Initializes a new .tsf testfile.\n \
\t" C_TF(RED, "-delete") "\t\t\t Delete local .tsf if any exists.\n \
\t" C_TF(RED, "-uninstall") "\t\t Uninstalls symlink from /usr/local/bin.\n\n \
" C_TAF(UND, MAG, "PROGRAM MODES:") "\n\n \
" C_TAF(BOL, MAG, "MODE 0:") " DEFAULT PROGRAM EVALUATION\n \
\t (When no arguments are provided)\n \
\t Searches for .tsf (testfile) file in the current directory.\n\n \
" C_TAF(BOL,MAG,"MODE 1:") " SINGLE PROGRAM EVALUATION\n \
\t (When arguments provided, max 1000 char).\n \
\t" C_TF(RED, "-i=") "<infile>\t\t The file to be read by the program.\n \
\t" C_TF(RED, "-o=") "<outfile>\t\t The expected output for the program.\n \
\t" C_TF(RED, "-c=") "<instruction string>\t The compilation instruction.\n \
\t" C_TF(RED, "-x=") "<instruction string>\t The execution instruction.\n \
\t" C_TF(RED, "-t=") "<timelimit>\t\t The time limit (in milliseconds).\n\n \
" C_TAF(BOL,MAG,"MODE 2:") " TEST FILE EVALUATION\n \
\t" C_TF(RED, "-f=") "<testfile>.tsf\t The test file holding instructions.\n\n \
" C_TAF(UND, MAG, "OPTIONAL ARGUMENTS:") "\n \
\t" C_TF(RED, "-d") "\t descriptive flag signals to output differences found.\n\n \
" C_TAF(UND,MAG, "TEST FILE CONIGURATION:") "\n \
\t A testfile contains a sequence of lines, each including the necessary arguments\n \
\t required for compiling, executing, and evaluating the output against the given\n  \
\t input file delimited by semicolon ';'. Note that including time is OPTIONAL.\n\n \
\t " C_TA(UND, "Example line:") " i=in.txt; o=out.txt; c=clang foo.c; x=./a.out; t=42;\n \
\t (line max 5000 char)\n\n";

/*
 * cbuf: command buffer: Stores constructed system call instructions.
 * fbuf: file buffer: Stores a file path.
 */
char fbuf[MAXLINE], cbuf[MAXLINE];

/* Stores all program supplied flags. */
FlagSet flags;

/*
 *******************************************************************************
 *                                     MAIN
 *******************************************************************************
 */

int main (int argc, const char *argv[]) {
    const char *illegal = NULL;
    
    if ((illegal = getflags(argc, argv))) {
        fprintf(stderr, C_TAF(BOL, RED, "error:") " illegal argument: %s\n", illegal);
        return 0;
    }
    
    if (flags.withConfig) {
        switch (flags.withConfig) {
            case config_help:
                fputs(usage, stdout);
                break;
            case config_init:
                return init();
            case config_delete:
                return delete();
            case config_uninstall:
                return uninstall();
        }
        return 0;
    }
    
    if (flags.withFile) {
        if (flags.withTime || flags.withIn || flags.withOut || flags.withComp || flags.withEx) {
            fprintf(stdout, C_TAF(BOL, YEL, "warning:") " program flags ignored when assessing by file!\n");
        }
        assessTestFile(flags.withFile);
    } else {
        if (argc == 1 || (argc == 2 && flags.withDiff)) {
            assessTestFile(DEF_EXT);
        } else if (!(flags.withOut && flags.withComp && flags.withEx)) {
            fprintf(stderr, C_TAF(BOL, RED, "error:") " incomplete/missing/bad arguments!\n");
        } else {
            assessProgram(flags.withIn, flags.withOut, flags.withTime, flags.withComp, flags.withEx);
        }
    }
    
    return 0;
}

/*
 *******************************************************************************
 *                              VECTOR ARG HANDLING
 *******************************************************************************
 */

/* Parses argument vector, sets flags. Returns NULL if successful. Else a pointer to illegal option */
static const char *getflags (int argc, const char *argv[]) {
    const char *sp = NULL;
    while (--argc > 0 && (*(++argv))[0] == '-') {
        sp = ++(*argv);
        switch (*sp) {
            case 'c':
                flags.withComp = sp + 2;
                break;
            case 'd':
                if (strcmp(sp, "d") == 0) {
                    flags.withDiff = stdout;
                    break;
                }
                if (strcmp(sp, "delete") == 0) {
                    flags.withConfig = config_delete;
                    break;
                }
                return --sp;
            case 'f':
                flags.withFile = sp + 2;
                break;
            case 'h':
                if (strcmp(sp, "help")) {
                    return --sp;
                }
                flags.withConfig = config_help;
                break;
            case 'i':
                if (strcmp(sp, "i") == 0) {
                    flags.withIn = sp + 2;
                    break;
                }
                if (strcmp(sp, "init") == 0) {
                    flags.withConfig = config_init;
                    break;
                }
                return --sp;
            case 'o':
                flags.withOut = sp + 2;
                break;
            case 't':
                flags.withTime = sp + 2;
                break;
            case 'u':
                if (strcmp(sp, "uninstall")) {
                    return --sp;
                }
                flags.withConfig = config_uninstall;
                break;
            case 'x':
                flags.withEx = sp + 2;
                break;
            default:
                return --sp;
                break;
        }
    }
    
    return NULL;
}

/*
 *******************************************************************************
 *                           FUNCTION IMPLEMENTATIONS
 *******************************************************************************
 */

/* Reads the current time to the timespec object provided. */
static void gettime (struct timespec *t) {
#ifdef __MACH__
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), REALTIME_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    t->tv_sec = mts.tv_sec;
    t->tv_nsec = mts.tv_nsec;
#else
    clock_gettime(CLOCK_MONOTONIC, t);
#endif
}

/* Outputs a formatted assessment result to stdout. */
static void printResult (const char *executable, int dline, double elapsed, double tmax) {
    if (dline) {
        fprintf(stdout, "Prgm:" C_TAF(BOL, MAG, "%-15s") " Status: " C_TAF(BOL, RED, "Failed") ".\n", executable);
        return;
    }
    if (tmax) {
        if (elapsed > tmax / 1000) {
            fprintf(stdout, "Prgm:" C_TAF(BOL, MAG, "%-15s") " Status: " C_TAF(BOL, YEL, "Time Limit Exceeded (%fs > %fs)") ".\n", executable, elapsed, tmax / 1000);
        } else {
            fprintf(stdout, "Prgm:" C_TAF(BOL, MAG, "%-15s") " Status: " C_TAF(BOL, GRN, "Passed (%fs <= %fs)") ".\n", executable, elapsed, tmax / 1000);
        }
    } else {
        fprintf(stdout, "Prgm:" C_TAF(BOL, MAG, "%-15s") " Status: " C_TAF(BOL, GRN, "Passed") ".\n", executable);
    }
}

/* Performs an assessment of the given file and writes the result to stdout. Returns zero if successful. Else returns nonzero. */
int assessProgram (const char *infile, const char *outfile, const char *time, const char *compileInstruction, const char *executable) {
    FILE *ifp = NULL, *ofp, *xfp;
    long dline, exitcode;
    struct timespec tic, toc;
    double elapsed, tmax = 0;
    
    /* If input/output couldn't be read, time was specified incorrectly,
     or exection instruction is too long for the buffer, then fail. */
    if (infile != NULL && (ifp = fopen(infile, "r")) == NULL) { // infile is optional, throw error on non-null.
        fprintf(stderr, C_TAF(BOL, RED, "error:") " could not read file %s.\n", infile);
        return 1;
    } else if ((ofp = fopen(outfile, "r")) == NULL) {
        fprintf(stderr, C_TAF(BOL, RED, "error:") " could not read file %s.\n", outfile);
        return 1;
    } else if (time != NULL && (tmax = strtol(time, NULL, 10)) == 0) {
        fprintf(stderr, C_TAF(BOL, RED, "error:") " time was specified but was either zero or unreadable: %s\n", time);
        return 1;
    } else if ((exitcode = system(compileInstruction)) != 0) {
        fprintf(stderr, C_TAF(BOL, RED, "error:") " received nonzero exit code (%ld) for compile instruction: %s\n",
                exitcode, compileInstruction);
        return 1;
    } else if (strlen(executable) + (infile ? strlen(infile) : 0) + 6 >= MAXLINE) { // constant (6) = "./" + " < " + '\0'
        fprintf(stderr, C_TAF(BOL, RED, "error:") " execution command exceeds buffer!\n");
        return 1;
    } else {
        cbuf[0] = '\0';
        strcat(cbuf, "./");
        strcat(cbuf, executable);
        if (infile) {
            strcat(cbuf, " < ");
            strcat(cbuf, infile);
        }
    }
    
    /* Initialize timer, execute program and read stdout to xfp */
    gettime(&tic);
    if ((xfp = popen(cbuf, "r")) == NULL) {
        fprintf(stderr, C_TAF(BOL, RED, "error:") " program execution call error or insufficient memory: %s!\n", cbuf);
        return 1;
    }
    
    dline = diff(outfile, executable, ofp, xfp, flags.withDiff);
    pclose(xfp);
    gettime(&toc);
    elapsed = (double)(toc.tv_sec - tic.tv_sec) + (toc.tv_nsec - tic.tv_nsec) / 1E9;
    
    /* Output results */
    printResult(executable, dline, elapsed, tmax);
    
    fclose(ofp);
    if (infile != NULL) {
        fclose(ifp);
    }
    
    return (dline || (time && elapsed > tmax));
}

/* Returns zero if a test file 'f' could be successfully assessed. Else returns nonzero */
int assessTestFile (const char *f) {
    const char *infile, *outfile, *time, *compileInstruction, *executable;
    infile = outfile = time = compileInstruction = executable = NULL;
    
    if (openFile(f)) {
        if (strcmp(f, DEF_EXT)) {
            fprintf(stderr, C_TAF(BOL, RED, "error:") " could not read file %s. Check it exists and program is run with sufficient permissions.\n", outfile);
        } else {
            fprintf(stderr, C_TAF(BOL, RED, "error:") " no existing testfile.\n" C_TAF(BOL, MAG, "Note: ") "To create a testfile and perform other actions, consult the help page with: " C_TAF(BOL, RED, "-help\n"));
        }
        return 1;
    }
    
    while (getNext()) {
        if (parseNext(&infile, &outfile, &time, &compileInstruction, &executable)) {
            fprintf(stderr, C_TAF(BOL, RED, "error:") " Couldn't read line %d of file %s\n", getLine(), f);
        } else {
            assessProgram(infile, outfile, time, compileInstruction, executable);
        }
        infile = outfile = time = compileInstruction = executable = NULL;
    }
    
    closeFile();
    return 0;
}
