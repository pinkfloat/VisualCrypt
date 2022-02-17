#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "deterministicShares.h"

typedef struct {
    BooleanMatrix* source;
    BooleanMatrix* dest;
    int sourceIdx;
    int destIdx;
} MatrixCopy;

/* calculate the pixel expansion */
static void calculateDeterministicPixelSize (int* deterministicHeight, int* deterministicWidth, int n, int m)
{
    if (n % 2) /* odd */
    {
        /*  the shares will have the same appearance as
            the source, and are just scaled a bit larger
        */
        *deterministicHeight = (int)sqrt(m);
        *deterministicWidth = *deterministicHeight;
    }
    else /* even */
    {
        /*  the shares will look a little
            "stretched" from left to right
        */
        *deterministicHeight = (int)sqrt(m/2);
        *deterministicWidth = *deterministicHeight * 2;
    }
}

int mallocDeterministicShareArrays(Image* source, Image** share, int n, int m)
{
    int err = 0;

    int deterministicHeight, deterministicWidth;
    calculateDeterministicPixelSize(&deterministicHeight, &deterministicWidth, n, m);

    /* for each share */
    for(int i = 0; i < n; i++)
    {
        if(!err)
        {
            share[i]->height = source->height * deterministicHeight;
            share[i]->width = source->width * deterministicWidth;
            err = mallocPixelArray(share[i]);
        }
        else
            share[i]->array = NULL;
    }
    return err;
}

/*  Sort-Function for randomSort
    copy the chosen column of a source matrix into the chosen column of a destination matrix
*/
static inline void copyMatrixColumn(MatrixCopy* copy)
{
    for(int row = 0; row < copy->source->n; row++)
        setPixel(copy->dest, row, copy->destIdx, getPixel(copy->source, row, copy->sourceIdx));
}

/*  Sort-Function for randomSort
    re-arrange an 1D-array (row of the permutation matrix) into an 2D-array (deterministicPixel)
*/
static inline void fillDeterministicPixel(MatrixCopy* copy)
{
    int row = copy->sourceIdx;
    for(int column = 0; column < copy->source->m; column++)
        copy->dest->array[column] = getPixel(copy->source, row, column);
}

/*  Copies rows/columns of a source array randomly new sorted to a
    destination array, without using a row/column of the source twice
*/
static void randomSort(int randNum, Pixel* checkList, MatrixCopy* copy, void (*sortFunc)(MatrixCopy*))
{
    for(int checkIdx = 0, zeroCount = 0;; checkIdx++)
    {
        if(checkList[checkIdx] == 0)
            zeroCount++;

        if(zeroCount == randNum)
        {
            /*  copy the random chosen row/column of the source matrix
                to the next empty row/column of the destination matrix
            */
            copy->sourceIdx = checkIdx;
            sortFunc(copy);
            checkList[checkIdx] = 1; /* mark row/column in checkList as used */
            break;
        }

    }
}

/*  copy the pixel array detPixel to the PixelArray of the share,
    starting at row posY and column posX of the share
*/
static void copyDeterministicPixelToShare(BooleanMatrix* detPixel, Image* share, int posY, int posX)
{
    /* for each pixel of detPixel */
    for(int i = 0; i < detPixel->n; i++)     /* rows */
    {
        for(int j = 0; j < detPixel->m; j++) /* columns */
        {
            share->array[(posY+i) * share->width + posX+j] = getPixel(detPixel, i, j);
        }
    }
}

int fillDeterministicShareArrays(Image* source, Image** share, BooleanMatrix* B0, BooleanMatrix* B1)
{
    int randNum;
    int n = B0->n;
    int m = B0->m;
    MatrixCopy copy;
    int deterministicHeight, deterministicWidth;
    calculateDeterministicPixelSize(&deterministicHeight, &deterministicWidth, n, m);

    /* initialize random number generator */
    srand(time(NULL));

    /*  create matrix of equal size as the basis matrices 
        to store the permutations of them.
    */
    BooleanMatrix permutation = createBooleanMatrix(n, m);
    if(!permutation.array)
        return -1;

    /*  every pixel of the source will be encrypted in arrays of pixel,
        printed to the shares. This matrix has the size of an array
        of an encrypted pixel and will temporarily hold all of the
        pixel-arrays printed to the shares.
    */
    BooleanMatrix deterministicPixel = createBooleanMatrix(deterministicHeight, deterministicWidth);
    if (!deterministicPixel.array)
        goto cleanupA;

    /*  create checklist of size n (/m) to store which rows (/columns)
        of the basis matrix has been already used in the destination
        matrix: 0 = unused row (/column), 1 = used
    */
    Pixel* rowCheckList = malloc(n * sizeof(Pixel));
    Pixel* columnCheckList = malloc(m * sizeof(Pixel));
    if (!columnCheckList || !rowCheckList)
        goto cleanupB;

    /* for each pixel of the source */
    for(int i = 0; i < source->height; i++)     /* rows */
    {
        for(int j = 0; j < source->width; j++)  /* columns */
        {
            memset(columnCheckList, 0, m*sizeof(Pixel));
            
            /*  permutate the columns of a basis matrix and store
                the permutation in the array "permutation"
            */
            copy.dest = &permutation;
            for (int permColumn = 0; permColumn < m; permColumn++)
            {
                randNum = rand() % (m-permColumn)+1; /* number between 1 and m-permColumn */
                copy.destIdx = permColumn;

                /* if the pixel is black */
                if (source->array[i * source->width + j])
                    /* permutate columns of basis matrix B1 */
                    copy.source = B1;

                /* if the pixel is white */
                else
                    /* permutate columns of basis matrix B0 */
                    copy.source = B0;

                randomSort(randNum, columnCheckList, &copy, copyMatrixColumn);
            }

            memset(rowCheckList, 0, n*sizeof(Pixel));

            /*  fill each permutation-array-row temporarily into the array 
                "deterministicPixel", where the row will be interpreted as
                2D-array. Then fill this 2D-array "deterministicPixel" randomly
                in one of the shares, so each share will finally get a different
                (2D-sorted) row of the permutation-array.
            */
            copy.dest = &deterministicPixel;
            copy.source = &permutation;

            /* for each share */
            for(int share_idx = 0; share_idx < n; share_idx++)
            {
                /*  choose random which share will get which permutation-array row */
                randNum = rand() % (n-share_idx)+1; /* number between 1 and "number-of-shares minus share_idx" */
                randomSort(randNum, rowCheckList, &copy, fillDeterministicPixel);
                copyDeterministicPixelToShare(&deterministicPixel, share[share_idx], i*deterministicHeight, j*deterministicWidth);
            }
        }
    }

    free(columnCheckList);
    free(rowCheckList);
    deleteBooleanMatrix(&deterministicPixel);
    deleteBooleanMatrix(&permutation);
    return 0;

    cleanupB:
        free(columnCheckList);
        free(rowCheckList);
        deleteBooleanMatrix(&deterministicPixel);
    cleanupA:
        deleteBooleanMatrix(&permutation);
        return -1;
}