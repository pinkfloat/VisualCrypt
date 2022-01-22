#ifndef HANDLEBMP_H
#define HANDLEBMP_H
 
#include <stdio.h>
#include <stdint.h>

typedef uint8_t Pixel; // black / white

int createBMP(FILE* file, const Pixel* pixelArray, int32_t width, int32_t height);

#endif /* HANDLEBMP_H */