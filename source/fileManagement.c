/*
*   Copyright: (c) 2023 Sabrina Otto. All rights reserved.
*   This work is licensed under the terms of the MIT license.
*/

#include "fileManagement.h"

#include <stdlib.h>

// Global
List *fileList = NULL;

FILE *xfopen(const char *filename, const char *mode) {
    List *newFile = malloc(sizeof(List));
    validatePointer(newFile, "ERR: allocate memory");
    newFile->data = fopen(filename, mode);
    validatePointer(newFile->data, "ERR: open file");
    appendOnList(newFile, &fileList);
    return newFile->data;
}

int xfclose(FILE *stream) {
    if (!stream) {
        return -1;
    }

    List *removed = removeFromList(stream, &fileList);
    if (!removed) {
        customExitOnFailure("ERR: close element not in fileList");
    }

    int ret = fclose(removed->data);
    free(removed);
    return ret;
}

void xcloseAll() {
    List *index = fileList, *tmp;

    while (index) {
        tmp = index;
        index = index->next;
        xfclose(tmp->data);
    }
}
