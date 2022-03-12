#include <stdio.h>
#include <malloc.h>
#include "include/custom_types.h"
#include "include/cube.h"
#include "include/piece.h"

const ubyte X = 10;
const ubyte Y = 10;
const ubyte Z = 10;

#define xyz(x, y, z) ((x) + ((y)*X) + ((z)*X*Y))

Cube* spaces = NULL;
const int nb_pieces = 12;
Piece **pieces = NULL;

void pieces_free() {
    if (pieces) {
        printf("Freeing blocks...\n");
        for (int i = 0; i <nb_pieces; ++i) {
            free(pieces[i]);
        }
        free(pieces);
        pieces = NULL;
    }
}
void spaces_free() {
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
    pieces = calloc(nb_pieces, sizeof(Piece *));
    for (int i=0; i<nb_pieces; i++) {
        pieces[i] = piece_create(2);
    }
    spaces = malloc(sizeof(Cube) * X * Y * Z);
    atexit(pieces_free);
    atexit(spaces_free);
    return 0;
}
