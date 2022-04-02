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

extern void worldPutAllBlocks(Cube* world, Block ***blocks);
extern bool worldCanPutBlock(
    Cube* world, Block *block, u_int16_t x, u_int16_t y, u_int16_t z
);
extern PosList *computePositionsToTry(Cube* world, ulong nbCubesInWorld);
#endif //INTERLOCK_WORLD_H
