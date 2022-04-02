//
// Created by olivier on 30/03/2022.
//

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "cube.h"
#include "block.h"
#include "world.h"

const ulong WORLD_SIZE_X = 60;
const ulong WORLD_SIZE_Y = 60;
const ulong WORLD_SIZE_Z = 60;
const ulong WORLD_SIZE = WORLD_SIZE_X * WORLD_SIZE_Y * WORLD_SIZE_Z;

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

void worldPutAllBlocks(Cube* world, Block ***blocks) {
    printfCube(blocks, 0, 0);
    // manual test:
    for (int i = 0; i < 12; ++i) {
        // all rotations of each block:
        worldPutBlock(world, blocks[i][0],  2, (i+1)*3, 2);
        worldPutBlock(world, blocks[i][1],  4, (i+1)*3, 2);
        worldPutBlock(world, blocks[i][2],  7, (i+1)*3, 2);
        worldPutBlock(world, blocks[i][3], 10, (i+1)*3, 2);
        worldPutBlock(world, blocks[i][4], 13, (i+1)*3, 2);
        worldPutBlock(world, blocks[i][5], 16, (i+1)*3, 2);
        worldPutBlock(world, blocks[i][6], 19, (i+1)*3, 2);
        worldPutBlock(world, blocks[i][7], 22, (i+1)*3, 2);
        worldPutBlock(world, blocks[i][8], 25, (i+1)*3, 2);
        worldPutBlock(world, blocks[i][9], 29, (i+1)*3, 2);
    }
}

CubeList *computeCubesToTry(Cube* world)
{
    CubeList *list = calloc(1, sizeof(CubeList));
    if (!list) {
        printf("Fatal: out of memory error.\n");
        exit(-1);
    }

    initCubeList(list, 1);
    for (ulong i = 0; i < WORLD_SIZE; ++i) {
        if (world[i].n) {
            appendCubeList(list, &(world[i]));
        }
    }
    return list;
}