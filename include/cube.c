#include <stdio.h>
#include <malloc.h>
#include "cube.h"

const char *CUBE_TO_STR = "(%c%c%c%c%c%c)";
char *cube_to_str(Cube c) {
    size_t needed = snprintf(NULL, 0, CUBE_TO_STR, c.n, c.e, c.s, c.w, c.f, c.b) + 1;
    char *buffer = malloc(needed);
    sprintf(buffer, CUBE_TO_STR, c.n, c.e, c.s, c.w, c.f, c.b);
    return buffer;
}