/*
*   Copyright: (c) 2023 Sabrina Otto. All rights reserved.
*   This work is licensed under the terms of the MIT license.
*/

#include "image.h"

#include <string.h>
#include <unistd.h>

#include "fileManagement.h"
#include "handleBMP.h"
#include "settings.h"

// note: sourcePath and sharePath are globals from visualCrypt.c

/*********************************************************************
 * Function:     openImageR
 *--------------------------------------------------------------------
 * Description:  Opens the binary file from "path" for read, and
 *               stores it in image->file.
 ********************************************************************/
static inline void openImageR(char *path, Image *image) {
    image->file = xfopen(path, "rb");
}

/*********************************************************************
 * Function:     openImageW
 *--------------------------------------------------------------------
 * Description:  Opens the binary file from "path" for write, and
 *               stores it in image->file.
 ********************************************************************/
static inline void openImageW(char *path, Image *image) {
    image->file = xfopen(path, "wb");
}

void createSourceImage(Image *image) {
    openImageR(sourcePath, image);
    readBMP(image);
}

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

void deleteShareFiles() {
    int i = 1;
    size_t pathLen = strlen(sharePath) + 13;
    char *path = xcalloc(pathLen, 1);
    do {
        snprintf(path, pathLen, "%s/share%02d.bmp", sharePath, i++);
    } while (remove(path) == 0);

    xfree(path);
}

void drawShareFiles(Image *share, int numberOfShares) {
    // for each share
    for (int i = 0; i < numberOfShares; i++) {
        createBMP(share + i);
    }
}

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
