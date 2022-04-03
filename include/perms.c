//
// Created by olivier on 03/04/2022.
//

#include <stdbool.h>
#include <stdlib.h>
#include "perms.h"

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

Perms *allocPerms(int length) {
    Perms *p;
    p = calloc(length, sizeof(Perms));
    p->elements = calloc(length, sizeof(int));
    for (int i=0; i < length; ++i) {
        p->elements[i] = i;
    }
    p->length = length;
    p->current = length-1;
    return p;
}
Perms *freePerms(Perms *permutations) {
    if (permutations) {
        free(permutations->elements);
        permutations->elements = NULL;
        permutations->current = 0;
        permutations->length = 0;
    }
    free(permutations);
    return NULL;
}

bool nextPerm(Perms *p, int arrayLength) {
    if (++p->elements[p->current] < arrayLength) {
        return true;
    }
    while (--p->current>=0) {
        ++p->elements[p->current];
        if (p->elements[p->current] < arrayLength-(p->length-p->current-1)) {
            for (int i = p->current + 1; i < p->length ; ++i) {
                p->elements[i] = p->elements[i-1] + 1;
            }
            p->current = p->length-1;
            return true;
        }
    }
    return false;
}
