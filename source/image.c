#include <stdlib.h>
#include <string.h>
#include "image.h"
#include "handleBMP.h"

/********************************************************************
* Function:     openImageR
*--------------------------------------------------------------------
* Description:  Opens the binary file on the location "path" for read
*               and stores it in image->file.
* Return:       0 on success, -1 on failure.
********************************************************************/
static int openImageR(char* path, Image* image)
{
    image->file = fopen(path, "rb");
    if ( image->file == NULL ){
        fprintf(stderr, "ERR: open readable image %s\n", path);
        return -1;
    }
    return 0;
}

/********************************************************************
* Function:     openImageW
*--------------------------------------------------------------------
* Description:  Opens the binary file on the location "path" for
*               write and stores it in image->file.
* Return:       0 on success, -1 on failure.
********************************************************************/
static int openImageW(char* path, Image* image)
{
    image->file = fopen(path, "wb");
    if ( image->file == NULL ){
        fprintf(stderr, "ERR: open writeable image %s\n", path);
        return -1;
    }
    return 0;
}

/********************************************************************
* Function:     mallocPixelArray
*--------------------------------------------------------------------
* Description:  Allocates a pixel array of the size image->width
*               * image->height and stores it in image->array.
* Return:       0 on success, -1 on failure.
********************************************************************/
int mallocPixelArray(Image* image)
{
    image->array = malloc(image->width * image->height);
	if (image->array == NULL) {
		fprintf(stderr, "ERR: allocate pArray buffer\n");
		return -1;
	}
    return 0;
}

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

/********************************************************************
* Function:     createShareFiles
*--------------------------------------------------------------------
* Description:  Creates empty .bmp files for the shares in the
*               directory "dirPath" and names them share01, share02,
*               etc. The opened files are stored in share->file.
* Return:       0 on success, -1 on failure.
********************************************************************/
int createShareFiles(char* dirPath, Image* share, int numberOfShares)
{
    int err = 0;
    char path[100];
    memset(path, '\0', sizeof(path));

    /* for each share */
    for(uint8_t i = 0; i < numberOfShares; i++)
    {
        if (!err)
        {
            /* give every .bmp an unique number to save it */
            snprintf(path, sizeof(path), "%s/share%02d.bmp", dirPath, i+1);
            err = openImageW(path, share+i);
        }
        else
            share[i].file = NULL;
    }
    return err;
}

/********************************************************************
* Function:     drawShareFiles
*--------------------------------------------------------------------
* Description:  Uses the data stored in share->array for each image
*               structure "share" to calculate / draw the rgb values
*               for each pixel of the opened bmp files (share->file).
* Return:       0 on success, -1 on failure.
********************************************************************/
int drawShareFiles(Image* share, int numberOfShares)
{
    int err;

    /* for each share */
    for(uint8_t i = 0; i < numberOfShares; i++)
    {
        err = createBMP(share+i);
        if (err)
            break;
    }
    return err;
}

/********************************************************************
* Function:     closeShareFiles
*--------------------------------------------------------------------
* Description:  Close the file share->file for each image structure
*               "share".
********************************************************************/
void closeShareFiles(Image* share, int numberOfShares)
{
     /* for each share */
    for(int i = 0; i < numberOfShares; i++)
    {
        if(share[i].file != NULL)
            fclose(share[i].file);
    }
}

/********************************************************************
* Function:     freeShareArrays
*--------------------------------------------------------------------
* Description:  Free the allocated buffer for each Image structure
*               "share" at share->array.
********************************************************************/
void freeShareArrays(Image* share, int n)
{
     /* for each share */
    for(int i = 0; i < n; i++)
        free(share[i].array);
}
