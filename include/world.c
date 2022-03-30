//
// Created by olivier on 30/03/2022.
//

#include <stdio.h>
#include <assert.h>
#include "cube.h"
#include "block.h"
#include "world.h"

const unsigned int WORLD_SIZE_X = 600;
const unsigned int WORLD_SIZE_Y = 1400;
const unsigned int WORLD_SIZE_Z = 600;

void worldPutBlock(
    Cube* world, Block *b, double long x, double long y, double long z
) {
    Part *p;
    for (int i = 0; i < b->total; ++i) {
        p = &(b->parts[i]);
        long long a = XYZ(x + p->offsetX, y + p->offsetY, z + p->offsetZ);
        /*printf(
            "%d -> %lld -> (%.2Lf, %.2Lf, %.2Lf)\n",
            i, a, x + p->offsetX, y + p->offsetY, z + p->offsetZ
        );*/
        assert(a >=0 );
        world[a] = p->c;
    }
}
