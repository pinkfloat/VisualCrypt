#include <stdio.h>
#include "memoryManagement.h"

/* Global */
xMemory *First = NULL;

/********************************************************************
* Function:     validateAllocation
*--------------------------------------------------------------------
* Description:  Print error messages, free all buffer allocated so
*               far and exit the programm if ptr is NULL.
********************************************************************/
static void validateAllocation(void* ptr)
{
    if (ptr == NULL)
    {
        fprintf(stderr, "ERR: allocate buffer\n");
        fprintf(stderr, "Free all buffer...\n");
        xfreeAll();
        fprintf(stderr, "Exit programm...\n");
        exit(-1);
    }
}

/********************************************************************
* Function:     appendOnMemList
*--------------------------------------------------------------------
* Description:  Append the xMemory element "ptr" to the start of the
*               global memory list, which holds pointer to all
*               allocated memory of the programm.
********************************************************************/
static void appendOnMemList(xMemory* ptr)
{
    ptr->next = First;
    First = ptr;
}

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
void* xmalloc(size_t size)
{
    xMemory* newMem = malloc(sizeof(xMemory));
    validateAllocation(newMem);
    newMem->data = malloc(size);
    validateAllocation(newMem->data);
    appendOnMemList(newMem);
    return newMem->data;
}

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
void* xcalloc(size_t nitems, size_t size)
{
    xMemory* newMem = malloc(sizeof(xMemory));
    validateAllocation(newMem);
    newMem->data = calloc(nitems, size);
    validateAllocation(newMem->data);
    appendOnMemList(newMem);
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
void xfree(void* ptr)
{
    xMemory *prev, *tmp;

    /* check if element exist */
    if (ptr == NULL)
        return;

    /* case 1: list empty */
    if (First == NULL)
        return;

    /* case 2: remove list start */
    if(First->data == ptr)
    {
        tmp = First->next;
        free(ptr);
        free(First);
        First = tmp;
        return;
    }

    /* case 3: search for the element to remove */
    prev = First;
    tmp = First->next;
    while (tmp)
    {
        /* found */
        if(tmp->data == ptr)
        {
            /* remove element from list */
            prev->next = tmp->next;
            
            free(ptr);
            free(tmp);
            return;
        }
        /* not found */
        prev = tmp;
        tmp = tmp->next;
    }
}

/********************************************************************
* Function:     xfreeAll
*--------------------------------------------------------------------
* Description:  Frees the buffer from every element that was
*               allocated with xmalloc or xcalloc.
********************************************************************/
void xfreeAll()
{
    xMemory *index = First, *tmp;

    while(index)
    {
      tmp = index;
      index = index->next;
      xfree(tmp->data);
    }
}

/********************************************************************
* Function:     xcustomExitOnFailure
*--------------------------------------------------------------------
* Description:  Exit the programm with the exit message "message".
*               It will free all allocates buffer by calling xfreeAll.
********************************************************************/
void xcustomExitOnFailure(const char *message)
{
    xfreeAll();
    fprintf(stderr, "%s\n", message);
    exit(-1);
}
