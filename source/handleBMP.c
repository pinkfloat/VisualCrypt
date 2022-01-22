#include <stdlib.h>
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

static inline uint32_t roundToMultipleOf4(uint32_t x)
{
    return (x + 3) &~ 3;
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

int createBMP(FILE* file, const Pixel* pixelArray, int32_t width, int32_t height)
{
    uint32_t bmpSize = SIZE_BMP_HEADER + roundToMultipleOf4(3*width) * height;

    /* create BMP file content */
    uint8_t* bmpBuffer = malloc(sizeof(BmpHeader) + width * height * BYTES_PER_RGB_PIXEL);
    if (bmpBuffer == NULL) {
        fprintf(stderr, "ERR: allocate buffer\n");
        return -1;
    }
    writeBmpHeader((BmpHeader*)bmpBuffer, width, height);
    writeBmpBody(pixelArray, bmpBuffer + sizeof(BmpHeader), width, height);

    /* write content to file */
    if (fwrite(bmpBuffer + 2, 1, bmpSize, file) != bmpSize) {
        free(bmpBuffer);
        fprintf(stderr, "ERR: write to file\n");
        return -1;
    }

    free(bmpBuffer);
    return 0;
}