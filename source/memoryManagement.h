#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

#include <stdlib.h>

#include "dataManagement.h"

extern List *memList;

/*********************************************************************
 * Function:     xmalloc
 *--------------------------------------------------------------------
 * Description:  Calls malloc, but also creates a list entry for the
 *               global memory list.
 *               If the allocation fails, the program will free the
 *               allocated memory, close all opened files and abort.
 * Input:        size = size of the memory block in bytes
 * Return:       pointer to the new allocated memory
 ********************************************************************/
void *xmalloc(size_t size);

/*********************************************************************
 * Function:     xcalloc
 *--------------------------------------------------------------------
 * Description:  Calls calloc, but also creates a list entry for the
 *               global memory list.
 *               If the allocation fails, the program will free the
 *               allocated memory, close all opened files and abort.
 * Input:        nitems = number of elements to allocate,
 *               size = size of the elements in bytes
 * Return:       pointer to the new allocated memory
 ********************************************************************/
void *xcalloc(size_t nitems, size_t size);

/*********************************************************************
 * Function:     xfree
 *--------------------------------------------------------------------
 * Description:  Calls free, but also removes the allocated element
 *               from the global memory list.
 * Input:        ptr = pointer to a memory block previously allocated
 *               with xmalloc or xcalloc.
 ********************************************************************/
void xfree(void *ptr);

/*********************************************************************
 * Function:     xfreeAll
 *--------------------------------------------------------------------
 * Description:  Frees the buffer from every element that was
 *               allocated with xmalloc or xcalloc.
 ********************************************************************/
void xfreeAll();

#endif /* MEMORY_MANAGEMENT_H */
