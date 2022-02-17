#include "deterministicShares.h"
#include "vcAlgorithms.h"

int deterministicAlgorithm(AlgorithmData* data)
{
    uint8_t n = data->numberOfShares;
    uint8_t m = 1 << (n-1);     /* number of pixels in a share per pixel in source file = 2^{n-1} */

    /* Create pixel-arrays for the shares */
	if (mallocDeterministicShareArrays(data->source, data->shares, n, m) != 0)
        return -1;

    /*  create set with n elements holding subsets
        with even and odd cardinalities of it
    */
    Set set = createSet(n, m);
    if(!set.even || !set.odd)
        return -1;

    printAllSubsets(&set);

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
            fillBasisMatrix(&B0, set.even, i, j);

            /*___FILL_B1___*/
            fillBasisMatrix(&B1, set.odd, i, j);
        }
    }

    printBooleanMatrix(&B0, "B0");
    printBooleanMatrix(&B1, "B1");
    
    /* Fill pixel-arrays of the shares */
    if (fillDeterministicShareArrays(data->source, data->shares, &B0, &B1) != 0)
        goto cleanupC;


    /* Cleanup */

    deleteBooleanMatrix(&B1);
    deleteBooleanMatrix(&B0);
    deleteSet(&set);
    return 0;

    cleanupC:
        deleteBooleanMatrix(&B1);
    cleanupB:
        deleteBooleanMatrix(&B0);
    cleanupA:
        deleteSet(&set);
    return -1;
}
