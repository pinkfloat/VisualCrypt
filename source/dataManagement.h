#ifndef DATA_MANAGEMENT_H
#define DATA_MANAGEMENT_H

typedef struct slist {
    void *data;
    struct slist *next;
} List;

/********************************************************************
 * Function:     customExitOnFailure
 *--------------------------------------------------------------------
 * Description:  Exit the programm with the exit message "message".
 *               It will free all allocated buffer and close all files
 *               opened so far, before exiting.
 ********************************************************************/
void customExitOnFailure(const char *message);

/********************************************************************
 * Function:     validatePointer
 *--------------------------------------------------------------------
 * Description:  Print error messages, free all allocated buffer,
 *               and close all files opened so far, before exiting
 *               if ptr is NULL.
 ********************************************************************/
static inline void validatePointer(void *ptr, const char *message) {
    if (ptr == NULL) {
        customExitOnFailure(message);
    }
}

/********************************************************************
 * Function:     appendOnList
 *--------------------------------------------------------------------
 * Description:  Append the List element "newElement" to the start of
 *               a list, the parameter "first" is pointing to.
 ********************************************************************/
static inline void appendOnList(List *newElement, List **first) {
    newElement->next = *first;
    *first = newElement;
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
List *removeFromList(void *dataPtr, List **first);

#endif /* DATA_MANAGEMENT_H */
