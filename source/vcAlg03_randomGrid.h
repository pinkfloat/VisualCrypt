#ifndef RANDOM_GRID_ALGORITHMS_H
#define RANDOM_GRID_ALGORITHMS_H

#include "random.h"
#include "vcAlgorithms.h"

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
* Function:     createSetOfN
*--------------------------------------------------------------------
* Description:  Create vector with values from 1 to n.
********************************************************************/
Pixel* createSetOfN(int n);

/********************************************************************
* Function:     randomSortVector
*--------------------------------------------------------------------
* Description:  This function will copy the contents of one vector
*               random sorted to another.
********************************************************************/
void randomSortVector(Copy* copy, Pixel* checkList, FILE* urandom, int n);

/********************************************************************
* Function:     writePixelToShares
*--------------------------------------------------------------------
* Description:  If the share number is part of the first k elements
*               of the random sorted set (of size n), the very share
*               will get the pixel that was calculated for one of
*               the shares, from the source image, before.
*               Shares with a number not contained in the first
*               k elements will get randomly a 0/1.
********************************************************************/
void writePixelToShares(Pixel* randSortedSetOfN, void* source, Image* shares, FILE* urandom, int n, int k, int i, Pixel (*getPixel)(void*, int, int));

/********************************************************************
* Function:     getKfromUser
*--------------------------------------------------------------------
* Description:  Ask the user the number of the k shares in a
*               (k,n) RG-Algorithm.
********************************************************************/
int getKfromUser(int n);

/********************************************************************
* Function:     callRandomGridAlgorithm
*--------------------------------------------------------------------
* Description:  Prepares data which is needed by all or at least
*               multiple random grid algorithms and calls the chosen
*               algorithm with the data.
********************************************************************/
void callRandomGridAlgorithm(AlgorithmData* data);

#endif /* #ifndef RANDOM_GRID_ALGORITHMS_H */
