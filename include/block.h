//
// Created by Olivier on 12/03/2022.
//

#ifndef INTERLOCK_BLOCK_H
#define INTERLOCK_BLOCK_H

#include "custom_types.h"
#include "piece.h"

typedef struct Block {
    ubyte total;
    Piece pieces[];
} Block;

extern Block *block_create(Block *b, uint nb_pieces);;
extern void block_free(Block **block);

#endif //INTERLOCK_BLOCK_H
