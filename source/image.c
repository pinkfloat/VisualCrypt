#include <string.h>
#include "memoryManagement.h"
#include "fileManagement.h"
#include "image.h"
#include "handleBMP.h"

/********************************************************************
* Function:     openImageR
*--------------------------------------------------------------------
* Description:  Opens the binary file on the location "path" for read
*               and stores it in image->file.
********************************************************************/
static void openImageR(char* path, Image* image)
{
    image->file = xfopen(path, "rb");
}

/********************************************************************
* Function:     openImageW
*--------------------------------------------------------------------
* Description:  Opens the binary file on the location "path" for
*               write and stores it in image->file.
********************************************************************/
static void openImageW(char* path, Image* image)
{
    image->file = xfopen(path, "wb");
}

/********************************************************************
* Function:     mallocPixelArray
*--------------------------------------------------------------------
* Description:  Allocates a pixel array of the size image->width
*               * image->height and stores it in image->array.
********************************************************************/
void mallocPixelArray(Image* image)
{
    image->array = xmalloc(image->width * image->height);
}

/********************************************************************
* Function:     createSourceImage
*--------------------------------------------------------------------
* Description:  Opens the bmp file located at "path" and stores the
*               opened file path, width, height and black-and-white
*               interpreted pixel array in the structure "image".
********************************************************************/
void createSourceImage(char* path, Image* image)
{
    /* open existing image file for read */
	openImageR(path, image);

    /* read image file */
    readBMP(image);
}

/********************************************************************
* Function:     createShareFiles
*--------------------------------------------------------------------
* Description:  Creates empty .bmp files for the shares in the
*               directory "dirPath" and names them share01, share02,
*               etc. The opened files are stored in share->file.
********************************************************************/
void createShareFiles(char* dirPath, Image* share, int numberOfShares)
{
    char path[100];
    memset(path, '\0', sizeof(path));

    /* for each share */
    for(uint8_t i = 0; i < numberOfShares; i++)
    {
            /* give every .bmp an unique number to save it */
            snprintf(path, sizeof(path), "%s/share%02d.bmp", dirPath, i+1);
            openImageW(path, share+i);
    }
}

/********************************************************************
* Function:     drawShareFiles
*--------------------------------------------------------------------
* Description:  Uses the data stored in share->array for each image
*               structure "share" to calculate / draw the rgb values
*               for each pixel of the opened bmp files (share->file).
********************************************************************/
void drawShareFiles(Image* share, int numberOfShares)
{
    /* for each share */
    for(uint8_t i = 0; i < numberOfShares; i++)
        createBMP(share+i);
}
