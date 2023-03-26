/*
*   Copyright: (c) 2023 Sabrina Otto. All rights reserved.
*   This work is licensed under the terms of the MIT license.
*/

#include "vcAlg03_randomGrid_V0.h"

#include <string.h>

#include "fileManagement.h"
#include "image.h"
#include "memoryManagement.h"
#include "menu.h"

/*********************************************************************
 * Function:     createRandomGrid
 *--------------------------------------------------------------------
 * Description:  This will turn a share array into a random grid,
 *               which means, that it will be filled completely random
 *               with black and white pixel.
 *               The function is used by the non-alternate RG
 *               algorithms.
 ********************************************************************/
static void createRandomGrid(Image *share, FILE *randomSrc) {
    int arraySize = share->width * share->height;
    Pixel *shareArray = share->array;

    // for each pixel
    for (int i = 0; i < arraySize; i++) {
        // get random 0/1
        shareArray[i] = getRandomNumber(randomSrc, 0, 2);
    }
}

/*********************************************************************
 * Function:     randomGrid_22
 *--------------------------------------------------------------------
 * Description:  This is an implementation of the (2,2) random grid
 *               algorithm introduced by O. Kafri and E. Karen. It
 *               will turn share1 into a random grid and calculates
 *               share2 by using share1 and the source.
 ********************************************************************/
static void randomGrid_22(Pixel *source, Image *shares, FILE *randomSrc, int arraySize) {
    Pixel *share1 = shares->array;
    Pixel *share2 = shares[1].array;

    createRandomGrid(shares, randomSrc);

    for (int i = 0; i < arraySize; i++) {
        if (source[i])  // source pixel is black
            share2[i] = share1[i] ? 0 : 1;

        else  // source pixel is white
            share2[i] = share1[i];
    }
}

void randomGrid_nn(Pixel *sourceArray, Image *shares, Pixel **storage, FILE *randomSrc, int arraySize,
                   int numberOfShares) {
    Pixel *tmp;

    // fill the first two shares
    randomGrid_22(sourceArray, shares, randomSrc, arraySize);

    if (numberOfShares > 2) {
        for (int idx = 2; idx < numberOfShares; idx++) {
            tmp = *storage;
            *storage = shares[idx - 1].array;
            shares[idx - 1].array = tmp;
            randomGrid_22(*storage, &shares[idx - 1], randomSrc, arraySize);
        }
    }
}

void randomGrid_2n(Pixel *sourceArray, Image *shares, FILE *randomSrc, int arraySize, int numberOfShares) {
    createRandomGrid(shares, randomSrc);

    // for each share
    for (int idx = 1; idx < numberOfShares; idx++) {
        // for each pixel
        for (int i = 0; i < arraySize; i++) {
            if (sourceArray[i])  // source pixel is black
                shares[idx].array[i] = getRandomNumber(randomSrc, 0, 2);

            else  // source pixel is white
                shares[idx].array[i] = shares->array[i];
        }
    }
}

/*********************************************************************
 * Function:     getPixelFromShare
 *--------------------------------------------------------------------
 * Description:  Get the value of a pixel from one of the additional
 *               shares in the non-alternate (k,n) RG version.
 ********************************************************************/
static inline Pixel getPixelFromShare(void *shares, int shareIdx, int matrixIdx) {
    Image *_shares = (Image *)shares;
    return _shares[shareIdx].array[matrixIdx];
}

void __randomGrid_kn(int *setOfN, Image *shares, Image *tmpShares, FILE *randomSrc, int arraySize, int n, int k) {
    // for each pixel
    for (int i = 0; i < arraySize; i++) {
        shuffleVector(setOfN, n, randomSrc);
        writePixelToShares(setOfN, tmpShares, shares, randomSrc, n, k, i, getPixelFromShare);
    }
}

/*********************************************************************
 * Function:     createTemporaryShares
 *--------------------------------------------------------------------
 * Description:  Allocates additional shares and returns them.
 ********************************************************************/
static inline Image *createTemporaryShares(Image *source, Pixel **storage, FILE *randomSrc, int arraySize,
                                           int numberOfShares) {
    Image *tmpShares = xmalloc(numberOfShares * sizeof(Image));
    mallocSharesOfSourceSize(source, tmpShares, numberOfShares);
    randomGrid_nn(source->array, tmpShares, storage, randomSrc, arraySize, numberOfShares);

    return tmpShares;
}

void randomGrid_kn(Image *source, Image *shares, Pixel **storage, FILE *randomSrc, int arraySize, int n) {
    Pixel *sourceArray = source->array;

    if (n == 2) {
        randomGrid_22(sourceArray, shares, randomSrc, arraySize);
        return;
    }

    int k = getKfromUser(n);

    int *setOfN = createSetOfN(n, 1);

    Image *tmpShares = createTemporaryShares(source, storage, randomSrc, arraySize, k);
    __randomGrid_kn(setOfN, shares, tmpShares, randomSrc, arraySize, n, k);
}
