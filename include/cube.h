//
// © Olivier Pons / HQF Development - 12/03/2022.
//

#ifndef INTERLOCK_CUBE_H
#define INTERLOCK_CUBE_H

#include "custom_types.h"

typedef enum {
    F_EMPTY=' ', F_WALL='0', F_PLUG='+', F_HOLE='.', F_LINK='|',
} Face;

typedef struct Cube {
    Face n;
    Face e;
    Face s;
    Face w;
    Face f;  /* front */
    Face b;  /* back */
} Cube;

extern void cubeToStr(Cube c);

#endif //INTERLOCK_CUBE_H
