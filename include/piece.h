//
// Created by Olivier on 12/03/2022.
//

#ifndef INTERLOCK_PIECE_H
#define INTERLOCK_PIECE_H

#include "custom_types.h"
#include "part.h"

/**
 * FAM: Flexible Array Members in a structure in C:
 * https://www.geeksforgeeks.org/flexible-array-members-structure-c/
 */
typedef struct Piece {
    ubyte total;
    Part tab[];
} Piece;

extern Piece *piece_create(int total);

#endif //INTERLOCK_PIECE_H