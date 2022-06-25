#include <stdio.h>
#include <time.h>
#include "memoryManagement.h"
#include "fileManagement.h"
#include "menu.h"
#include "vcAlgorithms.h"
#include "vcAlg01_deterministic.h"
#include "vcAlg02_probabilistic.h"
#include "vcAlg03_randomGrid.h"
#include "vcAlg03_randomGrid_V0.h"
#include "vcAlg03_randomGrid_V1.h"
#include "settings.h"
#include "timeMeasurement.h"

/********************************************************************
* Function:     printMeasuredTime
*--------------------------------------------------------------------
* Description:  Print the average measured time ins milliseconds or
*               seconds, and the total time over all loops in
*               (h:m:s) format to the file "fp".
********************************************************************/
static void printMeasuredTime(FILE* fp, struct timespec *start, struct timespec *stop, char *name)
{
    // calculate time of total milliseconds or seconds
    long seconds = stop->tv_sec - start->tv_sec;
    long nanoseconds = stop->tv_nsec - start->tv_nsec;
    double milliseconds = seconds * 1e3 + nanoseconds / 1e6;
    double _seconds = (double)seconds + nanoseconds / 1e9;

    // calculate time in hours : minutes : seconds
    int hours = (_seconds/3600);
	int remainingSeconds = _seconds - (hours*3600);
	int minutes = remainingSeconds/60;
	remainingSeconds = remainingSeconds - (minutes*60);

    // print average milliseconds or seconds
    if (_seconds / TIME_LOOPS < 1)
        fprintf(fp, "%s\naverage time (ms): %lf\n", name, milliseconds / TIME_LOOPS);
    else
        fprintf(fp, "%s\naverage time (s): %lf\n", name, _seconds / TIME_LOOPS);
    
    // print total time
    fprintf(fp, "total time (h:m:s): %02d:%02d:%02d\n\n", hours, minutes, remainingSeconds);
    fprintf(stdout, "measurement of %s algorithm done ...\n", name);
}

/********************************************************************
* Function:     timeMeasurement
*--------------------------------------------------------------------
* Description:  Measure the elapsed time for the different algorithms
********************************************************************/
void timeMeasurement()
{
    int n = getNfromUser();
    int k = getKfromUser(n);

    FILE* randomSrc = xfopen(RANDOM_FILE_PATH, "r");

    Image source;
    createSourceImage(&source);

    Image *expShares = xmalloc(n*sizeof(Image));
    AlgorithmData _dData = {
		.source = &source,
		.shares = expShares,
		.numberOfShares = n,
		.randomSrc = randomSrc
	};
    deterministicData* dData = prepareDeterministicAlgorithm(&_dData);

    Image *shares = xmalloc(n*sizeof(Image));
    AlgorithmData _pData = {
		.source = &source,
		.shares = shares,
		.numberOfShares = n,
		.randomSrc = randomSrc
	};
    probabilisticData* pData = prepareProbabilisticAlgorithm(&_pData);
    
    // prepare random grid algorithms
    int arraySize = source.width * source.height;
    Pixel* storage = xmalloc(arraySize);
    Pixel* sharePixel = xmalloc(k * sizeof(Pixel));
    Pixel* tmpSharePixel = xmalloc(n * sizeof(Pixel));
    Image* tmpShares = xmalloc(k*sizeof(Image));
    mallocSharesOfSourceSize(&source, tmpShares, k);
    int* setOfN = createSetOfN(n, 1);

/*_________________________ START TIME MEASUREMENT _________________________*/

    struct timespec start, stop;
    FILE* logFile = xfopen(TIME_LOG_PATH, "a");

    fprintf(stdout, "Start time measurement ...\n");

    fprintf(logFile, \
    "Time Measurement:\n"
    "algorithm loops: %d\n"
    "number of shares (n): %d\n"
    "number of shares to stack (k): %d\n"
    "Image size in px: %d x %d\n\n", \
    TIME_LOOPS, n, k, source.width, source.height);

    // deterministic algorithm
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    for(int i = 0; i<TIME_LOOPS; i++)
        __deterministicAlgorithm(dData);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
    printMeasuredTime(logFile, &start, &stop, "deterministic");

    // probabilistic algorithm
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    for(int i = 0; i<TIME_LOOPS; i++)
        __probabilisticAlgorithm(pData);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
    printMeasuredTime(logFile, &start, &stop, "probabilistic");

    // (n,n) random grid algorithm
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    for(int i = 0; i<TIME_LOOPS; i++)
        randomGrid_nn_Threshold(source.array, shares, &storage, randomSrc, arraySize, n);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
    printMeasuredTime(logFile, &start, &stop, "(n,n) random grid");

    // alternate (n,n) random grid algorithm
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    for(int i = 0; i<TIME_LOOPS; i++)
        alternate_nn_ThresholdRGA(source.array, shares, tmpSharePixel, randomSrc, arraySize, n);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
    printMeasuredTime(logFile, &start, &stop, "alternate (n,n) random grid");

    // (2,n) random grid algorithm
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    for(int i = 0; i<TIME_LOOPS; i++)
        randomGrid_2n_Threshold(source.array, shares, randomSrc, arraySize, n);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
    printMeasuredTime(logFile, &start, &stop, "(2,n) random grid");

    // alternate (2,n) random grid algorithm
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    for(int i = 0; i<TIME_LOOPS; i++)
        alternate_2n_ThresholdRGA(source.array, shares, randomSrc, arraySize, n);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
    printMeasuredTime(logFile, &start, &stop, "alternate (2,n) random grid");

    // (k,n) random grid algorithm
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    for(int i = 0; i<TIME_LOOPS; i++)
    {
        // since the (k,n) needs the additional shares filled before, the time must be added
        randomGrid_nn_Threshold(source.array, tmpShares, &storage, randomSrc, arraySize, k);
        __randomGrid_kn_Threshold(setOfN, shares, tmpShares, randomSrc, arraySize, n, k);
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
    printMeasuredTime(logFile, &start, &stop, "(k,n) random grid");

    // alternate (k,n) random grid algorithm
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    for(int i = 0; i<TIME_LOOPS; i++)
        __alternate_kn_ThresholdRGA(setOfN, source.array, sharePixel, shares, randomSrc, arraySize, n, k);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
    printMeasuredTime(logFile, &start, &stop, "alternate (k,n) random grid");

    fprintf(logFile, "__________________________________________________\n\n");

	// cleanup
	xcloseAll();
	xfreeAll();
	fprintf(stdout, "Success!\nResult was stored in %s\n", TIME_LOG_PATH);
}
