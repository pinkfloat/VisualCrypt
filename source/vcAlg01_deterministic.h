#ifndef DETERMINISTIC_ALGORITHMS_H
#define DETERMINISTIC_ALGORITHMS_H

#include "booleanMatrix.h"
#include "vcAlgorithms.h"

typedef struct {
    BooleanMatrix* B0;
    BooleanMatrix* B1;
    BooleanMatrix* permutation;
    BooleanMatrix* encryptedPixel;
    Pixel* sourceArray;
    Pixel* columnCheckList;
    Pixel* rowCheckList;
    Image* share;
    FILE* urandom;
    int width;
    int height;
    int deterministicWidth;
    int deterministicHeight;
} deterministicData;

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
void __deterministicAlgorithm(deterministicData* data);

/********************************************************************
* Function:     deterministicAlgorithm
*--------------------------------------------------------------------
* Description:  This is a wrapper for the "deterministic Algorithm"
*               from Moni Naor and Adi Shamir. It will allocate all 
*               data that needs allocation and prepares the basis
*               matrices which doesn't change for the same amount of
*               share files.
********************************************************************/
void deterministicAlgorithm(AlgorithmData* data);

#endif /* #ifndef DETERMINISTIC_ALGORITHMS_H */