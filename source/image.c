#include "image.h"

#include <string.h>
#include <unistd.h>

#include "fileManagement.h"
#include "handleBMP.h"
#include "settings.h"

// note: sourcePath and sharePath are globals from visualCrypt.c

/********************************************************************
 * Function:     openImageR
 *--------------------------------------------------------------------
 * Description:  Opens the binary file on the location "path" for read
 *               and stores it in image->file.
 ********************************************************************/
static inline void openImageR(char *path, Image *image) {
    image->file = xfopen(path, "rb");
}

/********************************************************************
 * Function:     openImageW
 *--------------------------------------------------------------------
 * Description:  Opens the binary file on the location "path" for
 *               write and stores it in image->file.
 ********************************************************************/
static inline void openImageW(char *path, Image *image) {
    image->file = xfopen(path, "wb");
}

/********************************************************************
 * Function:     createSourceImage
 *--------------------------------------------------------------------
 * Description:  Opens the bmp file located at sourcePath and stores
 *               the opened file path, width, height and
 *               black-and-white interpreted pixel array in the
 *               structure "image".
 ********************************************************************/
void createSourceImage(Image *image) {
    openImageR(sourcePath, image);
    readBMP(image);
}

/********************************************************************
 * Function:     createShareFiles
 *--------------------------------------------------------------------
 * Description:  Creates empty .bmp files for the shares and names
 *               them share01, share02, etc. The opened files are
 *               stored in share->file.
 ********************************************************************/
void createShareFiles(Image *share, int numberOfShares) {
    size_t pathLen = strlen(sharePath) + 13;
    char *path = xcalloc(pathLen, 1);

    // for each share
    for (int i = 0; i < numberOfShares; i++) {
        // give every .bmp an unique number to save it
        snprintf(path, pathLen, "%s/share%02d.bmp", sharePath, i + 1);
        openImageW(path, share + i);
    }

    xfree(path);
}

/********************************************************************
 * Function:     deleteShareFiles
 *--------------------------------------------------------------------
 * Description:  Remove all files named share*.bmp
 ********************************************************************/
void deleteShareFiles() {
    int i = 1;
    size_t pathLen = strlen(sharePath) + 13;
    char *path = xcalloc(pathLen, 1);
    do {
        snprintf(path, pathLen, "%s/share%02d.bmp", sharePath, i++);
    } while (remove(path) == 0);

    xfree(path);
}

/********************************************************************
 * Function:     drawShareFiles
 *--------------------------------------------------------------------
 * Description:  Uses the data stored in share->array for each image
 *               structure "share" to calculate / draw the rgb values
 *               for each pixel of the opened bmp files (share->file).
 ********************************************************************/
void drawShareFiles(Image *share, int numberOfShares) {
    // for each share
    for (int i = 0; i < numberOfShares; i++) {
        createBMP(share + i);
    }
}

/********************************************************************
 * Function:     readShareFiles
 *--------------------------------------------------------------------
 * Description:  Read share files starting at the share with the
 *               number given to first and end with the number given
 *               to last. They will be stored in Image structures that
 *               must have been allocated before.
 ********************************************************************/
void readShareFiles(Image *share, int first, int last) {
    size_t pathLen = strlen(sharePath) + 13;
    char *path = xcalloc(pathLen, 1);

    // for each viewed share
    for (int i = 0; i <= last - first; i++) {
        snprintf(path, pathLen, "%s/share%02d.bmp", sharePath, i + first);
        openImageR(path, share + i);
        readBMP(share + i);
    }

    xfree(path);
}

/********************************************************************
 * Function:     createDecryptedImageFile
 *--------------------------------------------------------------------
 * Description:  Create a file for the Image decryption of the share
 *               files. The name of the decrypted image will be
 *               decrypted01.bmp if this name isn't used already, and
 *               get counted up to a maximum of decrypted99.bmp.
 ********************************************************************/
void createDecryptedImageFile(Image *image) {
    int i = 1;
    size_t pathLen = strlen(sharePath) + 17;
    char *path = xcalloc(pathLen, 1);

    // don't overwrite already existing decrypted images if possible
    do {
        snprintf(path, pathLen, "%s/decrypted%02d.bmp", sharePath, i++);
        if (i > 99) {
            snprintf(path, pathLen, "%s/decrypted01.bmp", sharePath);
            remove(path);
            fprintf(stdout, "replaced %s\n", path);
            break;
        }
    } while (access(path, F_OK) != -1);

    openImageW(path, image);

    xfree(path);
}
