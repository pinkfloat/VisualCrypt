#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

#include <stdlib.h>

typedef struct xMem {
    void *data;
    struct xMem *next;
} xMemory;

extern xMemory *First;

/********************************************************************
* Function:     xmalloc
*--------------------------------------------------------------------
* Description:  Calls malloc, but also creates a list entry for the
*               global memory list of the programm, where pointer to
*               all allocated memory are stored.
*               If the allocation fails, the programm will free
*               all allocated memory and exit.
* Input:        size = size of the memory block in bytes
* Return:       pointer to the allocated memory
********************************************************************/
void* xmalloc(size_t size);

/********************************************************************
* Function:     xcalloc
*--------------------------------------------------------------------
* Description:  Calls calloc, but also creates a list entry for the
*               global memory list of the programm, where pointer to
*               all allocated memory are stored.
*               If the allocation fails, the programm will free
*               all allocated memory and exit.
* Input:        nitems = number of elements to allocate,
*               size = size of the elements in bytes
* Return:       pointer to the allocated memory
********************************************************************/
void* xcalloc(size_t nitems, size_t size);

/********************************************************************
* Function:     xfree
*--------------------------------------------------------------------
* Description:  Calls free, but also removes the allocated element
*               from the global memory list of the programm, where
*               pointer to all allocated memory are stored.
* Input:        ptr = pointer to a memory block previously allocated
*               with xmalloc or xcalloc.
********************************************************************/
void xfree(void* ptr);

/********************************************************************
* Function:     xfreeAll
*--------------------------------------------------------------------
* Description:  Frees the buffer from every element that was
*               allocated with xmalloc or xcalloc.
********************************************************************/
void xfreeAll();

/********************************************************************
* Function:     xcustomExitOnFailure
*--------------------------------------------------------------------
* Description:  Exit the programm with the exit message "message".
*               It will free all allocates buffer by calling xfreeAll.
********************************************************************/
void xcustomExitOnFailure(const char *message);

#endif /* MEMORY_MANAGEMENT_H */
