//
// © Olivier Pons / HQF Development - 12/03/2022.
//

#ifndef INTERLOCK_CUBE_H
#define INTERLOCK_CUBE_H

#include "custom_types.h"

typedef enum {
    EMPTY='e', WALL='0', PLUG='+', HOLE='.', LINK='|',
} Face;

typedef struct Cube {
    Face n;
    Face e;
    Face s;
    Face w;
    Face f;  /* front */
    Face b;  /* behind */
} Cube;

extern char *cubeToStr(Cube c);
#endif //INTERLOCK_CUBE_H
