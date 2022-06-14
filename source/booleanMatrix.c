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
void fillBasisMatrix(BooleanMatrix* B, SubSet* set, int i, int j)
{
    for (int e = 0; e < set[j].length; e++)
    {
        if (set[j].data[e] == i)
        {
            setPixel(*B, i, j, 1);
            return;
        }
    }
    setPixel(*B, i, j, 0);
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
    for(int i = 0; i < n; i++)     // rows
    {
        for(int j = 0; j < m; j++) // columns
        {
            fprintf(stdout, "%d", getPixel(*B, i, j));
        }
        fprintf(stdout, "\n");
    }
    fprintf(stdout, "\n");
}

/********************************************************************
* Function:     fillBasisMatrices
*--------------------------------------------------------------------
* Description:  Calculate the basis matrices by creating subsets
*               from a set with "n" elements. (n = number of shares.)
*               The even cardinality set is used to fill the basis
*               matrix B0. The odd cardinality set is used to fill B1.
********************************************************************/
void fillBasisMatrices(BooleanMatrix* B0, BooleanMatrix* B1)
{
    int n = B0->n;
    int m = B0->m;
    
    /*  create set with n elements, holding subsets
        with even and odd cardinalities of it
    */
    Set set = createSet(n, m);
    printAllSubsets(&set);

    for(int i = 0; i < n; i++) // rows
    {
        for(int j = 0; j < m; j++) // columns
        {
            fillBasisMatrix(B0, set.even, i, j);
            fillBasisMatrix(B1, set.odd, i, j);
        }
    }

    printBooleanMatrix(B0, "B0");
    printBooleanMatrix(B1, "B1");
}
