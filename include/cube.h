//
// © Olivier Pons / HQF Development - 12/03/2022.
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

enum { EMPTY='e', WALL='0', PLUG='+', HOLE='.', LINK='|'};

extern char *cube_to_str(Cube c);
#endif //INTERLOCK_CUBE_H
