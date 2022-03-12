#include <stdio.h>
#include <malloc.h>

typedef unsigned char ubyte;

typedef struct Cube {
    ubyte n;
    ubyte e;
    ubyte s;
    ubyte w;
    ubyte f;  /* front */
    ubyte b;  /* behind */
} Cube;

typedef struct Piece {
    Cube c;
    char offset_e;
    char offset_s;
    char offset_w;
    char offset_f;  /* front */
    char offset_b;  /* behind */
} Piece;

enum { EMPTY, WALL, BUMP, HOLE, LINK};
const ubyte X = 10;
const ubyte Y = 10;
const ubyte Z = 10;

#define xyz(x, y, z) ((x) + ((y)*X) + ((z)*X*Y))

Cube* spaces = NULL;
void cubes_free() {
    if (spaces) {
        printf("Freeing spaces...\n");
        free(spaces);
        spaces = NULL;
    }
}

Cube cube1[] = {
    /*   n      e      s      w      f      b */
    { WALL,  WALL,  LINK,  WALL,  BUMP,  WALL, },
    { LINK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, },
};

int main() {
    printf("Allocating spaces...\n");
    spaces = malloc(sizeof(Cube) * X * Y * Z);

    atexit(cubes_free);
    return 0;
}
