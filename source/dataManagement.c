#include <stdio.h>
#include <stdlib.h>

#include "fileManagement.h"
#include "memoryManagement.h"

/********************************************************************
 * Function:     customExitOnFailure
 *--------------------------------------------------------------------
 * Description:  Exit the programm with the exit message "message".
 *               It will free all allocated buffer and close all files
 *               opened so far, before exiting.
 ********************************************************************/
void customExitOnFailure(const char *message) {
    fprintf(stderr, "%s\n", message);
    fprintf(stderr, "Close all files...\n");
    xcloseAll();
    fprintf(stderr, "Free all buffer...\n");
    xfreeAll();
    fprintf(stderr, "Exit programm...\n");
    exit(EXIT_FAILURE);
}

/********************************************************************
 * Function:     removeFromList
 *--------------------------------------------------------------------
 * Description:  Removes the list element, that has a data pointer,
 *               to the parameter "dataPtr", from the list "first"
 *               is pointing to, and returns the deleted list element.
 *               If nothing is removed from the list, the function
 *               will return NULL instead.
 ********************************************************************/
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
