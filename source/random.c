#include "fileManagement.h"
#include "memoryManagement.h"
#include "random.h"

/********************************************************************
* Function:     getRandomNumber
*--------------------------------------------------------------------
* Description:  Return a random number between min and max, that was
*               calculated from a file containing random numbers.
********************************************************************/
uint64_t getRandomNumber(FILE* randomSrc, uint64_t min, uint64_t max)
{
    uint64_t randNum;
    xfread(&randNum, sizeof(randNum), 1, randomSrc, "ERR: read file with random numbers");

    // TODO: find a more secure way
    // Note: using uint64_t slows the algorithm extremely
    // https://stackoverflow.com/questions/49878942/why-is-rand6-biased
    randNum = min + (randNum % max);
    return randNum;
}

/********************************************************************
* Function:     createSetOfN
*--------------------------------------------------------------------
* Description:  Create vector with values from 1 to n
********************************************************************/
int* createSetOfN(int n, int start)
{            
    int* setOfN = xmalloc(n * sizeof(int));
    for(int i = 0; i < n; i++)
        setOfN[i] = i+start;
    return setOfN;
}

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
void shuffleVector(int* vector, int n, FILE* randomSrc)
{
    // Fisher–Yates shuffle
    // https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
    // https://stackoverflow.com/questions/42321370/fisher-yates-shuffling-algorithm-in-c

    int tmp, randNum;
    for (int i = n-1; i>0; i--) {
        randNum = getRandomNumber(randomSrc, 0, i+1);

        // swap elements
        tmp = vector[i];
        vector[i] = vector[randNum];
        vector[randNum] = tmp;
    }
}

/********************************************************************
* Function:     copyMatrixColumn
*--------------------------------------------------------------------
* Description:  Copy a matrix column of column number "srcIdx"
*               from src to column "destIdx" of dest.
********************************************************************/
static void copyMatrixColumn(BooleanMatrix* dest, BooleanMatrix* src, int destIdx, int srcIdx)
{
    int numRows = src->n;
    for(int row = 0; row < numRows; row++)
        setPixel(*dest, row, destIdx, getPixel(*src, row, srcIdx));
}

/********************************************************************
* Function:     shuffleColumns
*--------------------------------------------------------------------
* Description:  Copy matrix from src to dest in a column-shuffled way
*               by getting shuffled indices from parameter "indices".
********************************************************************/
void shuffleColumns(BooleanMatrix* dest, BooleanMatrix* src, FILE* randomSrc, int* indices)
{
    int numColumns = src->m;
    shuffleVector(indices, numColumns, randomSrc);

    for (int i = 0; i < numColumns; i++)
        copyMatrixColumn(dest, src, i, indices[i]);
}

/********************************************************************
* Function:     copyMatrixRow
*--------------------------------------------------------------------
* Description:  Copy a matrix row of row number "srcIdx"
*               from src to row "destIdx" of dest.
********************************************************************/
static void copyMatrixRow(BooleanMatrix* dest, BooleanMatrix* src, int destIdx, int srcIdx)
{
    int numColumns = src->m;
    for(int column = 0; column < numColumns; column++)
        setPixel(*dest, destIdx, column, getPixel(*src, srcIdx, column));
}

/********************************************************************
* Function:     shuffleRows
*--------------------------------------------------------------------
* Description:  Copy matrix from src to dest in a row-shuffled way by
*               getting shuffled indices from parameter "indices".
********************************************************************/
void shuffleRows(BooleanMatrix* dest, BooleanMatrix* src, FILE* randomSrc, int* indices)
{
    int numRows = src->n;
    shuffleVector(indices, numRows, randomSrc);

    for (int i = 0; i < numRows; i++)
        copyMatrixRow(dest, src, i, indices[i]);
}
