#include <math.h>
#include <time.h>
#include <string.h>
#include "memoryManagement.h"
#include "deterministicShares.h"

typedef struct {
    BooleanMatrix* source;
    BooleanMatrix* dest;
    int sourceIdx;
    int destIdx;
} MatrixCopy;

/********************************************************************
* Function:     calcPixelExpansion
*--------------------------------------------------------------------
* Description:  The shares, that will be created from the source
*               image, will have a greater amount of pixel as the
*               original source file. This function calculates the
*               pixel expansion i.e. the amount of pixel that encrypt
*               a source pixel in width and height with:
* Input:        n = number of shares,
*               m = number of pixels in a share per pixel in source,
* Output:       deterministicHeight = height of the pixel array
*               encrypting a pixel.
*               deterministicWidth = width of the pixel array
*               encrypting a pixel.
********************************************************************/
static void calcPixelExpansion (int* deterministicHeight, int* deterministicWidth, int n, int m)
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

/********************************************************************
* Function:     mallocDeterministicShareArrays
*--------------------------------------------------------------------
* Description:  An image is in fact a pixel array printed to a BMP
*               file. The pixels are stored as array values with
*               "1" being black and "0" being white. This function
*               will allocate the buffer of the pixel arrays, stored
*               in Image structs, for all of the shares, that will
*               be printed to BMPs later.
* Input:        source = containing width and height of the source
*               image,
*               n = number of shares,
*               m = number of pixels in a share per pixel in source,
* Output:       share->array will be correctly allocated for each
*               share.
********************************************************************/
void mallocDeterministicShareArrays(Image* source, Image* share, int n, int m)
{
    int deterministicHeight, deterministicWidth;
    calcPixelExpansion(&deterministicHeight, &deterministicWidth, n, m);

    /* for each share */
    for(int i = 0; i < n; i++)
    {
        share[i].height = source->height * deterministicHeight;
        share[i].width = source->width * deterministicWidth;
        mallocPixelArray(&share[i]);
    }
}

/********************************************************************
* Function:     copyMatrixColumn
*--------------------------------------------------------------------
* Description:  This is a sort-Function for randomSort().
*               The function will copy a chosen column of the source
*               matrix (copy->sourceIdx) into a chosen column of the
*               destination matrix (copy->destIdx).
********************************************************************/
static inline void copyMatrixColumn(MatrixCopy* copy)
{
    for(int row = 0; row < copy->source->n; row++)
        setPixel(copy->dest, row, copy->destIdx, getPixel(copy->source, row, copy->sourceIdx));
}

/********************************************************************
* Function:     fillDeterministicPixel
*--------------------------------------------------------------------
* Description:  This is a sort-Function for randomSort().
*               The function copies the values of a row of a
*               "permutation matrix" (1D-array) into a "deterministic
*               pixel" (2D-array). It is part of the encryption of a
*               specific pixel.
********************************************************************/
static inline void fillDeterministicPixel(MatrixCopy* copy)
{
    int row = copy->sourceIdx;
    for(int column = 0; column < copy->source->m; column++)
        copy->dest->array[column] = getPixel(copy->source, row, column);
}

/********************************************************************
* Function:     randomSort
*--------------------------------------------------------------------
* Description:  The function randomSort() copies a row/column of a
*               source matrix to a destination matrix, without using
*               a row/column of the source twice. For doing this, a
*               temporary vector "checkList" is used, that has the
*               same size as the numbers of rows/columns, to store,
*               which has already been taken. The "randNum"'th
*               unused row/column of the source will be chosen and
*               marked as "used" in the checkList.
*               The code written in here is only to sort out which
*               row/column of the source is to use. The copy process,
*               from one matrix to another, is part of the sorting
*               function, given as last parameter to randomSort.
********************************************************************/
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

/********************************************************************
* Function:     copyDeterministicPixelToShare
*--------------------------------------------------------------------
* Description:  The function copyDeterministicPixelToShare copies the
*               pixel array of detPixel to the pixel array of the
*               share, starting at row posY and column posX of the
*               share.
********************************************************************/
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

/********************************************************************
* Function:     fillDeterministicShareArrays
*--------------------------------------------------------------------
* Description:  This function will fill the pixel arrays of the
*               shares with the values "1" being black and "0" being
*               white. Therefore the already calculated basis
*               matrices B0 and B1 are used. The columns of the
*               basis matrices will be randomly permutated and each
*               row of the resulting permutations will be re-sorted,
*               randomly assigned and copied to one of the shares.
********************************************************************/
void fillDeterministicShareArrays(Image* source, Image* share, BooleanMatrix* B0, BooleanMatrix* B1)
{
    int randNum;
    int n = B0->n;
    int m = B0->m;
    MatrixCopy copy;
    int deterministicHeight, deterministicWidth;
    calcPixelExpansion(&deterministicHeight, &deterministicWidth, n, m);

    /* initialize random number generator */
    srand(time(NULL));

    /*  create matrix of equal size as the basis matrices 
        to store the permutations of them.
    */
    BooleanMatrix permutation = createBooleanMatrix(n, m);

    /*  every pixel of the source will be encrypted in arrays of pixel,
        printed to the shares. This matrix has the size of an array
        of an encrypted pixel and will temporarily hold all of the
        pixel-arrays printed to the shares.
    */
    BooleanMatrix deterministicPixel = createBooleanMatrix(deterministicHeight, deterministicWidth);

    /*  create checklist of size n (/m) to store which rows (/columns)
        of the basis matrix has been already used in the destination
        matrix: 0 = unused row (/column), 1 = used
    */
    Pixel* rowCheckList = xmalloc(n * sizeof(Pixel));
    Pixel* columnCheckList = xmalloc(m * sizeof(Pixel));

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
                randNum = rand() % (m-permColumn)+1; /* number between 1 and m minus permColumn */
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
                2D-array. Then fill the 2D-array "deterministicPixel" randomly
                in one of the shares, so each share will finally get a different
                (2D-sorted) row of the permutation-array.
            */
            copy.dest = &deterministicPixel;
            copy.source = &permutation;

            /* for each share */
            for(int shareIdx = 0; shareIdx < n; shareIdx++)
            {
                /* choose random which share will get which permutation-array row */
                randNum = rand() % (n-shareIdx)+1; /* number between 1 and "number-of-shares minus shareIdx" */
                randomSort(randNum, rowCheckList, &copy, fillDeterministicPixel);
                copyDeterministicPixelToShare(&deterministicPixel, &share[shareIdx], i*deterministicHeight, j*deterministicWidth);
            }
        }
    }
}