#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>
#include <stdint.h>
#include "memoryManagement.h"

#ifndef TYPE_PIXEL
#define TYPE_PIXEL

typedef uint8_t Pixel; // black / white

#endif // TYPE_PIXEL

typedef struct {
    FILE *file;
    Pixel *array;
    int32_t width;
    int32_t height;
} Image;

extern char* sourcePath;
extern char* sharePath;

/********************************************************************
* Function:     mallocPixelArray
*--------------------------------------------------------------------
* Description:  Allocates a pixel array of the size image->width
*               * image->height and stores it in image->array.
********************************************************************/
static inline void mallocPixelArray(Image* image)
{
    image->array = xmalloc(image->width * image->height);
}

/********************************************************************
* Function:     createSourceImage
*--------------------------------------------------------------------
* Description:  Opens the bmp file located at sourcePath and stores
*               the opened file path, width, height and
*               black-and-white interpreted pixel array in the
*               structure "image".
********************************************************************/
void createSourceImage(Image* image);

/********************************************************************
* Function:     createShareFiles
*--------------------------------------------------------------------
* Description:  Creates empty .bmp files for the shares and names
*               them share01, share02, etc. The opened files are
*               stored in share->file.
********************************************************************/
void createShareFiles(Image* share, int numberOfShares);

/********************************************************************
* Function:     deleteShareFiles
*--------------------------------------------------------------------
* Description:  Remove all files named share*.bmp
********************************************************************/
void deleteShareFiles();

/********************************************************************
* Function:     drawShareFiles
*--------------------------------------------------------------------
* Description:  Uses the data stored in share->array for each image
*               structure "share" to calculate / draw the rgb values
*               for each pixel of the opened bmp files (share->file).
********************************************************************/
void drawShareFiles(Image* share, int numberOfShares);

/********************************************************************
* Function:     readShareFiles
*--------------------------------------------------------------------
* Description:  Read share files starting at the share with the
*               number given to first and end with the number given
*               to last. They will be stored in Image structures that
*               must have been allocated before.
********************************************************************/
void readShareFiles(Image* share, int first, int last);

/********************************************************************
* Function:     createDecryptedImageFile
*--------------------------------------------------------------------
* Description:  Create a file for the Image decryption of the share
*               files. The name of the decrypted image will be
*               decrypted01.bmp if this name isn't used already, and
*               get counted up to a maximum of decrypted99.bmp.
********************************************************************/
void createDecryptedImageFile(Image* image);

#endif /* IMAGE_H */ 
