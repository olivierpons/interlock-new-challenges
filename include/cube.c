//
// © Olivier Pons / HQF Development - 12/03/2022.
//

#include <stdio.h>
#include <malloc.h>
#include "cube.h"

void cubeToStr(Cube c) {
    char s[255];
    printf("C (%c%c%c%c%c%c)\n", c.n, c.e, c.s, c.w, c.f, c.b);
}
