#ifndef PROBABILISTIC_ALGORITHMS_H
#define PROBABILISTIC_ALGORITHMS_H

#include "booleanMatrix.h"
#include "vcAlgorithms.h"

typedef struct {
    BooleanMatrix* B0;
    BooleanMatrix* B1;
    BooleanMatrix* columnVector;
    Pixel* sourceArray;
    int* rowIndices;
    Image* share;
    FILE* randomSrc;
    int width;
    int height;
} probabilisticData;

/********************************************************************
* Function:     __probabilisticAlgorithm
*--------------------------------------------------------------------
* Description:  This is an implementation of the so called
*               "probabilistic Algorithm" from Ryo Ito, Hidenori
*               Kuwakado and Hatsukazu Tanaka. It will calculate the
*               pixel of the share images from the pixel in the source
*               file, by creating basis matrices out of the number of
*               the share files, in the same way, the deterministic
*               algorithm does. For each pixel of the source file,
*               one column of the basis matrix will be randomly chosen
*               and each share will get a different element of the
*               column as pixel value.
********************************************************************/
void __probabilisticAlgorithm(probabilisticData* data);

/********************************************************************
* Function:     probabilisticAlgorithm
*--------------------------------------------------------------------
* Description:  This is a wrapper for the "probabilistic Algorithm"
*               from Ryo Ito, Hidenori Kuwakado and Hatsukazu Tanaka.
*               It will allocate all data that needs allocation and
*               prepares the basis matrices which doesn't change for
*               the same amount of share files.
********************************************************************/
void probabilisticAlgorithm(AlgorithmData* data);

#endif /* PROBABILISTIC_ALGORITHMS_H */
