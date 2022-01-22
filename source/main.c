#include <stdio.h>
#include "handleBMP.h"

#define PATH "../image/created.bmp"

int main(int argc, char* argv[])
{
	int ret;

	/* create empty file */
    FILE* file = fopen(PATH, "wb");
    if ( file == NULL ){
        fprintf(stderr, "ERR: create file\n");
        return -1;
    }

	/* create image */
	Pixel pArray[8] = {0, 0, 0, 1, 1, 0, 1, 1};
	
	/* draw image */
	ret = createBMP(file, pArray, 4, 2);
	fprintf(stdout, "Created File!");

	fclose(file);
	return 0;
}