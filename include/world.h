//
// Created by olivier on 30/03/2022.
//

#ifndef INTERLOCK_WORLD_H
#define INTERLOCK_WORLD_H

#include "custom_types.h"
#include "pos.h"
#include "block.h"

extern const ulong WORLD_SIZE_X;
extern const ulong WORLD_SIZE_Y;
extern const ulong WORLD_SIZE_Z;
extern const ulong WORLD_SIZE;

#define XYZ(_x, _y, _z) \
    ((_x) + ((_y)*WORLD_SIZE_X) + ((_z)*WORLD_SIZE_X*WORLD_SIZE_Y))

extern void worldPutBlocksFromInfos(
    Cube* world, BlockInformation *pBI, int length
);
extern void worldPutAllBlocks(Cube* world, Block ***blocks);
extern bool worldCanPutBlock(
    Cube* world, Block *block, uint16_t x, uint16_t y, uint16_t z
);
extern void computePositionsToTry(
    Cube* world, PosList *listDst, BlockInformation *blockInfos, int nbBlocks
);
#endif //INTERLOCK_WORLD_H
