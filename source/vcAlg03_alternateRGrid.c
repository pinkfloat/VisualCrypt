#include <string.h>
#include "image.h"
#include "menu.h"
#include "random.h"
#include "fileManagement.h"
#include "memoryManagement.h"
#include "vcAlgorithms.h"

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

void __alternate_nn_ThresholdRGA(Pixel* sourceArray, Image* shares, Pixel* tmpSharePixel, FILE* urandom, int arraySize, int numberOfShares)
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

void alternate_nn_ThresholdRGA(AlgorithmData* data)
{
    int numberOfShares = data->numberOfShares;
    Image* source = data->source;
    Image* shares = data->shares;
    int arraySize = source->width * source->height;

    /* allocate pixel-arrays for the shares */
	mallocSharesOfSourceSize(source, shares, numberOfShares);
    Pixel* tmpSharePixel = xmalloc(numberOfShares*sizeof(Pixel));

    /* open urandom, to get random numbers from it */
    FILE* urandom = xfopen("/dev/urandom", "r");

    __alternate_nn_ThresholdRGA(source->array, shares, tmpSharePixel, urandom, arraySize, numberOfShares);

    xfree(tmpSharePixel);
    xfclose(urandom);
}

void __alternate_2n_ThresholdRGA(Pixel* sourceArray, Image* shares, FILE* urandom, int arraySize, int numberOfShares)
{
    /* for each pixel ... */
    /* fill the other shares according to source and first share */
    for(int i = 0; i < arraySize; i++)
    {
        /* fill share 1 random */
        shares->array[i] = getRandomNumber(urandom,0,2);

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

void alternate_2n_ThresholdRGA(AlgorithmData* data)
{
    int numberOfShares = data->numberOfShares;
    Image* source = data->source;
    Image* shares = data->shares;
    int arraySize = source->width * source->height;

    /* allocate pixel-arrays for the shares */
	mallocSharesOfSourceSize(source, shares, numberOfShares);

    /* open urandom, to get random numbers from it */
    FILE* urandom = xfopen("/dev/urandom", "r");

    __alternate_2n_ThresholdRGA(source->array, shares, urandom, arraySize, numberOfShares);
    
    xfclose(urandom);
}

void __alternate_kn_ThresholdRGA(kn_randomGridData* data)
{
    Pixel* setOfN = data->setOfN;
    Pixel* randSortedSetOfN = data->randSortedSetOfN;
    Pixel* calculatedValues = data->calculatedValues;
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

    /* for each pixel of the source */
    for(int i = 0; i < arraySize; i++)
    {
        memset(checkList, 0, n*sizeof(Pixel));

        /*  fill randSortedSetOfN randomly with integers from setOfN */
        for(int idx = 0; idx < n; idx++)
        {
            copy.destIdx = idx;
            int randNum = getRandomNumber(urandom, 1, n-idx);
            randomSort(randNum, checkList, &copy, copyVectorElement);
        }

        /*  create values of calculatedValues according to traditional
            RG-based VSS to encode a pixel
        */
        fillPixelRG(sourceArray[i], calculatedValues, k, urandom);


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
                shares[idx].array[i] = calculatedValues[found];
            }
            else
            {
                shares[idx].array[i] = getRandomNumber(urandom,0,2);
            }
        }
    }
}

void alternate_kn_ThresholdRGA(AlgorithmData* data)
{
    int n = data->numberOfShares;
    int k = 2;
    Image* source = data->source;
    Image* shares = data->shares;
    int arraySize = source->width * source->height;

    /* allocate pixel-arrays for the shares */
	mallocSharesOfSourceSize(source, shares, n);

    if (n > 2)
    {
        /* get k from user */
        int valid = 0;
        char prompt[50];
        memset(prompt, '\0', sizeof(prompt));
        snprintf(prompt, sizeof(prompt), "Enter number for k:\n<min> = 2\n<max> = %d\n", n);
        do
        {
            clear();
            valid = getNumber(prompt, 2, n, &k);
        } while (!valid);
    }

    /* open urandom, to get random numbers from it */
    FILE* urandom = xfopen("/dev/urandom", "r");

    /* create vector with values from 1 to n */
    Pixel* setOfN = xmalloc(n * sizeof(Pixel));
    for (int i = 0; i < n; i++)
        setOfN[i] = i+1;

    Pixel* randSortedSetOfN = xmalloc(n * sizeof(Pixel));
    Pixel* calculatedValues = xmalloc(k * sizeof(Pixel));
    Pixel* checkList = xmalloc(n * sizeof(Pixel));

    kn_randomGridData rgData = {
        .setOfN = setOfN,
        .randSortedSetOfN = randSortedSetOfN,
        .checkList = checkList,
        .calculatedValues = calculatedValues,
        .sourceArray = source->array,
        .shares = shares,
        .extraShares = NULL,
        .urandom = urandom,
        .arraySize = arraySize,
        .n = n,
        .k = k
    };

    __alternate_kn_ThresholdRGA(&rgData);

    xfclose(urandom);
}
