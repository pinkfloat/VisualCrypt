#include "menu.h"
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
    for(int i = 0; i < height*width; i++)
        dest->array[i] |= source->array[i];
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
        orTwoPixelArrays(decrypted, share+i);
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
    int valid = 0, first, last;

	/* get number of first share from user*/
	do
	{
		clear();
        fprintf(stdout, "Shares can be decrypted from share01.bmp to share08.bmp\n"
                        "The result will be stored in the same directory and named\n"
                        "decrypted01.bmp to a maximum of decrypted99.bmp\n"
                        "(decrypted01.bmp will be overwritten if max is reached)\n\n");

		valid = getNumber("Enter number of the FIRST share to decrypt: ", 1, 8, &first);
	} while (!valid);

    /* get number of last share from user*/
	do
	{
		clear();
		valid = getNumber("Enter number of the LAST share to decrypt: ", 1, 8, &last);
	} while (!valid);

    if(first > last)
    {
        int tmp = first;
        first = last;
        last = tmp;
    }

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
