#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>
#include <stdint.h>

typedef uint8_t Pixel; /* black / white */

typedef struct {
    FILE *file;
    Pixel *array;
    int32_t width;
    int32_t height;
} Image;

int openImageR(char* path, Image* image);
int openImageW(char* path, Image* image);

int mallocPixelArray(Image* image);
int mallocImageOfEqSize(Image* source, Image* dest);
int copyImageContent(Image* source, Image* dest);

#endif /* IMAGE_H */ 
