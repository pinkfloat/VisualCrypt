#include <stdio.h>
#include <stdlib.h>
#include "setsNsubsets.h"

static SubSet createSubSet(uint8_t length)
{
    SubSet result;
    result.length = length;
    result.data = malloc(sizeof(uint8_t)*length);
    if(!result.data)
        fprintf(stderr, "ERR: allocate buffer for SubSet\n");
    return result;
}

static inline void deleteSubSet(SubSet* subset)
{
    free(subset->data);
}

/*  the ! math-operation 
    i.e. 4! = 4 * 3 * 2 * 1 = 24 
*/
static inline uint16_t factorial(uint8_t num)
{
    uint16_t fact = 1;
    for(uint8_t i = 1; i <= num; ++i)
        fact *= i;
    return fact;
}

/*  calculate number of (even or odd)
    subsets that will have the length r
    n = number of set elements,
    r = number of elements in a subset
*/
static inline uint8_t numOfSubsetsOfLen(uint8_t n, uint8_t r)
{
    return factorial(n) / (factorial(r)*factorial(n-r));
}

/*  fill subset from len-1 to 0, i.e.:
    len = 4 -> {3,2,1,0}
    len = 3 -> {2,1,0}
*/
static void fillInitialSubSet(uint8_t* set, int len)
{
    for (int i = 0, x = len - 1; i < len; i++)
        set[i] = x--;
}

static void copyDataInSet(SubSet set, uint8_t* data)
{
    for (int i = 0; i < set.length; i++)
        set.data[i] = data[i];
}

static void incrementSubSet(uint8_t n, uint8_t len, uint8_t* set)
{
    for(int offset = 0; offset < len; offset++)
    {
        /*  count element up if max value of the
            element is not reached i.e. 
            for 3 out of 5:
            {2,1,0} -> {3,1,0}
            ( the max value would be {4,3,2} )
        */
        if (set[offset] < n - 1 - offset)
        {
            set[offset]++;
            for(int idx = offset-1; idx >= 0; idx--)
            {
                /*  count element up when it has reached
                    its max and set every left from it to
                    (the same value (+ 1)*step next)
                    i.e. 3 ouf of 5, where the max was {4,3,2}
                    {4,3,0} -> {3,2,1}
                    the right is counted up and the other
                    ones left from it are counted one higher
                    than the right one each
                */
                set[idx] = set[idx+1]+1;
            }
            return;
        }
    }
}

static int fillSubSet(SubSet* set, uint8_t* tmpSet, uint8_t n, uint8_t len, uint8_t numSubsets)
{
    for(uint8_t idx = 0; idx < numSubsets; idx++)
    {
        /* allocate subset of correct length */
        *set = createSubSet(len);
        if(!set->data)
            return -1;

        /* save data from tmpSet in actual set */
        copyDataInSet(*set, tmpSet);

        /* fill data for the next loop in tmpSet */
        incrementSubSet(n, len, tmpSet);
        set++;
    }
    return 0;
}

/* n = number of set elements */
static int createAllSubSets(Set* set, uint8_t n)
{
    uint8_t numSubsets;
    int err;

    for(uint8_t len = 1; len <= n; len++)
    {
        numSubsets = numOfSubsetsOfLen(n, len);

        uint8_t* tmpSet = malloc(len);
        /*  fill the first subset with the lowest possible 
            value combination for len i.e. for 3 out of 5,
            where len is 3 and the set is {0,1,2,3,4}, the
            initial set would be: {0,1,2}, and since the 
            sets are filled reverted: {2,1,0}
        */
        if(!tmpSet){
            fprintf(stderr, "ERR: allocate buffer for temporary SubSet\n");
            return -1;
        }
        fillInitialSubSet(tmpSet, len);

        if(len % 2){ /* odd */
            err = fillSubSet(set->odd, tmpSet, n, len, numSubsets);
        }
        else{ /* even */
            err = fillSubSet(set->even, tmpSet, n, len, numSubsets);
        }

        free(tmpSet);
        if(err)
            return err;
    }
    return 0;
}

Set createSet(uint8_t n, uint8_t m)
{
    Set result;
    int err;
    /*  info: calloc is used to append a
        NULL-set to the even sets (that
        is needed for odd n's), by just
        not-initilaising the memory
    */
    result.even = calloc(sizeof(SubSet),m);
    result.odd = calloc(sizeof(SubSet),m);
    result.num_subsets = m;

    if(!result.even || !result.odd)
    {
        if (!result.odd)
            free(result.even);
        fprintf(stderr, "ERR: allocate buffer for Set\n");
        return result;
    }
    
    err = createAllSubSets(&result, n);
    if(err)
    {
        /* free everything allocated so far */
        deleteSet(&result);

        /* cause the programm to exit */
        result.even = NULL;
    }

    return result;
}

void deleteSet(Set* set)
{
    for (uint8_t i = 0; i < set->num_subsets; i++)
    {
        if(set->even+i)
            deleteSubSet(set->even+i);
        if(set->odd+i)
            deleteSubSet(set->odd+i);
    }
    free(set->even);
    free(set->odd);
} 

static void printSubset(SubSet* set, uint8_t num_sets, char sign)
{
    for(uint8_t i = 0; i < num_sets; i++)
    {
        fprintf(stdout, "%c%d = {", sign, i);
        for(uint8_t j = 0; j < set[i].length; j++)
        {
            fprintf(stdout, " %d", set[i].data[j]);
        }
        fprintf(stdout, " }\n");
    }
    fprintf(stdout, "\n");
}

void debugPrintAllSubsets(Set* set)
{
    /* print even subsets */
    printSubset(set->even, set->num_subsets, 'u');

    /* print odd subsets */
    printSubset(set->odd, set->num_subsets, 'v');
}
