/**
 * Author: Olivier Pons / HQF Development
 * Creation: 12/03/2022
 */

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

typedef struct BlockInformation {
    uint8_t rotationNo;
    Pos p;
} BlockInformation;



#define TO_INT(a,b,c,d) (int)(a), (int)(b), (int)(c), (int)(d)

// BTRx = Block template
#define BTRN(tpl, block, part_no, rot) ((tpl)[block][rot]->parts[part_no].c.n)
#define BTRE(tpl, block, part_no, rot) ((tpl)[block][rot]->parts[part_no].c.e)
#define BTRS(tpl, block, part_no, rot) ((tpl)[block][rot]->parts[part_no].c.s)
#define BTRW(tpl, block, part_no, rot) ((tpl)[block][rot]->parts[part_no].c.w)
#define BTRF(tpl, block, part_no, rot) ((tpl)[block][rot]->parts[part_no].c.f)
#define BTRB(tpl, block, part_no, rot) ((tpl)[block][rot]->parts[part_no].c.b)

#define BRN(block, part_no, rot) BTRN(blockTemplates, block, part_no, rot)
#define BRE(block, part_no, rot) BTRE(blockTemplates, block, part_no, rot)
#define BRS(block, part_no, rot) BTRS(blockTemplates, block, part_no, rot)
#define BRW(block, part_no, rot) BTRW(blockTemplates, block, part_no, rot)
#define BRF(block, part_no, rot) BTRF(blockTemplates, block, part_no, rot)
#define BRB(block, part_no, rot) BTRB(blockTemplates, block, part_no, rot)

// Rotation 0 is *always* the reference
// BT = Block templates, 0 = part n.0, R = rot. 0: no rotation = the reference:
#define B0N(block) BRN(block, 0, 0)
#define B0E(block) BRE(block, 0, 0)
#define B0S(block) BRS(block, 0, 0)
#define B0W(block) BRW(block, 0, 0)
#define B0F(block) BRF(block, 0, 0)
#define B0B(block) BRB(block, 0, 0)

// BT = Block templates, 1 = part n.1, R = rot. 0: no rotation = the reference:
#define B1N(block) BRN(block, 1, 0)
#define B1E(block) BRE(block, 1, 0)
#define B1S(block) BRS(block, 1, 0)
#define B1W(block) BRW(block, 1, 0)
#define B1F(block) BRF(block, 1, 0)
#define B1B(block) BRB(block, 1, 0)

extern Block *blockCreate(int total);
extern Block *blockCreateWithParts(int total, ... );
extern void worldPutBlock(
    Cube* world, Block *b, double long x, double long y, double long z
);
extern void printfCube(Block ***pBlock, ulong blockNo, ulong rotationNo);

#endif //INTERLOCK_BLOCK_H
