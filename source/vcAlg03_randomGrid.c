#include <string.h>
#include "image.h"
#include "menu.h"
#include "fileManagement.h"
#include "memoryManagement.h"
#include "vcAlg03_randomGrid.h"
#include "vcAlg03_randomGrid_V0.h"
#include "vcAlg03_randomGrid_V1.h"

/********************************************************************
* Function:     randomSortVector
*--------------------------------------------------------------------
* Description:  This function will copy the contents of one vector
*               random sorted to another.
********************************************************************/
void randomSortVector(Copy* copy, Pixel* checkList, FILE* urandom, int n)
{            
    for(int idx = 0; idx < n; idx++)
    {
        copy->destIdx = idx;
        int randNum = getRandomNumber(urandom, 1, n-idx);
        randomSort(randNum, checkList, copy, copyVectorElement);
    }
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
void writePixelToShares( Pixel* randSortedSetOfN,
                                void* source,
                                Image* shares,
                                FILE* urandom,
                                int n,
                                int k,
                                int i,
                                Pixel (*getPixel)(void*, int, int))
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
* Function:     getKfromUser
*--------------------------------------------------------------------
* Description:  Ask the user the number of the k shares in a
*               (k,n) RG-Algorithm.
********************************************************************/
int getKfromUser(int n)
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
