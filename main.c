#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <string.h>
#include <signal.h>
#include "include/custom_types.h"
#include "include/cube.h"
#include "include/block.h"
#include "include/obj_write.h"
#include "include/world.h"
#include "include/perms.h"
#include "include/debug.h"

#define NB_BLOCKS 12
#define NB_BLOCK_ROTATIONS 10

Cube *world = NULL;
Block ***blockTemplates = NULL;
PosList positionsToTry;
BlockInformation blockInfos[NB_BLOCKS];
Perms *blockIndexes = NULL;

// region - Functions that free everything -
void freeBlocks() {
    if (blockTemplates) {
        db("Freeing blocs...\n");
        for (int i = 0; i < NB_BLOCKS; ++i) {
            for (int j = 0; j < NB_BLOCK_ROTATIONS; ++j) {
                if (blockTemplates[i][j]) {
                    free(blockTemplates[i][j]);
                }
            }
            free(blockTemplates[i]);
        }
        free(blockTemplates);
        blockTemplates = NULL;
    }
}
void freeWorld() {
    if (world) {
        db("Freeing world...\n");
        free(world);
        world = NULL;
    }
}
void freePositionsToTry() {
    if (positionsToTry.array) {
        db("Freeing positionsToTry...\n");
        freePosList(&positionsToTry);
    }
}
void freeBlockIndexes() {
    if (blockIndexes) {
        db("Freeing blockIndexes...\n");
        blockIndexes = freePerms(blockIndexes);
    }
}

void freeAll() {
    atexit(freeBlocks);
    atexit(freeWorld);
    atexit(freePositionsToTry);
    atexit(freeBlockIndexes);
}

void sig_handler(int signo) {
    if (signo == SIGINT) {
        db("Received SIGINT\n");
    }
    if (signo == SIGTERM) {
        db("Received SIGTERM\n");
    }
    freeAll();
    exit(-1);
}

// endregion

void blockCreateAllRotations(Block ***b, ulong i) {
    // roll: 1/3
    unsigned char idx=1;
    b[i][idx] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        B0E(i), B0S(i), B0W(i), B0N(i), B0F(i), B0B(i), TO_INT(true, 0, 0, 0),
        B1E(i), B1S(i), B1W(i), B1N(i), B1F(i), B1B(i), TO_INT(false, 1, 0, 0)
    );
    // roll: 2/3
    b[i][++idx] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        B0S(i), B0W(i), B0N(i), B0E(i), B0F(i), B0B(i), TO_INT(true, 0, 0, 0),
        B1S(i), B1W(i), B1N(i), B1E(i), B1F(i), B1B(i), TO_INT(false, 0, 1, 0)
    );
    // roll: 3/3
    b[i][++idx] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        B0W(i), B0N(i), B0E(i), B0S(i), B0F(i), B0B(i), TO_INT(true, 0, 0, 0),
        B1W(i), B1N(i), B1E(i), B1S(i), B1F(i), B1B(i), TO_INT(false, -1, 0, 0)
    );
    // pitch: 1/3
    b[i][++idx] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        B0F(i), B0E(i), B0B(i), B0W(i), B0S(i), B0N(i), TO_INT(true, 0, 0, 0),
        B1F(i), B1E(i), B1B(i), B1W(i), B1S(i), B1N(i), TO_INT(false, 0, 0, 1)
    );
    // pitch: 2/3
    b[i][++idx] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        B0S(i), B0E(i), B0N(i), B0W(i), B0B(i), B0F(i), TO_INT(true, 0, 0, 0),
        B1S(i), B1E(i), B1N(i), B1W(i), B1B(i), B1F(i), TO_INT(false, 0, 1, 0)
    );
    // pitch: 3/3
    b[i][++idx] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        B0B(0), B0E(0), B0F(0), B0W(0), B0N(0), B0S(0), TO_INT(true, 0, 0, 0),
        B1B(0), B1E(0), B1F(0), B1W(0), B1N(0), B1S(0), TO_INT(false, 0, 0, -1)
    );
    // yaw: 1/3
    b[i][++idx] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        B0N(i), B0B(i), B0S(i), B0F(i), B0E(i), B0W(i), TO_INT(true, 0, 0, 0),
        B1N(i), B1B(i), B1S(i), B1F(i), B1E(i), B1W(i), TO_INT(false, 0, -1, 0)
    );
    // yaw: 2/3
    b[i][++idx] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        B0N(i), B0W(i), B0S(i), B0E(i), B0B(i), B0F(i), TO_INT(true, 0, 0, 0),
        B1N(i), B1W(i), B1S(i), B1E(i), B1B(i), B1F(i), TO_INT(false, 0, -1, 0)
    );
    // yaw: 3/3
    b[i][++idx] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        B0N(i), B0F(i), B0S(i), B0B(i), B0W(i), B0E(i), TO_INT(true, 0, 0, 0),
        B1N(i), B1F(i), B1S(i), B1B(i), B1W(i), B1E(i), TO_INT(false, 0, -1, 0)
    );
}


//void swap(int *a, int i, int j)
//{
//    int temp = a[i];
//    a[i] = a[j];
//    a[j] = temp;
//}
//
//bool LEFT_TO_RIGHT = true;
//bool RIGHT_TO_LEFT = false;
//
//// Functions for finding the position of the largest mobile integer in a[].
//int searchArr(const int a[], int n, int mobile)
//{
//    for (int i = 0; i < n; i++)
//        if (a[i] == mobile)
//            return i + 1;
//    return -1;
//}
//
//// To carry out step 1 of the algorithm i.e. to find the largest mobile integer.
//int getMobile(const int a[], const bool dir[], int n)
//{
//    int mobile_prev = 0, mobile = 0;
//    for (int i = 0; i < n; i++) {
//        // direction 0 represents RIGHT TO LEFT.
//        if (dir[a[i]-1] == RIGHT_TO_LEFT && i != 0) {
//            if (a[i] > a[i-1] && a[i] > mobile_prev) {
//                mobile = a[i];
//                mobile_prev = mobile;
//            }
//        }
//        // direction 1 represents LEFT TO RIGHT.
//        if (dir[a[i]-1] == LEFT_TO_RIGHT && i != n-1) {
//            if (a[i] > a[i+1] && a[i] > mobile_prev) {
//                mobile = a[i];
//                mobile_prev = mobile;
//            }
//        }
//    }
//    return (mobile == 0 && mobile_prev == 0) ? 0 : mobile;
//}
//
//// Prints a single permutation
//int printOnePerm(int a[], bool dir[], int n)
//{
//    int mobile = getMobile(a, dir, n);
//    int pos = searchArr(a, n, mobile);
//
//    // swapping the elements according to the direction i.e. dir[].
//    if (dir[a[pos - 1] - 1] ==  RIGHT_TO_LEFT) {
//        swap(a, pos - 1, pos - 2);
//    } else if (dir[a[pos - 1] - 1] == LEFT_TO_RIGHT) {
//        swap(a, pos, pos - 1);
//    }
//
//    // changing the directions for elements
//    // greater than the largest mobile integer.
//    for (int i = 0; i < n; i++) {
//        if (a[i] > mobile) {
//            if (dir[a[i] - 1] == LEFT_TO_RIGHT) {
//                dir[a[i] - 1] = RIGHT_TO_LEFT;
//            } else if (dir[a[i] - 1] == RIGHT_TO_LEFT) {
//                dir[a[i] - 1] = LEFT_TO_RIGHT;
//            }
//        }
//    }
//
//    for (int i = 0; i < n; i++) {
//        printf("%d ", a[i]);
//    }
//    printf("\n");
//}
//
//// To end the algorithm for efficiency it ends
//// at the factorial of n because number of
//// permutations possible is just n!.
//int fact(int n)
//{
//    int res = 1;
//    for (int i = 1; i <= n; i++)
//        res = res * i;
//    return res;
//}
//
//// This function mainly calls printOnePerm()
//// one by one to print all permutations.
//void printPermutation(int n)
//{
//    // To store current permutation
//    int a[n];
//
//    // To store current directions
//    bool dir[n];
//
//    // storing the elements from 1 to n and
//    // printing first permutation.
//    for (int i = 0; i < n; i++) {
//        a[i] = i + 1;
//        printf("%d ", a[i]);
//    }
//    printf("\n");
//
//    // initially all directions are set to RIGHT TO LEFT i.e. 0.
//    for (int i = 0; i < n; i++) {
//        dir[i] = RIGHT_TO_LEFT;
//    }
//
//    // for generating permutations in the order.
//    for (int i = 1; i < fact(n); i++) {
//        printOnePerm(a, dir, n);
//    }
//}

void testBlockAroundPreviousBlocks (int toTest, int start, int end)
{
    /**
     * Test block in the world around previous blocks
     */
    dbs(
        "Base: (p.%d / r.%d): ",
        blockIndexes->elements[start-1],
        blockInfos[start-1].rotationNo
    )
    for (int i = start; i < end; ++i) {
        /**
         * Reminder:
         *  list of template indexes = blockIndexes = template indexes
         *  blockInfos[i] = rotation + pos (x, y, z)
         *  ->
         *  BlockInformation {
         *      uint8_t rotationNo;
         *      Pos p;
         *  };
         *
         * Algorithm to do:
         * - use world AND the infos on position+rotation of ALL previous blocks
         *   to search for possible positioning of current block:
         *
         *   tries = getAllPossiblePositions(
         *       world, blockIndexes, blockInfos, 0, i
         *   );
         *
         *   int *getAllPossibleTestsAround(
         *       Cube *world,
         *       Perms *blockIndexes,
         *       BlockInformation *blockInfos,
         *       int toTest, int start, int end
         *   ) {
         *      for (int i = start; i < end-1; ++i) {
         *         block = information using blockInfos[ blockIndexes[i] ]
         *         for each parts of the block:
         *         for (int j = 0; j < block->total; ++j) {
         *             Part *p = block->parts[j];
         *
         *             look in the world
         *
         *         check for all available positions around this block *using*
         *
         *      }
         *   }
         *
         * - put the template in the world
         */
        dbs("(p.%d / r.%d)", blockIndexes->elements[i], blockInfos[i].rotationNo)
        if ((i+1) < end) {
            db(" - ");
        }
    }
    dbs(
        ", to test: (p.%d / r.%d) ",
        blockIndexes->elements[toTest],
        blockInfos[toTest].rotationNo
    )
    dbs("\n")
}

int main() {
    // make sure functions to free all memory are ALWAYS called:
    if (signal(SIGINT, sig_handler) == SIG_ERR) {
        db("\ncan't catch SIGINT\n");
        exit(-1);
    }
    if (signal(SIGTERM, sig_handler) == SIG_ERR) {
        db("\ncan't catch SIGTERM\n");
        exit(-1);
    }
    atexit(freeAll);

    const ulong worldSize = WORLD_SIZE_X * WORLD_SIZE_Y * WORLD_SIZE_Z;
    const ulong worldCenterX = WORLD_SIZE_X / 2;
    const ulong worldCenterY = WORLD_SIZE_Y / 2;
    const ulong worldCenterZ = WORLD_SIZE_Z / 2;
    clock_t start, end;
    double cpu_time_used;
    ulong count = 0;
    start = clock();

    // all allocations before loops:
    blockTemplates = calloc(NB_BLOCKS, sizeof(Block **));
    if (!blockTemplates) {
        exit(-1);
    }
    for (uint i = 0; i < NB_BLOCKS; ++i) {
        blockTemplates[i] = calloc(NB_BLOCK_ROTATIONS, sizeof(Block *));
        if (!blockTemplates[i]) {
            exit(-1);
        }
    }
    initPosList(&positionsToTry, 50);

    // region - Blocks creation -
    blockTemplates[0][0] = blockCreateWithParts(2,
        // n, e, s, w, f, b, isMain, offsetX, offsetY, offsetZ,
        F_HOLE, F_WALL, F_LINK, F_WALL, F_PLUG, F_WALL, TO_INT(true, 0, 0, 0),
        F_LINK, F_WALL, F_WALL, F_WALL, F_WALL, F_WALL, TO_INT(false, 0, -1, 0)
    );
    blockTemplates[1][0] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        F_WALL, F_HOLE, F_LINK, F_WALL, F_PLUG, F_WALL, TO_INT(true, 0, 0, 0),
        F_LINK, F_WALL, F_WALL, F_WALL, F_WALL, F_WALL, TO_INT(false, 0, -1, 0)
    );
    blockTemplates[2][0] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        F_PLUG, F_WALL, F_LINK, F_WALL, F_HOLE, F_WALL, TO_INT(true, 0, 0, 0),
        F_LINK, F_WALL, F_WALL, F_WALL, F_WALL, F_WALL, TO_INT(false, 0, -1, 0)
    );
    blockTemplates[3][0] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        F_PLUG, F_WALL, F_LINK, F_WALL, F_WALL, F_WALL, TO_INT(true, 0, 0, 0),
        F_LINK, F_WALL, F_WALL, F_WALL, F_HOLE, F_WALL, TO_INT(false, 0, -1, 0)
    );
    blockTemplates[4][0] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        F_WALL, F_WALL, F_LINK, F_WALL, F_PLUG, F_WALL, TO_INT(true, 0, 0, 0),
        F_LINK, F_WALL, F_HOLE, F_WALL, F_WALL, F_WALL, TO_INT(false, 0, -1, 0)
    );
    blockTemplates[5][0] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        F_WALL, F_WALL, F_LINK, F_WALL, F_PLUG, F_HOLE, TO_INT(true, 0, 0, 0),
        F_LINK, F_WALL, F_WALL, F_WALL, F_WALL, F_WALL, TO_INT(false, 0, -1, 0)
    );
    blockTemplates[6][0] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        F_WALL, F_WALL, F_LINK, F_WALL, F_PLUG, F_WALL, TO_INT(true, 0, 0, 0),
        F_LINK, F_WALL, F_WALL, F_WALL, F_HOLE, F_WALL, TO_INT(false, 0, -1, 0)
    );
    blockTemplates[7][0] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        F_WALL, F_WALL, F_LINK, F_WALL, F_PLUG, F_WALL, TO_INT(true, 0, 0, 0),
        F_LINK, F_WALL, F_WALL, F_WALL, F_WALL, F_HOLE, TO_INT(false, 0, -1, 0)
    );
    blockTemplates[8][0] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        F_PLUG, F_WALL, F_LINK, F_WALL, F_WALL, F_WALL, TO_INT(true, 0, 0, 0),
        F_LINK, F_WALL, F_HOLE, F_WALL, F_WALL, F_WALL, TO_INT(false, 0, -1, 0)
    );
    blockTemplates[9][0] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        F_WALL, F_WALL, F_LINK, F_HOLE, F_PLUG, F_WALL, TO_INT(true, 0, 0, 0),
        F_LINK, F_WALL, F_WALL, F_WALL, F_WALL, F_WALL, TO_INT(false, 0, -1, 0)
    );
    blockTemplates[10][0] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        F_WALL, F_WALL, F_LINK, F_WALL, F_PLUG, F_WALL, TO_INT(true, 0, 0, 0),
        F_LINK, F_WALL, F_WALL, F_HOLE, F_WALL, F_WALL, TO_INT(false, 0, -1, 0)
    );
    blockTemplates[11][0] = blockCreateWithParts(2,
        // n,    e,      s,      w,      f,      b,     isMain, offX, offY, offZ
        F_WALL, F_WALL, F_LINK, F_WALL, F_PLUG, F_WALL, TO_INT(true, 0, 0, 0),
        F_LINK, F_HOLE, F_WALL, F_WALL, F_WALL, F_WALL, TO_INT(false, 0, -1, 0)
    );
    for (ulong i = 0; i < 12; ++i) {
        blockCreateAllRotations(blockTemplates, i);
    }
    // endregion

    /**
     * blockTemplates = Block *** = list of arrays of "blockTemplates rotated"
     *                            = allocation: total blockTemplates
     * blockTemplates[x] = (Block **) = elements of "blockTemplates rotated"
     *                                = allocation: total of possible rotations
     * blockTemplates[x][y] = (Block *) = elements of blockTemplates
     * blockTemplates[x][y][z] = Block = template rotated = elements of pieces
     *
     *      "list "
     *      |
     *      + ...
     *      |
     *      + Block
     *      + ...
     *      + Block
     *          |
     *          +-- Part
     *                |
     *                +-- Cube +
     *                +-- offset_(n e s w f b)
     *          +-- Part
     *                |
     *                +-- Cube +
     *                +-- offset_(n e s w f b)
     *          +-- ...
     *          +-- Part
     */

    // creating the world:
    printf("Creating world: allocating %lu cubes.\n", worldSize);
    world = malloc(worldSize * sizeof(Cube));

    // always start with 2 blocks, maybe with only 2 there's a solution anyway:
    blockIndexes = allocPerms(1, NB_BLOCKS);
    for (uint8_t blocksPicked = 1; blocksPicked < NB_BLOCKS; ++blocksPicked) {

        /**
         * combinations vs permutations!
         *
         * Sample with 2 elements: blockIndexes 0, 1
         *
         * 0 = 12 rotations
         * 1 = 12 rotations
         * 2 = 12 rotations
         *
         * try each rotations: 0, 1, 2           (0, 1, 2, 3, 4, 5, 6, 7, 8, 9)
         *      example (in middle of loop):
         *      piece 0, r. 0
         *      piece 1, r. 6
         *      piece 2, r. 8
         *
         *      -> compute all combinations, example:
         *            example combination 0: (0, 0) - (1, 6) - (2, 8)
         *            example combination 1: (0, 0) - (2, 8) - (1, 6)
         *            example combination 2: (1, 6) - (0, 0) - (2, 8)
         *            example combination 3: (1, 6) - (2, 8) - (0, 0)
         *            example combination 4: (2, 8) - (1, 6) - (0, 0)
         *            example combination 5: (2, 8) - (0, 0) - (1, 6)
         *
         *            for each combinations, try to put the blockInfos:
         *
         *                example for combination 0:
         *                for (i = 0 .. nb blockInfos-1)
         *                   -> put piece[ combination[ i ] ]
         *                   -> computePositionsToTry(..)
         *                      for each (blockInfos found)
         *                          ok = true;
         *                          for (j = i+1 .. nb blockInfos-1)
         *                             if put a template = ok
         *                                computePositionsToTry(..)
         *                             else
         *                                 ok = false;
         *                                 break loop
         *                          if ok:
         *                              if "all flat":
         *                                  found a solution -> save world!
         */
        dbs("BlocksPicked: %2d: \n", blocksPicked)
        // blockIndexes = for "picking" blockTemplates:
        blockIndexes = resetPerms(blockIndexes, blocksPicked);
        // Init all blocks we're going to work with:
        for (int i = 0; i < blocksPicked; ++i) {
            blockInfos[i].rotationNo = 0;
        }
        do {
            printPerms(blockIndexes);
            printPerms(blockIndexes);
            // loop on all possible rotations for all blockInfos[]:
            int pos;
            for (;;) {
                pos = 0;
                /* reset world (TODO: optimize: zero-mem ONLY previous cubes) */
                memset(world, 0, worldSize * sizeof(Cube));
                /**
                 * Here we have:
                 *  blockInfos[i] = template x + rotation y + pos (x=y=z=0)
                 * Put first template in center of the world:
                 */
                blockInfos[0].p.x = worldCenterX;
                blockInfos[0].p.y = worldCenterY;
                blockInfos[0].p.z = worldCenterZ;

                /* put blockInfos[0] in the world */
                worldPutBlocksFromInfos(
                    world, blockInfos, 0, 0, blockTemplates
                );

                testBlockAroundPreviousBlocks(
                    blocksPicked - 1, 1, blocksPicked - 1
                );

                count++;

                /* - compute next rotation - */
                pos = blocksPicked - 1;
                while (++blockInfos[pos].rotationNo == 10) {
                    blockInfos[pos].rotationNo = 0;
                    pos--;
                    if (pos < 0) {
                        break;
                    }
                }
                if (pos < 0) {
                    end = clock();
                    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
                    setlocale(LC_NUMERIC, "en_US.utf-8");
                    dbs("Done!\n%'lu tries, cpu time used=%'.2fs\n",
                        count, cpu_time_used)
                    break;
                }
            }
        } while (nextPerm(blockIndexes, 12));

        objWriteFullWorld(world);
        break;
    }
    freeBlockIndexes();
    freeWorld();

    return 0;
}
