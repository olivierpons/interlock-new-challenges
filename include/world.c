//
// Created by olivier on 30/03/2022.
//

#include <assert.h>
#include <stdio.h>
#include "cube.h"
#include "block.h"
#include "world.h"
#include "pos.h"
#include "debug.h"

const ulong WORLD_SIZE_X = 10;
const ulong WORLD_SIZE_Y = 10;
const ulong WORLD_SIZE_Z = 10;
const ulong WORLD_SIZE_XY = WORLD_SIZE_X * WORLD_SIZE_Y;
const ulong WORLD_SIZE = WORLD_SIZE_X * WORLD_SIZE_Y * WORLD_SIZE_Z;

void worldPutBlocksFromInfos(Cube* world, BlockInformation *pBI, int nbBi)
{
//    dbStart("worldPutBlocksFromInfos()\n");
    for (int i = 0; i < nbBi; ++i) {
//        dbs("worldPutBlocksFromInfos(): loop %i: "
//            "pBI[%i]=%p / %p / block = %p, rot = %d -> %p.\n",
//            i, i, pBI, &(pBI[i]), pBI[i].block, pBI[i].rotationNo,
//            pBI[i].block[ pBI[i].rotationNo ]
//        );
        worldPutBlock(
            world, pBI[i].block[ pBI[i].rotationNo ],
            pBI[i].p.x, pBI[i].p.y, pBI[i].p.z
        );
    }
//    dbEnd("worldPutBlocksFromInfos() done.\n");
}

void worldPutBlock(
    Cube* world, Block *b, double long x, double long y, double long z
) {
    Part *p;
    for (int i = 0; i < b->total; ++i) {
        p = &(b->parts[i]);
        long long a = XYZ(x + p->offsetX, y + p->offsetY, z + p->offsetZ);
//        dbs(
//            "worldPutBlock: %d -> %lld -> (%.2Lf, %.2Lf, %.2Lf)\n",
//            i, a, x + p->offsetX, y + p->offsetY, z + p->offsetZ
//        );/**/
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

bool cubeIsEmpty(Cube* world, ulong x, ulong y, ulong z)
{
    if (
        (x>=0) && (x<WORLD_SIZE_X) &&
        (y>=0) && (y<WORLD_SIZE_Y) &&
        (z>=0) && (z<WORLD_SIZE_Z)
    ) {
        if (CUBE_EMPTY(world[XYZ(x, y, z)])) {
            dbs("- ok: (%3lu/%3lu/%3lu)\n", x-1, y, z);
            return true;
        }
    }
    return false;
}

void computePositionsToTry(
    Cube* world, PosList *listDst, BlockInformation *blockInfos, int blocksInWorld
)
{
    int listIdx = 0;
    for (int i = 0; i < blocksInWorld; ++i) {
        for (int j = 0; j < 2; ++j) {
            Block *b = blockInfos[i].block[j];
            // loop on parts of this block:
            for (int k = 0; k < b->total; ++k) {

            }

        }
    }
    /*
    ulong found = 0;
    assert(list);

    initPosList(list, 1);
    for (ulong i = 0; i < WORLD_SIZE; ++i) {

        if (CUBE_NOT_EMPTY(world[i])) {
            ulong x = i % WORLD_SIZE_X;
            // both y computations work:
            // ulong y = (i / WORLD_SIZE_X) %  WORLD_SIZE_Y;
            ulong y = (i % WORLD_SIZE_XY) / WORLD_SIZE_X;
            ulong z = i / WORLD_SIZE_XY;
            db("-------------------------------\n");
            dbs("%4lu: %3lu / %3lu / %3lu\n", i, x, y, z);
            appendPosListOnceIfCubeEmpty(world, list, x - 1, y, z);
            appendPosListOnceIfCubeEmpty(world, list, x + 1, y, z);
            appendPosListOnceIfCubeEmpty(world, list, x, y - 1, z);
            appendPosListOnceIfCubeEmpty(world, list, x, y + 1, z);
            appendPosListOnceIfCubeEmpty(world, list, x, y, z - 1);
            appendPosListOnceIfCubeEmpty(world, list, x, y, z + 1);

            // optimization: no need to continue if all cubes are tested:
            if ((++found) == nbCubesInWorld) {
                return list;
            }
        }
    }
    dbs("Fatal: didn't find all the cubes: expected %lu, found: %lu.\n",
        nbCubesInWorld, found);
    exit(-1);
    */
}

bool worldCanPutBlock(
    Cube* world, Block *block, uint16_t x, uint16_t y, uint16_t z
) {
    for (int i = 0; i <block->total; ++i) {
        if (!cubeIsEmpty(world, x, y, z)) {
            return false;
        }
    }
    return true;
}

void appendPosListOnceIfCubeEmpty(
    Cube* world, PosList *list, ulong x, ulong y, ulong z
) {
    if (cubeIsEmpty(world, x, y, z)) {
        appendPosListOnce(list, x, y, z);
    }
}
