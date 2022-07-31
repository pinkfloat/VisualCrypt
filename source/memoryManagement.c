#include "memoryManagement.h"

// Global
List *memList = NULL;

/********************************************************************
 * Function:     xmalloc
 *--------------------------------------------------------------------
 * Description:  Calls malloc, but also creates a list entry for the
 *               global memory list of the programm, where pointer to
 *               all allocated memory are stored.
 *               If the allocation fails, the programm will free
 *               all allocated memory, close all opened files and exit.
 * Input:        size = size of the memory block in bytes
 * Return:       pointer to the allocated memory
 ********************************************************************/
void *xmalloc(size_t size) {
    List *newMem = malloc(sizeof(List));
    validatePointer(newMem, "ERR: allocate memory");
    newMem->data = malloc(size);
    validatePointer(newMem->data, "ERR: allocate memory");
    appendOnList(newMem, &memList);
    return newMem->data;
}

/********************************************************************
 * Function:     xcalloc
 *--------------------------------------------------------------------
 * Description:  Calls calloc, but also creates a list entry for the
 *               global memory list of the programm, where pointer to
 *               all allocated memory are stored.
 *               If the allocation fails, the programm will free
 *               all allocated memory, close all opened files and exit.
 * Input:        nitems = number of elements to allocate,
 *               size = size of the elements in bytes
 * Return:       pointer to the allocated memory
 ********************************************************************/
void *xcalloc(size_t nitems, size_t size) {
    List *newMem = malloc(sizeof(List));
    validatePointer(newMem, "ERR: allocate memory");
    newMem->data = calloc(nitems, size);
    validatePointer(newMem->data, "ERR: allocate memory");
    appendOnList(newMem, &memList);
    return newMem->data;
}

/********************************************************************
 * Function:     xfree
 *--------------------------------------------------------------------
 * Description:  Calls free, but also removes the allocated element
 *               from the global memory list of the programm, where
 *               pointer to all allocated memory are stored.
 * Input:        ptr = pointer to a memory block previously allocated
 *               with xmalloc or xcalloc.
 ********************************************************************/
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

/********************************************************************
 * Function:     xfreeAll
 *--------------------------------------------------------------------
 * Description:  Frees the buffer from every element that was
 *               allocated with xmalloc or xcalloc.
 ********************************************************************/
void xfreeAll() {
    List *index = memList, *tmp;

    while (index) {
        tmp = index;
        index = index->next;
        xfree(tmp->data);
    }
}
