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
static void createRandomGrid(Image* share)
{
    int width = share->width;
    int height = share->height;

    /* open urandom, to get random numbers from it */
    FILE* urandom = xfopen("/dev/urandom", "r");

    /* for each pixel of the array */
    for(int i = 0; i < height; i++)     /* rows */
    {
        for(int j = 0; j < width; j++)  /* columns */
        {
            /* get random 0/1 */
            share->array[i * width + j] = getRandomNumber(urandom,0,2);
        }
    }
    xfclose(urandom);
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
static void randomGrid_22_Threshold(Image* source, Image* share1, Image* share2)
{
    int width = source->width;
    int height = source->height;

    /* make the first share a random grid */
    createRandomGrid(share1);

    /* fill the second share according to source and first share*/
    for(int i = 0; i < height; i++)     /* rows */
    {
        for(int j = 0; j < width; j++)  /* columns */
        {
            /* if the source pixel is black */
            if (source->array[i * width + j])
                /* use the complementary value of share 1 */
                share2->array[i * width + j] = share1->array[i * width + j] ? 0 : 1;

            /* if the source pixel is white */
            else
                /* copy value of share 1 */
                share2->array[i * width + j] = share1->array[i * width + j];
        }
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
void randomGrid_nn_Threshold(AlgorithmData* data)
{
    Image* source = data->source;
    Image* shares = data->shares;
    int n = data->numberOfShares;

    /* allocate pixel-arrays for the shares */
	mallocSharesOfSourceSize(source, shares, n);

    /* fill the first two shares */
    randomGrid_22_Threshold(source, shares, &shares[1]);

    if(n > 2)
    {
        Image storage = {
            .width = source->width,
            .height = source->height
        };
        mallocPixelArray(&storage);
        Pixel* tmp;

        /* for number of shares */
        for(int idx = 2; idx < n; idx++)
        {
            tmp = storage.array;
            storage.array = shares[idx-1].array;
            shares[idx-1].array = tmp;
            randomGrid_22_Threshold(&storage, &shares[idx-1], &shares[idx]);
        }
        xfree(storage.array);
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
void randomGrid_2n_Threshold(AlgorithmData* data)
{
    int n = data->numberOfShares;
    Image* source = data->source;
    Image* shares = data->shares;
    int width = source->width;
    int height = source->height;

    /* allocate pixel-arrays for the shares */
	mallocSharesOfSourceSize(source, shares, n);

    /* make the first share a random grid */
    createRandomGrid(shares);

    /* open urandom, to get random numbers from it */
    FILE* urandom = xfopen("/dev/urandom", "r");

    /* for number of shares */
    for(int idx = 1; idx < n; idx++)
    {
        /* for each pixel ... */
        /* fill the other shares according to source and first share */
        for(int i = 0; i < height; i++)     /* rows */
        {
            for(int j = 0; j < width; j++)  /* columns */
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
    int width = source->width;
    int height = source->height;
    int n = data->numberOfShares;

    /* allocate pixel-arrays for the shares */
	mallocSharesOfSourceSize(source, shares, n);

    if(n == 2)
    {
        /* just call randomGrid_22_Threshold */
        randomGrid_22_Threshold(source, shares, &shares[1]);
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
    BooleanMatrix setOfN = createBooleanMatrix(n, 1);
    for (int i = 0; i < n; i++)
    {
        setPixel(&setOfN, i, 0, i+1);
    }

    /* open urandom, to get random numbers from it */
    FILE* urandom = xfopen("/dev/urandom", "r");

    /*  create vector with n elements */
    BooleanMatrix columnVector = createBooleanMatrix(n, 1);

    /*  create checklist of size n to store which values from 1 to n
        has already been used in columnVector
        0 = unused element, 1 = used
    */
    Pixel* checkList = xmalloc(n * sizeof(Pixel));

    MatrixCopy copy;
    copy.source = &setOfN;

    /* for each pixel of the source */
    for(int i = 0; i < height; i++)     /* rows */
    {
        for(int j = 0; j < width; j++)  /* columns */
        {
            memset(checkList, 0, n*sizeof(Pixel));

            /*  fill columnVector randomly with integers from setOfN */
            for(int idx = 0; idx < n; idx++)
            {
                copy.dest = (BooleanMatrix*) &columnVector.array[idx];
                int randNum = getRandomNumber(urandom, 1, n-idx);
                randomSort(randNum, checkList, &copy, copyColumnElement);
            }

            /* for each share */
            for(int idx = 0; idx < n; idx++)
            {
                int found = -1;
                /* if idx+1 is part of the first k elements of columnVector */
                for (int idk = 0; idk < k; idk++)
                {
                   if (columnVector.array[idk] == idx+1)
                   {
                       found = idk;
                       break;
                   } 
                }
                if (found != -1)
                    shares[idx].array[i * width + j] = storage[found].array[i * width + j];
                else
                    shares[idx].array[i * width + j] = getRandomNumber(urandom,0,2);
            }
        }
    }
}
