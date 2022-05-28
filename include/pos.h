/**
 * Author: Olivier Pons / HQF Development
 * Creation: 02/04/2022
 */
#ifndef INTERLOCK_POS_H
#define INTERLOCK_POS_H

#include <stdio.h>
#include <stdint.h>

typedef struct Pos {
    uint16_t x;
    uint16_t y;
    uint16_t z;
} Pos;

typedef struct PosList {
    size_t used;
    size_t size;
    Pos *array;
} PosList;

extern void initPosList(PosList *a, size_t initialSize);
extern void appendPosListOnce(PosList *a, uint16_t x, uint16_t y, uint16_t z);
extern void appendPosList(PosList *a, uint16_t x, uint16_t y, uint16_t z);
extern void freePosList(PosList *a);

#endif //INTERLOCK_POS_H
