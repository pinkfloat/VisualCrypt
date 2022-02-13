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

static inline void copyMatrixColumn(BooleanMatrix* source, BooleanMatrix* dest, int sourceColumnNum, int destColumnNum)
{
    for(int row = 0; row < source->n; row++)
    {
        setPixel(dest, row, destColumnNum, getPixel(source, row, sourceColumnNum));
    }
}

/*  switch the columns of a given basis matrix and copy
    them into another (new) "permutation matrix"
*/
BooleanMatrix permuteBasisMatrix(BooleanMatrix* basis)
{
    int m = basis->m;

    /* create matrix of equal size as the basis matrix */
    BooleanMatrix permutation = createBooleanMatrix(basis->n, m);
    if (!permutation.array)
        return permutation;

    /*  create checklist of size m to store which columns
        of the basis matrix has been already used in the
        permutation matrix: 0 = unused column, 1 = used
    */
    Pixel* checkList = calloc(m, sizeof(Pixel));
    if (!checkList)
    {
        deleteBooleanMatrix(&permutation);
        return permutation;
    }

    /* initialize random number generator */
    time_t t;
    srand((unsigned) time(&t));

    /*  fill columns of the permutation matrix in
        a random way with the columns of the basis
        matrix
    */
    int randNum, idx, zeroCount;
    for (int j = 0; j < m; j++)
    {
        /* number between 1 and m-j */
        randNum = rand() % (m-j)+1;

        /*  search for the randNum-th zero
            in the checkList
        */
        for(idx = 0, zeroCount = 0;; idx++)
        {
            if(checkList[idx] == 0)
                zeroCount++;

            if(zeroCount == randNum)
            {
                /*  copy the random chosen column of the basis matrix
                    to the next empty column of the permutation matrix
                */
                copyMatrixColumn(basis, &permutation, idx, j);
                checkList[idx] = 1; /* mark column in checkList as used */
                break;
            }

        }
    }
    free(checkList);
    return permutation;
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
