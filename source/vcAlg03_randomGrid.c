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
* Function:     __randomGrid_22_Threshold
*--------------------------------------------------------------------
* Description:  This is an implementation of the (2,2)-threshold
*               random grid algorithm introduced by O. Kafri and
*               E. Karen. It will make share1 to a random grid i.e.
*               a completely random filled image and calculates
*               share2 by using share1 and the source.
********************************************************************/
static void __randomGrid_22_Threshold(Pixel* source, Image* shares, FILE* urandom, int arraySize)
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
* Function:     randomGrid_22_Threshold
*--------------------------------------------------------------------
* Description:  This is a wrapper for the (2,2)-threshold
*               random grid algorithm introduced by O. Kafri and
*               E. Karen.
********************************************************************/
static void randomGrid_22_Threshold(AlgorithmData* data)
{
    int arraySize = data->source->width * data->source->height;
    Pixel* sourceArray = data->source->array;
    Image* shares = data->shares;

    /* open urandom, to get random numbers from it */
    FILE* urandom = xfopen("/dev/urandom", "r");

    __randomGrid_22_Threshold(sourceArray, shares, urandom, arraySize);

    xfclose(urandom);
}

/********************************************************************
* Function:     __randomGrid_nn_Threshold
*--------------------------------------------------------------------
* Description:  This is an implementation of a (n,n)-threshold random
*               grid algorithm introduced by Tzung-Her Chen and
*               Kai-Hsiang Tsao. It will calculate the pixel of the
*               share images by calling recursively the (2,2)-threshold
*               random grid algorithm from O. Kafri and E. Karen.
********************************************************************/
void __randomGrid_nn_Threshold(Pixel* sourceArray, Image* shares, Pixel** storage, FILE* urandom, int arraySize, int numberOfShares)
{
    Pixel* tmp;
    
    /* fill the first two shares */
    __randomGrid_22_Threshold(sourceArray, shares, urandom, arraySize);

    if(numberOfShares > 2)
    {
        for(int idx = 2; idx < numberOfShares; idx++)
        {
            tmp = *storage;
            *storage = shares[idx-1].array;
            shares[idx-1].array = tmp;
            __randomGrid_22_Threshold(*storage, &shares[idx-1], urandom, arraySize);
        }
    }
}

/********************************************************************
* Function:     randomGrid_nn_Threshold
*--------------------------------------------------------------------
* Description:  This is a wrapper for the (n,n)-threshold random
*               grid algorithm introduced by Tzung-Her Chen and
*               Kai-Hsiang Tsao.
********************************************************************/
void randomGrid_nn_Threshold(AlgorithmData* data)
{
    Image* source = data->source;
    Image* shares = data->shares;
    int numberOfShares = data->numberOfShares;
    int arraySize = source->width * source->height;

    /* allocate pixel-arrays for the shares */
	mallocSharesOfSourceSize(source, shares, numberOfShares);
    Pixel* storage = xmalloc(arraySize);

    /* open urandom, to get random numbers from it */
    FILE* urandom = xfopen("/dev/urandom", "r");

    __randomGrid_nn_Threshold(source->array, shares, &storage, urandom, arraySize, numberOfShares);

    xfree(storage);
    xfclose(urandom);
}

/********************************************************************
* Function:     __randomGrid_2n_Threshold
*--------------------------------------------------------------------
* Description:  This is an implementation of a (2,n)-threshold random
*               grid algorithm introduced by Tzung-Her Chen and
*               Kai-Hsiang Tsao. In contradistinction to the (n,n)
*               algorithms this algorithm reveals the secret image
*               as soon as two of the shares are stacked together,
*               independent from the amount of shares existing.
*               If more than two shares are stacked, the revealed
*               image becomes clearer.
********************************************************************/
void __randomGrid_2n_Threshold(Pixel* sourceArray, Image* shares, FILE* urandom, int arraySize, int numberOfShares)
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
* Function:     randomGrid_2n_Threshold
*--------------------------------------------------------------------
* Description:  This is a wrapper for the (2,n)-threshold random
*               grid algorithm introduced by Tzung-Her Chen and
*               Kai-Hsiang Tsao.
********************************************************************/
void randomGrid_2n_Threshold(AlgorithmData* data)
{
    int numberOfShares = data->numberOfShares;
    Image* source = data->source;
    Image* shares = data->shares;
    int arraySize = source->width * source->height;

    /* allocate pixel-arrays for the shares */
	mallocSharesOfSourceSize(source, shares, numberOfShares);

    /* open urandom, to get random numbers from it */
    FILE* urandom = xfopen("/dev/urandom", "r");

    __randomGrid_2n_Threshold(source->array, shares, urandom, arraySize, numberOfShares);
    
    xfclose(urandom);
}

/********************************************************************
* Function:     randomGrid_kn_Threshold
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
void randomGrid_kn_Threshold(AlgorithmData* data)
{
    Image* source = data->source;
    Image* shares = data->shares;
    int arraySize = source->width * source->height;
    int n = data->numberOfShares;

    /* allocate pixel-arrays for the shares */
	mallocSharesOfSourceSize(source, shares, n);

    if(n == 2)
    {
        /* just call randomGrid_22_Threshold */
        randomGrid_22_Threshold(data);
        return;
    }

    /* get k from user */
    int valid = 0, k;
    char prompt[50];
    memset(prompt, '\0', sizeof(prompt));
    snprintf(prompt, sizeof(prompt), "Enter number for k:\n<min> = 2\n<max> = %d\n", n);
    do
    {
        clear();
        valid = getNumber(prompt, 2, n, &k);
    } while (!valid);

    /* create k storage shares */
    Image* storage = xmalloc(k*sizeof(Image));

    AlgorithmData randgrids = {
        .numberOfShares = k,
        .shares = storage,
        .source = source
    };

    /* fill the temporary shares */
    randomGrid_nn_Threshold(&randgrids);

    /* create vector with values from 1 to n */
    Pixel* setOfN = xmalloc(n * sizeof(Pixel));
    for (int i = 0; i < n; i++)
    {
        setOfN[i] = i+1;
    }

    /* open urandom, to get random numbers from it */
    FILE* urandom = xfopen("/dev/urandom", "r");

    Pixel* randSortedSetOfN = xmalloc(n * sizeof(Pixel));
    Pixel* checkList = xmalloc(n * sizeof(Pixel));

    Copy copy = {
        .source = setOfN,
        .dest = randSortedSetOfN
    };

    /* for each pixel */
    for(int i = 0; i < arraySize; i++)
    {
        memset(checkList, 0, n*sizeof(Pixel));

        /* fill randSortedSetOfN randomly with integers from setOfN */
        for(int idx = 0; idx < n; idx++)
        {
            copy.destIdx = idx;
            int randNum = getRandomNumber(urandom, 1, n-idx);
            randomSort(randNum, checkList, &copy, copyVectorElement);
        }

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
                shares[idx].array[i] = storage[found].array[i];
            else
                shares[idx].array[i] = getRandomNumber(urandom,0,2);
        }
    }
}
