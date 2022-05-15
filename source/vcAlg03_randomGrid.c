 
#include <string.h>
#include "image.h"
#include "menu.h"
#include "random.h"
#include "fileManagement.h"
#include "memoryManagement.h"
#include "vcAlgorithms.h"

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
* Function:     randomSortVector
*--------------------------------------------------------------------
* Description:  This function will copy the contents of one vector
*               random sorted to another.
********************************************************************/
static void randomSortVector(Copy* copy, Pixel* checkList, FILE* urandom, int n)
{            
    for(int idx = 0; idx < n; idx++)
    {
        copy->destIdx = idx;
        int randNum = getRandomNumber(urandom, 1, n-idx);
        randomSort(randNum, checkList, copy, copyVectorElement);
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
* Function:     writePixelToShares
*--------------------------------------------------------------------
* Description:  If the share number is part of the first k elements
*               of the random sorted set (of size n), the very share
*               will get the pixel that was calculated for one of
*               the shares, from the source image, before.
*               Shares with a number not contained in the first
*               k elements will get randomly a 0/1.
********************************************************************/
static void writePixelToShares(Pixel* randSortedSetOfN, void* source, Image* shares, FILE* urandom, int n, int k, int i, Pixel (*getPixel)(void*, int, int))
{
    /* for each share */
    for(int idx = 0; idx < n; idx++)
    {
        int found = -1;
        /* if idx+1 is part of the first k elements of randSortedSetOfN */
        for (int idk = 0; idk < k; idk++)
        {
            if (randSortedSetOfN[idk] == idx+1)
            {
                found = idk;
                break;
            }
        }
        if (found != -1)
        {
            shares[idx].array[i] = getPixel(source, found, i);
        }
        else
        {
            shares[idx].array[i] = getRandomNumber(urandom,0,2);
        }
    }
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
* Function:     getKfromUser
*--------------------------------------------------------------------
* Description:  Ask the user the number of the k shares in a
*               (k,n) RG-Algorithm.
********************************************************************/
static int getKfromUser(int n)
{
    int valid = 0, k;
    char prompt[50];
    memset(prompt, '\0', sizeof(prompt));
    snprintf(prompt, sizeof(prompt), "Enter number for k:\n<min> = 2\n<max> = %d\n", n);
    do
    {
        clear();
        valid = getNumber(prompt, 2, n, &k);
    } while (!valid);
    
    return k;
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

    /* create vector with values from 1 to n */
    Pixel* setOfN = xmalloc(n * sizeof(Pixel));
    for (int i = 0; i < n; i++)
        setOfN[i] = i+1;

    Pixel* randSortedSetOfN = xmalloc(n * sizeof(Pixel));
    Pixel* checkList = xmalloc(n * sizeof(Pixel));

    /* create additional k shares */
    Image* additShares = xmalloc(k*sizeof(Image));
    for(int i = 0; i < k; i++)
    {
        additShares[i].width = source->width;
        additShares[i].height = source->height;
        mallocPixelArray(&additShares[i]);
    }

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

    /* create vector with values from 1 to n */
    Pixel* setOfN = xmalloc(n * sizeof(Pixel));
    for (int i = 0; i < n; i++)
        setOfN[i] = i+1;

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

/********************************************************************
* Function:     callRandomGridAlgorithm(
*--------------------------------------------------------------------
* Description:  Prepares data which is needed by all or at least
*               multiple random grid algorithms and calls the chosen
*               algorithm with the data.
********************************************************************/
void callRandomGridAlgorithm(AlgorithmData* data)
{
    Image* source = data->source;
    Image* shares = data->shares;
    uint8_t n = data->numberOfShares;
    uint8_t algorithmNumber = data->algorithmNumber;
    FILE* urandom = data->urandom;

    Pixel* sourceArray = source->array;
    int arraySize = source->width * source->height;

    /* allocate pixel-arrays for the shares */
    mallocSharesOfSourceSize(source, shares, n);
    Pixel* storage = xmalloc(arraySize);
    Pixel* tmpSharePixel = xmalloc(n*sizeof(Pixel));

    switch(algorithmNumber)
	{
		case 1: randomGrid_nn_Threshold(sourceArray, shares, &storage, urandom, arraySize, n);              break;
		case 2: randomGrid_2n_Threshold(sourceArray, shares, urandom, arraySize, n);                        break;
		case 3:	randomGrid_kn_Threshold(source, shares, &storage, urandom, arraySize, n);                   break;

        case 4: alternate_nn_ThresholdRGA(source->array, shares, tmpSharePixel, urandom, arraySize, n);     break;
        case 5: alternate_2n_ThresholdRGA(source->array, shares, urandom, arraySize, n);                    break;
        case 6: alternate_kn_ThresholdRGA(source, shares, urandom, arraySize, n);                           break;
		default: break;
	}
}
