#include <stdio.h>
#include <stdlib.h>
#include "handleBMP.h"

#define SOURCE_PATH "../image/colortest.bmp"
#define DEST_PATH "../image/blacknwhite.bmp"

int main(int argc, char* argv[])
{
	int32_t width, height;
	Pixel* pixelArray;

	/* open existing image file 01 */
	FILE* file01 = fopen(SOURCE_PATH, "rb");
    if ( file01 == NULL ){
        fprintf(stderr, "ERR: open image file\n");
        return -1;
    }

	/* create new image file 02 */
    FILE* file02 = fopen(DEST_PATH, "wb");
    if ( file02 == NULL ){
        fprintf(stderr, "ERR: create image file\n");
		goto cleanupA;
    }

	/* read image file 01 */
	if (readBMP(file01, &pixelArray, &width, &height) != 0){
		fprintf(stderr, "ERR: readBMP\n");
		goto cleanupB;
	}

	/* draw image file 02 */
	if (createBMP(file02, pixelArray, width, height) != 0){
		fprintf(stderr, "ERR: createBMP\n");
		goto cleanupC;
	}
	
	fprintf(stdout, "Created File!");
	free(pixelArray);
	fclose(file02);
	fclose(file01);
	return 0;

	cleanupC:
		free(pixelArray);
	cleanupB:
		fclose(file02);
	cleanupA:
		fclose(file01);
		return -1;
}