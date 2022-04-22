#ifndef SETTINGS_H
#define SETTINGS_H

/* Main Settings */
#define NUMBER_OF_SHARES 4

/*  SOURCE_PATH = the secret image
    SHARE_PATH = the directory where the shares
    and decryptions of the shares will be stored

    These defines are only used in image.c

    If changed to something much longer
    please adjust PATH_LENGTH too to something
    that is at least 20 characters larger than
    the paths.
*/
#define SOURCE_PATH "../image/colortest.bmp"
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

#endif /* SETTINGS_H */  
