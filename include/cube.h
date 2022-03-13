//
// Created by Olivier on 12/03/2022.
//

#ifndef INTERLOCK_CUBE_H
#define INTERLOCK_CUBE_H

#include "custom_types.h"

typedef struct Cube {
    ubyte n;
    ubyte e;
    ubyte s;
    ubyte w;
    ubyte f;  /* front */
    ubyte b;  /* behind */
} Cube;

enum { EMPTY, WALL, PLUG, HOLE, LINK};

#endif //INTERLOCK_CUBE_H
