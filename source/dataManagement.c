#include <stdio.h>
#include "memoryManagement.h"
#include "fileManagement.h"

/********************************************************************
* Function:     customExitOnFailure
*--------------------------------------------------------------------
* Description:  Exit the programm with the exit message "message".
*               It will free all allocated buffer and close all files
*               opened so far, before exiting.
********************************************************************/
void customExitOnFailure(const char *message)
{
    fprintf(stderr, "%s\n", message);
    fprintf(stderr, "Close all files...\n");
    xcloseAll();
    fprintf(stderr, "Free all buffer...\n");
    xfreeAll();
    fprintf(stderr, "Exit programm...\n");
    exit(-1);
}

/********************************************************************
* Function:     validatePointer
*--------------------------------------------------------------------
* Description:  Print error messages, free all allocated buffer,
*               and close all files opened so far, before exiting
*               if ptr is NULL.
********************************************************************/
void validatePointer(void* ptr, const char *message)
{
    if (ptr == NULL)
        customExitOnFailure(message);
}

/********************************************************************
* Function:     appendOnList
*--------------------------------------------------------------------
* Description:  Append the List element "newElement" to the start of
*               a list, the parameter "first" is pointing to.
********************************************************************/
void appendOnList(List* newElement, List** first)
{
    newElement->next = *first;
    *first = newElement;
}

/********************************************************************
* Function:     removeFromList
*--------------------------------------------------------------------
* Description:  Removes the list-element, that has a data pointer,
*               to the parameter "dataPtr", from the list "first"
*               is pointing to, and returns the deleted list element.
*               If nothing is removed from the list, the function
*               will return NULL instead.
********************************************************************/
List* removeFromList(void* dataPtr, List** first)
{
    List *prev, *tmp;

    /* check if element exist */
    if (dataPtr == NULL)
        return NULL;

    /* case 1: list empty */
    if (first == NULL)
        return NULL;

    /* case 2: remove list start */
    if((*first)->data == dataPtr)
    {
        tmp = *first;
        *first = (*first)->next;
        return tmp;
    }

    /* case 3: search for the element to remove */
    prev = *first;
    tmp = (*first)->next;
    while (tmp)
    {
        /* found */
        if(tmp->data == dataPtr)
        {
            /* remove element from list */
            prev->next = tmp->next;
            return tmp;
        }
        /* not found */
        prev = tmp;
        tmp = tmp->next;
    }
    return NULL;
}
