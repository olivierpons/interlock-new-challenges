//
// Created by Olivier on 12/03/2022.
//

#ifndef INTERLOCK_PART_H
#define INTERLOCK_PART_H

#include "cube.h"
#include <stdbool.h>

typedef struct Part {
    Cube c;
    bool is_main;
    char offset_n;
    char offset_e;
    char offset_s;
    char offset_w;
    char offset_f;  /* front */
    char offset_b;  /* behind */
} Part;

#endif //INTERLOCK_PART_H
