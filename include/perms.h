//
// Created by olivier on 03/04/2022.
//

#ifndef INTERLOCK_PERMS_H
#define INTERLOCK_PERMS_H

typedef struct Pointers {
    int current;
    size_t used;
    int *elements;
} Perms;

extern Perms *allocPerms(int used, size_t size);
extern Perms *resetPerms(Perms *p, int used);
extern Perms *freePerms(Perms *permutations);
extern void printPerms(Perms *p);
extern bool nextPerm(Perms *p, size_t limit);

#endif //INTERLOCK_PERMS_H
