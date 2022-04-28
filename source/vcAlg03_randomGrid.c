#include "image.h"
#include "random.h"
#include "fileManagement.h"
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
