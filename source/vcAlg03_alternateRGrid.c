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

void alternate_nn_ThresholdRGA(AlgorithmData* data)
{
    int n = data->numberOfShares;
    Image* source = data->source;
    Image* shares = data->shares;
    int width = source->width;
    int height = source->height;

    /* allocate pixel-arrays for the shares */
	mallocSharesOfSourceSize(source, shares, n);

    Pixel* sharePixel = xmalloc(n*sizeof(Pixel));

    /* open urandom, to get random numbers from it */
    FILE* urandom = xfopen("/dev/urandom", "r");

    for(int i = 0; i < height; i++)     /* rows */
    {
        for(int j = 0; j < width; j++)  /* columns */
        {
            fillPixelRG(source->array[i * width + j], sharePixel, n, urandom);
            for (int idx = 0; idx < n; idx++)
            {
                shares[idx].array[i * width + j] = sharePixel[idx];
            }
        }
    }
    xfree(sharePixel);
    xfclose(urandom);
}

void alternate_2n_ThresholdRGA(AlgorithmData* data)
{
    int n = data->numberOfShares;
    Image* source = data->source;
    Image* shares = data->shares;
    int width = source->width;
    int height = source->height;

    /* allocate pixel-arrays for the shares */
	mallocSharesOfSourceSize(source, shares, n);

    /* open urandom, to get random numbers from it */
    FILE* urandom = xfopen("/dev/urandom", "r");

    /* for each pixel ... */
    /* fill the other shares according to source and first share */
    for(int i = 0; i < height; i++)     /* rows */
    {
        for(int j = 0; j < width; j++)  /* columns */
        {
            /* fill share 1 random */
            shares->array[i * width + j] = getRandomNumber(urandom,0,2);

            /* for number of shares */
            for(int idx = 1; idx < n; idx++)
            {
                /* if the source pixel is black */
                if (source->array[i * width + j])
                    /* get random 0/1 */
                    shares[idx].array[i * width + j] = getRandomNumber(urandom,0,2);

                /* if the source pixel is white */
                else
                    /* copy value of share 1 */
                    shares[idx].array[i * width + j] = shares->array[i * width + j];
            }
        }
    }
    xfclose(urandom);
}

void alternate_kn_ThresholdRGA(AlgorithmData* data)
{
    Image* source = data->source;
    Image* shares = data->shares;
    int width = source->width;
    int height = source->height;
    int n = data->numberOfShares;
    int k = 2;

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

    /* create vector with values from 1 to n */
    Pixel* setOfN = xmalloc(n * sizeof(Pixel));
    for (int i = 0; i < n; i++)
    {
        setOfN[i] = i+1;
    }

    /* open urandom, to get random numbers from it */
    FILE* urandom = xfopen("/dev/urandom", "r");

    /* create vector with n elements */
    Pixel* randSortedSetOfN = xmalloc(n * sizeof(Pixel));
    Pixel* calculatedValues = xmalloc(k * sizeof(Pixel));

    /*  create checklist of size n to store which values from 1 to n
        has already been used in randSortedSetOfN
        0 = unused element, 1 = used
    */
    Pixel* checkList = xmalloc(n * sizeof(Pixel));

    Copy copy = {
        .source = setOfN,
        .dest = randSortedSetOfN
    };

    /* for each pixel of the source */
    for(int i = 0; i < height; i++)     /* rows */
    {
        for(int j = 0; j < width; j++)  /* columns */
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
            fillPixelRG(source->array[i * width + j], calculatedValues, k, urandom);

            //###debug print###
            if (i<3 && j < 5) {
                printf("{");
                for(int idx = 0; idx < n; idx++)
                {
                    printf(" %d,", randSortedSetOfN[idx]);
                }
                printf("}\n{");
                for(int idk = 0; idk < k; idk++)
                {
                    printf(" %d,", randSortedSetOfN[idk]);
                }
                printf("}\n");
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
                {
                    shares[idx].array[i * width + j] = calculatedValues[found];

                    //###debug print###
                    if (i<3 && j < 5) {
                        printf("share %d got r %d\n", idx+1, found+1);
                    }
                }
                else
                {
                    shares[idx].array[i * width + j] = getRandomNumber(urandom,0,2);

                    //###debug print###
                    if (i<3 && j < 5) {
                        printf("share %d got random number\n", idx+1);
                    }
                }

                //###debug print###
                if (i<3 && j < 5 && idx == n-1) {
                    printf("\n");
                }
            }
        }
    }
}
