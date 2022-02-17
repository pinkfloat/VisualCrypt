#include <stdlib.h>
#include "vcAlgorithms.h"

#define SOURCE_PATH "../image/colortest.bmp"
#define DEST_PATH "../image"

int callAlgorithm(int numberOfShares, int (*algorithm)(AlgorithmData*))
{   
	Image source, *shares = malloc(numberOfShares*sizeof(Image));
	if(!shares)
		return -1;

    /* open and read image file */
	if (createSourceImage(SOURCE_PATH, &source) != 0)
		goto cleanupA;

	/* create new share files */
    if (createShareFiles(DEST_PATH, shares, numberOfShares) != 0)
		goto cleanupB;

	/* call the algorithm */
	AlgorithmData data = {
		.source = &source,
		.shares = shares,
		.numberOfShares = numberOfShares,
	};
	if(algorithm(&data) != 0)
		goto cleanupC;

	/* draw the share-bmps */
	if (drawShareFiles(shares, numberOfShares) != 0)
		goto cleanupC;
	
	fprintf(stdout, "Success!\n");
	freeShareArrays(shares, numberOfShares);
	closeShareFiles(shares, numberOfShares);
	free(source.array);
	fclose(source.file);
	free(shares);
	return 0;

	cleanupC:
		fprintf(stderr, "Failed!\n");
		freeShareArrays(shares, numberOfShares);
		closeShareFiles(shares, numberOfShares);
	cleanupB:
		free(source.array);
		fclose(source.file);
	cleanupA:
		free(shares);
		return -1;
}
