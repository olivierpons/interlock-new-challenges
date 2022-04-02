#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/custom_types.h"
#include "include/cube.h"
#include "include/block.h"
#include "include/obj_write.h"
#include "include/world.h"

const int NB_BLOCKS = 12;
const int NB_BLOCK_ROTATIONS = 10;

Cube* world = NULL;
Block ***blocks = NULL;
PosList *availablePositions = NULL;
BlockPosition positions[12];

// region - Functions that free everything -
void blocksFree() {
    if (blocks) {
        for (int i = 0; i < NB_BLOCKS; ++i) {
            for (int j = 0; j < NB_BLOCK_ROTATIONS; ++j) {
                if (blocks[i][j]) {
                    free(blocks[i][j]);
                }
            }
            free(blocks[i]);
        }
        free(blocks);
        blocks = NULL;
    } else {
        printf("? No blocks to free ?\n");
    }
}
void worldFree() {
    if (world) {
        free(world);
        world = NULL;
    } else {
        printf("? No world to free ?\n");
    }
}
void availablePositionsFree() {
    if (availablePositions) {
        freePosList(availablePositions);
        free(availablePositions);
        availablePositions = NULL;
    } else {
        printf("? No availablePositions to free ?\n");
    }
}
// endregion

void blockCreateAllRotations(Block ***b, ulong i) {
    // roll: 1/3
    b[i][1] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        B0E(i), B0S(i), B0W(i), B0N(i), B0F(i), B0B(i), TO_INT(true, 0, 0, 0),
        B1E(i), B1S(i), B1W(i), B1N(i), B1F(i), B1B(i), TO_INT(false, 1, 0, 0)
    );
    // roll: 2/3
    b[i][2] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        B0S(i), B0W(i), B0N(i), B0E(i), B0F(i), B0B(i), TO_INT(true, 0, 0, 0),
        B1S(i), B1W(i), B1N(i), B1E(i), B1F(i), B1B(i), TO_INT(false, 0, 1, 0)
    );
    // roll: 3/3
    b[i][3] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        B0W(i), B0N(i), B0E(i), B0S(i), B0F(i), B0B(i), TO_INT(true, 0, 0, 0),
        B1W(i), B1N(i), B1E(i), B1S(i), B1F(i), B1B(i), TO_INT(false, -1, 0, 0)
    );
    // pitch: 1/3
    b[i][4] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        B0F(i), B0E(i), B0B(i), B0W(i), B0S(i), B0N(i), TO_INT(true, 0, 0, 0),
        B1F(i), B1E(i), B1B(i), B1W(i), B1S(i), B1N(i), TO_INT(false, 0, 0, 1)
    );
    // pitch: 2/3
    b[i][5] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        B0S(i), B0E(i), B0N(i), B0W(i), B0B(i), B0F(i), TO_INT(true, 0, 0, 0),
        B1S(i), B1E(i), B1N(i), B1W(i), B1B(i), B1F(i), TO_INT(false, 0, 1, 0)
    );
    // pitch: 3/3
    b[i][6] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        B0B(0), B0E(0), B0F(0), B0W(0), B0N(0), B0S(0), TO_INT(true, 0, 0, 0),
        B1B(0), B1E(0), B1F(0), B1W(0), B1N(0), B1S(0), TO_INT(false, 0, 0, -1)
    );
    // yaw: 1/3
    b[i][7] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        B0N(i), B0B(i), B0S(i), B0F(i), B0E(i), B0W(i), TO_INT(true, 0, 0, 0),
        B1N(i), B1B(i), B1S(i), B1F(i), B1E(i), B1W(i), TO_INT(false, 0, -1, 0)
    );
    // yaw: 2/3
    b[i][8] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        B0N(i), B0W(i), B0S(i), B0E(i), B0B(i), B0F(i), TO_INT(true, 0, 0, 0),
        B1N(i), B1W(i), B1S(i), B1E(i), B1B(i), B1F(i), TO_INT(false, 0, -1, 0)
    );
    // yaw: 3/3
    b[i][9] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        B0N(i), B0F(i), B0S(i), B0B(i), B0W(i), B0E(i), TO_INT(true, 0, 0, 0),
        B1N(i), B1F(i), B1S(i), B1B(i), B1W(i), B1E(i), TO_INT(false, 0, -1, 0)
    );
}

int main() {
    blocks = calloc(NB_BLOCKS, sizeof(Block **));
    if (!blocks) {
        exit(-1);
    }
    atexit(blocksFree);
    for (uint i = 0; i < NB_BLOCKS; ++i) {
        blocks[i] = calloc(NB_BLOCK_ROTATIONS, sizeof(Block *));
        if (!blocks[i]) {
            exit(-1);
        }
    }
    // region - Blocks creation -
    blocks[0][0] = blockCreateWithParts(2,
        // n, e, s, w, f, b, isMain, offsetX, offsetY, offsetZ,
        F_HOLE, F_WALL, F_LINK, F_WALL, F_PLUG, F_WALL, TO_INT(true, 0, 0, 0),
        F_LINK, F_WALL, F_WALL, F_WALL, F_WALL, F_WALL, TO_INT(false, 0, -1, 0)
    );
    blocks[1][0] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        F_WALL, F_HOLE, F_LINK, F_WALL, F_PLUG, F_WALL, TO_INT(true, 0, 0, 0),
        F_LINK, F_WALL, F_WALL, F_WALL, F_WALL, F_WALL, TO_INT(false, 0, -1, 0)
    );
    blocks[2][0] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        F_PLUG, F_WALL, F_LINK, F_WALL, F_HOLE, F_WALL, TO_INT(true, 0, 0, 0),
        F_LINK, F_WALL, F_WALL, F_WALL, F_WALL, F_WALL, TO_INT(false, 0, -1, 0)
    );
    blocks[3][0] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        F_PLUG, F_WALL, F_LINK, F_WALL, F_WALL, F_WALL, TO_INT(true, 0, 0, 0),
        F_LINK, F_WALL, F_WALL, F_WALL, F_HOLE, F_WALL, TO_INT(false, 0, -1, 0)
    );
    blocks[4][0] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        F_WALL, F_WALL, F_LINK, F_WALL, F_PLUG, F_WALL, TO_INT(true, 0, 0, 0),
        F_LINK, F_WALL, F_HOLE, F_WALL, F_WALL, F_WALL, TO_INT(false, 0, -1, 0)
    );
    blocks[5][0] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        F_WALL, F_WALL, F_LINK, F_WALL, F_PLUG, F_HOLE, TO_INT(true, 0, 0, 0),
        F_LINK, F_WALL, F_WALL, F_WALL, F_WALL, F_WALL, TO_INT(false, 0, -1, 0)
    );
    blocks[6][0] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        F_WALL, F_WALL, F_LINK, F_WALL, F_PLUG, F_WALL, TO_INT(true, 0, 0, 0),
        F_LINK, F_WALL, F_WALL, F_WALL, F_HOLE, F_WALL, TO_INT(false, 0, -1, 0)
    );
    blocks[7][0] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        F_WALL, F_WALL, F_LINK, F_WALL, F_PLUG, F_WALL, TO_INT(true, 0, 0, 0),
        F_LINK, F_WALL, F_WALL, F_WALL, F_WALL, F_HOLE, TO_INT(false, 0, -1, 0)
    );
    blocks[8][0] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        F_PLUG, F_WALL, F_LINK, F_WALL, F_WALL, F_WALL, TO_INT(true, 0, 0, 0),
        F_LINK, F_WALL, F_HOLE, F_WALL, F_WALL, F_WALL, TO_INT(false, 0, -1, 0)
    );
    blocks[9][0] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        F_WALL, F_WALL, F_LINK, F_HOLE, F_PLUG, F_WALL, TO_INT(true, 0, 0, 0),
        F_LINK, F_WALL, F_WALL, F_WALL, F_WALL, F_WALL, TO_INT(false, 0, -1, 0)
    );
    blocks[10][0] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        F_WALL, F_WALL, F_LINK, F_WALL, F_PLUG, F_WALL, TO_INT(true, 0, 0, 0),
        F_LINK, F_WALL, F_WALL, F_HOLE, F_WALL, F_WALL, TO_INT(false, 0, -1, 0)
    );
    blocks[11][0] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        F_WALL, F_WALL, F_LINK, F_WALL, F_PLUG, F_WALL, TO_INT(true, 0, 0, 0),
        F_LINK, F_HOLE, F_WALL, F_WALL, F_WALL, F_WALL, TO_INT(false, 0, -1, 0)
    );
    for (ulong i = 0; i < 12; ++i) {
        blockCreateAllRotations(blocks, i);
    }
    // endregion

    /**
     * blocks = Block *** = list of arrays of "blocks rotated"
     *                    = allocation: total blocks
     *    blocks[x] = (Block **) = array of "blocks rotated"
     *                           = allocation: total of possible rotations
     *    blocks[x][y] = (Block *) = array of blocks
     *    blocks[x][y][z] = Block = block rotated (or not) = array of pieces
     *         "list "
     *         |
     *         + ...
     *         |
     *         + Block
     *         + ...
     *         + Block
     *             |
     *             +-- Part
     *                   |
     *                   +-- Cube +
     *                   +-- offset_(n e s w f b)
     *             +-- Part
     *                   |
     *                   +-- Cube +
     *                   +-- offset_(n e s w f b)
     *             +-- ...
     *             +-- Part
     */

    ulong worldSize = WORLD_SIZE_X * WORLD_SIZE_Y * WORLD_SIZE_Z;
    worldFree();
    printf("Allocating %lu cells.\n", worldSize);
    world = calloc(worldSize, sizeof(Cube));
    atexit(worldFree);

    // worldPutAllBlocks(world, blocks);
    worldPutBlock(world, blocks[0][0], 2, 3, 2);
    worldPutBlock(world, blocks[1][0], 4, 3, 2);
    worldPutBlock(world, blocks[2][0], 6, 3, 2);
    worldPutBlock(world, blocks[3][1], 8, 3, 2);
    availablePositions = computePositionsToTry(world, 2);
    for (ulong i = 0; i < availablePositions->used; ++i) {
        printf("To try: (%hu, %hu, %hu)\n",
            availablePositions->array[i].x,
            availablePositions->array[i].y,
            availablePositions->array[i].z
        );
    }
    atexit(availablePositionsFree);

    objWriteFullWorld(world);

    return 0;
}
