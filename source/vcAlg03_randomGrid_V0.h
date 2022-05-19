#ifndef RANDOM_GRID_ALGORITHMS_V0_H
#define RANDOM_GRID_ALGORITHMS_V0_H

#include "vcAlg03_randomGrid.h"

/********************************************************************
* Function:     randomGrid_nn_Threshold
*--------------------------------------------------------------------
* Description:  This is an implementation of a (n,n)-threshold random
*               grid algorithm introduced by Tzung-Her Chen and
*               Kai-Hsiang Tsao. It will calculate the pixel of the
*               share images by calling recursively the (2,2)-threshold
*               random grid algorithm from O. Kafri and E. Karen.
********************************************************************/
void randomGrid_nn_Threshold(Pixel* sourceArray, Image* shares, Pixel** storage, FILE* urandom, int arraySize, int numberOfShares);

/********************************************************************
* Function:     randomGrid_2n_Threshold
*--------------------------------------------------------------------
* Description:  This is an implementation of a (2,n)-threshold random
*               grid algorithm introduced by Tzung-Her Chen and
*               Kai-Hsiang Tsao. In contradistinction to the (n,n)
*               algorithms this algorithm reveals the secret image
*               as soon as two of the shares are stacked together,
*               independent from the amount of shares existing.
*               If more than two shares are stacked, the revealed
*               image becomes darker.
********************************************************************/
void randomGrid_2n_Threshold(Pixel* sourceArray, Image* shares, FILE* urandom, int arraySize, int numberOfShares);

/********************************************************************
* Function:     __randomGrid_kn_Threshold
*--------------------------------------------------------------------
* Description:  This is an implementation of a (k,n)-threshold random
*               grid algorithm introduced by Tzung-Her Chen and
*               Kai-Hsiang Tsao. In contradistinction to the (n,n)
*               algorithms this algorithm reveals the secret image
*               as soon as <k> of the shares are stacked together,
*               independent from the amount of shares existing.
*               If more than <k> shares are stacked, the noise
*               decreases the image quality.
********************************************************************/
void __randomGrid_kn_Threshold(kn_randomGridData* data);

/********************************************************************
* Function:     randomGrid_kn_Threshold
*--------------------------------------------------------------------
* Description:  This is a wrapper for the (k,n)-threshold random
*               grid algorithm introduced by Tzung-Her Chen and
*               Kai-Hsiang Tsao.
********************************************************************/
void randomGrid_kn_Threshold(Image* source, Image* shares, Pixel** storage, FILE* urandom, int arraySize, int n);

#endif /* #ifndef RANDOM_GRID_ALGORITHMS_V0_H */
