#ifndef BOOLEANMATRIX_H
#define BOOLEANMATRIX_H

#include <stdint.h>
#include "setsNsubsets.h"

#ifndef TYPE_PIXEL
#define TYPE_PIXEL

typedef uint8_t Pixel; /* black / white */

#endif /* TYPE_PIXEL */

typedef struct {
    int height;
    int width;
    Pixel* array;
} BooleanMatrix;

/********************************************************************
* Function:     createBooleanMatrix
*--------------------------------------------------------------------
* Description:  Saves the size of a BooleanMatrix and allocates a 2D
*               pixel array of the size, that is stored in the
*               BooleanMatrix struct as array.
* Return:       The created BooleanMatrix.
********************************************************************/
BooleanMatrix createBooleanMatrix(int height, int width);

/********************************************************************
* Function:     deleteBooleanMatrix
*--------------------------------------------------------------------
* Description:  Free the allocated pixel array of a Booleanmatrix.
********************************************************************/
void deleteBooleanMatrix(BooleanMatrix* matrix);

/********************************************************************
* Function:     getPixel
*--------------------------------------------------------------------
* Description:  Return the value of the Pixel of a Booleanmatrix at
*               row "i" and column "j". 
* Return:       "1" is considered as black pixel and "0" as white.
********************************************************************/
static inline Pixel getPixel(BooleanMatrix matrix, int i, int j)
{
    return matrix.array[i * matrix.width + j];
}

/********************************************************************
* Function:     setPixel
*--------------------------------------------------------------------
* Description:  Set the Pixel of a BooleanMatrix on row "i" and
*               columns "j" to the value "value".
********************************************************************/
static inline void setPixel(BooleanMatrix matrix, int i, int j, Pixel value)
{
    matrix.array[i * matrix.width + j] = value;
}

/********************************************************************
* Function:     fillBasisMatrix
*--------------------------------------------------------------------
* Description:  Fill an element of the array of a BooleanMatrix on
*               row "i" and column "j". The value of the element will
*               be 1 (black) if the number behind "i" is part of the
*               SubSet "set" and 0 (white) if not.
********************************************************************/
void fillBasisMatrix(BooleanMatrix* B, SubSet* set, int i, int j);

/********************************************************************
* Function:     printBooleanMatrix
*--------------------------------------------------------------------
* Description:  Print the content of the array of a BooleanMatrix.
*               To clarify which matrix is printed, a name with the
*               length of two characters has to be given to it.
********************************************************************/
void printBooleanMatrix(BooleanMatrix* B, char* name);

/********************************************************************
* Function:     fillBasisMatrices
*--------------------------------------------------------------------
* Description:  Calculate the basis matrices by creating subsets
*               from a set with "n" elements. (n = number of shares.)
*               The even cardinality set is used to fill the basis
*               matrix B0. The odd cardinality set is used to fill B1.
********************************************************************/
void fillBasisMatrices(BooleanMatrix* B0, BooleanMatrix* B1);

#endif /* BOOLEANMATRIX_H */
