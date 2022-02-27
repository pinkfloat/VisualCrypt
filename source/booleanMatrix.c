#include <stdio.h>
#include <time.h>

#include "memoryManagement.h"
#include "booleanMatrix.h"

/********************************************************************
* Function:     createBooleanMatrix
*--------------------------------------------------------------------
* Description:  Saves the size of a BooleanMatrix (n = height, m = 
*               width) and allocates a 2D pixel array of the size,
*               that is stored in the BooleanMatrix struct as array.
* Return:       The created BooleanMatrix.
********************************************************************/
BooleanMatrix createBooleanMatrix(int n, int m)
{
    BooleanMatrix result;
    result.n = n;
    result.m = m;
    result.array = xmalloc(sizeof(Pixel)*n*m);
    return result;
}

/********************************************************************
* Function:     getPixel
*--------------------------------------------------------------------
* Description:  Return the value of the Pixel of a Booleanmatrix at
*               row "i" and column "j". 
* Return:       "1" is considered as black pixel and "0" as white.
********************************************************************/
Pixel getPixel(BooleanMatrix* matrix, int i, int j)
{
    return matrix->array[i * matrix->m + j];
}

/********************************************************************
* Function:     setPixel
*--------------------------------------------------------------------
* Description:  Set the Pixel of a BooleanMatrix on row "i" and
*               columns "j" to the value "value".
********************************************************************/
void setPixel(BooleanMatrix* matrix, int i, int j, uint8_t value)
{
    matrix->array[i * matrix->m + j] = value;
}

/********************************************************************
* Function:     deleteBooleanMatrix
*--------------------------------------------------------------------
* Description:  Free the allocated pixel array of a Booleanmatrix.
********************************************************************/
void deleteBooleanMatrix(BooleanMatrix* matrix)
{
    xfree(matrix->array);
}

/********************************************************************
* Function:     fillBasisMatrix
*--------------------------------------------------------------------
* Description:  Fill an element of the array of a BooleanMatrix on
*               row "i" and column "j". The value of the element will
*               be 1 (black) if the number behind "i" is part of the
*               SubSet "set" and 0 (white) if not.
********************************************************************/
void fillBasisMatrix(BooleanMatrix* B, SubSet* set, uint8_t i, uint8_t j)
{
    uint8_t found = 0;
    if (set[j].length) /* the subset is not NULL */
    {
        for (int e = 0; e < set[j].length; e++) /* e = element of a subset */
        {
            /* check if element i is part of the subset */
            if (set[j].data[e] == i){
                found = 1;
                break;
            }
        }
        if(found)
            setPixel(B, i, j, 1);
        else
            setPixel(B, i, j, 0);
    }
    else /* the subset is NULL */
        setPixel(B, i, j, 0);
}

/********************************************************************
* Function:     printBooleanMatrix
*--------------------------------------------------------------------
* Description:  Print the content of the array of a BooleanMatrix.
*               To clarify which matrix is printed, a name with the
*               length of two characters has to be given to it.
********************************************************************/
void printBooleanMatrix(BooleanMatrix* B, char* name)
{
    int n = B->n;
    int m = B->m;

    fprintf(stdout, "%2s:\n", name);
    for(int i = 0; i < n; i++)     /* rows */
    {
        for(int j = 0; j < m; j++) /* columns */
        {
            fprintf(stdout, "%d", getPixel(B, i, j));
        }
        fprintf(stdout, "\n");
    }
    fprintf(stdout, "\n");
}
