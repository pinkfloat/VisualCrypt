#include "booleanMatrix.h"

#include <stdio.h>
#include <time.h>

#include "memoryManagement.h"
#include "settings.h"

BooleanMatrix createBooleanMatrix(int height, int width) {
    BooleanMatrix result;
    result.height = height;
    result.width = width;
    result.array = xmalloc(sizeof(Pixel) * height * width);
    return result;
}

void deleteBooleanMatrix(BooleanMatrix *matrix) {
    xfree(matrix->array);
}

void fillBasisMatrix(BooleanMatrix *B, SubSet *subSet, int i, int j) {
    for (int e = 0; e < subSet[j].length; e++) {
        if (subSet[j].data[e] == i) {
            setPixel(*B, i, j, 1);
            return;
        }
    }
    setPixel(*B, i, j, 0);
}

void printBooleanMatrix(BooleanMatrix *B, char *name) {
    int n = B->height;
    int m = B->width;

    fprintf(stdout, "%2s:\n", name);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            fprintf(stdout, "%d", getPixel(*B, i, j));
        }
        fprintf(stdout, "\n");
    }
    fprintf(stdout, "\n");
}

void fillBasisMatrices(BooleanMatrix *B0, BooleanMatrix *B1) {
    int n = B0->height;
    int m = B0->width;

    Set set = createSet(n, m);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            fillBasisMatrix(B0, set.even, i, j);
            fillBasisMatrix(B1, set.odd, i, j);
        }
    }

    if (PRINT_SETS_AND_BASIS_MATRICES) {
        printAllSubsets(&set);
        printBooleanMatrix(B0, "B0");
        printBooleanMatrix(B1, "B1");
    }
}
