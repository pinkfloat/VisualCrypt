#include <stdlib.h>
#include "image.h"

int openImageR(char* path, Image* image)
{
    image->file = fopen(path, "rb");
    if ( image->file == NULL ){
        fprintf(stderr, "ERR: open readable image %s\n", path);
        return -1;
    }
    return 0;
}

int openImageW(char* path, Image* image)
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

int mallocImageOfEqSize(Image* source, Image* dest)
{
    dest->width = source->width;
    dest->height = source->height;
    return mallocPixelArray(dest);
}

int copyImageContent(Image* source, Image* dest)
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