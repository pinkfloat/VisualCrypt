#ifndef HANDLEBMP_H
#define HANDLEBMP_H
 
#include "image.h"

/********************************************************************
* Function:     createBMP
*--------------------------------------------------------------------
* Description:  The function createBMP will create a black and white
*               bmp file from the pure pixel data of a boolean array,
*               stored in image->array. It will fill the empty bmp 
*               file opened in image->file with a valid bmp header
*               and the array contents, restructured as rgb-values.
* Return:       0 on success, -1 on failure.
********************************************************************/
int createBMP(Image* image);

/********************************************************************
* Function:     readBMP
*--------------------------------------------------------------------
* Description:  The function readBMP will read a colored bmp opened
*               in image->file and get the information: width, height
*               , and the pixel data from it, to store them into
*               the image structure "image".
* Info:         Allocates buffer for image->array without freeing it
*               on success.
* Return:       0 on success, -1 on failure.
********************************************************************/
int readBMP(Image* image);

#endif /* HANDLEBMP_H */