#include "vcAlg02_probabilistic.h"

#include <string.h>

#include "fileManagement.h"
#include "memoryManagement.h"
#include "random.h"

/*********************************************************************
 * Function:     copyColumnOfBasisMatrix
 *--------------------------------------------------------------------
 * Description:  Copy the column of "srcMatrix" at number "column"
 *               to "destVector".
 ********************************************************************/
static inline void copyColumnOfBasisMatrix(BooleanMatrix *destVector, BooleanMatrix *srcMatrix, int column) {
    int numberOfRows = srcMatrix->height;
    for (int row = 0; row < numberOfRows; row++) {
        setPixel(*destVector, row, 0, getPixel(*srcMatrix, row, column));
    }
}

/*********************************************************************
 * Function:     getRandomMatrixColumn
 *--------------------------------------------------------------------
 * Description:  Gets a random column of either B0 or B1 and stores
 *               it in "columnVector".
 * Input:        B0 = Basis matrix for white share-pixels
 *               B1 = Basis matrix for black share-pixels
 *               sourcePixel = pixel of the secret image (0/1)
 *               randomSrc = file containing random numbers
 * Output:       columnVector = stores the randomly chosen column
 ********************************************************************/
static void getRandomMatrixColumn(BooleanMatrix *B0, BooleanMatrix *B1, BooleanMatrix *columnVector, Pixel sourcePixel,
                                  FILE *randomSrc) {
    int m = B0->width;  // number of columns
    BooleanMatrix *basisMatrix;

    if (sourcePixel)  // source pixel is black
    {
        basisMatrix = B1;
    } else  // source pixel is white
    {
        basisMatrix = B0;
    }

    int randNum = getRandomNumber(randomSrc, 0, m);
    copyColumnOfBasisMatrix(columnVector, basisMatrix, randNum);
}

/*********************************************************************
 * Function:     copyColumnElementsToShares
 *--------------------------------------------------------------------
 * Description:  Copy one random element of "columnVector" to each
 *               share, whithout using a vector element twice.
 * Input:        columnVector = a randomly chosen column of a basis
 *               matrix
 *               sharePixelPosition = location where the pixel should
 *               be copied to
 *               randomSrc = file containing random numbers
 *               rowIndices = a vector containing numbers from 0 to n
 * Output:       share = pixel arrays that'll be filled. If they are
 *               stacked together per OR-function, the secret image
 *               can be seen.
 ********************************************************************/
static void copyColumnElementsToShares(BooleanMatrix *columnVector, Image *share, int sharePixelPosition,
                                       int *rowIndices, FILE *randomSrc) {
    int n = columnVector->height;
    Pixel randPixel;
    shuffleVector(rowIndices, n, randomSrc);
    Pixel *pxVector = columnVector->array;

    // for each share
    for (int shareIdx = 0; shareIdx < n; shareIdx++) {
        randPixel = pxVector[rowIndices[shareIdx]];
        share[shareIdx].array[sharePixelPosition] = randPixel;
    }
}

probabilisticData *prepareProbabilisticAlgorithm(AlgorithmData *data) {
    int n = data->numberOfShares;
    int m = 1 << (n - 1);

    mallocSharesOfSourceSize(data->source, data->shares, n);

    BooleanMatrix B0 = createBooleanMatrix(n, m);
    BooleanMatrix B1 = createBooleanMatrix(n, m);
    fillBasisMatrices(&B0, &B1);

    BooleanMatrix columnVector = createBooleanMatrix(n, 1);
    int *rowIndices = createSetOfN(n, 0);

    probabilisticData *pData = xmalloc(sizeof(probabilisticData));
    pData->B0 = B0;
    pData->B1 = B1;
    pData->columnVector = columnVector;
    pData->sourceArray = data->source->array;
    pData->rowIndices = rowIndices;
    pData->share = data->shares;
    pData->randomSrc = data->randomSrc;
    pData->width = data->source->width;
    pData->height = data->source->height;

    return pData;
}

void __probabilisticAlgorithm(probabilisticData *data) {
    BooleanMatrix *B0 = &data->B0;
    BooleanMatrix *B1 = &data->B1;
    BooleanMatrix *columnVector = &data->columnVector;
    Pixel *sourceArray = data->sourceArray;
    int *rowIndices = data->rowIndices;
    Image *share = data->share;
    FILE *randomSrc = data->randomSrc;
    int imageSize = data->width * data->height;

    // for each pixel of the secret image
    for (int i = 0; i < imageSize; i++) {
        int sourcePixel = sourceArray[i];
        getRandomMatrixColumn(B0, B1, columnVector, sourcePixel, randomSrc);
        copyColumnElementsToShares(columnVector, share, i, rowIndices, randomSrc);
    }
}

void probabilisticAlgorithm(AlgorithmData *data) {
    probabilisticData *pData = prepareProbabilisticAlgorithm(data);
    __probabilisticAlgorithm(pData);
}
