//
// © Olivier Pons / HQF Development - 12/03/2022.
//

#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include "cube.h"

void cubeToStr(Cube c) {
    printf("C (%c%c%c%c%c%c)\n", c.n, c.e, c.s, c.w, c.f, c.b);
}

void initCubeList(CubeList *a, size_t initialSize) {
    a->array = calloc(initialSize, sizeof(Cube));
    a->used = 0;
    a->size = initialSize;
}

void appendCubeList(CubeList *a, Cube *cube) {
    Cube *tmp;
    if (a->used == a->size) {
        a->size *= 2;
        tmp = realloc(a->array, a->size * sizeof(Cube));
        if (!tmp) {
            printf("Fatal: out of memory error.\n");
            exit(-1);
        }
        a->array = tmp;
    }
    a->array[a->used++] = *cube;
}

void freeCubeList(CubeList *a) {
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}
