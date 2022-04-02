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
#define CUBE_NOT_EMPTY(cube) \
    ((cube).n || (cube).e || (cube).s || (cube).w || (cube).f || (cube).b)
#define CUBE_EMPTY(cube) \
    (!((cube).n && (cube).e && (cube).s && (cube).w && (cube).f && (cube).b))

typedef struct Cubes {
    ubyte total;
    Cube cubes[];
} Cubes;

extern void cubeToStr(Cube c);

#endif //INTERLOCK_CUBE_H
