#include <string.h>
#include "fileManagement.h"
#include "memoryManagement.h"
#include "random.h"
#include "booleanMatrix.h"
#include "vcAlgorithms.h"

/********************************************************************
* Function:     getRandomMatrixColumn
*--------------------------------------------------------------------
* Description:  Get a random column of either B0 or B1 and store
*               it the vector "columnVector".
* Input:        B0 = Basis matrix for white share-pixels
*               B1 = Basis matrix for black share-pixels
*               source = the secret image, containing an array of
*               black and white pixel
*               i = line of the source pixel we are looking at
*               j = columns of the source pixel we are looking at
*               urandom = the opened file /dev/urandom, containing
*               random numbers
* Output:       columnVector = stores the randomly chosen column
********************************************************************/
static void getRandomMatrixColumn(  BooleanMatrix* B0,
                                    BooleanMatrix* B1, 
                                    BooleanMatrix* columnVector,
                                    Image* source,
                                    int i,
                                    int j,
                                    FILE* urandom)
{
    int n = B0->n; /* number of rows */
    int m = B0->m; /* number of columns */

    int randNum = getRandomNumber(urandom, 0, m);

    /* if the source pixel is black */
    if (source->array[i * source->width + j])
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
* Function:     copyRandomPixelToShare
*--------------------------------------------------------------------
* Description:  The function will copy value of "randPixel" to the
*               share file array on row posY and column posX.
********************************************************************/
static inline void copyRandomPixelToShare(Pixel* randPixel, Image* share, int posY, int posX)
{
    share->array[posY * share->width + posX] = *randPixel;
}

/********************************************************************
* Function:     copyColumnElementsToShares
*--------------------------------------------------------------------
* Description:  Get use the vector "columnVector" to copy one random
*               element of it to each share, whithout using an
*               vector element twice.
* Input:        columnVector = a randomly chosen column of a basis
*               matrix
*               i = line of the destination pixel in a share,
*               j = column of the destination pixel in a share,
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
                                        int i,
                                        int j,
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
        randNum = getRandomNumber(urandom, 1, n-shareIdx); /* number between 1 and "number-of-shares minus shareIdx" */
        randomSort(randNum, checkList, &copy, copyColumnElement);
        copyRandomPixelToShare(&randPixel, &share[shareIdx], i, j);
    }
}

/********************************************************************
* Function:     fillProbabilisticShareArrays
*--------------------------------------------------------------------
* Description:  This function will fill the pixel arrays of the
*               shares with the values "1" being black and "0" being
*               white. Therefore the already calculated basis
*               matrices B0 and B1 are used. For a black Pixel,
*               each share will get an other element of a random
*               column of B1, and for a white Pixel, the same
*               happens with a column of B0.
********************************************************************/
static void fillProbabilisticShareArrays(Image* source, Image* share, BooleanMatrix* B0, BooleanMatrix* B1)
{
    int n = B0->n;

    /* open urandom, to get random numbers from it */
    FILE* urandom = xfopen("/dev/urandom", "r");

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

    /* for each pixel of the source */
    for(int i = 0; i < source->height; i++)     /* rows */
    {
        for(int j = 0; j < source->width; j++)  /* columns */
        {
            getRandomMatrixColumn(B0, B1, &columnVector, source, i, j, urandom);
            copyColumnElementsToShares(&columnVector, share, i, j, checkList, urandom);
        }
    }
    xfclose(urandom);
}

/********************************************************************
* Function:     probabilisticAlgorithm
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
void probabilisticAlgorithm(AlgorithmData* data)
{
    uint8_t n = data->numberOfShares;
    uint8_t m = 1 << (n-1);

    /* allocate pixel-arrays for the shares */
	mallocSharesOfSourceSize(data->source, data->shares, n);

    /* allocate basis matrices */
    BooleanMatrix B0 = createBooleanMatrix(n,m);
    BooleanMatrix B1 = createBooleanMatrix(n,m);

    fillBasisMatrices(&B0, &B1);
    
    /* Fill pixel-arrays of the shares */
    fillProbabilisticShareArrays(data->source, data->shares, &B0, &B1);
}
