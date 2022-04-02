//
// © Olivier Pons / HQF Development - 12/03/2022.
//

#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include "cube.h"

void cubeToStr(Cube c) {
    printf("C (%c%c%c%c%c%c)\n", c.n, c.e, c.s, c.w, c.f, c.b);
}

Cubes *cubesCreate(int total) {
    Cubes *b = malloc(sizeof(*b) + sizeof(Cube[total]));
    if (b) {
        b->total = (ubyte)total;
        memset(b->cubes, 0, total * sizeof(Cube));
    }
    return b;
}
