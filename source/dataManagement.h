/*
*   Copyright: (c) 2023 Sabrina Otto. All rights reserved.
*   This work is licensed under the terms of the MIT license.
*/

#ifndef DATA_MANAGEMENT_H
#define DATA_MANAGEMENT_H

typedef struct slist {
    void *data;
    struct slist *next;
} List;

/*********************************************************************
 * Function:     customExitOnFailure
 *--------------------------------------------------------------------
 * Description:  Exit the program with message "message".
 *               This will free all allocated buffer and close
 *               all opened files.
 ********************************************************************/
void customExitOnFailure(const char *message);

/*********************************************************************
 * Function:     validatePointer
 *--------------------------------------------------------------------
 * Description:  Call customExitOnFailure if ptr is NULL.
 ********************************************************************/
static inline void validatePointer(void *ptr, const char *message) {
    if (ptr == NULL) {
        customExitOnFailure(message);
    }
}

/*********************************************************************
 * Function:     appendOnList
 *--------------------------------------------------------------------
 * Description:  Append the List element "newElement" to the start of
 *               the list, where the parameter "first" is pointing to.
 ********************************************************************/
static inline void appendOnList(List *newElement, List **first) {
    newElement->next = *first;
    *first = newElement;
}

/*********************************************************************
 * Function:     removeFromList
 *--------------------------------------------------------------------
 * Description:  Removes the element "dataPtr" from List "first".
 * Return:       The deleted list element on success, NULL on failure.
 ********************************************************************/
List *removeFromList(void *dataPtr, List **first);

#endif /* DATA_MANAGEMENT_H */
