#ifndef DETERMINISTICSHARES_H
#define DETERMINISTICSHARES_H

#include "image.h"
#include "booleanMatrix.h"

int mallocDeterministicShareArrays(Image* source, Image* share, int n, int m);
int fillDeterministicShareArrays(Image* source, Image* share, BooleanMatrix* B0, BooleanMatrix* B1);

#endif /* DETERMINISTICSHARES_H */
