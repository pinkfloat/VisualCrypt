#include <string.h>
#include "image.h"
#include "fileManagement.h"
#include "memoryManagement.h"
#include "menu.h"
#include "vcAlg03_randomGrid_V0.h"

/********************************************************************
* Function:     createRandomGrid
*--------------------------------------------------------------------
* Description:  This function will make the share array to a random
*               grid, which means, that it will be filled completely
*               random with black and white pixel.
*               It is used by the non-alternate RG algorithms.
********************************************************************/
static void createRandomGrid(Image* share, FILE* randomSrc)
{
    int arraySize = share->width * share->height;
    Pixel* shareArray = share->array;

    // for each pixel
    for(int i = 0; i < arraySize; i++)
    {
        // get random 0/1
        shareArray[i] = getRandomNumber(randomSrc,0,2);
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
static void randomGrid_22_Threshold(Pixel* source, Image* shares, FILE* randomSrc, int arraySize)
{
    Pixel* share1 = shares->array;
    Pixel* share2 = shares[1].array;

    // make the first share a random grid
    createRandomGrid(shares, randomSrc);

    // fill the second share according to source and first share
    for(int i = 0; i < arraySize; i++)
    {
        if (source[i]) // source pixel is black
            share2[i] = share1[i] ? 0 : 1; // use complementary value of share 1

        else // source pixel is white
            share2[i] = share1[i]; // copy value of share 1
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
void randomGrid_nn_Threshold(Pixel* sourceArray, Image* shares, Pixel** storage, FILE* randomSrc, int arraySize, int numberOfShares)
{
    Pixel* tmp;
    
    // fill the first two shares
    randomGrid_22_Threshold(sourceArray, shares, randomSrc, arraySize);

    if(numberOfShares > 2)
    {
        for(int idx = 2; idx < numberOfShares; idx++)
        {
            /* use calculated share from last call as source and overwrite
               source buffer from two calls before (content is not longer needed)
            */
            tmp = *storage;
            *storage = shares[idx-1].array;
            shares[idx-1].array = tmp;
            randomGrid_22_Threshold(*storage, &shares[idx-1], randomSrc, arraySize);
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
void randomGrid_2n_Threshold(Pixel* sourceArray, Image* shares, FILE* randomSrc, int arraySize, int numberOfShares)
{
    // make the first share a random grid
    createRandomGrid(shares, randomSrc);

    // for each share
    for(int idx = 1; idx < numberOfShares; idx++)
    {
        // for each pixel
        // fill the other shares according to source and first share
        for(int i = 0; i < arraySize; i++)
        {
            if (sourceArray[i]) //source pixel is black
                shares[idx].array[i] = getRandomNumber(randomSrc,0,2); // get random 0/1

            else  //source pixel is white
                shares[idx].array[i] = shares->array[i]; // copy value of share 1
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
    int* setOfN = data->setOfN;
    Image* shares = data->shares;
    Image* additShares = data->additShares;
    FILE* randomSrc = data->randomSrc;
    int arraySize = data->arraySize;
    int n = data->n;
    int k = data->k;

    // for each pixel
    for(int i = 0; i < arraySize; i++)
    {
        shuffleVector(setOfN, n, randomSrc);
        writePixelToShares(setOfN, additShares, shares, randomSrc, n, k, i, getPixelFromShare);
    }
}

/********************************************************************
* Function:     randomGrid_kn_Threshold
*--------------------------------------------------------------------
* Description:  This is a wrapper for the (k,n)-threshold random
*               grid algorithm introduced by Tzung-Her Chen and
*               Kai-Hsiang Tsao.
********************************************************************/
void randomGrid_kn_Threshold(Image* source, Image* shares, Pixel** storage, FILE* randomSrc, int arraySize, int n)
{
    Pixel* sourceArray = source->array;

    if(n == 2)
    {
        randomGrid_22_Threshold(sourceArray, shares, randomSrc, arraySize);
        return;
    }

    int k = getKfromUser(n);

    int* setOfN = createSetOfN(n, 1);

    // create additional k shares
    Image* additShares = xmalloc(k*sizeof(Image));
    mallocSharesOfSourceSize(source, additShares, k);

    kn_randomGridData rgData = {
        .setOfN = setOfN,
        .shares = shares,
        .additShares = additShares,
        .randomSrc = randomSrc,
        .arraySize = arraySize,
        .n = n,
        .k = k
    };

    // fill temporary shares
    randomGrid_nn_Threshold(sourceArray, additShares, storage, randomSrc, arraySize, k);

    // call algorithm
    __randomGrid_kn_Threshold(&rgData);
}
