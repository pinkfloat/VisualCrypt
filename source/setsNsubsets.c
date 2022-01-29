//#include <stdio.h>
#include <stdlib.h>
#include "setsNsubsets.h"

static SubSet createSubSet(uint8_t length)
{
    SubSet result;
    result.data = malloc(sizeof(uint8_t)*length);
    result.length = length;
    //fprintf(stdout, "created subset\n");
    return result;
}

static inline void deleteSubSet(SubSet* subset)
{
    free(subset->data);
    //fprintf(stdout, "deleted subset\n");
}

Set createSet(uint8_t num_of_subsets)
{
    Set result;
    result.even = malloc(sizeof(SubSet)*num_of_subsets);
    result.odd = malloc(sizeof(SubSet)*num_of_subsets);
    
    for(uint8_t i = 0; i < num_of_subsets; i++){
        *(result.even+i) = createSubSet(2); /* dummy value */
        *(result.odd+i) = createSubSet(2);
    }

    result.sub_num = num_of_subsets;
    //fprintf(stdout, "created set\n");
    return result;
}

void deleteSet(Set* set)
{
    for (uint8_t i = 0; i < set->sub_num; i++){
        deleteSubSet(set->even+i);
        deleteSubSet(set->odd+i);
    }
    free(set->even);
    free(set->odd);
    //fprintf(stdout, "deleted set\n");
} 
