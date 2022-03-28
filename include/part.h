//
// © Olivier Pons / HQF Development - 12/03/2022.
//

#ifndef INTERLOCK_PART_H
#define INTERLOCK_PART_H

#include "cube.h"
#include <stdbool.h>

typedef struct Part {
    Cube c;
    bool isMain;
    char offsetX; /* left  / right  */
    char offsetY; /* top   / bottom */
    char offsetZ; /* front / back   */
} Part;

#endif //INTERLOCK_PART_H
