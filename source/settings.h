#ifndef SETTINGS_H
#define SETTINGS_H

/* Main Settings */
#define NUMBER_OF_SHARES 4
#define SOURCE_PATH "../image/colortest.bmp"
#define DEST_PATH "../image"

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
*/
#define THRESHOLD 127

#endif /* SETTINGS_H */  
