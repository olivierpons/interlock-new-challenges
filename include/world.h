//
// Created by olivier on 30/03/2022.
//

#ifndef INTERLOCK_WORLD_H
#define INTERLOCK_WORLD_H

#include "custom_types.h"

extern const unsigned int WORLD_SIZE_X;
extern const unsigned int WORLD_SIZE_Y;
extern const unsigned int WORLD_SIZE_Z;

#define XYZ(_x, _y, _z) \
    ((_x) + ((_y)*WORLD_SIZE_X) + ((_z)*WORLD_SIZE_X*WORLD_SIZE_Y))

#endif //INTERLOCK_WORLD_H
