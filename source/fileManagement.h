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
