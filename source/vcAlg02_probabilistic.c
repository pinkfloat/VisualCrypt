#include <string.h>
#include "fileManagement.h"
#include "memoryManagement.h"
#include "random.h"
#include "vcAlg02_probabilistic.h"

/********************************************************************
* Function:     getRandomMatrixColumn
*--------------------------------------------------------------------
* Description:  Get a random column of either B0 or B1 and store
*               it the vector "columnVector".
* Input:        B0 = Basis matrix for white share-pixels
*               B1 = Basis matrix for black share-pixels
*               sourcePixel = pixel of the secret image (0/1)
*               urandom = the opened file /dev/urandom, containing
*               random numbers
* Output:       columnVector = stores the randomly chosen column
********************************************************************/
static void getRandomMatrixColumn(  BooleanMatrix* B0,
                                    BooleanMatrix* B1, 
                                    BooleanMatrix* columnVector,
                                    Pixel sourcePixel,
                                    FILE* urandom)
{
    int n = B0->n; /* number of rows */
    int m = B0->m; /* number of columns */

    int randNum = getRandomNumber(urandom, 0, m);

    /* if the source pixel is black */
    if (sourcePixel)
    {
        /* get column of basis matrix B1 */
        for(int idx = 0; idx < n; idx++)
            setPixel(columnVector, idx, 0, getPixel(B1, idx, randNum));
    }
    /* if the source pixel is white */
    else
    {
        /* get column of basis matrix B0 */
        for(int idx = 0; idx < n; idx++)
            setPixel(columnVector, idx, 0, getPixel(B0, idx, randNum));
    }
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
*               urandom = the opened file /dev/urandom, containing
*               random numbers
* Output:       share = containing pixel arrays that need to be
*               filled. If they are stacked together per OR-function,
*               the secret image can be seen.
********************************************************************/
static void copyColumnElementsToShares( BooleanMatrix* columnVector,
                                        Image* share,
                                        int sharePixelPosition,
                                        Pixel* checkList,
                                        FILE* urandom)
{
    int n = columnVector->n;
    int randNum;
    Pixel randPixel;
    Copy copy = {
        .dest = &randPixel,
        .source = columnVector
    };
    memset(checkList, 0, n*sizeof(Pixel));

    /* for each share */
    for(int shareIdx = 0; shareIdx < n; shareIdx++)
    {
        /* choose random which share will get which column element */
        randNum = getRandomNumber(urandom, 1, n-shareIdx);
        randomSort(randNum, checkList, &copy, copyColumnElement);
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
    Pixel* checkList = data->checkList;
    Image* share = data->share;
    FILE* urandom = data->urandom;
    int width = data->width;
    int height = data->height;

    /* for each pixel of the source */
    for(int i = 0; i < height; i++)     /* rows */
    {
        for(int j = 0; j < width; j++)  /* columns */
        {
            int sharePixelPosition = i * width + j;
            int sourcePixel = sourceArray[sharePixelPosition];
            getRandomMatrixColumn(B0, B1, columnVector, sourcePixel, urandom);
            copyColumnElementsToShares(columnVector, share, sharePixelPosition, checkList, urandom);
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
    uint8_t n = data->numberOfShares;
    uint8_t m = 1 << (n-1);

    /* allocate pixel-arrays for the shares */
	mallocSharesOfSourceSize(data->source, data->shares, n);

    /* create basis matrices */
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
    Pixel* checkList = xmalloc(n * sizeof(Pixel));

    probabilisticData pData = {
        .B0 = &B0,
        .B1 = &B1,
        .columnVector = &columnVector,
        .sourceArray = data->source->array,
        .checkList = checkList,
        .share = data->shares,
        .urandom = data->urandom,
        .width = data->source->width,
        .height = data->source->height
    };

    __probabilisticAlgorithm(&pData);
}
