#ifndef DETERMINISTIC_ALGORITHMS_H
#define DETERMINISTIC_ALGORITHMS_H

#include "booleanMatrix.h"
#include "vcAlgorithms.h"

typedef struct {
    BooleanMatrix B0;
    BooleanMatrix B1;
    BooleanMatrix permutation;
    Pixel *sourceArray;
    int *columnIndices;
    int *rowIndices;
    Image *share;
    FILE *randomSrc;
    int width;
    int height;
    int deterministicWidth;
    int deterministicHeight;
} deterministicData;

/********************************************************************
 * Function:     calcPixelExpansion
 *--------------------------------------------------------------------
 * Description:  The shares, that will be created from the source
 *               image, will have a greater amount of pixel as the
 *               original source file. This function calculates the
 *               pixel expansion i.e. the amount of pixel that encrypt
 *               a source pixel in width and height with:
 * Input:        n = number of shares,
 *               m = number of pixels in a share per pixel in source,
 * Output:       deterministicHeight = height of the pixel array
 *               encrypting a pixel.
 *               deterministicWidth = width of the pixel array
 *               encrypting a pixel.
 ********************************************************************/
void calcPixelExpansion(int *deterministicHeight, int *deterministicWidth, int n, int m);

/********************************************************************
 * Function:     mallocPixelExpandedShares
 *--------------------------------------------------------------------
 * Description:  This function will allocate the buffer of the share
 *               pixel arrays, that are going to be printed to the
 *               BMPs later. Since the deterministic algorithm has
 *               pixel expansion, they will be larger than the
 *               source file.
 * Input:        source = containing width and height of the source
 *               image,
 *               n = number of shares,
 *               m = number of pixels in a share per pixel in source,
 * Output:       share->array will be correctly allocated for each
 *               share.
 ********************************************************************/
void mallocPixelExpandedShares(Image *source, Image *share, int n, int m);

/********************************************************************
 * Function:     prepareDeterministicAlgorithm
 *--------------------------------------------------------------------
 * Description:  This function will allocate all data that needs
 *               allocation for the deterministic algorithm and
 *               prepares the basis matrices which doesn't change for
 *               the same amount of share files.
 ********************************************************************/
deterministicData *prepareDeterministicAlgorithm(AlgorithmData *data);

/********************************************************************
 * Function:     __deterministicAlgorithm
 *--------------------------------------------------------------------
 * Description:  This is an implementation of the so called
 *               "deterministic Algorithm" from Moni Naor and Adi
 *               Shamir. It will calculate the pixel of the share
 *               images from the pixel in the source file, by creating
 *               basis matrices out of the number of the share files.
 *               The basis matrices will be afterwards permutated in
 *               columns and each share will get a different row of
 *               every permutation per source pixel.
 ********************************************************************/
void __deterministicAlgorithm(deterministicData *data);

/********************************************************************
 * Function:     deterministicAlgorithm
 *--------------------------------------------------------------------
 * Description:  This is a wrapper for the "deterministic Algorithm"
 *               from Moni Naor and Adi Shamir. It will prepare the
 *               resources needed by the algorithm and call it
 *               afterwards.
 ********************************************************************/
void deterministicAlgorithm(AlgorithmData *data);

#endif /* DETERMINISTIC_ALGORITHMS_H */
