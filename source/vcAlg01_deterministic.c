#include <math.h>
#include <time.h>
#include <string.h>
#include "image.h"
#include "booleanMatrix.h"
#include "memoryManagement.h"
#include "vcAlgorithms.h"

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
static void mallocDeterministicShareArrays(Image* source, Image* share, int n, int m)
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
* Function:     fillBasisMatrices
*--------------------------------------------------------------------
* Description:  Calculate the basis matrices by creating subsets
*               from a set with "n" elements. (n = number of shares.)
*               The even cardinality set is used to fill the basis
*               matrix B0. The odd cardinality set is used to fill B1.
********************************************************************/
static void fillBasisMatrices(BooleanMatrix* B0, BooleanMatrix* B1)
{
    uint8_t n = B0->n;
    uint8_t m = B0->m;
    
    /*  create set with n elements, holding subsets
        with even and odd cardinalities of it
    */
    Set set = createSet(n, m);
    printAllSubsets(&set);

    for(uint8_t i = 0; i < n; i++)     /* rows */
    {
        for(uint8_t j = 0; j < m; j++) /* columns */
        {
            fillBasisMatrix(B0, set.even, i, j);
            fillBasisMatrix(B1, set.odd, i, j);
        }
    }

    printBooleanMatrix(B0, "B0");
    printBooleanMatrix(B1, "B1");
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
* Function:     fillEncryptedPixel
*--------------------------------------------------------------------
* Description:  This is a sort-Function for randomSort().
*               The function copies the values of a row of a
*               "permutation matrix" (1D-array) into a "deterministic
*               pixel" (2D-array). It is part of the encryption of a
*               specific pixel.
********************************************************************/
static inline void fillEncryptedPixel(MatrixCopy* copy)
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
* Function:     permutateBasisMatrix
*--------------------------------------------------------------------
* Description:  Permutate the columns of a basis matrix and store
*               the permutation in the array "permutation".
* Input:        B0 = Basis matrix for white share-pixels
*               B1 = Basis matrix for black share-pixels
*               source = the secret image, containing an array of
*               black and white pixel
*               i = line of the source pixel we are looking at
*               j = columns of the source pixel we are looking at
*               columnCheckList = containing zero's for unused
*               basis matrix columns and one's for allready used
*               ones
* Output:       permutation = either the column-permutation of
*               B0 or B1
********************************************************************/
static void permutateBasisMatrix(   BooleanMatrix* B0,
                                    BooleanMatrix* B1, 
                                    BooleanMatrix* permutation,
                                    Image* source,
                                    int i,
                                    int j,
                                    Pixel* columnCheckList)
{
    int m = B0->m;
    int randNum;
    MatrixCopy copy;

    memset(columnCheckList, 0, m*sizeof(Pixel));
    copy.dest = permutation;
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
}

/********************************************************************
* Function:     copyEncryptedPixelToShare
*--------------------------------------------------------------------
* Description:  The function copyEncryptedPixelToShare copies the
*               pixel array of encPixel to the pixel array of the
*               share, starting at row posY and column posX of the
*               share.
********************************************************************/
static void copyEncryptedPixelToShare(BooleanMatrix* encPixel, Image* share, int posY, int posX)
{
    /* for each pixel of encPixel */
    for(int i = 0; i < encPixel->n; i++)     /* rows */
    {
        for(int j = 0; j < encPixel->m; j++) /* columns */
        {
            share->array[(posY+i) * share->width + posX+j] = getPixel(encPixel, i, j);
        }
    }
}

/********************************************************************
* Function:     fillPixelEncryptionToShares
*--------------------------------------------------------------------
* Description:  fill each permutation-array-row temporarily into the
*               array "encryptedPixel", where the row will be
*               interpreted as 2D-array. Then fill the 2D-array
*               "encryptedPixel" randomly in one of the shares,
*               so each share will finally get a different
*               (2D-sorted) row of the permutation-array.
* Input:        permutation = the columns permutation of a basis
*               matrix
*               i = the row-position of the the first pixel of an
*               "encrypted pixel".
*               j = the column-position of the the first pixel of an
*               "encrypted pixel".
*               -> encrypted pixel = a source pixel will be encrypted
*               with an array of pixel per share.
*               rowCheckList = containing zero's for unused basis
*               matrix row and one's for allready used ones
* Output:       encryptedPixel = temporary storage for an
*               "encrypted pixel" before it is printed to one of the
*               shares
*               share = containing pixel arrays that need to be
*               filled. If they are stacked together per OR-function,
*               the secret image can be seen.
********************************************************************/
static void fillPixelEncryptionToShares(    BooleanMatrix* permutation,
                                            BooleanMatrix* encryptedPixel,
                                            Image* share,
                                            int i,
                                            int j,
                                            Pixel* rowCheckList)
{
    int n = permutation->n;
    int randNum;
    MatrixCopy copy;
    memset(rowCheckList, 0, n*sizeof(Pixel));

    copy.dest = encryptedPixel;
    copy.source = permutation;

    /* for each share */
    for(int shareIdx = 0; shareIdx < n; shareIdx++)
    {
        /* choose random which share will get which permutation-array row */
        randNum = rand() % (n-shareIdx)+1; /* number between 1 and "number-of-shares minus shareIdx" */
        randomSort(randNum, rowCheckList, &copy, fillEncryptedPixel);
        copyEncryptedPixelToShare(encryptedPixel, &share[shareIdx], i, j);
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
static void fillDeterministicShareArrays(Image* source, Image* share, BooleanMatrix* B0, BooleanMatrix* B1)
{
    int n = B0->n;
    int m = B0->m;
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
    BooleanMatrix encryptedPixel = createBooleanMatrix(deterministicHeight, deterministicWidth);

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
            permutateBasisMatrix(B0, B1, &permutation, source, i, j, columnCheckList);
            fillPixelEncryptionToShares(&permutation, &encryptedPixel, share,
                                        i*deterministicHeight, j*deterministicWidth, rowCheckList);
        }
    }
}

/********************************************************************
* Function:     deterministicAlgorithm
*--------------------------------------------------------------------
* Description:  This is an implementation of the so called
*               "deterministic Algorithm" from Moni Naor and Adi
*               Shamir. It will calculate the pixel of the share
*               images from the pixel in the source file, by creating
*               basis matrices out of the number of the share files.
*               The basis matrices will be afterwards permutated in
*               columns and each share will get a different row of
*               every permutation per source pixel.
********************************************************************/
void deterministicAlgorithm(AlgorithmData* data)
{
    uint8_t n = data->numberOfShares;
    uint8_t m = 1 << (n-1);     /* number of pixels in a share per pixel in source file = 2^{n-1} */

    /* allocate pixel-arrays for the shares */
	mallocDeterministicShareArrays(data->source, data->shares, n, m);

    /* allocate basis matrices */
    BooleanMatrix B0 = createBooleanMatrix(n,m);
    BooleanMatrix B1 = createBooleanMatrix(n,m);

    fillBasisMatrices(&B0, &B1);
    
    /* Fill pixel-arrays of the shares */
    fillDeterministicShareArrays(data->source, data->shares, &B0, &B1);
}
