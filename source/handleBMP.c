#include "settings.h"
#include "memoryManagement.h"
#include "fileManagement.h"
#include "handleBMP.h"

#define SIZE_BMP_HEADER 54
#define BYTES_PER_RGB_PIXEL 3

typedef struct
{
    char padForAlignment[2];    // extend header size to multiple of 4 bytes

    // 14 Byte BMP Fileheader
    char bitmapSignatureBytes[2];
    uint32_t fileSize;
    uint32_t reserved;
    uint32_t pixelDataOffset;

    // 40 Byte BitMapInfoHeader
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

/********************************************************************
* Function:     writeBmpHeader
*--------------------------------------------------------------------
* Description:  Each BMP file has a header in front of its pixel
*               data. This function will create a valid BMP header in
*               order to create a new BMP file.
* Input:        width = width of the new BMP file in pixel,
*               height = height of the new BMP file in pixel
********************************************************************/
static void writeBmpHeader(BmpHeader* bmpHeader, int32_t width, int32_t height)
{
   	// 14 Byte BMP Fileheader
    bmpHeader->bitmapSignatureBytes[0] 	 = 'B';
    bmpHeader->bitmapSignatureBytes[1] 	 = 'M';
    bmpHeader->fileSize          		 = SIZE_BMP_HEADER + width * height * BYTES_PER_RGB_PIXEL;
    bmpHeader->reserved            		 = 0xdeadbeef;
    bmpHeader->pixelDataOffset 			 = SIZE_BMP_HEADER;

    // 40 Byte BitMapInfoHeader
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

/********************************************************************
* Function:     writeBmpBody
*--------------------------------------------------------------------
* Description:  This function has to be called after writeBmpHeader()
*               to create a bmp file properly. It will take the
*               content of the pixel array "source", where the value
*               "0" is considered as white and every other number is
*               considered as black.
*               If the source pixel is white, the rgb values of the
*               corresponding destination pixel, interpreted as bmp
*               color data, are all set to 255. For black pixel they
*               are set to 0.
* Input:        source = most likely a boolean pixel array with
*                        the values 0 = white and 1 = black,
*               width = width of the new BMP file in pixel,
*               height = height of the new BMP file in pixel,
* Output:       destination = array that will get the rgb values of
*               the bmp file that will be created.
********************************************************************/
static void writeBmpBody(const Pixel* source, Pixel* destination, int32_t width, int32_t height)
{
    Pixel p_source;
    int32_t paddedWidth = roundToMultipleOf4(BYTES_PER_RGB_PIXEL*width);
    for (int32_t row = 0; row < height; row++) {
        for (int32_t column = 0; column < width; column++) {
            p_source = source[row * width + column] ? 0 : 255;          // black = 0, white = 255
            destination[row * paddedWidth + column * 3]     = p_source; // blue 
            destination[row * paddedWidth + column * 3 + 1] = p_source; // green
            destination[row * paddedWidth + column * 3 + 2] = p_source; // red
        }
    }
}

/********************************************************************
* Function:     createBMP
*--------------------------------------------------------------------
* Description:  The function createBMP will create a black and white
*               bmp file from the pure pixel data of a boolean array,
*               stored in image->array. It will fill the empty bmp 
*               file opened in image->file with a valid bmp header
*               and the array contents, restructured as rgb-values.
********************************************************************/
void createBMP(Image* image)
{
    int32_t width = image->width;
    int32_t height = image->height;
    uint32_t bmpSize = SIZE_BMP_HEADER + roundToMultipleOf4(BYTES_PER_RGB_PIXEL*width) * height;

    // create BMP file content
    uint8_t* bmpBuffer = xmalloc(bmpSize + 2);
    writeBmpHeader((BmpHeader*)bmpBuffer, width, height);
    writeBmpBody(image->array, bmpBuffer + sizeof(BmpHeader), width, height);

    // write content to file
    xfwrite(bmpBuffer + 2, 1, bmpSize, image->file,"ERR: create BMP");
    xfree(bmpBuffer);
}

/*_____________________________________READ_OPERATIONS_____________________________________*/

/********************************************************************
* Function:     readBmpHeader
*--------------------------------------------------------------------
* Description:  Each BMP file has a header in front of its pixel
*               data. This function will read to the end of the
*               bmp header information, and stores them in a
*               BmpHeader structure. In addition to that, it is
*               positioning the file offset of "file" to the start
*               of the pixel data.
********************************************************************/
static inline void readBmpHeader(FILE* file, BmpHeader* headerInformation)
{
    size_t bufferSize = sizeof(BmpHeader);
    xfread( ((uint8_t*)headerInformation) + 2, 1, bufferSize - 2, file, "ERR: read BMP header information");
}

/********************************************************************
* Function:     readBmpBody
*--------------------------------------------------------------------
* Description:  This function has to be called after readBmpHeader().
*               It will read the rgb values of the opened bmp file
*               image->file and, since the results are going to be
*               black and white, calculates if a colored pixel is
*               considered to be white(0) or black(1). The boolean
*               interpretation of the image will be stored in
*               image->array.
********************************************************************/
static void readBmpBody(Image* image)
{
    uint32_t width = image->width;
    uint32_t height = image->height;

    uint32_t paddedWidth = roundToMultipleOf4(width * BYTES_PER_RGB_PIXEL) ;
    uint32_t bmpSize = paddedWidth * height;
    uint8_t* bmpBuffer = xmalloc(bmpSize);

    // read remaining file stream to buffer after readBmpHeader
    xfread(bmpBuffer, 1, bmpSize, image->file, "ERR: invalid BMP body information");

    // calculate pixel Array
    uint8_t* pBuffer = NULL;
    float red, green, blue;

    for (uint32_t row = 0; row < height; row++) {
        for (uint32_t column = 0; column < width; column++) {

            /* weight the color values of an rgb-image and determine whether
            a pixel of the result is supposed to be black or white */
            pBuffer = bmpBuffer + (row * paddedWidth + column * BYTES_PER_RGB_PIXEL);
            red = *pBuffer * 0.2126;
            green = pBuffer[1] * 0.7152;
            blue = pBuffer[2] * 0.0722;
            image->array[row * width + column] = (blue + green + red) > THRESHOLD ? 0 : 1; // white = 0, black = 1
        }
    }
    xfree(bmpBuffer);
}

/********************************************************************
* Function:     readBMP
*--------------------------------------------------------------------
* Description:  The function readBMP will read a colored bmp opened
*               in image->file and get the information: width, height
*               and the pixel data from it, to store them into
*               the image structure "image".
********************************************************************/
void readBMP(Image* image)
{
    BmpHeader headerInformation;

    readBmpHeader(image->file, &headerInformation);

    image->width = headerInformation.widthInPixel;
	image->height = headerInformation.heightInPixel;

	mallocPixelArray(image);
	readBmpBody(image);
}
