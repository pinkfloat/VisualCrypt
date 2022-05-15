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
*               image becomes clearer.
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
    Image* extraShares = data->extraShares;
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
                shares[idx].array[i] = extraShares[found].array[i];
            else
                shares[idx].array[i] = getRandomNumber(urandom,0,2);
        }
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

    /* create vector with values from 1 to n */
    Pixel* setOfN = xmalloc(n * sizeof(Pixel));
    for (int i = 0; i < n; i++)
        setOfN[i] = i+1;

    Pixel* randSortedSetOfN = xmalloc(n * sizeof(Pixel));
    Pixel* checkList = xmalloc(n * sizeof(Pixel));

    /* create additional k shares */
    Image* extraShares = xmalloc(k*sizeof(Image));
    for(int i = 0; i < k; i++)
    {
        extraShares[i].width = source->width;
        extraShares[i].height = source->height;
        mallocPixelArray(&extraShares[i]);
    }

    kn_randomGridData rgData = {
        .setOfN = setOfN,
        .randSortedSetOfN = randSortedSetOfN,
        .checkList = checkList,
        .calculatedValues = NULL,
        .sourceArray = NULL,
        .shares = shares,
        .extraShares = extraShares,
        .urandom = urandom,
        .arraySize = arraySize,
        .n = n,
        .k = k
    };

    /* fill the temporary shares */
    randomGrid_nn_Threshold(sourceArray, extraShares, storage, urandom, arraySize, k);

    __randomGrid_kn_Threshold(&rgData);
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

    switch(algorithmNumber)
	{
		case 1: randomGrid_nn_Threshold(sourceArray, shares, &storage, urandom, arraySize, n);  break;
		case 2: randomGrid_2n_Threshold(sourceArray, shares, urandom, arraySize, n);            break;
		case 3:	randomGrid_kn_Threshold(source, shares, &storage, urandom, arraySize, n);       break;
		default: 	break;
	}
}
