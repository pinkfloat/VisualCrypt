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
    /* open urandom, to get random numbers from it */
    FILE* urandom = xfopen("/dev/urandom", "r");

    /* for each pixel of the array */
    for(int i = 0; i < share->height; i++)     /* rows */
    {
        for(int j = 0; j < share->width; j++)  /* columns */
        {
            /* get random 0/1 */
            share->array[i * share->width + j] = getRandomNumber(urandom,0,2);
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
    /* make the first share a random grid */
    createRandomGrid(share1);

    /* fill the second share according to source and first share*/
    for(int i = 0; i < share2->height; i++)     /* rows */
    {
        for(int j = 0; j < share2->width; j++)  /* columns */
        {
            /* if the source pixel is black */
            if (source->array[i * source->width + j])
                /* use the complementary value of share 1 */
                share2->array[i * share2->width + j] = share1->array[i * share1->width + j] ? 0 : 1;

            /* if the source pixel is white */
            else
                /* copy value of share 1 */
                share2->array[i * share2->width + j] = share1->array[i * share1->width + j];
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
    /* TODO: Finish for different amounts of shares */

    /* allocate pixel-arrays for the shares */
	mallocSharesOfSourceSize(data->source, data->shares, data->numberOfShares);

    /* fill two shares */
    randomGrid_22_Threshold(data->source, &data->shares[0], &data->shares[1]);
}
