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
static void fillPixelRG(Pixel sourcePixel, Pixel* sharePixel, int numberOfShares, FILE* urandom)
{
    Pixel tmp = sourcePixel;
    for (int idx = 1; idx < numberOfShares; idx++)
    {
        sharePixel[idx-1] = getRandomNumber(urandom,0,2);
        /* if the source pixel is black */
        if (tmp)
            /* use complementary value of previous share */
            sharePixel[idx] = sharePixel[idx-1] ? 0 : 1;

        /* if the source pixel is white */
        else
            /* copy value of previous share */
            sharePixel[idx] = sharePixel[idx-1];
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
void alternate_nn_ThresholdRGA(Pixel* sourceArray, Image* shares, Pixel* tmpSharePixel, FILE* urandom, int arraySize, int numberOfShares)
{
    /* for each pixel */
    for(int i = 0; i < arraySize; i++)
    {
        fillPixelRG(sourceArray[i], tmpSharePixel, numberOfShares, urandom);
        /* for each share */
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
void alternate_2n_ThresholdRGA(Pixel* sourceArray, Image* shares, FILE* urandom, int arraySize, int numberOfShares)
{
    uint8_t* randomGrid = shares->array;

    /* for each pixel ... */
    /* fill the other shares according to source and first share */
    for(int i = 0; i < arraySize; i++)
    {
        /* fill share 1 random */
        randomGrid[i] = getRandomNumber(urandom,0,2);

        /* for each share */
        for(int idx = 1; idx < numberOfShares; idx++)
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
    Pixel* setOfN = data->setOfN;
    Pixel* randSortedSetOfN = data->randSortedSetOfN;
    Pixel* sharePixel = data->sharePixel;
    Pixel* sourceArray = data->sourceArray;
    Pixel* checkList = data->checkList;
    Image* shares = data->shares;
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

        /*  create values of sharePixel according to traditional
            RG-based VSS to encode a pixel
        */
        fillPixelRG(sourceArray[i], sharePixel, k, urandom);

        writePixelToShares(randSortedSetOfN, sharePixel, shares, urandom, n, k, i, getSharePixel);
    }
}

/********************************************************************
* Function:     alternate_kn_ThresholdRGA
*--------------------------------------------------------------------
* Description:  This is a wrapper for the alternate (k,n)-threshold
*               random grid algorithm introduced by Tzung-Her Chen
*               and Kai-Hsiang Tsao.
********************************************************************/
void alternate_kn_ThresholdRGA(Image* source, Image* shares, FILE* urandom, int arraySize, int n)
{
    int k = 2;

    if (n > 2)
        k = getKfromUser(n);

    Pixel* setOfN = createSetOfN(n);
    Pixel* randSortedSetOfN = xmalloc(n * sizeof(Pixel));
    Pixel* sharePixel = xmalloc(k * sizeof(Pixel));
    Pixel* checkList = xmalloc(n * sizeof(Pixel));

    kn_randomGridData rgData = {
        .setOfN = setOfN,
        .randSortedSetOfN = randSortedSetOfN,
        .checkList = checkList,
        .sharePixel = sharePixel,
        .sourceArray = source->array,
        .shares = shares,
        .additShares = NULL,
        .urandom = urandom,
        .arraySize = arraySize,
        .n = n,
        .k = k
    };

    __alternate_kn_ThresholdRGA(&rgData);
}
