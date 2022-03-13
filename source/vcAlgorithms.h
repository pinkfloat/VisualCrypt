#ifndef VCALGORITHMS_H
#define VCALGORITHMS_H

#include "image.h"

typedef struct {
    Image* source;
    Image* shares;
    uint8_t numberOfShares;
} AlgorithmData;

/********************************************************************
* Function:     callAlgorithm
*--------------------------------------------------------------------
* Description:  The function callAlgorithm will extract the data of
*               the source bmp, call the algorithm given to it as
*               parameter and draw all of the share bmps, after
*               the algorithm is finished. It will use the settings
*               stored in "settings.h".
********************************************************************/
void callAlgorithm(void (*algorithm)(AlgorithmData*));

/********************************************************************
* Function:     deterministicAlgorithm
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
void deterministicAlgorithm(AlgorithmData* data);

/********************************************************************
* Function:     probabilisticAlgorithm
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
void probabilisticAlgorithm(AlgorithmData* data);

#endif /* VCALGORITHMS_H */  
