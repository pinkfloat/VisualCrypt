#ifndef SETSNSUBSETS_H
#define SETSNSUBSETS_H

#include <stdint.h>

typedef struct {
    uint8_t* data;
    uint8_t length;
} SubSet;

typedef struct {
    uint8_t numSetElements;
    SubSet* even;
    SubSet* odd;
    uint8_t numSubsets; // number of subsets behind "even" or "odd"
} Set;

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
Set createSet(uint8_t n, uint8_t m);

/********************************************************************
* Function:     printAllSubsets
*--------------------------------------------------------------------
* Description:  Print all subsets of the set "set". The even subsets
*               will be printed behind the letter "u", while the odd
*               subsets are receiving the letter "v".
********************************************************************/
void printAllSubsets(Set* set);

#endif /* SETSNSUBSETS_H */ 
