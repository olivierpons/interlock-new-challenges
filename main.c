#include <stdio.h>
#include <malloc.h>
#include "include/custom_types.h"
#include "include/cube.h"
#include "include/blocks.h"

const ubyte X = 10;
const ubyte Y = 10;
const ubyte Z = 10;

#define xyz(x, y, z) ((x) + ((y)*X) + ((z)*X*Y))

Cube* spaces = NULL;
Blocks *blocks = NULL;
void blocks_free() {
    if (blocks) {
        free(blocks);
        blocks = NULL;
    }
}
void spaces_free() {
    if (spaces) {
        printf("Freeing spaces...\n");
        free(spaces);
        spaces = NULL;
    }
}


Cube cube1[] = {
    /*   n      e      s      w      f      b */
    { WALL,  WALL,  LINK,  WALL,  BUMP,  WALL, },
    { LINK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, },
};

int main() {
    printf("Allocating spaces...\n");
    blocks = blocks_create(blocks, 12);
    for (int i=0; i<12; i++) {
        block_create(&(blocks->tab[i]), 2);
    }
    spaces = malloc(sizeof(Cube) * X * Y * Z);

    atexit(blocks_free);
    atexit(spaces_free);
    return 0;
}
