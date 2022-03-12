//
// Created by Olivier on 12/03/2022.
//

#include <malloc.h>
#include <memory.h>
#include "blocks.h"

Blocks *blocks_create(Blocks *b, int total) {
    b = malloc(sizeof(*b) + sizeof(Block[total]));
    if (b) {
        b->total = (ubyte)total;
        memset(b->tab, total, sizeof(Block));
    }
    return b;
}
