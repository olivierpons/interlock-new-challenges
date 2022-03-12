//
// Created by Olivier on 12/03/2022.
//

#ifndef INTERLOCK_BLOCKS_H
#define INTERLOCK_BLOCKS_H

#include "custom_types.h"
#include "block.h"

/**
 * FAM: Flexible Array Members in a structure in C:
 * https://www.geeksforgeeks.org/flexible-array-members-structure-c/
 */
typedef struct Blocks {
    ubyte total;
    Block tab[];
} Blocks;

extern Blocks *blocks_create(Blocks *b, int total);

#endif //INTERLOCK_BLOCKS_H
