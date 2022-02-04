#ifndef BOOLEANMATRIX_H
#define BOOLEANMATRIX_H

#include <stdint.h>
#include "setsNsubsets.h"

#ifndef TYPE_PIXEL
#define TYPE_PIXEL

typedef uint8_t Pixel; /* black / white */

#endif /* TYPE_PIXEL */

typedef struct {
    int n;
    int m;
    Pixel* array;
} BooleanMatrix;

BooleanMatrix createBooleanMatrix(int n, int m);
Pixel getPixel(BooleanMatrix* matrix, int i, int j);
void setPixel(BooleanMatrix* matrix, int i, int j, uint8_t value);
void deleteBooleanMatrix(BooleanMatrix* matrix);

void fillBasisMatrix(BooleanMatrix* B, SubSet* set, uint8_t i, uint8_t j);
void printBooleanMatrix(BooleanMatrix* B, char* name, uint8_t n, uint8_t m);

#endif /* BOOLEANMATRIX_H */
