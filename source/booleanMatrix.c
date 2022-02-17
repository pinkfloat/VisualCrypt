#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "booleanMatrix.h"

BooleanMatrix createBooleanMatrix(int n, int m)
{
    BooleanMatrix result;
    result.n = n;
    result.m = m;
    result.array = malloc(sizeof(Pixel)*n*m);
    if (!result.array)
        fprintf(stderr, "ERR: allocate buffer for BooleanMatrix\n");
    return result;
}

Pixel getPixel(BooleanMatrix* matrix, int i, int j)
{
    return matrix->array[i * matrix->m + j];
}

void setPixel(BooleanMatrix* matrix, int i, int j, uint8_t value)
{
    matrix->array[i * matrix->m + j] = value;
}

void deleteBooleanMatrix(BooleanMatrix* matrix)
{
    free(matrix->array);
}

void fillBasisMatrix(BooleanMatrix* B, SubSet* set, uint8_t i, uint8_t j)
{
    uint8_t found = 0;
    if (set[j].length) /* the subset is not NULL */
    {
        for (int e = 0; e < set[j].length; e++) /* e = element of a subset */
        {
            /* check if element i is part of the subset */
            if (set[j].data[e] == i){
                found = 1;
                break;
            }
        }
        if(found)
            setPixel(B, i, j, 1);
        else
            setPixel(B, i, j, 0);
    }
    else /* the subset is NULL */
        setPixel(B, i, j, 0);
}

void printBooleanMatrix(BooleanMatrix* B, char* name)
{
    int n = B->n;
    int m = B->m;

    fprintf(stdout, "%2s:\n", name);
    for(int i = 0; i < n; i++)     /* rows */
    {
        for(int j = 0; j < m; j++) /* columns */
        {
            fprintf(stdout, "%d", getPixel(B, i, j));
        }
        fprintf(stdout, "\n");
    }
    fprintf(stdout, "\n");
}
