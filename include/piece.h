//
// Created by Olivier on 12/03/2022.
//

#ifndef INTERLOCK_PIECE_H
#define INTERLOCK_PIECE_H

#include "cube.h"
#include<stdbool.h>

typedef struct Piece {
    Cube c;
    bool is_main;
    char offset_e;
    char offset_s;
    char offset_w;
    char offset_f;  /* front */
    char offset_b;  /* behind */
} Piece;

#endif //INTERLOCK_PIECE_H
