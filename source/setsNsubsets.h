#ifndef SETSNSUBSETS_H
#define SETSNSUBSETS_H

#include <stdint.h>

typedef struct {
    uint8_t* data;
    uint8_t length;
} SubSet;

typedef struct {
    SubSet* even;
    SubSet* odd;
    uint8_t numSubsets; /* number of subsets behind "even" or "odd" */
} Set;

Set createSet(uint8_t n, uint8_t m);
void deleteSet(Set* set);
void printAllSubsets(Set* set);

#endif /* SETSNSUBSETS_H */ 
