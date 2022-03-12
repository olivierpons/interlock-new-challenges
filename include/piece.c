//
// Created by Olivier on 12/03/2022.
//
#include <malloc.h>
#include <memory.h>
#include "piece.h"

Piece *piece_create(int total) {
    Piece *b = malloc(sizeof(*b) + sizeof(Part[total]));
    if (b) {
        b->total = (ubyte)total;
        memset(b->tab, 0, total * sizeof(Part));
        b->tab[0].is_main = true;
    }
    return b;
}
