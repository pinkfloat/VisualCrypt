#include "random.h"

#include "fileManagement.h"
#include "memoryManagement.h"

#define MAX_UINT -1

uint8_t getRandomNumber(FILE *randomSrc, uint8_t min, uint8_t max) {
    uint8_t randNum, inRangeNum, limit = MAX_UINT - max;

    do {
        xfread(&randNum, sizeof(randNum), 1, randomSrc, "ERR: read file with random numbers");
        inRangeNum = min + (randNum % max);
    } while (randNum - inRangeNum > limit);  // remove bias

    return inRangeNum;
}

int *createSetOfN(int n, int start) {
    int *setOfN = xmalloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        setOfN[i] = i + start;
    }
    return setOfN;
}

void shuffleVector(int *vector, int n, FILE *randomSrc) {
    int tmp, randNum;
    for (int i = n - 1; i > 0; i--) {
        randNum = getRandomNumber(randomSrc, 0, i + 1);

        // swap elements
        tmp = vector[i];
        vector[i] = vector[randNum];
        vector[randNum] = tmp;
    }
}

/*********************************************************************
 * Function:     copyMatrixColumn
 *--------------------------------------------------------------------
 * Description:  Copy the matrix column of column number "srcIdx"
 *               from "src" to column "destIdx" of "dest".
 ********************************************************************/
static void copyMatrixColumn(BooleanMatrix *dest, BooleanMatrix *src, int destIdx, int srcIdx) {
    int numRows = src->height;
    for (int row = 0; row < numRows; row++) {
        setPixel(*dest, row, destIdx, getPixel(*src, row, srcIdx));
    }
}

void shuffleColumns(BooleanMatrix *dest, BooleanMatrix *src, FILE *randomSrc, int *indices) {
    int numColumns = src->width;
    shuffleVector(indices, numColumns, randomSrc);

    for (int i = 0; i < numColumns; i++) {
        copyMatrixColumn(dest, src, i, indices[i]);
    }
}
