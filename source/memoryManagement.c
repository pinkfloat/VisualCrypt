/*
*   Copyright: (c) 2023 Sabrina Otto. All rights reserved.
*   This work is licensed under the terms of the MIT license.
*/

#include "memoryManagement.h"

// Global
List *memList = NULL;

void *xmalloc(size_t size) {
    List *newMem = malloc(sizeof(List));
    validatePointer(newMem, "ERR: allocate memory");
    newMem->data = malloc(size);
    validatePointer(newMem->data, "ERR: allocate memory");
    appendOnList(newMem, &memList);
    return newMem->data;
}

void *xcalloc(size_t nitems, size_t size) {
    List *newMem = malloc(sizeof(List));
    validatePointer(newMem, "ERR: allocate memory");
    newMem->data = calloc(nitems, size);
    validatePointer(newMem->data, "ERR: allocate memory");
    appendOnList(newMem, &memList);
    return newMem->data;
}

void xfree(void *ptr) {
    if (!ptr) {
        return;
    }

    List *removed = removeFromList(ptr, &memList);
    if (!removed) {
        customExitOnFailure("ERR: free element not in memList");
    }

    free(removed->data);
    free(removed);
}

void xfreeAll() {
    List *index = memList, *tmp;

    while (index) {
        tmp = index;
        index = index->next;
        xfree(tmp->data);
    }
}
