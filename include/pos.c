/**
 * Author: Olivier Pons / HQF Development
 * Creation: 02/04/2022
 */
#include "pos.h"
#include "debug.h"
#include "custom_types.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

void initPosList(PosList *a, size_t initialSize) {
    a->array = calloc(initialSize, sizeof(Pos));
    a->used = 0;
    a->size = initialSize;
}

void appendPosListOnce(PosList *a, uint16_t x, uint16_t y, uint16_t z) {
    for (ulong i = 0; i <a->used; ++i) {
//        dbs("(%hu, %hu, %hu): Trying... (%hu, %hu, %hu)\n",
//            x, y, z,
//            a->elements[i].x, a->elements[i].y, a->elements[i].z
//        );
        if (
            (a->array[i].x == x) &&
            (a->array[i].y == y) &&
            (a->array[i].z == z)
        ) {
            dbs("Already present: (%hu, %hu, %hu)\n", x, y, z)
            return;
        }
    }
    appendPosList(a, x, y, z);
}

void appendPosList(PosList *a, uint16_t x, uint16_t y, uint16_t z) {
    Pos *tmp;
    if (a->used == a->size) {
        a->size *= 2;
        tmp = realloc(a->array, a->size * sizeof(Pos));
        if (!tmp) {
            printf("Fatal: out of memory error.\n");
            exit(-1);
        }
        a->array = tmp;
    }
    a->array[a->used].x = x;
    a->array[a->used].y = y;
    a->array[a->used].z = z;
    a->used++;
}

void freePosList(PosList *a)
{
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}
