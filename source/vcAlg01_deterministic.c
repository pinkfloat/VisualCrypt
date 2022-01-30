#include "booleanMatrix.h"
#include "setsNsubsets.h"
#include "vcAlgorithms.h"

static inline void fillBooleanMatrix(BooleanMatrix* B, SubSet* set, uint8_t i, uint8_t j)
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

static void debugPrintMatrix(BooleanMatrix* B, uint8_t num, uint8_t n, uint8_t m){
    fprintf(stdout, "B%d:\n", num);
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

int deterministicAlgorithm(Image* source, Image** shares, uint8_t number_of_shares)
{
    uint8_t n = number_of_shares;
    uint8_t m = 1 << (n-1);     /* number of pixels in a share per pixel in source file = 2^{n-1} */

    /*  create set with n elements holding subsets
        with even and odd cardinalities of it
    */
    Set set = createSet(n, m);
    if(!set.even || !set.odd)
        return -1;

    debugPrintAllSubsets(&set);

    /* create basis matrices */
    BooleanMatrix B0 = createBooleanMatrix(n,m);
    if(!B0.array)
        goto cleanupA;

    BooleanMatrix B1 = createBooleanMatrix(n,m);
    if(!B1.array)
        goto cleanupB;

    for(uint8_t i = 0; i < n; i++)     /* rows */
    {
        for(uint8_t j = 0; j < m; j++) /* columns */
        {
            /*___FILL_B0___*/
            fillBooleanMatrix(&B0, set.even, i, j);

            /*___FILL_B1___*/
            fillBooleanMatrix(&B1, set.odd, i, j);
        }
    }

    debugPrintMatrix(&B0, 0, n, m);
    debugPrintMatrix(&B1, 1, n, m);

    deleteBooleanMatrix(&B1);
    deleteBooleanMatrix(&B0);
    deleteSet(&set);
    return 0;

    cleanupB:
        deleteBooleanMatrix(&B0);
    cleanupA:
        deleteSet(&set);
    return -1;
}
