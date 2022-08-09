#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "decrypt.h"
#include "menu.h"
#include "settings.h"
#include "timeMeasurement.h"
#include "vcAlg01_deterministic.h"
#include "vcAlg02_probabilistic.h"
#include "vcAlg03_randomGrid.h"
#include "vcAlgorithms.h"

#define EXIT_ON_HELP 2

// Global
char *sourcePath = NULL;
char *sharePath = NULL;

/*********************************************************************
 * Function:     usage
 *--------------------------------------------------------------------
 * Description:  Print program usage.
 ********************************************************************/
static inline void usage() {
    fprintf(stdout,
            "Usage:\n"
            "visualCrypt [options] <parameters>\n\n"
            "Options:\n"
            " -h                            display this help\n"
            " -s <source path>              set path to a secret .bmp\n"
            " -d <destination path>         set path to a result storing directory\n\n");
}

/*********************************************************************
 * Function:     getPathsFromProgramParameter
 *--------------------------------------------------------------------
 * Description:  Check the program parameters and set the global paths
 *				 according to them.
 * Return:       0 on success, 1 on failure, 2 for help options.
 ********************************************************************/
static int getPathsFromProgramParameter(int argc, char *argv[]) {
    int c = '?';
    while ((c = getopt(argc, argv, "hs:d:")) != -1) {
        switch (c) {
            case 'h':
                usage();
                return EXIT_ON_HELP;
            case 's':
                sourcePath = optarg;
                break;
            case 'd':
                sharePath = optarg;
                break;
            case ':':
                fprintf(stderr, "ERR: option -%c requires an operand\n", optopt);
                return EXIT_FAILURE;
            case '?':
                fprintf(stderr, "ERR: unrecognized option: '-%c'\n", optopt);
                return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

/*********************************************************************
 * Function:     setPaths
 *--------------------------------------------------------------------
 * Description:  Set paths relative to the location, where the program
 *               "visualCrypt" was called from, if they weren't
 *               already changed by program parameters.
 ********************************************************************/
void setPaths(char *argv[], char **logPath) {
    char *programPath = argv[0];
    size_t programPathLen = strlen(programPath) - 11;

    if (!sourcePath) {
        sourcePath = xcalloc(programPathLen + strlen(SOURCE_PATH) + 1, 1);
        strncpy(sourcePath, programPath, programPathLen);
        strncpy(sourcePath + programPathLen, SOURCE_PATH, strlen(SOURCE_PATH) + 1);
    }

    if (!sharePath) {
        sharePath = xcalloc(programPathLen + strlen(SHARE_PATH) + 1, 1);
        strncpy(sharePath, programPath, programPathLen);
        strncpy(sharePath + programPathLen, SHARE_PATH, strlen(SHARE_PATH) + 1);
    }

    *logPath = xcalloc(programPathLen + strlen(TIME_LOG_PATH) + 1, 1);
    strncpy(*logPath, programPath, programPathLen);
    strncpy(*logPath + programPathLen, TIME_LOG_PATH, strlen(TIME_LOG_PATH) + 1);
}

/*********************************************************************
 * Function:     main
 *--------------------------------------------------------------------
 * Description:  Ask the user which algorithm shall run and call the
 *               chosen one.
 * Return:       0 on success, 1 on failure.
 ********************************************************************/
int main(int argc, char *argv[]) {
    int choice;
    char *logPath = NULL;

    int exitStatus = getPathsFromProgramParameter(argc, argv);
    if (exitStatus) {
        return exitStatus;
    }

    setPaths(argv, &logPath);

    char *menu[] = {"(n,n) deterministic algorithm",
                    "(n,n) probabilistic algorithm",
                    "(n,n) random grid algorithm",
                    "(2,n) random grid algorithm",
                    "(k,n) random grid algorithm",
                    "decrypt shares",
                    "time measurement",
                    "exit"};

    choice = getMenu("Visual Crypt Algorithms", menu, 8, "Your Choice: ");
    switch (choice) {
        case 1:
            callAlgorithm(deterministicAlgorithm, 0);
            break;
        case 2:
            callAlgorithm(probabilisticAlgorithm, 0);
            break;
        case 3:
            callAlgorithm(callRandomGridAlgorithm, 1);
            break;
        case 4:
            callAlgorithm(callRandomGridAlgorithm, 2);
            break;
        case 5:
            callAlgorithm(callRandomGridAlgorithm, 3);
            break;
        case 6:
            decryptShareFiles();
            break;
        case 7:
            timeMeasurement(logPath);
            break;
        case 8:
            return EXIT_SUCCESS;
        default:
            break;
    }
    return EXIT_SUCCESS;
}
