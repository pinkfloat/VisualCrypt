#include <stdlib.h>
#include "settings.h"
#include "handleBMP.h"

#define SIZE_BMP_HEADER 54
#define BYTES_PER_RGB_PIXEL 3

typedef struct
{
    char padForAlignment[2]; 			    /* extend header size to multiple of 4 bytes */

    /* 14 Byte BMP Fileheader */
    char bitmapSignatureBytes[2];
    uint32_t fileSize;
    uint32_t reserved;
    uint32_t pixelDataOffset;

    /* 40 Byte BitMapInfoHeader */
    uint32_t infoHeaderSize;
    int32_t widthInPixel;
    int32_t heightInPixel;
    uint16_t numColorPlanes;
    uint16_t bitsPerPixel;
    uint32_t compressionMethod;
    uint32_t bmpPixelArraySize;
    uint32_t horizontalResolution;
    uint32_t verticalResolution;
    uint32_t numColorsInPalette;
    uint32_t numImportantColors;
} BmpHeader;

static inline uint32_t roundToMultipleOf4(uint32_t x)
{
    return (x + 3) &~ 3;
}

/*_____________________________________WRITE_OPERATIONS_____________________________________*/

static void writeBmpHeader(BmpHeader* bmpHeader, int32_t width, int32_t height)
{
   	/* 14 Byte BMP Fileheader */
    bmpHeader->bitmapSignatureBytes[0] 	 = 'B';
    bmpHeader->bitmapSignatureBytes[1] 	 = 'M';
    bmpHeader->fileSize          		 = SIZE_BMP_HEADER + width * height * BYTES_PER_RGB_PIXEL;
    bmpHeader->reserved            		 = 0xdeadbeef;
    bmpHeader->pixelDataOffset 			 = SIZE_BMP_HEADER;

    /* 40 Byte BitMapInfoHeader */
    bmpHeader->infoHeaderSize        	 = 40;
    bmpHeader->widthInPixel          	 = width;
    bmpHeader->heightInPixel          	 = height;
    bmpHeader->numColorPlanes        	 = 1;
    bmpHeader->bitsPerPixel          	 = BYTES_PER_RGB_PIXEL * 8;
    bmpHeader->compressionMethod     	 = 0;
    bmpHeader->bmpPixelArraySize     	 = width * height * BYTES_PER_RGB_PIXEL;
    bmpHeader->horizontalResolution  	 = 2048;
    bmpHeader->verticalResolution    	 = 2048;
    bmpHeader->numColorsInPalette    	 = 0;
    bmpHeader->numImportantColors    	 = 0;
}

/* take binary source value (black / white) and write it to RGB destination */
static void writeBmpBody(const Pixel* source, Pixel* destination, int32_t width, int32_t height)
{
    Pixel p_source;
    int32_t paddedWidth = roundToMultipleOf4(3*width);
    for (int32_t row = 0; row < height; row++) {
        for (int32_t column = 0; column < width; column++) {
            p_source = source[row * width + column] ? 255 : 0;          /* white = 255, black = 0 */
            destination[row * paddedWidth + column * 3]     = p_source; /* blue  */
            destination[row * paddedWidth + column * 3 + 1] = p_source; /* green */
            destination[row * paddedWidth + column * 3 + 2] = p_source; /* red   */
        }
    }
}

int createBMP(Image* image)
{
    int32_t width = image->width;
    int32_t height = image->height;
    uint32_t bmpSize = SIZE_BMP_HEADER + roundToMultipleOf4(3*width) * height;

    /* create BMP file content */
    uint8_t* bmpBuffer = malloc(bmpSize + 2);
    if (bmpBuffer == NULL) {
        fprintf(stderr, "ERR: allocate buffer\n");
        return -1;
    }
    writeBmpHeader((BmpHeader*)bmpBuffer, width, height);
    writeBmpBody(image->array, bmpBuffer + sizeof(BmpHeader), width, height);

    /* write content to file */
    if (fwrite(bmpBuffer + 2, 1, bmpSize, image->file) != bmpSize) {
        free(bmpBuffer);
        fprintf(stderr, "ERR: write to file\n");
        return -1;
    }

    free(bmpBuffer);
    return 0;
}

/*_____________________________________READ_OPERATIONS_____________________________________*/

static int readBmpHeader(FILE* file, BmpHeader* headerInformation)
{
    size_t bufferSize = sizeof(BmpHeader);

    if (fread( ((uint8_t*)headerInformation) + 2, 1, bufferSize - 2, file) != bufferSize - 2) {
        fprintf(stderr, "ERR: get header information\n");
        return -1;
    }
    return 0;
}

static int readBmpBody(Image* image)
{
    int32_t width = image->width;
    int32_t height = image->height;

    uint32_t bmpSize = roundToMultipleOf4(3*width) * height;
    uint8_t* bmpBuffer = malloc(width * height * BYTES_PER_RGB_PIXEL);

    if (bmpBuffer == NULL) {
        fprintf(stderr, "ERR: allocate buffer\n");
        return -1;
    }

    /* read remaining file to buffer after readBmpHeader */
    if (fread(bmpBuffer, 1, bmpSize, image->file) != bmpSize) {
        free(bmpBuffer);
        fprintf(stderr, "ERR: invalid BMP file\n");
        return -1;
    }

    /* calculate pixel Array */
    uint8_t* pBuffer = bmpBuffer;
    float red, green, blue;

    for (int32_t row = 0; row < height; row++) {
        for (int32_t column = 0; column < width; column++) {

            /* weighted the color values of an rgb-image and determines whether
            a pixel of the result is supposed to be black or white */
            red = *pBuffer * 0.2616;
            green = *(pBuffer+1) * 0.7152;
            blue = *(pBuffer+2) * 0.0722;
            image->array[row * width + column] = (blue + green + red) > THRESHOLD ? 1 : 0; /* white = 1, black = 0 */

            pBuffer += 3;
        }
    }

    free(bmpBuffer);
    return 0;
}

/* Info: allocates buffer without freeing on success */
int readBMP(Image* image)
{
    BmpHeader headerInformation;

    if(readBmpHeader(image->file, &headerInformation) != 0){
		fprintf(stderr, "ERR: read BMP Header\n");
        return -1;
	}

    image->width = headerInformation.widthInPixel;
	image->height = headerInformation.heightInPixel;

	if (mallocPixelArray(image) != 0)
        return -1;

	if (readBmpBody(image) != 0){
		fprintf(stderr, "ERR: read BMP Body\n");
        free(image->array);
		return -1;
	}

    return 0;
}
