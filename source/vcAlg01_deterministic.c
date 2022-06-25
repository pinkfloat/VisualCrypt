#include <math.h>
#include <string.h>
#include "image.h"
#include "random.h"
#include "fileManagement.h"
#include "memoryManagement.h"
#include "vcAlg01_deterministic.h"

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
void calcPixelExpansion (int* deterministicHeight, int* deterministicWidth, int n, int m)
{
    if (n % 2) // odd
    {
        /*  the shares will have the same appearance as
            the source, and are just scaled a bit larger
        */
        *deterministicHeight = (int)sqrt(m);
        *deterministicWidth = *deterministicHeight;
    }
    else // even
    {
        /*  the shares will look a little
            "stretched" from left to right
        */
        *deterministicHeight = (int)sqrt(m/2);
        *deterministicWidth = *deterministicHeight * 2;
    }
}

/********************************************************************
* Function:     mallocPixelExpandedShares
*--------------------------------------------------------------------
* Description:  This function will allocate the buffer of the share
*               pixel arrays, that are going to be printed to the
*               BMPs later. Since the deterministic algorithm has
*               pixel expansion, they will be larger than the
*               source file.
* Input:        source = containing width and height of the source
*               image,
*               n = number of shares,
*               m = number of pixels in a share per pixel in source,
* Output:       share->array will be correctly allocated for each
*               share.
********************************************************************/
void mallocPixelExpandedShares(Image* source, Image* share, int n, int m)
{
    int deterministicHeight, deterministicWidth;
    calcPixelExpansion(&deterministicHeight, &deterministicWidth, n, m);

    // for each share
    for(int i = 0; i < n; i++)
    {
        share[i].height = source->height * deterministicHeight;
        share[i].width = source->width * deterministicWidth;
        mallocPixelArray(&share[i]);
    }
}

/********************************************************************
* Function:     permutateBasisMatrix
*--------------------------------------------------------------------
* Description:  Permutate the columns of a basis matrix and store
*               the permutation in the array "permutation".
* Input:        B0 = Basis matrix for white share-pixels
*               B1 = Basis matrix for black share-pixels
*               sourcePixel = pixel of the secret image (0/1)
*               columnCheckList = containing zero's for unused
*               basis matrix columns and one's for allready used
*               ones
*               randomSrc = file containing random numbers
* Output:       permutation = either the column-permutation of
*               B0 or B1
********************************************************************/
static void permutateBasisMatrix(   BooleanMatrix* B0,
                                    BooleanMatrix* B1, 
                                    BooleanMatrix* permutation,
                                    Pixel sourcePixel,
                                    int* columnIndices,
                                    FILE* randomSrc)
{
    BooleanMatrix* basisMatrix;
    if (sourcePixel) // source pixel is black
    {
        basisMatrix = B1;
    }
    else // source pixel is white
    {
        basisMatrix = B0;
    }

    shuffleColumns(permutation, basisMatrix, randomSrc, columnIndices);
}

/********************************************************************
* Function:     copyMatrixRowToShares
*--------------------------------------------------------------------
* Description:  The function copyMatrixRowToShares copies the
*               pixel array of matrixRow2D to the pixel array of a
*               share, starting at row posY and column posX of the
*               share.
********************************************************************/
static void copyMatrixRowToShares(BooleanMatrix matrixRow2D, Image* share, int posY, int posX)
{
    int width = share->width;

    // for each pixel of matrixRow2D
    for(int i = 0; i < matrixRow2D.height; i++) // rows
    {
        for(int j = 0; j < matrixRow2D.width; j++) // columns
        {
            share->array[(posY+i) * width + posX+j] = getPixel(matrixRow2D, i, j);
        }
    }
}

/********************************************************************
* Function:     fillPixelEncryptionToShares
*--------------------------------------------------------------------
* Description:  point with "matrixRow2D" temporarily to each
*               permutation-array-row, where the row will be
*               interpreted as 2D-array. Then fill the 2D-array
*               "matrixRow2D" randomly in one of the shares,
*               so each share will finally get a different
*               (2D-sorted) row of the permutation-array.
********************************************************************/
static void fillPixelEncryptionToShares(    BooleanMatrix* permutation,
                                            Image* share,
                                            int i,
                                            int j,
                                            int deterministicWidth,
                                            int deterministicHeight,
                                            int* rowIndices,
                                            FILE* randomSrc)
{
    int n = permutation->height;
    int m = permutation->width;
    int randNum;

    BooleanMatrix matrixRow2D;
    matrixRow2D.width = deterministicWidth;
    matrixRow2D.height = deterministicHeight;
    shuffleVector(rowIndices, n, randomSrc);
    
    Pixel* shuffledBasisMatrix = permutation->array;

    // for each share
    for(int shareIdx = 0; shareIdx < n; shareIdx++)
    {
        randNum = rowIndices[shareIdx];
        matrixRow2D.array = &(shuffledBasisMatrix[randNum * m]); // first element of a random matrix row
        copyMatrixRowToShares(matrixRow2D, &share[shareIdx], i*deterministicHeight, j*deterministicWidth);
    }
}

/********************************************************************
* Function:     prepareDeterministicAlgorithm
*--------------------------------------------------------------------
* Description:  This function will allocate all data that needs
*               allocation for the deterministic algorithm and
*               prepares the basis matrices which doesn't change for
*               the same amount of share files.
********************************************************************/
deterministicData* prepareDeterministicAlgorithm(AlgorithmData* data)
{
    int n = data->numberOfShares;
    int m = 1 << (n-1);     // number of pixels in a share per pixel in source file = 2^{n-1}

    // allocate pixel-arrays for the shares
	mallocPixelExpandedShares(data->source, data->shares, n, m);

    // create basis matrices
    BooleanMatrix B0 = createBooleanMatrix(n,m);
    BooleanMatrix B1 = createBooleanMatrix(n,m);
    fillBasisMatrices(&B0, &B1);
    
    int deterministicHeight, deterministicWidth;
    calcPixelExpansion(&deterministicHeight, &deterministicWidth, n, m);

    /*  create matrix of equal size as the basis matrices 
        to store the permutations of them
    */
    BooleanMatrix permutation = createBooleanMatrix(n, m);

    /*  create checklist of size n (/m) to shuffle the rows (/columns)
        of the basis matrices
    */
    int* rowIndices = createSetOfN(n, 0);
    int* columnIndices = createSetOfN(m, 0);

    deterministicData* dData = xmalloc(sizeof(deterministicData));
    dData->B0 = B0;
    dData->B1 = B1;
    dData->permutation = permutation;
    dData->sourceArray = data->source->array;
    dData->columnIndices = columnIndices;
    dData->rowIndices = rowIndices;
    dData->share = data->shares;
    dData->randomSrc = data->randomSrc;
    dData->width = data->source->width;
    dData->height = data->source->height;
    dData->deterministicWidth = deterministicWidth;
    dData->deterministicHeight = deterministicHeight;

    return dData;
}

/********************************************************************
* Function:     __deterministicAlgorithm
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
void __deterministicAlgorithm(deterministicData* data)
{
    BooleanMatrix* B0 = &data->B0;
    BooleanMatrix* B1 = &data->B1;
    BooleanMatrix* permutation = &data->permutation;
    Pixel* sourceArray = data->sourceArray;
    int* columnIndices = data->columnIndices;
    int* rowIndices = data->rowIndices;
    Image* share = data->share;
    FILE* randomSrc = data->randomSrc;
    int width = data->width;
    int height = data->height;
    int deterministicWidth = data->deterministicWidth;
    int deterministicHeight = data->deterministicHeight;

    // for each pixel of the source
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            Pixel sourcePixel = sourceArray[i * width + j];
            permutateBasisMatrix(B0, B1, permutation, sourcePixel, columnIndices, randomSrc);
            fillPixelEncryptionToShares(permutation, share, i,
                                        j, deterministicWidth, deterministicHeight, rowIndices, randomSrc);
        }
    }
}

/********************************************************************
* Function:     deterministicAlgorithm
*--------------------------------------------------------------------
* Description:  This is a wrapper for the "deterministic Algorithm"
*               from Moni Naor and Adi Shamir. It will prepare the
*               resources needed by the algorithm and call it
*               afterwards.
********************************************************************/
void deterministicAlgorithm(AlgorithmData* data)
{
    deterministicData* dData = prepareDeterministicAlgorithm(data);
    __deterministicAlgorithm(dData);
}
