//
// Created by olivier on 30/03/2022.
//

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "cube.h"
#include "block.h"
#include "world.h"
#include "pos.h"

const ulong WORLD_SIZE_X = 30;
const ulong WORLD_SIZE_Y = 30;
const ulong WORLD_SIZE_Z = 30;
const ulong WORLD_SIZE_XY = WORLD_SIZE_X * WORLD_SIZE_Y;
const ulong WORLD_SIZE = WORLD_SIZE_X * WORLD_SIZE_Y * WORLD_SIZE_Z;

void worldPutBlock(
    Cube* world, Block *b, double long x, double long y, double long z
) {
    Part *p;
    for (int i = 0; i < b->total; ++i) {
        p = &(b->parts[i]);
        long long a = XYZ(x + p->offsetX, y + p->offsetY, z + p->offsetZ);
        printf(
            "%d -> %lld -> (%.2Lf, %.2Lf, %.2Lf)\n",
            i, a, x + p->offsetX, y + p->offsetY, z + p->offsetZ
        );/**/
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

bool CubeIsEmpty(Cube* world, ulong x, ulong y, ulong z)
{
    if (
        (x>=0) && (x<WORLD_SIZE_X) &&
        (y>=0) && (y<WORLD_SIZE_Y) &&
        (z>=0) && (z<WORLD_SIZE_Z)
    ) {
        if (CUBE_EMPTY(world[XYZ(x, y, z)])) {
            printf("- ok : %3lu / %3lu / %3lu\n", x-1, y, z);
            return true;
        }
    }
    return false;
}

bool worldCanPutBlock(
    Cube* world, Block *block, u_int16_t x, u_int16_t y, u_int16_t z
) {
    for (int i = 0; i <block->total; ++i) {
        if (!CubeIsEmpty(world, x, y, z)) {
            return false;
        }
    }
    return true;
}

void appendPosListOnceIfCubeEmpty(
    Cube* world, PosList *list, ulong x, ulong y, ulong z
) {
    if (CubeIsEmpty(world, x, y, z)) {
        appendPosListOnce(list, x, y, z);
    }
}

PosList *computePositionsToTry(Cube* world, ulong nbCubesInWorld)
{
    PosList *list = calloc(1, sizeof(PosList));
    ulong found = 0;
    if (!list) {
        printf("Fatal: out of memory error.\n");
        exit(-1);
    }

    initPosList(list, 1);
    for (ulong i = 0; i < WORLD_SIZE; ++i) {

        if (CUBE_NOT_EMPTY(world[i])) {
            ulong x = i % WORLD_SIZE_X;
            // both y computations work:
            // ulong y = (i / WORLD_SIZE_X) %  WORLD_SIZE_Y;
            ulong y = (i % WORLD_SIZE_XY) / WORLD_SIZE_X;
            ulong z = i / WORLD_SIZE_XY;
            printf("-------------------------------\n");
            printf("%4lu: %3lu / %3lu / %3lu\n", i, x, y, z);
            appendPosListOnceIfCubeEmpty(world, list, x - 1, y, z);
            appendPosListOnceIfCubeEmpty(world, list, x + 1, y, z);
            appendPosListOnceIfCubeEmpty(world, list, x, y - 1, z);
            appendPosListOnceIfCubeEmpty(world, list, x, y + 1, z);
            appendPosListOnceIfCubeEmpty(world, list, x, y, z - 1);
            appendPosListOnceIfCubeEmpty(world, list, x, y, z + 1);

            /* optimization: no need to continue if all cubes are tested: */
            if ((++found) == nbCubesInWorld) {
                return list;
            }
        }
    }
    printf("Fatal: didn't find all the cubes.\n");
    exit(-1);
}
