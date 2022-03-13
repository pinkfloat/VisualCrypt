#include "fileManagement.h"
#include "memoryManagement.h"
#include "random.h"

/********************************************************************
* Function:     getRandomNumber
*--------------------------------------------------------------------
* Description:  Return a random number between min and max, that was
*               calculated from a value of /dev/urandom, containing
*               cryptographically secure random numbers.
********************************************************************/
uint8_t getRandomNumber(FILE* urandom, uint8_t min, uint8_t max)
{
    uint8_t randNum;
    xfread(&randNum, sizeof(randNum), 1, urandom, "ERR: read /dev/urandom");
    randNum = min + (randNum % max);
    return randNum;
}

/********************************************************************
* Function:     copyColumnElement
*--------------------------------------------------------------------
* Description:  This is a sort-Function for randomSort().
*               The function will copy a chosen column element of
*               the source vector (n x 1 matrix = matrix column)
*               to the destination which is in this case just a pixel.
********************************************************************/
void copyColumnElement(MatrixCopy* copy)
{
    Pixel* randPixel = (Pixel*) copy->dest;
    *randPixel = getPixel(copy->source, copy->sourceIdx, 0);
}

/********************************************************************
* Function:     copyMatrixColumn
*--------------------------------------------------------------------
* Description:  This is a sort-Function for randomSort().
*               The function will copy a chosen column of the source
*               matrix (copy->sourceIdx) into a chosen column of the
*               destination matrix (copy->destIdx).
********************************************************************/
void copyMatrixColumn(MatrixCopy* copy)
{
    for(int row = 0; row < copy->source->n; row++)
        setPixel(copy->dest, row, copy->destIdx, getPixel(copy->source, row, copy->sourceIdx));
}

/********************************************************************
* Function:     fillEncryptedPixel
*--------------------------------------------------------------------
* Description:  This is a sort-Function for randomSort().
*               The function copies the values of a row of a
*               "permutation matrix" (1D-array) into a "deterministic
*               pixel" (2D-array). It is part of the encryption of a
*               specific pixel.
********************************************************************/
void fillEncryptedPixel(MatrixCopy* copy)
{
    int row = copy->sourceIdx;
    for(int column = 0; column < copy->source->m; column++)
        copy->dest->array[column] = getPixel(copy->source, row, column);
}

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
void randomSort(int randNum, Pixel* checkList, MatrixCopy* copy, void (*sortFunc)(MatrixCopy*))
{
    for(int checkIdx = 0, zeroCount = 0;; checkIdx++)
    {
        if(checkList[checkIdx] == 0)
            zeroCount++;

        if(zeroCount == randNum)
        {
            /*  copy the random chosen row/column of the source matrix
                to the next empty row/column of the destination matrix
            */
            copy->sourceIdx = checkIdx;
            sortFunc(copy);
            checkList[checkIdx] = 1; /* mark row/column in checkList as used */
            break;
        }
    }
} 
