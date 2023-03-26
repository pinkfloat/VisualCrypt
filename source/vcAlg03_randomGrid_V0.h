/*
*   Copyright: (c) 2023 Sabrina Otto. All rights reserved.
*   This work is licensed under the terms of the MIT license.
*/

#ifndef RANDOM_GRID_ALGORITHMS_V0_H
#define RANDOM_GRID_ALGORITHMS_V0_H

#include "vcAlg03_randomGrid.h"

/*********************************************************************
 * Function:     randomGrid_nn
 *--------------------------------------------------------------------
 * Description:  This is an implementation of a (n,n) random grid
 *               algorithm introduced by Tzung-Her Chen and Kai-Hsiang
 *               Tsao. It will calculate the pixel of the share images
 *               by calling recursively the (2,2) random grid
 *               algorithm from O. Kafri and E. Karen.
 ********************************************************************/
void randomGrid_nn(Pixel *sourceArray, Image *shares, Pixel **storage, FILE *randomSrc, int arraySize,
                   int numberOfShares);

/*********************************************************************
 * Function:     randomGrid_2n
 *--------------------------------------------------------------------
 * Description:  This is an implementation of a (2,n) random grid
 *               algorithm introduced by Tzung-Her Chen and Kai-Hsiang
 *               Tsao. In contradistinction to the (n,n) algorithms,
 *               this algorithm reveals the secret image as soon as
 *               two of the shares are stacked together, independent
 *               from the amount of shares existing.
 ********************************************************************/
void randomGrid_2n(Pixel *sourceArray, Image *shares, FILE *randomSrc, int arraySize, int numberOfShares);

/*********************************************************************
 * Function:     __randomGrid_kn
 *--------------------------------------------------------------------
 * Description:  This is an implementation of a (k,n) random grid
 *               algorithm introduced by Tzung-Her Chen and Kai-Hsiang
 *               Tsao. In contradistinction to the (n,n) algorithms,
 *               this algorithm reveals the secret image as soon as
 *               "k" of the shares are stacked together, independent
 *               from the amount of shares existing.
 ********************************************************************/
void __randomGrid_kn(int *setOfN, Image *shares, Image *tmpShares, FILE *randomSrc, int arraySize, int n, int k);

/*********************************************************************
 * Function:     randomGrid_kn
 *--------------------------------------------------------------------
 * Description:  This is a wrapper for the (k,n) random grid algorithm
 *               introduced by Tzung-Her Chen and Kai-Hsiang Tsao.
 ********************************************************************/
void randomGrid_kn(Image *source, Image *shares, Pixel **storage, FILE *randomSrc, int arraySize, int n);

#endif /* RANDOM_GRID_ALGORITHMS_V0_H */
