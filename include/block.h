//
// © Olivier Pons / HQF Development - 12/03/2022.
//

#ifndef INTERLOCK_BLOCK_H
#ifndef INTERLOCK_BLOCK_H
#define INTERLOCK_BLOCK_H

#include "custom_types.h"
#include "part.h"

/**
 * FAM: Flexible Array Members in a structure in C:
 * https://www.geeksforgeeks.org/flexible-array-members-structure-c/
 */
typedef struct Block {
    ubyte total;
    Part parts[];
} Block;

#define TO_INT(a,b,c,d) (int)(a), (int)(b), (int)(c), (int)(d)

extern Block *blockCreate(int total);
extern Block *blockCreateWithParts(int total, ... );

#endif //INTERLOCK_BLOCK_H
