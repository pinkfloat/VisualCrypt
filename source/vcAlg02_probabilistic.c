#include <string.h>
#include "fileManagement.h"
#include "memoryManagement.h"
#include "random.h"
#include "vcAlg02_probabilistic.h"

static inline void copyColumnOfBasisMatrix(BooleanMatrix* destVector, BooleanMatrix* srcMatrix, int column)
{
    int numberOfRows = srcMatrix->n;
    for (int row = 0; row < numberOfRows; row++)
        setPixel(*destVector, row, 0, getPixel(*srcMatrix, row, column));
}

/********************************************************************
* Function:     getRandomMatrixColumn
*--------------------------------------------------------------------
* Description:  Get a random column of either B0 or B1 and store
*               it the vector "columnVector".
* Input:        B0 = Basis matrix for white share-pixels
*               B1 = Basis matrix for black share-pixels
*               sourcePixel = pixel of the secret image (0/1)
*               randomSrc = file containing random numbers
* Output:       columnVector = stores the randomly chosen column
********************************************************************/
static void getRandomMatrixColumn(  BooleanMatrix* B0,
                                    BooleanMatrix* B1, 
                                    BooleanMatrix* columnVector,
                                    Pixel sourcePixel,
                                    FILE* randomSrc)
{
    int m = B0->m; // number of columns
    BooleanMatrix* basisMatrix;

    if (sourcePixel) // source pixel is black
    {
        basisMatrix = B1;
    }
    else // source pixel is white
    {
        basisMatrix = B0;
    }

    int randNum = getRandomNumber(randomSrc, 0, m);
    copyColumnOfBasisMatrix(columnVector, basisMatrix, randNum);
}

/********************************************************************
* Function:     copyColumnElementsToShares
*--------------------------------------------------------------------
* Description:  Get use the vector "columnVector" to copy one random
*               element of it to each share, whithout using an
*               vector element twice.
* Input:        columnVector = a randomly chosen column of a basis
*               matrix
*               sharePixelPosition = location where the pixel should
*               be copied to
*               checkList = containing zero's for unused elements
*               one's for allready used ones
*               randomSrc = file containing random numbers
* Output:       share = containing pixel arrays that need to be
*               filled. If they are stacked together per OR-function,
*               the secret image can be seen.
********************************************************************/
static void copyColumnElementsToShares( BooleanMatrix* columnVector,
                                        Image* share,
                                        int sharePixelPosition,
                                        int* rowIndices,
                                        FILE* randomSrc)
{
    int n = columnVector->n;
    Pixel randPixel;
    shuffleVector(rowIndices, n, randomSrc);
    Pixel* pxVector = columnVector->array;

    // for each share
    for(int shareIdx = 0; shareIdx < n; shareIdx++)
    {
        // choose random which share will get which vector element
        randPixel = pxVector[rowIndices[shareIdx]];
        share[shareIdx].array[sharePixelPosition] = randPixel;
    }
}

/********************************************************************
* Function:     __probabilisticAlgorithm
*--------------------------------------------------------------------
* Description:  This is an implementation of the so called
*               "probabilistic Algorithm" from Ryo Ito, Hidenori
*               Kuwakado and Hatsukazu Tanaka. It will calculate the
*               pixel of the share images from the pixel in the source
*               file, by creating basis matrices out of the number of
*               the share files, in the same way, the deterministic
*               algorithm does. For each pixel of the source file,
*               one column of the basis matrix will be randomly chosen
*               and each share will get a different element of the
*               column as pixel value.
********************************************************************/
void __probabilisticAlgorithm(probabilisticData* data)
{
    BooleanMatrix* B0 = data->B0;
    BooleanMatrix* B1 = data->B1;
    BooleanMatrix* columnVector = data->columnVector;
    Pixel* sourceArray = data->sourceArray;
    int* rowIndices = data->rowIndices;
    Image* share = data->share;
    FILE* randomSrc = data->randomSrc;
    int width = data->width;
    int height = data->height;

    // for each pixel of the source
    for(int i = 0; i < height; i++)     // rows
    {
        for(int j = 0; j < width; j++)  // columns
        {
            int sharePixelPosition = i * width + j;
            int sourcePixel = sourceArray[sharePixelPosition];
            getRandomMatrixColumn(B0, B1, columnVector, sourcePixel, randomSrc);
            copyColumnElementsToShares(columnVector, share, sharePixelPosition, rowIndices, randomSrc);
        }
    }
}

/********************************************************************
* Function:     probabilisticAlgorithm
*--------------------------------------------------------------------
* Description:  This is a wrapper for the "probabilistic Algorithm"
*               from Ryo Ito, Hidenori Kuwakado and Hatsukazu Tanaka.
*               It will allocate all data that needs allocation and
*               prepares the basis matrices which doesn't change for
*               the same amount of share files.
********************************************************************/
void probabilisticAlgorithm(AlgorithmData* data)
{
    int n = data->numberOfShares;
    int m = 1 << (n-1);

    // allocate pixel-arrays for the shares
	mallocSharesOfSourceSize(data->source, data->shares, n);

    // create basis matrices
    BooleanMatrix B0 = createBooleanMatrix(n,m);
    BooleanMatrix B1 = createBooleanMatrix(n,m);
    fillBasisMatrices(&B0, &B1);

    /*  create vector with the length of a matrix column
        to store a random column later on in it
    */
    BooleanMatrix columnVector = createBooleanMatrix(n, 1);

    /*  create checklist of size n to store which column elements
        of the random basis matrix column has been already used
        for a share file:
        0 = unused element, 1 = used
    */
    int* rowIndices = createSetOfN(n, 0);

    probabilisticData pData = {
        .B0 = &B0,
        .B1 = &B1,
        .columnVector = &columnVector,
        .sourceArray = data->source->array,
        .rowIndices = rowIndices,
        .share = data->shares,
        .randomSrc = data->randomSrc,
        .width = data->source->width,
        .height = data->source->height
    };

    __probabilisticAlgorithm(&pData);
}
