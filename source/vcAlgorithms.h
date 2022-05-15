#ifndef VCALGORITHMS_H
#define VCALGORITHMS_H

#include "image.h"
#include "booleanMatrix.h"

typedef struct {
    Image* source;
    Image* shares;
    uint8_t numberOfShares;
    uint8_t algorithmNumber;
    FILE* urandom;
} AlgorithmData;

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

typedef struct {
    BooleanMatrix* B0;
    BooleanMatrix* B1;
    BooleanMatrix* columnVector;
    Pixel* sourceArray;
    Pixel* checkList;
    Image* share;
    FILE* urandom;
    int width;
    int height;
} probabilisticData;

typedef struct {
    Pixel* setOfN;
    Pixel* randSortedSetOfN;
    Pixel* checkList;
    Pixel* sharePixel;
    Pixel* sourceArray;
    Image* shares;
    Image* additShares;
    FILE* urandom;
    int arraySize;
    int n;
    int k;
} kn_randomGridData;

/********************************************************************
* Function:     callAlgorithm
*--------------------------------------------------------------------
* Description:  The function callAlgorithm will extract the data of
*               the source bmp, call the algorithm given to it as
*               parameter and draw all of the share bmps, after
*               the algorithm is finished. It will use the settings
*               stored in "settings.h".
********************************************************************/
void callAlgorithm(void (*algorithm)(AlgorithmData*), uint8_t algorithmNumber);

/********************************************************************
* Function:     mallocSharesOfSourceSize
*--------------------------------------------------------------------
* Description:  This function will allocate the buffer of the share
*               pixel arrays, that are going to be printed to the
*               BMPs later. The size will be the same as for the
*				source file.
* Input:        source = containing width and height of the source
*               image,
*               numberOfShares = amount of shares that will be
*               created,
* Output:       share->array will be correctly allocated for each
*               share.
********************************************************************/
void mallocSharesOfSourceSize(Image* source, Image* share, int numberOfShares);

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

/********************************************************************
* Function:     callRandomGridAlgorithm
*--------------------------------------------------------------------
* Description:  Prepares data which is needed by all or at least
*               multiple random grid algorithms and calls the chosen
*               algorithm with the data.
********************************************************************/
void callRandomGridAlgorithm(AlgorithmData* data);

#endif /* VCALGORITHMS_H */  
