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
    uint8_t sub_num;
} Set;

Set createSet(uint8_t num_of_subsets);
void deleteSet(Set* set);

#endif /* SETSNSUBSETS_H */ 
