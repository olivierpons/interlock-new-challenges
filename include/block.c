//
// Created by Olivier on 12/03/2022.
//
#include <malloc.h>
#include "block.h"

Block *block_create(Block *b, uint nb_pieces) {
    Block *block = malloc(sizeof(*b) + sizeof(Piece[nb_pieces]));
    if (block) {
        b->total = nb_pieces;
    }
    return block;
}

void block_free(Block **block) {
    free(*block);
    *block = NULL;
}
