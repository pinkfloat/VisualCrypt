/*
*   Copyright: (c) 2023 Sabrina Otto. All rights reserved.
*   This work is licensed under the terms of the MIT license.
*/

#include "decrypt.h"

#include "fileManagement.h"
#include "handleBMP.h"
#include "memoryManagement.h"
#include "menu.h"

/*********************************************************************
 * Function:     orTwoPixelArrays
 *--------------------------------------------------------------------
 * Description:  The pixel array of "dest" will be the result of
 *               itself OR-ed with the pixel array from "source".
 ********************************************************************/
static void orTwoPixelArrays(Image *dest, Image *source) {
    int height = dest->height;
    int width = dest->width;

    // for each pixel
    for (int i = 0; i < height * width; i++) {
        dest->array[i] |= source->array[i];
    }
}

/*********************************************************************
 * Function:     fillDecryptedImage
 *--------------------------------------------------------------------
 * Description:  The decrypted image is supposed to be the result of
 *               overlapping multiple share pixel arrays. This makes
 *               use of the OR-function for each pixel of the shares
 *               and will put them all together in "decrypted".
 ********************************************************************/
static void fillDecryptedImage(Image *decrypted, Image *share, int numberOfShares) {
    decrypted->height = share->height;
    decrypted->width = share->width;
    decrypted->array = share->array;

    // for each share
    for (int i = 1; i < numberOfShares; i++) {
        orTwoPixelArrays(decrypted, share + i);
    }
}

void decryptShareFiles() {
    int valid = 0, first, last;

    // get number of the first share from user
    do {
        clear();
        fprintf(stdout,
                "Shares can be decrypted from share01.bmp to share08.bmp\n"
                "The result will be stored in the same directory and named\n"
                "decrypted01.bmp to a maximum of decrypted99.bmp\n"
                "(decrypted01.bmp will be overwritten if max is reached)\n\n");

        valid = getNumber("Enter number of the FIRST share to decrypt: ", 1, 8, &first);
    } while (!valid);

    // get number of the last share from user
    do {
        clear();
        valid = getNumber("Enter number of the LAST share to decrypt: ", 1, 8, &last);
    } while (!valid);

    if (first > last) {
        int tmp = first;
        first = last;
        last = tmp;
    }

    int numberOfShares = (1 + last - first);

    Image result, *shares = xmalloc(numberOfShares * sizeof(Image));
    readShareFiles(shares, first, last);
    createDecryptedImageFile(&result);
    fillDecryptedImage(&result, shares, numberOfShares);
    createBMP(&result);

    xcloseAll();
    xfreeAll();
    fprintf(stdout, "Success!\n");
}
