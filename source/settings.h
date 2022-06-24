#ifndef SETTINGS_H
#define SETTINGS_H

/* PATHS */

/*  SOURCE_PATH = the secret image
    SHARE_PATH = the directory where the shares
    and decryptions of the shares will be stored

    If changed to something much longer
    please adjust PATH_LENGTH too to something
    that is at least 20 characters larger than
    the paths.

    Note: Used in image.c
*/
#define SOURCE_PATH "../image/cameraman.bmp"
#define SHARE_PATH "../image"
#define PATH_LENGTH 100

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

/* Number of shares in time measurement:
    Define n and k for the time measurement.

    n = number of shares
    k = number of shares that must be stacked
        in (k,n) algorithms to reveal information
        about the source image

    Note:   -   Used in timeMeasurement.c
            -   The images won't be printet in
                time measurement, but changing these
                values will lead to different performance
                results
*/
#define TIME_N 3
#define TIME_K 2

/* Time measurement loops:
    Define the number of loops per algorithm,
    over which will be measured and averaged.

    Note: Used in timeMeasurement.c
*/
#define TIME_LOOPS 1000

#endif /* SETTINGS_H */  
