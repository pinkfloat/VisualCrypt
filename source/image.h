#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>
#include <stdint.h>

#ifndef TYPE_PIXEL
#define TYPE_PIXEL

typedef uint8_t Pixel; /* black / white */

#endif /* TYPE_PIXEL */

typedef struct {
    FILE *file;
    Pixel *array;
    int32_t width;
    int32_t height;
} Image;

/********************************************************************
* Function:     mallocPixelArray
*--------------------------------------------------------------------
* Description:  Allocates a pixel array of the size image->width
*               * image->height and stores it in image->array.
* Return:       0 on success, -1 on failure.
********************************************************************/
int mallocPixelArray(Image* image);

/********************************************************************
* Function:     createSourceImage
*--------------------------------------------------------------------
* Description:  Opens the bmp file located at "path" and stores the
*               opened file path, width, height and black-and-white
*               interpreted pixel array in the structure "image".
* Info:         Allocates buffer for image->array without freeing it
*               on success.
* Return:       0 on success, -1 on failure.
********************************************************************/
int createSourceImage(char* path, Image* image);

/********************************************************************
* Function:     createShareFiles
*--------------------------------------------------------------------
* Description:  Creates empty .bmp files for the shares in the
*               directory "dirPath" and names them share01, share02,
*               etc. The opened files are stored in share->file.
* Return:       0 on success, -1 on failure.
********************************************************************/
int createShareFiles(char* dirPath, Image* share, int numberOfShares);

/********************************************************************
* Function:     drawShareFiles
*--------------------------------------------------------------------
* Description:  Uses the data stored in share->array for each image
*               structure "share" to calculate / draw the rgb values
*               for each pixel of the opened bmp files (share->file).
* Return:       0 on success, -1 on failure.
********************************************************************/
int drawShareFiles(Image* share, int numberOfShares);

/********************************************************************
* Function:     closeShareFiles
*--------------------------------------------------------------------
* Description:  Close the file share->file for each image structure
*               "share".
********************************************************************/
void closeShareFiles(Image* share, int numberOfShares);

/********************************************************************
* Function:     freeShareArrays
*--------------------------------------------------------------------
* Description:  Free the allocated buffer for each Image structure
*               "share" at share->array.
********************************************************************/
void freeShareArrays(Image* share, int n);

#endif /* IMAGE_H */ 
