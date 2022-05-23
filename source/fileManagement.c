#include <stdlib.h>
#include "fileManagement.h"

// Global
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
