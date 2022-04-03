//
// © Olivier Pons / HQF Development - 12/03/2022.
//

#ifndef INTERLOCK_BLOCK_H
#define INTERLOCK_BLOCK_H

#include "custom_types.h"
#include "part.h"
#include "pos.h"
#include <stdint.h>

/**
 * FAM: Flexible Array Members in a structure in C:
 * https://www.geeksforgeeks.org/flexible-elements-members-structure-c/
 */
typedef struct Block {
    ubyte total;
    Part parts[];
} Block;

typedef struct BlockPosition {
     uint8_t rotationNo;
     Pos p;
} BlockPosition;



#define TO_INT(a,b,c,d) (int)(a), (int)(b), (int)(c), (int)(d)

// 0 is *always* the reference:
#define BN(piece, no_part) blocks[piece][0]->parts[no_part].c.n
#define BE(piece, no_part) blocks[piece][0]->parts[no_part].c.e
#define BS(piece, no_part) blocks[piece][0]->parts[no_part].c.s
#define BW(piece, no_part) blocks[piece][0]->parts[no_part].c.w
#define BF(piece, no_part) blocks[piece][0]->parts[no_part].c.f
#define BB(piece, no_part) blocks[piece][0]->parts[no_part].c.b

#define B0N(piece) BN(piece, 0)
#define B0E(piece) BE(piece, 0)
#define B0S(piece) BS(piece, 0)
#define B0W(piece) BW(piece, 0)
#define B0F(piece) BF(piece, 0)
#define B0B(piece) BB(piece, 0)

#define B1N(piece) BN(piece, 1)
#define B1E(piece) BE(piece, 1)
#define B1S(piece) BS(piece, 1)
#define B1W(piece) BW(piece, 1)
#define B1F(piece) BF(piece, 1)
#define B1B(piece) BB(piece, 1)

extern Block *blockCreate(int total);
extern Block *blockCreateWithParts(int total, ... );
extern void worldPutBlock(
    Cube* world, Block *b, double long x, double long y, double long z
);
extern void printfCube(Block ***pBlock, ulong blockNo, ulong rotationNo);

#endif //INTERLOCK_BLOCK_H
