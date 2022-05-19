#include <string.h>
#include "image.h"
#include "fileManagement.h"
#include "memoryManagement.h"
#include "vcAlg03_randomGrid_V0.h"

/********************************************************************
* Function:     createRandomGrid
*--------------------------------------------------------------------
* Description:  This function will make the share array to a random
*               grid, which means, that it will be filled completely
*               random with black and white pixel.
*               It is used by the non-alternate RG algorithms.
********************************************************************/
static void createRandomGrid(Image* share, FILE* urandom)
{
    int arraySize = share->width * share->height;
    Pixel* shareArray = share->array;

    /* for each pixel */
    for(int i = 0; i < arraySize; i++)
    {
        /* get random 0/1 */
        shareArray[i] = getRandomNumber(urandom,0,2);
    }
}

/********************************************************************
* Function:     randomGrid_22_Threshold
*--------------------------------------------------------------------
* Description:  This is an implementation of the (2,2)-threshold
*               random grid algorithm introduced by O. Kafri and
*               E. Karen. It will make share1 to a random grid i.e.
*               a completely random filled image and calculates
*               share2 by using share1 and the source.
********************************************************************/
static void randomGrid_22_Threshold(Pixel* source, Image* shares, FILE* urandom, int arraySize)
{
    Pixel* share1 = shares->array;
    Pixel* share2 = shares[1].array;

    /* make the first share a random grid */
    createRandomGrid(shares, urandom);

    /* fill the second share according to source and first share*/
    for(int i = 0; i < arraySize; i++)
    {
        /* if the source pixel is black */
        if (source[i])
            /* use the complementary value of share 1 */
            share2[i] = share1[i] ? 0 : 1;

        /* if the source pixel is white */
        else
            /* copy value of share 1 */
            share2[i] = share1[i];
    }
}

/********************************************************************
* Function:     randomGrid_nn_Threshold
*--------------------------------------------------------------------
* Description:  This is an implementation of a (n,n)-threshold random
*               grid algorithm introduced by Tzung-Her Chen and
*               Kai-Hsiang Tsao. It will calculate the pixel of the
*               share images by calling recursively the (2,2)-threshold
*               random grid algorithm from O. Kafri and E. Karen.
********************************************************************/
void randomGrid_nn_Threshold(Pixel* sourceArray, Image* shares, Pixel** storage, FILE* urandom, int arraySize, int numberOfShares)
{
    Pixel* tmp;
    
    /* fill the first two shares */
    randomGrid_22_Threshold(sourceArray, shares, urandom, arraySize);

    if(numberOfShares > 2)
    {
        for(int idx = 2; idx < numberOfShares; idx++)
        {
            tmp = *storage;
            *storage = shares[idx-1].array;
            shares[idx-1].array = tmp;
            randomGrid_22_Threshold(*storage, &shares[idx-1], urandom, arraySize);
        }
    }
}

/********************************************************************
* Function:     randomGrid_2n_Threshold
*--------------------------------------------------------------------
* Description:  This is an implementation of a (2,n)-threshold random
*               grid algorithm introduced by Tzung-Her Chen and
*               Kai-Hsiang Tsao. In contradistinction to the (n,n)
*               algorithms this algorithm reveals the secret image
*               as soon as two of the shares are stacked together,
*               independent from the amount of shares existing.
*               If more than two shares are stacked, the revealed
*               image becomes darker.
********************************************************************/
void randomGrid_2n_Threshold(Pixel* sourceArray, Image* shares, FILE* urandom, int arraySize, int numberOfShares)
{
    /* make the first share a random grid */
    createRandomGrid(shares, urandom);

    /* for each share */
    for(int idx = 1; idx < numberOfShares; idx++)
    {
        /* for each pixel ... */
        /* fill the other shares according to source and first share */
        for(int i = 0; i < arraySize; i++)
        {
            /* if the source pixel is black */
            if (sourceArray[i])
                /* get random 0/1 */
                shares[idx].array[i] = getRandomNumber(urandom,0,2);

            /* if the source pixel is white */
            else
                /* copy value of share 1 */
                shares[idx].array[i] = shares->array[i];
        }
    }
}

/********************************************************************
* Function:     getPixelFromShare
*--------------------------------------------------------------------
* Description:  This function is used to get the value of a pixel
*               from one of the additional shares in the
*               non-alternate (k,n)RG version.
********************************************************************/
static inline Pixel getPixelFromShare(void* shares, int shareIdx, int matrixIdx)
{
    Image* _shares = (Image*) shares;
    return _shares[shareIdx].array[matrixIdx];
}

/********************************************************************
* Function:     __randomGrid_kn_Threshold
*--------------------------------------------------------------------
* Description:  This is an implementation of a (k,n)-threshold random
*               grid algorithm introduced by Tzung-Her Chen and
*               Kai-Hsiang Tsao. In contradistinction to the (n,n)
*               algorithms this algorithm reveals the secret image
*               as soon as <k> of the shares are stacked together,
*               independent from the amount of shares existing.
*               If more than <k> shares are stacked, the noise
*               decreases the image quality.
********************************************************************/
void __randomGrid_kn_Threshold(kn_randomGridData* data)
{
    Pixel* setOfN = data->setOfN;
    Pixel* randSortedSetOfN = data->randSortedSetOfN;
    Pixel* checkList = data->checkList;
    Image* shares = data->shares;
    Image* additShares = data->additShares;
    FILE* urandom = data->urandom;
    int arraySize = data->arraySize;
    int n = data->n;
    int k = data->k;

    Copy copy = {
        .source = setOfN,
        .dest = randSortedSetOfN
    };

    /* for each pixel */
    for(int i = 0; i < arraySize; i++)
    {
        memset(checkList, 0, n*sizeof(Pixel));
        randomSortVector(&copy, checkList, urandom, n);

        writePixelToShares(randSortedSetOfN, additShares, shares, urandom, n, k, i, getPixelFromShare);
    }
}

/********************************************************************
* Function:     randomGrid_kn_Threshold
*--------------------------------------------------------------------
* Description:  This is a wrapper for the (k,n)-threshold random
*               grid algorithm introduced by Tzung-Her Chen and
*               Kai-Hsiang Tsao.
********************************************************************/
void randomGrid_kn_Threshold(Image* source, Image* shares, Pixel** storage, FILE* urandom, int arraySize, int n)
{
    Pixel* sourceArray = source->array;

    if(n == 2)
    {
        randomGrid_22_Threshold(sourceArray, shares, urandom, arraySize);
        return;
    }

    int k = getKfromUser(n);

    Pixel* setOfN = createSetOfN(n);
    Pixel* randSortedSetOfN = xmalloc(n * sizeof(Pixel));
    Pixel* checkList = xmalloc(n * sizeof(Pixel));

    /* create additional k shares */
    Image* additShares = xmalloc(k*sizeof(Image));
    mallocSharesOfSourceSize(source, additShares, k);

    kn_randomGridData rgData = {
        .setOfN = setOfN,
        .randSortedSetOfN = randSortedSetOfN,
        .checkList = checkList,
        .sharePixel = NULL,
        .sourceArray = NULL,
        .shares = shares,
        .additShares = additShares,
        .urandom = urandom,
        .arraySize = arraySize,
        .n = n,
        .k = k
    };

    /* fill the temporary shares */
    randomGrid_nn_Threshold(sourceArray, additShares, storage, urandom, arraySize, k);

    __randomGrid_kn_Threshold(&rgData);
}
