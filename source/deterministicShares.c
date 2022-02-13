#include <math.h>
#include <stdlib.h>
#include "deterministicShares.h" 

int mallocDeterministicShareArrays(Image* source, Image** share, int n, int m)
{
    int err = 0;

    int32_t deterministicHeight, deterministicWidth;

    /* calculate the pixel expansion */
    if (n % 2) /* odd */
    {
        /*  the shares will have the same appearance as
            the source, and are just scaled a bit larger
        */
        deterministicHeight = (int32_t)sqrt(m);
        deterministicWidth = deterministicHeight;
    }
    else /* even */
    {
        /*  the shares will look a little
            "stretched" from left to right
        */
        deterministicHeight = (int32_t)sqrt(m/2);
        deterministicWidth = deterministicHeight * 2;
    }

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

void freeShareArrays(Image** share, int n)
{
     /* for each share */
    for(int i = 0; i < n; i++)
        free(share[i]->array);
}

static BooleanMatrix createDeterministicPixel(BooleanMatrix* permutation, int row)
{

}

static int copyDeterministicPixelToShare(BooleanMatrix* detPixel, Image* share)
{

}

int fillDeterministicShareArrays(Image* source, Image** share, BooleanMatrix* B0, BooleanMatrix* B1)
{
    /* Test if the permutation works */
    BooleanMatrix permutation = permuteBasisMatrix(B0);
    if(!permutation.array)
        return -1;

    printBooleanMatrix(&permutation, "P0");
    deleteBooleanMatrix(&permutation);
    return 0;
}