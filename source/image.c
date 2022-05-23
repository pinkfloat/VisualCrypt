#include <string.h>
#include <unistd.h>
#include "fileManagement.h"
#include "image.h"
#include "handleBMP.h"
#include "settings.h"

/********************************************************************
* Function:     openImageR
*--------------------------------------------------------------------
* Description:  Opens the binary file on the location "path" for read
*               and stores it in image->file.
********************************************************************/
static inline void openImageR(char* path, Image* image)
{
    image->file = xfopen(path, "rb");
}

/********************************************************************
* Function:     openImageW
*--------------------------------------------------------------------
* Description:  Opens the binary file on the location "path" for
*               write and stores it in image->file.
********************************************************************/
static inline void openImageW(char* path, Image* image)
{
    image->file = xfopen(path, "wb");
}

/********************************************************************
* Function:     createSourceImage
*--------------------------------------------------------------------
* Description:  Opens the bmp file located at SOURCE_PATH and stores
*               the opened file path, width, height and
*               black-and-white interpreted pixel array in the
*               structure "image".
********************************************************************/
void createSourceImage(Image* image)
{
	openImageR(SOURCE_PATH, image);
    readBMP(image);
}

/********************************************************************
* Function:     createShareFiles
*--------------------------------------------------------------------
* Description:  Creates empty .bmp files for the shares and names
*               them share01, share02, etc. The opened files are
*               stored in share->file.
********************************************************************/
void createShareFiles(Image* share, int numberOfShares)
{
    char path[PATH_LENGTH];
    memset(path, '\0', sizeof(path));

    // for each share
    for(uint8_t i = 0; i < numberOfShares; i++)
    {
            // give every .bmp an unique number to save it
            snprintf(path, sizeof(path), "%s/share%02d.bmp", SHARE_PATH, i+1);
            openImageW(path, share+i);
    }
}

/********************************************************************
* Function:     deleteShareFiles
*--------------------------------------------------------------------
* Description:  Remove all files named share*.bmp
********************************************************************/
void deleteShareFiles()
{
    uint8_t i = 1;
    char path[PATH_LENGTH];
    memset(path, '\0', sizeof(path));
    do
    {
        snprintf(path, sizeof(path), "%s/share%02d.bmp", SHARE_PATH, i++);
    } while(remove(path) == 0);
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
    // for each share
    for(uint8_t i = 0; i < numberOfShares; i++)
        createBMP(share+i);
}

/********************************************************************
* Function:     readShareFiles
*--------------------------------------------------------------------
* Description:  Read share files starting at the share with the
*               number given to first and end with the number given
*               to last. They will be stored in Image structures that
*               must have been allocated before.
********************************************************************/
void readShareFiles(Image* share, int first, int last)
{
    char path[PATH_LENGTH];
    memset(path, '\0', sizeof(path));

    // for each viewed share
    for(uint8_t i = 0; i <= last-first; i++)
    {
        snprintf(path, sizeof(path), "%s/share%02d.bmp", SHARE_PATH, i+first);
        openImageR(path, share+i);
        readBMP(share+i);
    }
}

/********************************************************************
* Function:     createDecryptedImageFile
*--------------------------------------------------------------------
* Description:  Create a file for the Image decryption of the share
*               files. The name of the decrypted image will be
*               decrypted01.bmp if this name isn't used already, and
*               get counted up to a maximum of decrypted99.bmp.
********************************************************************/
void createDecryptedImageFile(Image* image)
{
    int i = 1;
    char path[PATH_LENGTH];
    memset(path, '\0', sizeof(path));

    // don't overwrite already existing decrypted images if possible
    do
    {
        snprintf(path, sizeof(path), "%s/decrypted%02d.bmp", SHARE_PATH, i++);
        if (i > 99)
        {
            snprintf(path, sizeof(path), "%s/decrypted01.bmp", SHARE_PATH);
            remove(path);
            fprintf(stdout, "replaced %s\n", path);
            break;
        }
    } while( access(path, F_OK) != -1);

	openImageW(path, image);
}
