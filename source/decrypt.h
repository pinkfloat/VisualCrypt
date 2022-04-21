#ifndef DECRYPT_H
#define DECRYPT_H

#include "image.h"

/********************************************************************
* Function:     decryptShareFiles
*--------------------------------------------------------------------
* Description:  Read share files in dirPath starting at the share
*               with the number given to first and end with the
*               number given to last. They will be stored in Image
*               structures that must have been allocated before.
********************************************************************/
void decryptShareFiles();

#endif /* DECRYPT_H */
