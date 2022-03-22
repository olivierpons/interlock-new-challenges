//
// Created by olivier on 18/03/2022.
//

#ifndef INTERLOCK_3D_H
#define INTERLOCK_3D_H
typedef struct Pos {
    long double x;
    long double y;
    long double z;
} Pos;

typedef struct Quad {
    Pos a;
    Pos b;
    Pos c;
    Pos d;
} Quad;
#endif //INTERLOCK_3D_H
