//
// © Olivier Pons / HQF Development - 12/03/2022.
//

#ifndef INTERLOCK_PART_H
#define INTERLOCK_PART_H

#include "cube.h"
#include <stdbool.h>

typedef struct Part {
    Cube c;
    bool is_main;
    char offset_x; /* left  / right  */
    char offset_y; /* top   / bottom */
    char offset_z; /* front / behind */
} Part;

#endif //INTERLOCK_PART_H
