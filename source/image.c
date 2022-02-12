#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "image.h"
#include "handleBMP.h"

static int openImageR(char* path, Image* image)
{
    image->file = fopen(path, "rb");
    if ( image->file == NULL ){
        fprintf(stderr, "ERR: open readable image %s\n", path);
        return -1;
    }
    return 0;
}

static int openImageW(char* path, Image* image)
{
    image->file = fopen(path, "wb");
    if ( image->file == NULL ){
        fprintf(stderr, "ERR: open writeable image %s\n", path);
        return -1;
    }
    return 0;
}

int mallocPixelArray(Image* image)
{
    image->array = malloc(image->width * image->height);
	if (image->array == NULL) {
		fprintf(stderr, "ERR: allocate pArray buffer\n");
		return -1;
	}
    return 0;
}

static int mallocImageOfEqSize(Image* source, Image* dest)
{
    dest->width = source->width;
    dest->height = source->height;
    return mallocPixelArray(dest);
}

static int copyImageContent(Image* source, Image* dest)
{
    if((dest->width != source->width) || (dest->height != source->height)){
        fprintf(stderr, "ERR: can't copy image of different size\n");
		return -1;
    }

    int32_t width = dest->width;
    int32_t height = dest->height;

    for (int32_t row = 0; row < height; row++) {
        for (int32_t column = 0; column < width; column++) {
            dest->array[row*width+column] = source->array[row*width+column];
        }
    }
    return 0;
}

 /* Info:
    allocates buffer on success
    for image->array (and fills
    it with data from the BMP)
 */
int createSourceImage(char* path, Image* image)
{
    /* open existing image file for read */
	if (openImageR(path, image) != 0)
		return -1;

    /* read image file */
	if (readBMP(image) != 0) /* allocates buffer on success */
	{
		fprintf(stderr, "ERR: readBMP\n");
		fclose(image->file);
        return -1;
	}
    return 0;
}

/* creates empty .bmp files for the shares */
int createShareFiles(char* dirPath, Image** share, int number_of_shares)
{
    int err = 0;
    char path[100];
    memset(path, '\0', sizeof(path));

    /* for each share */
    for(uint8_t i = 0; i < number_of_shares; i++)
    {
        if (!err)
        {
            /* give every .bmp an unique number to save it */
            snprintf(path, sizeof(path), "%s/share%02d.bmp", dirPath, i+1);
            err = openImageW(path, share[i]);
        }
        else
            share[i]->file = NULL;
    }
    return err;
}

void closeShareFiles(Image** share, int number_of_shares)
{
     /* for each share */
    for(int i = 0; i < number_of_shares; i++)
    {
        if(share[i]->file != NULL)
            fclose(share[i]->file);
    }
}

int mallocDeterministicShareArrays(Image* source, Image** share, int number_of_shares)
{
    int err = 0;

     /* number of pixels in a share per pixel in source file = 2^{n-1} */
    uint8_t m = 1 << (number_of_shares-1);

    int32_t deterministicHeight, deterministicWidth;

    /* calculate the pixel expansion */
    if (number_of_shares % 2) /* odd */
    {
        /*  the shares will have the same appearance as
            the source, and are just scaled a bit larger
        */
        deterministicHeight = (int32_t)sqrt(m);
        deterministicWidth = deterministicHeight;
    }
    else /* even */
    {
        /*  the shares will look a little
            "stretched" from left to right
        */
        deterministicHeight = (int32_t)sqrt(m/2);
        deterministicWidth = deterministicHeight * 2;
    }

    /* for each share */
    for(int i = 0; i < number_of_shares; i++)
    {
        if(!err)
        {
            share[i]->height = source->height * deterministicHeight;
            share[i]->width = source->width * deterministicWidth;
            err = mallocPixelArray(share[i]);
        }
        else
            share[i]->array = NULL;
    }
    return err;
}

void freeShareArrays(Image** share, int number_of_shares)
{
     /* for each share */
    for(int i = 0; i < number_of_shares; i++)
        free(share[i]->array);
}
