#include "vcAlg03_randomGrid_V1.h"

#include <string.h>

#include "fileManagement.h"
#include "image.h"
#include "memoryManagement.h"
#include "menu.h"

/*********************************************************************
 * Function:     fillPixelRG
 *--------------------------------------------------------------------
 * Description:  This function fills a sharePixel according to the
 *               source and the pixel of the share from before.
 *               It is used by the alternate RG algorithms.
 ********************************************************************/
static void fillPixelRG(Pixel sourcePixel, Pixel *sharePixel, int numberOfShares, FILE *randomSrc) {
    Pixel tmp = sourcePixel;
    for (int idx = 1; idx < numberOfShares; idx++) {
        sharePixel[idx - 1] = getRandomNumber(randomSrc, 0, 2);
        if (tmp)  // source pixel is black
            sharePixel[idx] = sharePixel[idx - 1] ? 0 : 1;

        else  // source pixel is white
            sharePixel[idx] = sharePixel[idx - 1];
        tmp = sharePixel[idx];
    }
}

void alternate_nn_RGA(Pixel *sourceArray, Image *shares, Pixel *tmpSharePixel, FILE *randomSrc, int arraySize,
                      int numberOfShares) {
    // for each pixel
    for (int i = 0; i < arraySize; i++) {
        fillPixelRG(sourceArray[i], tmpSharePixel, numberOfShares, randomSrc);
        // for each share
        for (int idx = 0; idx < numberOfShares; idx++) {
            shares[idx].array[i] = tmpSharePixel[idx];
        }
    }
}

void alternate_2n_RGA(Pixel *sourceArray, Image *shares, FILE *randomSrc, int arraySize, int numberOfShares) {
    Pixel *randomGrid = shares->array;

    // for each pixel
    for (int i = 0; i < arraySize; i++) {
        randomGrid[i] = getRandomNumber(randomSrc, 0, 2);

        // for share 2 to n
        for (int idx = 1; idx < numberOfShares; idx++) {
            if (sourceArray[i])  // source pixel is black
                shares[idx].array[i] = getRandomNumber(randomSrc, 0, 2);

            else  // source pixel is white
                shares[idx].array[i] = shares->array[i];
        }
    }
}

/*********************************************************************
 * Function:     getSharePixel
 *--------------------------------------------------------------------
 * Description:  This function is used to get the actual pixel from
 *               one of the shares in the alternate (k,n) RG version.
 ********************************************************************/
static inline Pixel getSharePixel(void *sharePixel, int shareIdx, __attribute__((unused)) int matrixIdx) {
    Pixel *_sharePixel = (Pixel *)sharePixel;
    return _sharePixel[shareIdx];
}

void __alternate_kn_RGA(int *setOfN, Pixel *sourceArray, Pixel *sharePixel, Image *shares, FILE *randomSrc,
                        int arraySize, int n, int k) {
    // for each pixel
    for (int i = 0; i < arraySize; i++) {
        fillPixelRG(sourceArray[i], sharePixel, k, randomSrc);
        shuffleVector(setOfN, n, randomSrc);
        writePixelToShares(setOfN, sharePixel, shares, randomSrc, n, k, i, getSharePixel);
    }
}

void alternate_kn_RGA(Image *source, Image *shares, FILE *randomSrc, int arraySize, int n) {
    int k = 2;

    if (n > 2) {
        k = getKfromUser(n);
    }

    int *setOfN = createSetOfN(n, 1);
    Pixel *sharePixel = xmalloc(k * sizeof(Pixel));

    __alternate_kn_RGA(setOfN, source->array, sharePixel, shares, randomSrc, arraySize, n, k);
}
