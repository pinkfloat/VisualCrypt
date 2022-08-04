#ifndef RANDOM_GRID_ALGORITHMS_V1_H
#define RANDOM_GRID_ALGORITHMS_V1_H

#include "vcAlg03_randomGrid.h"

/*********************************************************************
 * Function:     alternate_nn_RGA
 *--------------------------------------------------------------------
 * Description:  This alternate variant of the (n,n) random grid
 *               algorithm by Tzung-Her Chen and Kai-Hsiang Tsao
 *               calculates the contentes of all shares pixel by pixel,
 *               instead of filling the shares one after another.
 ********************************************************************/
void alternate_nn_RGA(Pixel *sourceArray, Image *shares, Pixel *tmpSharePixel, FILE *randomSrc, int arraySize,
                      int numberOfShares);

/*********************************************************************
 * Function:     alternate_2n_RGA
 *--------------------------------------------------------------------
 * Description:  This alternate variant of the (2,n) random grid
 *               algorithm by Tzung-Her Chen and Kai-Hsiang Tsao,
 *               calculates the contentes of all shares pixel by pixel,
 *               instead of filling the shares one after another.
 ********************************************************************/
void alternate_2n_RGA(Pixel *sourceArray, Image *shares, FILE *randomSrc, int arraySize, int numberOfShares);

/*********************************************************************
 * Function:     __alternate_kn_RGA
 *--------------------------------------------------------------------
 * Description:  This alternate variant of the (k,n) random grid
 *               algorithm by Tzung-Her Chen and Kai-Hsiang Tsao
 *               calculates the contentes of all shares pixel by pixel,
 *               instead of filling the shares one after another.
 ********************************************************************/
void __alternate_kn_RGA(int *setOfN, Pixel *sourceArray, Pixel *sharePixel, Image *shares, FILE *randomSrc,
                        int arraySize, int n, int k);

/*********************************************************************
 * Function:     alternate_kn_RGA
 *--------------------------------------------------------------------
 * Description:  This is a wrapper for the alternate (k,n)
 *               random grid algorithm introduced by Tzung-Her Chen
 *               and Kai-Hsiang Tsao.
 ********************************************************************/
void alternate_kn_RGA(Image *source, Image *shares, FILE *randomSrc, int arraySize, int n);

#endif /* RANDOM_GRID_ALGORITHMS_V1_H */
