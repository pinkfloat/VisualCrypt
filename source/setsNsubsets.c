#include <stdio.h>
#include "memoryManagement.h"
#include "setsNsubsets.h"

/********************************************************************
* Function:     createSubSet
*--------------------------------------------------------------------
* Description:  Allocates buffer for data of a SubSet of size
*               "length".
* Return:       The created SubSet.
********************************************************************/
static SubSet createSubSet(uint8_t length)
{
    SubSet result;
    result.length = length;
    result.data = xmalloc(sizeof(uint8_t)*length);
    return result;
}

/********************************************************************
* Function:     factorial
*--------------------------------------------------------------------
* Description:  Performs the ! math-operation for "num".
*               i.e.: 4! = 4 * 3 * 2 * 1 = 24 
* Return:       The result of num!
********************************************************************/
static inline uint16_t factorial(uint8_t num)
{
    uint16_t fact = 1;
    for(uint8_t i = 1; i <= num; ++i)
        fact *= i;
    return fact;
}

/********************************************************************
* Function:     numOfSubsetsOfLen
*--------------------------------------------------------------------
* Description:  Calculates the number of subsets of a set with "n"
*               elements, that will have the length "r".
*               i.e.:
*               given:    set = {0, 1, 2} -> n=3
*               searched: how many subsets of length 2 exist? -> r=2
*               There would be: {0,1}, {0,2} and {1,2}
*               -> The result of this function is 3
* Input:        n = number of set elements,
*               r = number of elements in a subset
* Return:       number of subsets of length "r".
********************************************************************/
static inline uint8_t numOfSubsetsOfLen(uint8_t n, uint8_t r)
{
    return factorial(n) / (factorial(r)*factorial(n-r));
}

/********************************************************************
* Function:     fillInitialSubSet
*--------------------------------------------------------------------
* Description:  Fills the data of a SubSet from length-1 to 0.
*               i.e.:
*               length = 4 -> {3,2,1,0}
*               length = 3 -> {2,1,0}
********************************************************************/
static inline void fillInitialSubSet(uint8_t* data, int length)
{
    for (int i = 0, x = length - 1; i < length; i++)
        data[i] = x--;
}

/********************************************************************
* Function:     copyDataInSet
*--------------------------------------------------------------------
* Description:  Copies an integer stream to the data of a SubSet.
********************************************************************/
static inline void copyDataInSet(SubSet set, uint8_t* data)
{
    for (int i = 0; i < set.length; i++)
        set.data[i] = data[i];
}

/********************************************************************
* Function:     incrementSubSet
*--------------------------------------------------------------------
* Description:  In order to find all subsets of a specific length of
*               a set, this function will calculate the "next" subset
*               that would follow after the one, given to this
*               function.
*               i.e.: for 3 out of 5, this function - called in a
*               loop - will fill the SubSet data "set" with:
*               start: {2,1,0} -> result: {3,1,0}
*               start: {3,1,0} -> result: {4,1,0}
*               start: {4,1,0} -> result: {3,2,1}
*               start: {3,2,1} -> result: {4,2,1}
*               and so on.
* Input:        n = number of set elements (in the example above its 5)
*               len = length of the subset (in the example above its 3)
* In/Out:       set = a subset filled with values (start) for which
*               this function will calculate the next values (result)
********************************************************************/
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

/********************************************************************
* Function:     fillSubSet
*--------------------------------------------------------------------
* Description:  This function uses the integer stream "tmpSet"
*               to temporarily store the results of the function
*               incrementSubSet() and saves them permanently
*               in the newly allocated SubSets "set".
* Input:        n = number of set elements,
*               len = length of the subsets
*               numSubsets = number of subsets of length len
* Output:       set = pointer to the next address of a SubSet that
*               will be created and filled with data.
* In/Out:       tmpSet = subset values getting updated by
*               incrementSubSet()
********************************************************************/
static void fillSubSet(SubSet** set, uint8_t* tmpSet, uint8_t n, uint8_t len, uint8_t numSubsets)
{
    for(uint8_t idx = 0; idx < numSubsets; idx++)
    {
        /* allocate subset of correct length */
        **set = createSubSet(len);

        /* save data from tmpSet in actual set */
        copyDataInSet(**set, tmpSet);

        /* fill data for the next loop in tmpSet */
        incrementSubSet(n, len, tmpSet);
        (*set)++;
    }
}

/********************************************************************
* Function:     createAllSubSets
*--------------------------------------------------------------------
* Description:  Calculates all subsets of a set with n elements and
*               sorts them in the SubSet arrays of the Set structure,
*               after having even or odd cardinality.
********************************************************************/
static void createAllSubSets(Set* set)
{
    uint8_t n = set->numSetElements;
    SubSet* pOdd = set->odd;
    SubSet* pEven = set->even;
    uint8_t numSubsets;

    for(uint8_t len = 1; len <= n; len++)
    {
        numSubsets = numOfSubsetsOfLen(n, len);

        uint8_t* tmpSet = xmalloc(len);
        fillInitialSubSet(tmpSet, len);

        if(len % 2){ /* odd */
            fillSubSet(&pOdd, tmpSet, n, len, numSubsets);
        }
        else{ /* even */
            fillSubSet(&pEven, tmpSet, n, len, numSubsets);
        }

        xfree(tmpSet);
    }
}

/********************************************************************
* Function:     createSet
*--------------------------------------------------------------------
* Description:  A Set is (in this programm) considered a structure
*               that is holding all Subsets of a Set with n elements,
*               sorted in arrays of SubSets after having even or odd
*               cardinality. This function will create one of them.
* Input:        n = number of set elements,
*               m = number of subsets having respectively even or
*               odd cardinality.
* Return:       The created Set.
********************************************************************/
Set createSet(uint8_t n, uint8_t m)
{
    Set result;
    /*  info: xcalloc is used to append a
        NULL-set to the even sets (that
        is needed for odd n's), by just
        not-initilaising the memory
    */
    result.numSetElements = n;
    result.even = xcalloc(sizeof(SubSet),m);
    result.odd = xcalloc(sizeof(SubSet),m);
    result.numSubsets = m;
    
    createAllSubSets(&result);
    return result;
}

/********************************************************************
* Function:     printSubsets
*--------------------------------------------------------------------
* Description:  Print the SubSets from the array of subsets "set".
*               Each will be printed behind the letter "sign" and a
*               distinct number to tell them apart.
********************************************************************/
static void printSubsets(SubSet* set, uint8_t num_sets, char sign)
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

/********************************************************************
* Function:     printAllSubsets
*--------------------------------------------------------------------
* Description:  Print all subsets of the set "set". The even subsets
*               will be printed behind the letter "u", while the odd
*               subsets are receiving the letter "v".
********************************************************************/
void printAllSubsets(Set* set)
{
    /* print even subsets */
    printSubsets(set->even, set->numSubsets, 'u');

    /* print odd subsets */
    printSubsets(set->odd, set->numSubsets, 'v');
}
