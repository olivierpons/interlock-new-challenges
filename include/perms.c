//
// Created by olivier on 03/04/2022.
//

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "perms.h"
#include "debug.h"

Perms *allocPerms(int used, size_t size) {
    Perms *p;
    p = calloc(size, sizeof(Perms));
    p->elements = calloc(size, sizeof(int));
    dbs("allocPerms(): p->elements = %p\n", p->elements)
    return resetPerms(p, used);
}

Perms *resetPerms(Perms *p, int used) {
    for (int i=0; i < used; ++i) {
        p->elements[i] = i;
    }
    p->used = used;
    p->current = used - 1;
    dbs("resetPerms(): p->used = %zu, p->current = %d\n", p->used, p->current)
    return p;
}

Perms *freePerms(Perms *permutations) {
    if (permutations) {
        free(permutations->elements);
        permutations->elements = NULL;
        permutations->current = 0;
        permutations->used = 0;
    }
    free(permutations);
    return NULL;
}

void printPerms(Perms *p) {
    char tmp[20];
    db("> ");
    for (size_t i=0; i<p->used; ++i) {
        sprintf(tmp, "%d ", p->elements[i]);
        db(tmp);
    }
    db("\n");
}
bool nextPerm(Perms *p, size_t limit) {
    /**
     * Inspired by stackoverflow here:
     * https://stackoverflow.com/a/44036562/106140
     *
     * Lets say your elements of letters looks like this: "ABCDEFGH".
     * You have three indices (i, j, k) indicating which letters you are
     * going to use for the current word, You start with:
     *
     * A B C D E F G H
     * ^ ^ ^
     * i j k
     * First you vary k, so the next step looks like that:
     *
     * A B C D E F G H
     * ^ ^   ^
     * i j   k
     * If you reached the end you go on and vary j and then k again.
     *
     * A B C D E F G H
     * ^   ^ ^
     * i   j k
     *
     * A B C D E F G H
     * ^   ^   ^
     * i   j   k
     * Once you j reached G you start also to vary i.
     *
     * A B C D E F G H
     *   ^ ^ ^
     *   i j k
     *
     * A B C D E F G H
     *   ^ ^   ^
     *   i j   k
     * ...
     */
    ++p->elements[p->current];
    if (p->current == p->used - 1) {
        if (p->elements[p->current] < limit) {
            return true;
        }
    } else if (p->elements[p->current] < p->elements[p->current+1]) {
        return true;
    }
    --p->elements[p->current];
    while (--p->current>=0) {
        ++p->elements[p->current];
        if (p->elements[p->current] < p->elements[p->current+1]-1) {
            for (size_t i = p->current + 2; i < p->used ; ++i) {
                p->elements[i] = p->elements[i-1] + 1;
            }
            return true;
        }
    }
    return false;
}
