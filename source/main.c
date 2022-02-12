#include <stdlib.h>
#include "vcAlgorithms.h"
#include "image.h"

#define SOURCE_PATH "../image/colortest.bmp"
#define DEST_PATH "../image"

int main(int argc, char* argv[])
{
	Image source, share01, share02, share03;
	Image* shares[] = {&share01, &share02, &share03};
	int number_of_shares = 3;

	/* open and read image file */
	if (createSourceImage(SOURCE_PATH, &source) != 0)
		return -1;

	/* create new share files */
    if (createShareFiles(DEST_PATH, shares, number_of_shares) != 0)
		goto cleanupA;


	/* create pixel-arrays for the shares */
	if (mallocDeterministicShareArrays(&source, shares, number_of_shares) != 0)
		goto cleanupB;


	/* call the algorithm */
	if(deterministicAlgorithm(&source, shares, number_of_shares) != 0)
		goto cleanupC;
	
	fprintf(stdout, "Success!\n");
	freeShareArrays(shares, number_of_shares);
	closeShareFiles(shares, number_of_shares);
	free(source.array);
	fclose(source.file);
	return 0;

	cleanupC:
		freeShareArrays(shares, number_of_shares);
	cleanupB:
		closeShareFiles(shares, number_of_shares);
	cleanupA:
		free(source.array);
		fclose(source.file);
		return -1;
}