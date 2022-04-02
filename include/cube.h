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

typedef struct CubeList {
    size_t used;
    size_t size;
    Cube *array;
} CubeList;

extern void cubeToStr(Cube c);
extern void initCubeList(CubeList *a, size_t initialSize);
extern void appendCubeList(CubeList *a, Cube *cube);
extern void freeCubeList(CubeList *a);

#endif //INTERLOCK_CUBE_H
