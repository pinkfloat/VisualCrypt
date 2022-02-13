#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "deterministicShares.h" 

/* calculate the pixel expansion */
static void calculateDeterministicPixelSize (int* deterministicHeight, int* deterministicWidth, int n, int m)
{
    if (n % 2) /* odd */
    {
        /*  the shares will have the same appearance as
            the source, and are just scaled a bit larger
        */
        *deterministicHeight = (int)sqrt(m);
        *deterministicWidth = *deterministicHeight;
    }
    else /* even */
    {
        /*  the shares will look a little
            "stretched" from left to right
        */
        *deterministicHeight = (int)sqrt(m/2);
        *deterministicWidth = *deterministicHeight * 2;
    }
}

int mallocDeterministicShareArrays(Image* source, Image** share, int n, int m)
{
    int err = 0;

    int deterministicHeight, deterministicWidth;
    calculateDeterministicPixelSize(&deterministicHeight, &deterministicWidth, n, m);

    /* for each share */
    for(int i = 0; i < n; i++)
    {
        if(!err)
        {
            share[i]->height = source->height * deterministicHeight;
            share[i]->width = source->width * deterministicWidth;
            err = mallocPixelArray(share[i]);
        }
        else
            share[i]->array = NULL;
    }
    return err;
}

/* re-arrange an 1D array (row of the permutation matrix) into an 2D-array (deterministicPixel) */
static inline void fillDeterministicPixel(BooleanMatrix* deterministicPixel, BooleanMatrix* permutation, int row)
{
    for(int j = 0; j < permutation->m; j++)
        deterministicPixel->array[j] = getPixel(permutation, row, j);
}

/*  copy the pixel array detPixel to the PixelArray of the share,
    starting at row posY and column posX of the share
*/
static void copyDeterministicPixelToShare(BooleanMatrix* detPixel, Image* share, int posY, int posX)
{
    /* for each pixel of detPixel */
    for(int i = 0; i < detPixel->n; i++)     /* rows */
    {
        for(int j = 0; j < detPixel->m; j++) /* columns */
        {
            share->array[(posY+i) * share->width + posX+j] = getPixel(detPixel, i, j);
        }
    }
}

int fillDeterministicShareArrays(Image* source, Image** share, BooleanMatrix* B0, BooleanMatrix* B1)
{
    int n = B0->n;
    int m = B0->m;
    int deterministicHeight, deterministicWidth;
    calculateDeterministicPixelSize(&deterministicHeight, &deterministicWidth, n, m);

    /* initialize random number generator */
    srand(time(NULL));
    int randNum, checkIdx, zeroCount;

    /*  create matrix of equal size as the basis matrices 
        to store the permutations of them.
    */
    BooleanMatrix permutation = createBooleanMatrix(n, m);
    if(!permutation.array)
        return -1;

    /*  every pixel of the source will be encrypted in arrays of pixel,
        printed to the shares. This matrix has the size of an array
        of an encrypted pixel and will temporarily hold all of the
        pixel-arrays printed to the shares.
    */
    BooleanMatrix deterministicPixel = createBooleanMatrix(deterministicHeight, deterministicWidth);
    if (!deterministicPixel.array)
        goto cleanupA;

    Pixel* checkList = malloc(n * sizeof(Pixel));
    if (!checkList)
        goto cleanupB;

    /* for each pixel of the source */
    for(int i = 0; i < source->height; i++)     /* rows */
    {
        for(int j = 0; j < source->width; j++)  /* columns */
        {
            /* if the pixel is black */
            if (source->array[i * source->width + j])
            {
                /* permutate B1 */
                if (permuteBasisMatrix(B1, &permutation) != 0)
                    goto cleanupC;
            }
            /* if the pixel is white */
            else
            {
                /* permutate B0 */
                if (permuteBasisMatrix(B0, &permutation) != 0)
                    goto cleanupC;
            }

            /* clear checkList */
            memset(checkList, 0, n*sizeof(Pixel));

            /* for each share */
            for(int share_idx = 0; share_idx < n; share_idx++)
            {
                /*  choose random which share will get which permutation-array row
                    with a number between 1 and number-of-shares - share_idx
                */
                randNum = rand() % (n-share_idx)+1;

                /*  search for the randNum-th zero
                    in the checkList
                */
                for(checkIdx = 0, zeroCount = 0;; checkIdx++)
                {
                    if(checkList[checkIdx] == 0)
                        zeroCount++;

                    if(zeroCount == randNum)
                    {
                        /*  copy the random chosen row of the permutation matrix
                            into the deterministicPixel
                        */
                        fillDeterministicPixel(&deterministicPixel, &permutation, checkIdx);
                        checkList[checkIdx] = 1; /* mark row in checkList as used */
                        break;
                    }

                }
                copyDeterministicPixelToShare(&deterministicPixel, share[share_idx], i*deterministicHeight, j*deterministicWidth);
            }
        }
    }

    free(checkList);
    deleteBooleanMatrix(&deterministicPixel);
    deleteBooleanMatrix(&permutation);
    return 0;

    cleanupC:
        free(checkList);
    cleanupB:
        deleteBooleanMatrix(&deterministicPixel);
    cleanupA:
        deleteBooleanMatrix(&permutation);
        return -1;
}