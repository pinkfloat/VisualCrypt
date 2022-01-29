#include <stdio.h>
#include <stdlib.h>
#include "setsNsubsets.h"

static SubSet createSubSet(uint8_t length)
{
    SubSet result;
    result.data = malloc(sizeof(uint8_t)*length);
    result.length = length;
    return result;
}

static void fillSubSet(SubSet set, uint8_t* data)
{
    for (int i = 0; i < set.length; i++)
    {
        set.data[i] = data[i];
    }
}

static inline void deleteSubSet(SubSet* subset)
{
    free(subset->data);
}

/* the ! math-operation */
static inline uint16_t factorial(uint8_t num)
{
    uint16_t fact = 1;
    for(uint8_t i = 1; i <= num; ++i)
        fact *= i;
    return fact;
}

/* n = number of set elements, r = number of elements in a subset */
static inline uint8_t numOfSubsetsOfLen(uint8_t n, uint8_t r)
{
    return factorial(n) / (factorial(r)*factorial(n-r));
}

static void fillInitialSubSet(uint8_t* set, int len)
{
    for (int i = 0, x = len - 1; i < len; i++)
    {
        set[i] = x--;
    }
}

static void incrementSubSet(uint8_t n, uint8_t len, uint8_t* set)
{
    int offset = 0;
    while (offset < len)
    {
        if (set[offset] < n - 1 - offset)
        {
            set[offset]++;
            for(int idx = offset-1; idx >= 0; idx--)
            {
                if(set[idx] != (n-1-idx))
                    set[idx] = set[idx+1]+1;
            }
            return;
        }
        offset++;
    }
}

/* n = number of set elements */
static void createAllSubSets(Set* set, uint8_t n)
{
    SubSet* pOdd = set->odd;
    SubSet* pEven = set->even;
    uint8_t numSubsets;

    for(uint8_t len = 1; len <= n; len++)
    {
        numSubsets = numOfSubsetsOfLen(n, len);

        uint8_t* tmpSet = malloc(len);
        fillInitialSubSet(tmpSet, len);

        if(len % 2){ /* odd */
            for(uint8_t idx = 0; idx < numSubsets; idx++)
            {
                /* create subset of length len */
                *pOdd = createSubSet(len);
                fillSubSet(*pOdd, tmpSet);

                incrementSubSet(n, len, tmpSet);
                pOdd++;
            }
        }
        else{ /* even */
            for(uint8_t idx = 0; idx < numSubsets; idx++)
            {
                /* create subset of length len */
                *pEven = createSubSet(len);
                fillSubSet(*pEven, tmpSet);

                incrementSubSet(n, len, tmpSet);
                pEven++;
            }
        }
        free(tmpSet);
    }
}

static void debugPrintAllSubsets(Set* set, uint8_t n)
{
    /* print even subsets */
    for(uint8_t i = 0; i < set->num_subsets; i++)
    {
        fprintf(stdout, "u%d = {", i);
        for(uint8_t j = 0; j < set->even[i].length; j++)
        {
            fprintf(stdout, " %d", set->even[i].data[j]);
        }
        fprintf(stdout, " }\n");
    }
    fprintf(stdout, "\n");

    /* print odd subsets */
    for(uint8_t i = 0; i < set->num_subsets; i++)
    {
        fprintf(stdout, "v%d = {", i);
        for(uint8_t j = 0; j < set->odd[i].length; j++)
        {
            fprintf(stdout, " %d", set->odd[i].data[j]);
        }
        fprintf(stdout, " }\n");
    }
    fprintf(stdout, "\n");
}

Set createSet(uint8_t n, uint8_t m)
{
    Set result;
    result.even = calloc(sizeof(SubSet),m);
    result.odd = calloc(sizeof(SubSet),m);
    result.num_subsets = m;
    
    createAllSubSets(&result, n);

    /* debug print */
    debugPrintAllSubsets(&result, n);

    return result;
}

void deleteSet(Set* set)
{
    for (uint8_t i = 0; i < set->num_subsets; i++){
        deleteSubSet(set->even+i);
        deleteSubSet(set->odd+i);
    }
    free(set->even);
    free(set->odd);
} 
