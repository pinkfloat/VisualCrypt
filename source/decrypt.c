#include "settings.h"
#include "decrypt.h"
#include "handleBMP.h"
#include "fileManagement.h"
#include "memoryManagement.h"

/********************************************************************
* Function:     orTwoPixelArrays
*--------------------------------------------------------------------
* Description:  The pixel array of dest will be the result of
*               itself OR-ed with the source pixel array.
********************************************************************/
static void orTwoPixelArrays(Image* dest, Image* source)
{
    int height = dest->height;
    int width = dest->width;

    /* for each pixel */
    for(int i = 0; i < height; i++)     /* rows */
    {
        for(int j = 0; j < width; j++)  /* columns */
        {
            dest->array[i * width + j] |= source->array[i * width + j];
        }
    }
}

/********************************************************************
* Function:     fillDecryptedImage
*--------------------------------------------------------------------
* Description:  The decrypted image is supposed to be the result of
*               overlapping all share pixel arrays. This function
*               will fill its data with the data from the shares
*               and makes use of the OR-function for each pixel
*               of the shares to put them all together.
********************************************************************/
static void fillDecryptedImage(Image* decrypted, Image* share, int numberOfShares)
{
    decrypted->height = share->height;
    decrypted->width = share->width;

    /* use the already allocated array of the first share */
    decrypted->array = share->array;

    /* for each share */
    for(uint8_t i = 1; i < numberOfShares; i++)
    {
        orTwoPixelArrays(decrypted, share+i);
    }
}

/********************************************************************
* Function:     decryptShareFiles
*--------------------------------------------------------------------
* Description:  Read share files in dirPath starting at the share
*               with the number given to first and end with the
*               number given to last. They will be stored in Image
*               structures that must have been allocated before.
********************************************************************/
void decryptShareFiles()
{
    int first = 1;
    int last = NUMBER_OF_SHARES;

/*
    if(first < 1)
        first = 1;

    if(last > 99)
        last = 99;

    if(first > last)
    {
        int tmp = first;
        first = last;
        last = tmp;
    }
*/

    int numberOfShares = (1+last-first);

    Image result, *shares = xmalloc(numberOfShares*sizeof(Image));
    readShareFiles(shares, first, last);
    createDecryptedImageFile(&result);
    fillDecryptedImage(&result, shares, numberOfShares);
    createBMP(&result);

    /* cleanup */
	xcloseAll();
	xfreeAll();
    fprintf(stdout, "Success!\n");
}
