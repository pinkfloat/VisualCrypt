#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h>
#include <stdio.h>
#include "booleanMatrix.h"

#ifndef TYPE_PIXEL
#define TYPE_PIXEL

typedef uint8_t Pixel; // black / white

#endif // TYPE_PIXEL

/********************************************************************
* Function:     getRandomNumber
*--------------------------------------------------------------------
* Description:  Return a random number between min and max, that was
*               calculated from a file containing random numbers.
********************************************************************/
uint64_t getRandomNumber(FILE* randomSrc, uint64_t min, uint64_t max);

/********************************************************************
* Function:     createSetOfN
*--------------------------------------------------------------------
* Description:  Create vector with values from 1 to n
********************************************************************/
int* createSetOfN(int n, int start);

/********************************************************************
* Function:     shuffleVector
*--------------------------------------------------------------------
* Description:  The function randomSortVector() shifts the vector
*               element random to a different place.
*               The Fisher-Yates shuffle is used for this purpose.
* Input:        n = number of elements / size of the vector
*               randomSrc = file with random numbers
* In/Out:       vector = the vector which elements will be shifted
********************************************************************/
void shuffleVector(int* vector, int n, FILE* randomSrc);

/********************************************************************
* Function:     shuffleColumns
*--------------------------------------------------------------------
* Description:  Copy matrix from src to dest in a column-shuffled way
*               by getting shuffled indices from parameter "indices".
********************************************************************/
void shuffleColumns(BooleanMatrix* dest, BooleanMatrix* src, FILE* randomSrc, int* indices);

/********************************************************************
* Function:     shuffleRows
*--------------------------------------------------------------------
* Description:  Copy matrix from src to dest in a row-shuffled way by
*               getting shuffled indices from parameter "indices".
********************************************************************/
// void shuffleRows(BooleanMatrix* dest, BooleanMatrix* src, FILE* randomSrc, int* indices);

#endif /* RANDOM_H */
