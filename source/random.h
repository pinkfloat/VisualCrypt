#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h>
#include <stdio.h>

#ifndef TYPE_PIXEL
#define TYPE_PIXEL

typedef uint8_t Pixel; /* black / white */

#endif /* TYPE_PIXEL */

typedef struct {
    void* source;
    void* dest;
    int sourceIdx;
    int destIdx;
} Copy;

/********************************************************************
* Function:     getRandomNumber
*--------------------------------------------------------------------
* Description:  Return a random number between min and max, that was
*               calculated from a value of /dev/urandom, containing
*               cryptographically secure random numbers.
********************************************************************/
uint8_t getRandomNumber(FILE* urandom, uint8_t min, uint8_t max);

/********************************************************************
* Function:     copyColumnElement
*--------------------------------------------------------------------
* Description:  This is a sort-Function for randomSort().
*               The function will copy a chosen column element of
*               the source vector (n x 1 matrix = matrix column)
*               to the destination which is in this case just a pixel.
********************************************************************/
void copyColumnElement(Copy* copy);

/********************************************************************
* Function:     copyVectorElement
*--------------------------------------------------------------------
* Description:  This is a sort-Function for randomSort().
*               The function will copy an element of the source
*               vector to the destination vector.
********************************************************************/
void copyVectorElement(Copy* copy);

/********************************************************************
* Function:     copyMatrixColumn
*--------------------------------------------------------------------
* Description:  This is a sort-Function for randomSort().
*               The function will copy a chosen column of the source
*               matrix (copy->sourceIdx) into a chosen column of the
*               destination matrix (copy->destIdx).
********************************************************************/
void copyMatrixColumn(Copy* copy);

/********************************************************************
* Function:     fillEncryptedPixel
*--------------------------------------------------------------------
* Description:  This is a sort-Function for randomSort().
*               The function copies the values of a row of a
*               "permutation matrix" (1D-array) into a "deterministic
*               pixel" (2D-array). It is part of the encryption of a
*               specific pixel.
********************************************************************/
void fillEncryptedPixel(Copy* copy);

/********************************************************************
* Function:     randomSort
*--------------------------------------------------------------------
* Description:  The function randomSort() copies a row/column of a
*               source matrix to a destination matrix, without using
*               a row/column of the source twice. For doing this, a
*               temporary vector "checkList" is used, that has the
*               same size as the numbers of rows/columns, to store,
*               which has already been taken. The "randNum"'th
*               unused row/column of the source will be chosen and
*               marked as "used" in the checkList.
*               The code written in here is only to sort out which
*               row/column of the source is to use. The copy process,
*               from one matrix to another, is part of the sorting
*               function, given as last parameter to randomSort.
********************************************************************/
void randomSort(int randNum, Pixel* checkList, Copy* copy, void (*sortFunc)(Copy*));

#endif /* RANDOM_H */
