#ifndef BOOLEANMATRIX_H
#define BOOLEANMATRIX_H

#include <stdint.h>
#include <stdlib.h>

#ifndef TYPE_PIXEL
#define TYPE_PIXEL

typedef uint8_t Pixel; /* black / white */

#endif /* TYPE_PIXEL */

typedef struct {
    int n;
    int m;
    Pixel* array;
} BooleanMatrix;

static inline BooleanMatrix createBooleanMatrix(int n, int m)
{
    BooleanMatrix result;
    result.n = n;
    result.m = m;
    result.array = malloc(sizeof(Pixel)*n*m);
    return result;
}

static inline Pixel getPixel(BooleanMatrix* matrix, int i, int j)
{
    return matrix->array[i * matrix->m + j];
}

static inline void setPixel(BooleanMatrix* matrix, int i, int j, uint8_t value)
{
    matrix->array[i * matrix->m + j] = value;
}

static inline void deleteBooleanMatrix(BooleanMatrix* matrix)
{
    free(matrix->array);
}

#endif /* BOOLEANMATRIX_H */
