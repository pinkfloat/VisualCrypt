#ifndef DETERMINISTICSHARES_H
#define DETERMINISTICSHARES_H

#include "image.h"
#include "booleanMatrix.h"

/********************************************************************
* Function:     mallocDeterministicShareArrays
*--------------------------------------------------------------------
* Description:  An image is in fact a pixel array printed to a BMP
*               file. The pixels are stored as array values with
*               "1" being black and "0" being white. This function
*               will allocate the buffer of the pixel arrays, stored
*               in Image structs, for all of the shares, that will
*               be printed to BMPs later.
* Input:        source = containing width and height of the source
*               image,
*               n = number of shares,
*               m = number of pixels in a share per pixel in source,
* Output:       share->array will be correctly allocated for each
*               share.
* Return:       0 on success, -1 on failure.
********************************************************************/
int mallocDeterministicShareArrays(Image* source, Image* share, int n, int m);

/********************************************************************
* Function:     fillDeterministicShareArrays
*--------------------------------------------------------------------
* Description:  This function will fill the pixel arrays of the
*               shares with the values "1" being black and "0" being
*               white. Therefore the already calculated basis
*               matrices B0 and B1 are used. The columns of the
*               basis matrices will be randomly permutated and each
*               row of the resulting permutations will be re-sorted,
*               randomly assigned and copied to one of the shares.
* Return:       0 on success, -1 on failure.
********************************************************************/
int fillDeterministicShareArrays(Image* source, Image* share, BooleanMatrix* B0, BooleanMatrix* B1);

#endif /* DETERMINISTICSHARES_H */
