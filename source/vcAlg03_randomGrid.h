#ifndef RANDOM_GRID_ALGORITHMS_H
#define RANDOM_GRID_ALGORITHMS_H

#include "random.h"
#include "vcAlgorithms.h"

/********************************************************************
 * Function:     writePixelToShares
 *--------------------------------------------------------------------
 * Description:  If the share number is part of the first k elements
 *               of the random sorted set (of size n), the very share
 *               will get the pixel, which was calculated for one of
 *               the shares, from the source image, before.
 *               Shares with a number not contained in the first
 *               k elements will get randomly a 0/1.
 ********************************************************************/
void writePixelToShares(int *randSortedSetOfN, void *source, Image *shares, FILE *randomSrc, int n, int k, int i,
                        Pixel (*getPixel)(void *, int, int));

/********************************************************************
 * Function:     callRandomGridAlgorithm
 *--------------------------------------------------------------------
 * Description:  Prepares data which is needed by all, or at least
 *               multiple, random grid algorithms. It'll call the
 *               chosen algorithm with the data.
 ********************************************************************/
void callRandomGridAlgorithm(AlgorithmData *data);

#endif /* RANDOM_GRID_ALGORITHMS_H */
