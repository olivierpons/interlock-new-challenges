//
// Created by olivier on 03/04/2022.
//

#ifndef INTERLOCK_PERMS_H
#define INTERLOCK_PERMS_H

typedef struct Pointers {
    int current;
    int length;
    int *elements;
} Perms;

extern Perms *allocPerms(int length);
extern void freePerms(Perms **pointers);
extern bool nextPerm(Perms *p, int arrayLength);

#endif //INTERLOCK_PERMS_H
