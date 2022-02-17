#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>
#include <stdint.h>

#ifndef TYPE_PIXEL
#define TYPE_PIXEL

typedef uint8_t Pixel; /* black / white */

#endif /* TYPE_PIXEL */

typedef struct {
    FILE *file;
    Pixel *array;
    int32_t width;
    int32_t height;
} Image;

int mallocPixelArray(Image* image);

int createSourceImage(char* path, Image* image);
int createShareFiles(char* dirPath, Image* share, int numberOfShares);
int drawShareFiles(Image* share, int numberOfShares);
void closeShareFiles(Image* share, int numberOfShares);

void freeShareArrays(Image* share, int n);

#endif /* IMAGE_H */ 
