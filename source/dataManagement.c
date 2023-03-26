/*
*   Copyright: (c) 2023 Sabrina Otto. All rights reserved.
*   This work is licensed under the terms of the MIT license.
*/

#include <stdio.h>
#include <stdlib.h>

#include "fileManagement.h"
#include "memoryManagement.h"

void customExitOnFailure(const char *message) {
    fprintf(stderr, "%s\n", message);
    fprintf(stderr, "Close all files...\n");
    xcloseAll();
    fprintf(stderr, "Free all buffer...\n");
    xfreeAll();
    fprintf(stderr, "Exit program...\n");
    exit(EXIT_FAILURE);
}

List *removeFromList(void *dataPtr, List **first) {
    List *prev, *tmp;

    // check if element exist
    if (dataPtr == NULL) {
        return NULL;
    }

    // case 1: list empty
    if (first == NULL) {
        return NULL;
    }

    // case 2: remove list start
    if ((*first)->data == dataPtr) {
        tmp = *first;
        *first = (*first)->next;
        return tmp;
    }

    // case 3: search for the element to remove
    prev = *first;
    tmp = (*first)->next;
    while (tmp) {
        // found
        if (tmp->data == dataPtr) {
            // remove element from list
            prev->next = tmp->next;
            return tmp;
        }
        // not found
        prev = tmp;
        tmp = tmp->next;
    }
    return NULL;
}
