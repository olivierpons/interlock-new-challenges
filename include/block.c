//
// Created by Olivier on 12/03/2022.
//
#include <malloc.h>
#include <memory.h>
#include "block.h"

Block *block_create(Block *b, int total) {
    b = malloc(sizeof(*b) + sizeof(Piece[total]));
    if (b) {
        b->total = (ubyte)total;
        memset(b->tab, total, sizeof(Piece));
        b->tab[0].is_main = true;
    }
    return b;
}
