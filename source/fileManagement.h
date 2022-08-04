#ifndef FILE_MANAGEMENT_H
#define FILE_MANAGEMENT_H

#include <stdio.h>

#include "dataManagement.h"

extern List *fileList;

/*********************************************************************
 * Function:     xfopen
 *--------------------------------------------------------------------
 * Description:  Calls fopen, but also creates a list entry for the
 *               global file list of the program, where pointer to
 *               all opened files are stored.
 *               This will make a clean abort, if fopen() fails.
 * Input:        filename − name of the file to be opened,
 *               mode − The file access mode (r/w/a)
 * Return:       pointer to the opened file
 ********************************************************************/
FILE *xfopen(const char *filename, const char *mode);

/*********************************************************************
 * Function:     xfread
 *--------------------------------------------------------------------
 * Description:  Calls fread to read from a file, but aborts the
 *               program, if the amount of characters, that were read
 *               aren't correct.
 * Input:        ptr = block of memory with a minimum size of
 *                     size*nmemb bytes,
 *               size = size in bytes of each element,
 *               nmemb = number of elements of "size",
 *               stream = pointer to an opened FILE input stream,
 *               errMessage = message printed on failure
 ********************************************************************/
static inline void xfread(void *ptr, size_t size, size_t nmemb, FILE *stream, const char *errMessage) {
    if (fread(ptr, size, nmemb, stream) != nmemb) {
        customExitOnFailure(errMessage);
    }
}

/*********************************************************************
 * Function:     xfwrite
 *--------------------------------------------------------------------
 * Description:  Calls fwrite to write to a file, but aborts the
 *               program, if the amount of characters, that were wrote
 *               aren't correct.
 * Input:        ptr = block of memory with a minimum size of
 *                     size*nmemb bytes,
 *               size = size in bytes of each element to write,
 *               nmemb = number of elements of "size",
 *               errMessage = message printet on failure
 * Output:       stream = pointer to an opened FILE output stream
 ********************************************************************/
static inline void xfwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream, const char *errMessage) {
    if (fwrite(ptr, size, nmemb, stream) != nmemb) {
        customExitOnFailure(errMessage);
    }
}

/*********************************************************************
 * Function:     xfclose
 *--------------------------------------------------------------------
 * Description:  Calls fclose, but also removes the opened file
 *               from the global file list of the program.
 * Input:        stream − the FILE object to be closed.
 * Return:       Zero if the stream is successfully closed,
 *               EOF or -1 on failure.
 ********************************************************************/
int xfclose(FILE *stream);

/*********************************************************************
 * Function:     xcloseAll
 *--------------------------------------------------------------------
 * Description:  Close every file which was opened with xfopen.
 ********************************************************************/
void xcloseAll();

#endif /* FILE_MANAGEMENT_H */
