#ifndef RANDOM_GRID_ALGORITHMS_V1_H
#define RANDOM_GRID_ALGORITHMS_V1_H

#include "vcAlg03_randomGrid.h"

/********************************************************************
* Function:     alternate_nn_ThresholdRGA
*--------------------------------------------------------------------
* Description:  This alternate variant of the (n,n) random grid
*               algorithm by Tzung-Her Chen and Kai-Hsiang Tsao
*               calculates the contentes of all shares pixel by pixel,
*               instead of filling the shares one after another.
********************************************************************/
void alternate_nn_ThresholdRGA(Pixel* sourceArray, Image* shares, Pixel* tmpSharePixel, FILE* urandom, int arraySize, int numberOfShares);

/********************************************************************
* Function:     alternate_2n_ThresholdRGA
*--------------------------------------------------------------------
* Description:  This alternate variant of the (2,n) random grid
*               algorithm by Tzung-Her Chen and Kai-Hsiang Tsao
*               calculates the contentes of all shares pixel by pixel,
*               instead of filling the shares one after another.
********************************************************************/
void alternate_2n_ThresholdRGA(Pixel* sourceArray, Image* shares, FILE* urandom, int arraySize, int numberOfShares);

/********************************************************************
* Function:     __alternate_kn_ThresholdRGA
*--------------------------------------------------------------------
* Description:  This alternate variant of the (k,n) random grid
*               algorithm by Tzung-Her Chen and Kai-Hsiang Tsao
*               calculates the contentes of all shares pixel by pixel,
*               instead of filling the shares one after another.
********************************************************************/
void __alternate_kn_ThresholdRGA(kn_randomGridData* data);

/********************************************************************
* Function:     alternate_kn_ThresholdRGA
*--------------------------------------------------------------------
* Description:  This is a wrapper for the alternate (k,n)-threshold
*               random grid algorithm introduced by Tzung-Her Chen
*               and Kai-Hsiang Tsao.
********************************************************************/
void alternate_kn_ThresholdRGA(Image* source, Image* shares, FILE* urandom, int arraySize, int n);

#endif /* #ifndef RANDOM_GRID_ALGORITHMS_V1_H */
