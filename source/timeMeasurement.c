#include <time.h>
#include "memoryManagement.h"
#include "fileManagement.h"
#include "vcAlgorithms.h"
#include "vcAlg01_deterministic.h"
#include "vcAlg02_probabilistic.h"
#include "vcAlg03_randomGrid.h"
#include "vcAlg03_randomGrid_V0.h"
#include "vcAlg03_randomGrid_V1.h"
#include "timeMeasurement.h"

//debug
#include <stdio.h> //printf

/********************************************************************
* Function:     printMeasuredTime
*--------------------------------------------------------------------
* Description:  Print the measured time ins milliseconds or seconds.
********************************************************************/
static void printMeasuredTime(struct timespec *start, struct timespec *stop, char *name)
{
    long seconds = stop->tv_sec - start->tv_sec;
    long nanoseconds = stop->tv_nsec - start->tv_nsec;
    double milliseconds = seconds * 1e3 + nanoseconds / 1e6;
    double _seconds = (double)seconds + nanoseconds / 1e9;

    if (_seconds < 1)
        printf("%s\ntime (ms): %lf\n\n", name, milliseconds);
    else
        printf("%s\ntime (s): %lf\n\n", name, _seconds);
}

/********************************************************************
* Function:     timeMeasurement
*--------------------------------------------------------------------
* Description:  Measure the elapsed time for the different algorithms
********************************************************************/
void timeMeasurement()
{
/*__________________________________________________________________________*/
/*__ ALLOCATE MEMORY AND CREATE VALUES NEEDED IN THE DIFFERENT ALGORITHMS __*/
/*__________________________________________________________________________*/

    uint8_t n = 2; // number of shares
    uint8_t m = 1 << (n-1);
    uint8_t k = 2;

    /* open urandom, to get random numbers from it */
    FILE* urandom = xfopen("/dev/urandom", "r");

    /* read source image */
    Image source;
    createSourceImage(&source);

    /* create basis matrices */
    BooleanMatrix B0 = createBooleanMatrix(n,m);
    BooleanMatrix B1 = createBooleanMatrix(n,m);
    fillBasisMatrices(&B0, &B1);

    /* allocate shares */
    Image *shares = xmalloc(n*sizeof(Image));
    mallocSharesOfSourceSize(&source, shares, n);

    Image *expShares = xmalloc(n*sizeof(Image));
    mallocPixelExpandedShares(&source, expShares, n, m);

    Image* additShares = xmalloc(k*sizeof(Image));
    mallocSharesOfSourceSize(&source, additShares, k);

    Pixel* checkList = xmalloc(n * sizeof(Pixel));

    /* prepare deterministic algorithm */
    int deterministicHeight, deterministicWidth;
    calcPixelExpansion(&deterministicHeight, &deterministicWidth, n, m);
    BooleanMatrix permutation = createBooleanMatrix(n, m);
    BooleanMatrix encryptedPixel = createBooleanMatrix(deterministicHeight, deterministicWidth);
    Pixel* columnCheckList = xmalloc(m * sizeof(Pixel));
    deterministicData dData = {
        .B0 = &B0,
        .B1 = &B1,
        .permutation = &permutation,
        .encryptedPixel = &encryptedPixel,
        .sourceArray = source.array,
        .columnCheckList = columnCheckList,
        .rowCheckList = checkList,
        .share = expShares,
        .urandom = urandom,
        .width = source.width,
        .height = source.height,
        .deterministicWidth = deterministicWidth,
        .deterministicHeight = deterministicHeight
    };

    /* prepare probabilistic algorithm */
    BooleanMatrix columnVector = createBooleanMatrix(n, 1);
        probabilisticData pData = {
        .B0 = &B0,
        .B1 = &B1,
        .columnVector = &columnVector,
        .sourceArray = source.array,
        .checkList = checkList,
        .share = shares,
        .urandom = urandom,
        .width = source.width,
        .height = source.height
    };

    /* prepare random grid algorithms */
    int arraySize = source.width * source.height;
    Pixel* storage = xmalloc(arraySize);
    Pixel* sharePixel = xmalloc(k * sizeof(Pixel));
    Pixel* setOfN = createSetOfN(n);
    Pixel* randSortedSetOfN = xmalloc(n * sizeof(Pixel));
    kn_randomGridData rgData = {
        .setOfN = setOfN,
        .randSortedSetOfN = randSortedSetOfN,
        .checkList = checkList,
        .sharePixel = sharePixel,
        .sourceArray = source.array,
        .shares = shares,
        .additShares = additShares,
        .urandom = urandom,
        .arraySize = arraySize,
        .n = n,
        .k = k
    };

/*__________________________________________________________________________*/
/*_________________________ START TIME MEASUREMENT _________________________*/
/*__________________________________________________________________________*/

    struct timespec start, stop;

    /* deterministic algorithm */
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    __deterministicAlgorithm(&dData);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
    printMeasuredTime(&start, &stop, "deterministic");

    /* probabilistic algorithm */
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    __probabilisticAlgorithm(&pData);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
    printMeasuredTime(&start, &stop, "probabilistic");

    /* (n,n) random grid */
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    randomGrid_nn_Threshold(source.array, shares, &storage, urandom, arraySize, n);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
    printMeasuredTime(&start, &stop, "(n,n) random grid");

    /* alternate (n,n) random grid */
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    alternate_nn_ThresholdRGA(source.array, shares, randSortedSetOfN, urandom, arraySize, n);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
    printMeasuredTime(&start, &stop, "alternate (n,n) random grid");

    /* (2,n) random grid */
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    randomGrid_2n_Threshold(source.array, shares, urandom, arraySize, n);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
    printMeasuredTime(&start, &stop, "(2,n) random grid");

    /* alternate (2,n) random grid */
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    alternate_2n_ThresholdRGA(source.array, shares, urandom, arraySize, n);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
    printMeasuredTime(&start, &stop, "alternate (2,n) random grid");

    /* (k,n) random grid */
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    //since the (k,n) needs the additional shares filled before, the time must be added:
    randomGrid_nn_Threshold(source.array, additShares, &storage, urandom, arraySize, k);
    __randomGrid_kn_Threshold(&rgData);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
    printMeasuredTime(&start, &stop, "(k,n) random grid");

    /* alternate (k,n) random grid */
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    __alternate_kn_ThresholdRGA(&rgData);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
    printMeasuredTime(&start, &stop, "alternate (k,n) random grid");    

	/* cleanup */
	xcloseAll();
	xfreeAll();
	fprintf(stdout, "Success!\n");
}
