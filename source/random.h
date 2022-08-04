#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h>
#include <stdio.h>

#include "booleanMatrix.h"

#ifndef TYPE_PIXEL
#define TYPE_PIXEL

typedef uint8_t Pixel;  // black / white

#endif  // TYPE_PIXEL

/*********************************************************************
 * Function:     getRandomNumber
 *--------------------------------------------------------------------
 * Description:  Return a random number between [min, max), generated
 *               from a file containing random numbers.
 *               To avoid bias, the "java algorithm" is used.
 ********************************************************************/
uint8_t getRandomNumber(FILE *randomSrc, uint8_t min, uint8_t max);

/*********************************************************************
 * Function:     createSetOfN
 *--------------------------------------------------------------------
 * Description:  Create vector containing values from "start" to "n".
 ********************************************************************/
int *createSetOfN(int n, int start);

/*********************************************************************
 * Function:     shuffleVector
 *--------------------------------------------------------------------
 * Description:  The function shuffleVector() shifts the vector
 *               elements randomly to a different place.
 *               The Fisher-Yates shuffle algorithm is used for this
 *               purpose.
 * Input:        n = number of elements / size of the vector
 *               randomSrc = file with random numbers
 * In/Out:       vector = the vector, which elements will be shifted
 ********************************************************************/
void shuffleVector(int *vector, int n, FILE *randomSrc);

/*********************************************************************
 * Function:     shuffleColumns
 *--------------------------------------------------------------------
 * Description:  Copy matrix from "src" to "dest" in a column-shuffled
 *               way, by getting shuffled numbers from "indices".
 ********************************************************************/
void shuffleColumns(BooleanMatrix *dest, BooleanMatrix *src, FILE *randomSrc, int *indices);

#endif /* RANDOM_H */
