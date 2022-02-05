#include <stdlib.h>
#include "handleBMP.h"
#include "vcAlgorithms.h"

#define SOURCE_PATH "../image/colortest.bmp"
#define DEST_PATH01 "../image/share01.bmp"
#define DEST_PATH02 "../image/share02.bmp"

int main(int argc, char* argv[])
{
	Image source, share01, share02;
	Image* shares[] = {&share01, &share02};
	int number_of_shares = 3;

	/* open existing image file for read */
	if (openImageR(SOURCE_PATH, &source) != 0)
		return -1;

	/* create new writeable share file */
    if (openImageW(DEST_PATH01, &share01) != 0)
		goto cleanupA;

	/* create new writeable share file */
    if (openImageW(DEST_PATH02, &share02) != 0)
		goto cleanupB;

	/* read image file */
	if (readBMP(&source) != 0) /* allocates buffer on success */
	{
		fprintf(stderr, "ERR: readBMP\n");
		goto cleanupC;
	}

	/* create pixel-array for share01 */
	if(mallocImageOfEqSize(&source, &share01) != 0)
	{
		fprintf(stderr, "ERR: createShare01\n");
		goto cleanupD;
	}

	/* create pixel-array for share02 */
	if(mallocImageOfEqSize(&source, &share02) != 0)
	{
		fprintf(stderr, "ERR: createShare02\n");
		goto cleanupE;
	}

	/* dummy-fill of the arrays */
	if(copyImageContent(&source, &share01) != 0)
		goto cleanupF;

	if(copyImageContent(&source, &share02) != 0)
		goto cleanupF;

	/* call the algorithm */
	if(deterministicAlgorithm(&source, shares, number_of_shares) != 0)
		goto cleanupF;

	/* draw image files */
	if (createBMP(&share01) != 0 || createBMP(&share02) != 0 )
	{
		fprintf(stderr, "ERR: createBMP\n");
		goto cleanupF;
	}
	
	fprintf(stdout, "Created File!\n");
	free(share02.array);
	free(share01.array);
	free(source.array);
	fclose(share02.file);
	fclose(share01.file);
	fclose(source.file);
	return 0;

	cleanupF:
		free(share02.array);
	cleanupE:
		free(share01.array);
	cleanupD:
		free(source.array);
	cleanupC:
		fclose(share02.file);
	cleanupB:
		fclose(share01.file);
	cleanupA:
		fclose(source.file);
		return -1;
}