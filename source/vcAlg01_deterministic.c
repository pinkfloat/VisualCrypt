#include "deterministicShares.h"
#include "vcAlgorithms.h"

/********************************************************************
* Function:     deterministicAlgorithm
*--------------------------------------------------------------------
* Description:  This is an implementation of the so called
*               "deterministic Algorithm" from Moni Naor and Adi
*               Shamir. It will calculate the pixel of the share
*               images from the pixel in the source file, by creating
*               basis matrices out of the number of the share files.
*               The basis matrices will be afterwards permutated in
*               columns and each share will get a different row of
*               every permutation per source pixel.
********************************************************************/
void deterministicAlgorithm(AlgorithmData* data)
{
    uint8_t n = data->numberOfShares;
    uint8_t m = 1 << (n-1);     /* number of pixels in a share per pixel in source file = 2^{n-1} */

    /* Create pixel-arrays for the shares */
	mallocDeterministicShareArrays(data->source, data->shares, n, m);

    /*  create set with n elements, holding subsets
        with even and odd cardinalities of it
    */
    Set set = createSet(n, m);

    printAllSubsets(&set);

    /* create basis matrices */
    BooleanMatrix B0 = createBooleanMatrix(n,m);
    BooleanMatrix B1 = createBooleanMatrix(n,m);

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
    fillDeterministicShareArrays(data->source, data->shares, &B0, &B1);
}
