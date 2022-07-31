#ifndef SETTINGS_H
#define SETTINGS_H

/* PATHS */

/*  SOURCE_PATH = the secret image
    SHARE_PATH = directory where the shares and
    decryptions of the shares will be stored.

    These are the standard values for source image
    and result directory, if they are not changed
    via command line options when the program is
    called.

    Note: Used in visualCrypt.c
*/
#define SOURCE_PATH "../image/cameraman.bmp"
#define SHARE_PATH  "../image"

/*  RANDOM_FILE_PATH = the file used as source
    to get random numbers

    /dev/urandom is faster to get numbers from,
    but only /dev/random is considered cryptographically
    secure and should be used to generate "true shares"

    Note: Used in vcAlgorithms.c and
    timeMeasurement.c
*/
#define RANDOM_FILE_PATH "/dev/urandom"

/*  TIME_LOG_PATH = the log file of the
    time measurement

    Note: Used in timeMeasurement.c
*/
#define TIME_LOG_PATH "../timeMeasurement.log"

/* ADJUSTMENTS */

/*  Threshold:
    Color level to decide whether a pixel of the
    secret image is considered black or white.
    Small numbers will cause a brighter result image,
    when the shares are stacked together, and a high
    numbers vice versa a dark result image.
    Min: 0, Max: 255
    (Values out of range will have the same result as
    Min and Max: An all-white or all-black result image.)

    Note: Used in handleBMP.c
*/
#define THRESHOLD 127

/*  Print Sets and Basis Matrices:
    If this Option is non-zero, the programm
    will print the calculated sets, subsets and
    basis matrices, if either the deterministic or
    probabilistic algorithm is called

    Note: Used in booleanMatrix.c
*/
#define PRINT_SETS_AND_BASIS_MATRICES 0

/*  Random Grid Algorithms:
    All callable RG-Algorithms exist in two versions
    (for time measurement reasons), choose if the
    normal or alternate versions should be called
    (the resulting shares are the same).

    0 = normal versions
    1 = alternate versions

    Note: Used in vcAlgorithms.c
*/
#define RG_VERSION 0

/* TIME MEASUREMENT OPTIONS */

/* Time measurement loops:
    Define the number of loops per algorithm,
    over which will be measured and averaged.

    Note: Used in timeMeasurement.c
*/
#define TIME_LOOPS 1000

#endif /* SETTINGS_H */
