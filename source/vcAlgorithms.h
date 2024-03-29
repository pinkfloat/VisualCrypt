/*
*   Copyright: (c) 2023 Sabrina Otto. All rights reserved.
*   This work is licensed under the terms of the MIT license.
*/

#ifndef VCALGORITHMS_H
#define VCALGORITHMS_H

#include "image.h"

typedef struct {
    Image *source;
    Image *shares;
    int numberOfShares;
    int algorithmNumber;
    FILE *randomSrc;
} AlgorithmData;

/*********************************************************************
 * Function:     callAlgorithm
 *--------------------------------------------------------------------
 * Description:  The function callAlgorithm will extract the data of
 *               the source bmp, call the algorithm given to it as
 *               parameter, and draw all of the share bmps, after
 *               the algorithm is finished. It'll use the settings
 *               stored in "settings.h".
 ********************************************************************/
void callAlgorithm(void (*algorithm)(AlgorithmData *), int algorithmNumber);

/*********************************************************************
 * Function:     mallocSharesOfSourceSize
 *--------------------------------------------------------------------
 * Description:  This will allocate the buffer of the share pixel
 *               arrays, which are going to be printed to the BMPs
 *               later. The size will be the same as for the source
 *               file.
 * Input:        source = containing width and height of the source
 *               image,
 *               numberOfShares = amount of shares that will be
 *               created,
 * Output:       share->array will be correctly allocated for each
 *               share.
 ********************************************************************/
void mallocSharesOfSourceSize(Image *source, Image *share, int numberOfShares);

#endif /* VCALGORITHMS_H */
