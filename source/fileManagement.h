#ifndef FILE_MANAGEMENT_H
#define FILE_MANAGEMENT_H

#include <stdio.h>
#include "dataManagement.h"

extern List *fileList;

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
FILE* xfopen(const char *filename, const char *mode);

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
void xfread(void *ptr, size_t size, size_t nmemb, FILE *stream, const char *errMessage);

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
void xfwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream, const char *errMessage);

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
int xfclose(FILE *stream);

/********************************************************************
* Function:     xcloseAll
*--------------------------------------------------------------------
* Description:  Close every file that was opened with xfopen.
********************************************************************/
void xcloseAll();

#endif /* FILE_MANAGEMENT_H */
