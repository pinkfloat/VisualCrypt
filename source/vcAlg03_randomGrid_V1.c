#include <string.h>
#include "image.h"
#include "fileManagement.h"
#include "memoryManagement.h"
#include "vcAlg03_randomGrid_V1.h"

/********************************************************************
* Function:     fillPixelRG
*--------------------------------------------------------------------
* Description:  This function fills a sharePixel according to the
*               source and the pixel of the share from before.
*               It is used by the alternate RG algorithms.
********************************************************************/
static void fillPixelRG(Pixel sourcePixel, Pixel* sharePixel, int numberOfShares, FILE* randomSrc)
{
    Pixel tmp = sourcePixel;
    for (int idx = 1; idx < numberOfShares; idx++)
    {
        sharePixel[idx-1] = getRandomNumber(randomSrc,0,2);
        if (tmp) // source pixel is black
            sharePixel[idx] = sharePixel[idx-1] ? 0 : 1; // use complementary value of previous share

        else // source pixel is white
            sharePixel[idx] = sharePixel[idx-1]; // copy value of previous share
        tmp = sharePixel[idx];
    }
}

/********************************************************************
* Function:     alternate_nn_ThresholdRGA
*--------------------------------------------------------------------
* Description:  This alternate variant of the (n,n) random grid
*               algorithm by Tzung-Her Chen and Kai-Hsiang Tsao
*               calculates the contentes of all shares pixel by pixel,
*               instead of filling the shares one after another.
********************************************************************/
void alternate_nn_ThresholdRGA(Pixel* sourceArray, Image* shares, Pixel* tmpSharePixel, FILE* randomSrc, int arraySize, int numberOfShares)
{
    // for each pixel
    for(int i = 0; i < arraySize; i++)
    {
        fillPixelRG(sourceArray[i], tmpSharePixel, numberOfShares, randomSrc);
        // for each share
        for (int idx = 0; idx < numberOfShares; idx++)
            shares[idx].array[i] = tmpSharePixel[idx];
    }
}

/********************************************************************
* Function:     alternate_2n_ThresholdRGA
*--------------------------------------------------------------------
* Description:  This alternate variant of the (2,n) random grid
*               algorithm by Tzung-Her Chen and Kai-Hsiang Tsao
*               calculates the contentes of all shares pixel by pixel,
*               instead of filling the shares one after another.
********************************************************************/
void alternate_2n_ThresholdRGA(Pixel* sourceArray, Image* shares, FILE* randomSrc, int arraySize, int numberOfShares)
{
    Pixel* randomGrid = shares->array;

    // for each pixel
    for(int i = 0; i < arraySize; i++)
    {
        // fill pixel of share 1 random
        randomGrid[i] = getRandomNumber(randomSrc,0,2);

        // for share 2 to n
        for(int idx = 1; idx < numberOfShares; idx++)
        {
            if (sourceArray[i]) // source pixel is black
                shares[idx].array[i] = getRandomNumber(randomSrc,0,2); // get random 0/1

            else //source pixel is white
                shares[idx].array[i] = shares->array[i]; // copy value of share 1
        }
    }
}

/********************************************************************
* Function:     getSharePixel
*--------------------------------------------------------------------
* Description:  This function is used to get the actual pixel
*               from one of the shares in the alternate (k,n)RG
*               version.
********************************************************************/
static inline Pixel getSharePixel(void* sharePixel, int shareIdx, __attribute__((unused)) int matrixIdx)
{
    Pixel* _sharePixel = (Pixel*) sharePixel;
    return _sharePixel[shareIdx];
}

/********************************************************************
* Function:     __alternate_kn_ThresholdRGA
*--------------------------------------------------------------------
* Description:  This alternate variant of the (k,n) random grid
*               algorithm by Tzung-Her Chen and Kai-Hsiang Tsao
*               calculates the contentes of all shares pixel by pixel,
*               instead of filling the shares one after another.
********************************************************************/
void __alternate_kn_ThresholdRGA(kn_randomGridData* data)
{
    int* setOfN = data->setOfN;
    Pixel* sharePixel = data->sharePixel;
    Pixel* sourceArray = data->sourceArray;
    Image* shares = data->shares;
    FILE* randomSrc = data->randomSrc;
    int arraySize = data->arraySize;
    int n = data->n;
    int k = data->k;

    // for each pixel
    for(int i = 0; i < arraySize; i++)
    {
        /*  create values of sharePixel according to traditional
            RG-based VSS to encode a pixel
        */
        fillPixelRG(sourceArray[i], sharePixel, k, randomSrc);
        shuffleVector(setOfN, n, randomSrc);
        writePixelToShares(setOfN, sharePixel, shares, randomSrc, n, k, i, getSharePixel);
    }
}

/********************************************************************
* Function:     alternate_kn_ThresholdRGA
*--------------------------------------------------------------------
* Description:  This is a wrapper for the alternate (k,n)-threshold
*               random grid algorithm introduced by Tzung-Her Chen
*               and Kai-Hsiang Tsao.
********************************************************************/
void alternate_kn_ThresholdRGA(Image* source, Image* shares, FILE* randomSrc, int arraySize, int n)
{
    int k = 2;

    if (n > 2)
        k = getKfromUser(n);

    int* setOfN = createSetOfN(n, 1);
    Pixel* sharePixel = xmalloc(k * sizeof(Pixel));

    kn_randomGridData rgData = {
        .setOfN = setOfN,
        .sharePixel = sharePixel,
        .sourceArray = source->array,
        .shares = shares,
        .randomSrc = randomSrc,
        .arraySize = arraySize,
        .n = n,
        .k = k
    };

    __alternate_kn_ThresholdRGA(&rgData);
}
