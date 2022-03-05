#include <stdlib.h>
#include "fileManagement.h"

/* Global */
List *fileList = NULL;

/********************************************************************
* Function:     xfopen
*--------------------------------------------------------------------
* Description:  Calls fopen, but also creates a list entry for the
*               global file list of the programm, where pointer to
*               all opened files are stored.
*               If the opening fails, the programm will close all
*               opened files, free all allocated memory and exit.
* Input:        filename − name of the file to be opened,
*               mode − The file access mode (r/w/a)
* Return:       pointer to the opened file
********************************************************************/
FILE* xfopen(const char *filename, const char *mode)
{
    List* newFile = malloc(sizeof(List));
    validatePointer(newFile, "ERR: allocate memory");
    newFile->data = fopen(filename, mode);
    validatePointer(newFile->data, "ERR: open file");
    appendOnList(newFile, &fileList);
    return newFile->data;
}

/********************************************************************
* Function:     xfread
*--------------------------------------------------------------------
* Description:  Calls fread to read from a file, but exit the
*               programm if the amount of characters that were read
*               aren't correct.
* Input:        ptr = block of memory with a minimum size of
*                     size*nmemb bytes.
*               size = size in bytes of each element to be read,
*               nmemb = number of elements of "size"
*               stream = pointer to an opened FILE input stream,
*               errMessage = message printet on failure
********************************************************************/
void xfread(void *ptr, size_t size, size_t nmemb, FILE *stream, const char *errMessage)
{
    if (fread(ptr, size, nmemb, stream) != nmemb)
        customExitOnFailure(errMessage);
}

/********************************************************************
* Function:     xfwrite
*--------------------------------------------------------------------
* Description:  Calls fwrite to write to a file, but exit the
*               programm if the amount of characters that were wrote
*               aren't correct.
* Input:        ptr = block of memory with a minimum size of
*                     size*nmemb bytes.
*               size = size in bytes of each element to write,
*               nmemb = number of elements of "size"
*               errMessage = message printet on failure
* Output:       stream = pointer to an opened FILE output stream,
********************************************************************/
void xfwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream, const char *errMessage)
{
    if (fwrite(ptr, size, nmemb, stream) != nmemb)
        customExitOnFailure(errMessage);
}

/********************************************************************
* Function:     xfclose
*--------------------------------------------------------------------
* Description:  Calls fclose, but also removes the opened file
*               from the global file list of the programm, where
*               pointer to all opened files are stored.
* Input:        stream − the FILE object to be closed.
* Return:       Zero if the stream is successfully closed.
*               On failure, EOF or -1 is returned.
********************************************************************/
int xfclose(FILE *stream)
{
    if (!stream)
        return -1;

    List* removed = removeFromList(stream, &fileList);
    if (!removed)
        customExitOnFailure("ERR: close element not in fileList");
        
    int ret = fclose(removed->data);
    free(removed);
    return ret;
}

/********************************************************************
* Function:     xcloseAll
*--------------------------------------------------------------------
* Description:  Close every file that was opened with xfopen.
********************************************************************/
void xcloseAll()
{
    List *index = fileList, *tmp;

    while(index)
    {
      tmp = index;
      index = index->next;
      xfclose(tmp->data);
    }
}
