#ifndef VCALGORITHMS_H
#define VCALGORITHMS_H

#include "image.h"

typedef struct {
    Image* source;
    Image* shares;
    uint8_t numberOfShares;
} AlgorithmData;

int callAlgorithm(int (*algorithm)(AlgorithmData*));

int deterministicAlgorithm(AlgorithmData* data);

#endif /* VCALGORITHMS_H */  
