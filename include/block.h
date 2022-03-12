//
// Created by Olivier on 12/03/2022.
//

#ifndef INTERLOCK_BLOCK_H
#define INTERLOCK_BLOCK_H

#include "custom_types.h"
#include "piece.h"

/**
 * FAM: Flexible Array Members in a structure in C:
 * https://www.geeksforgeeks.org/flexible-array-members-structure-c/
 */
typedef struct Block {
    ubyte total;
    Piece tab[];
} Block;

extern Block *block_create(Block *b, int total);

#endif //INTERLOCK_BLOCK_H
