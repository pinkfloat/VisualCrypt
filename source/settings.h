#ifndef SETTINGS_H
#define SETTINGS_H

/*  SOURCE_PATH = the secret image
    SHARE_PATH = the directory where the shares
    and decryptions of the shares will be stored

    These defines are only used in image.c

    If changed to something much longer
    please adjust PATH_LENGTH too to something
    that is at least 20 characters larger than
    the paths.
*/
#define SOURCE_PATH "../image/cameraman.bmp"
#define SHARE_PATH "../image"
#define PATH_LENGTH 100

/* Adjustments */

/*  Threshold:
    Color level to decide whether a pixel of the
    secret image is considered black or white.
    Small numbers will cause a brighter result image,
    when the shares are stacked together, and a high
    numbers vice versa a dark result image.
    Min: 0, Max: 255
    (Values out of range will have the same result as
    Min and Max: An all-white or all-black result image.)

    Note: This is only used in handleBMP.c
*/
#define THRESHOLD 127

/*  RandomGridAlgorithms:
    All callable RG-Algorithms exist in two versions
    (for time measurement reasons), choose if the
    normal or alternate versions should be called
    (the resulting shares are the same).

    0 = normal versions
    1 = alternate versions

    Note: This is only used in vcAlgorithms.c
*/
#define RG_VERSION 0

#endif /* SETTINGS_H */  
