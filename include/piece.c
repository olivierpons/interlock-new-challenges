//
// © Olivier Pons / HQF Development - 12/03/2022.
//
#include <malloc.h>
#include <memory.h>
#include "piece.h"

Piece *pieceCreate(int total) {
    Piece *b = malloc(sizeof(*b) + sizeof(Part[total]));
    if (b) {
        b->total = (ubyte)total;
        memset(b->parts, 0, total * sizeof(Part));
        b->parts[0].isMain = true;
    }
    return b;
}
