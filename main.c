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

Cube cubes[12][2] = {
    {/* 01   n      e      s      w      f     b */
        { WALL,  WALL,  LINK,  WALL,  PLUG,  WALL},
        { LINK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    },
    {/* 01   n      e      s      w      f     b */
        { WALL,  WALL,  LINK,  WALL,  PLUG,  WALL},
        { LINK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    },
    {/* 01   n      e      s      w      f     b */
        { WALL,  WALL,  LINK,  WALL,  PLUG,  WALL},
        { LINK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    },
    {/* 01   n      e      s      w      f     b */
        { WALL,  WALL,  LINK,  WALL,  PLUG,  WALL},
        { LINK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    },
    {/* 01   n      e      s      w      f     b */
        { WALL,  WALL,  LINK,  WALL,  PLUG,  WALL},
        { LINK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    },
    {/* 01   n      e      s      w      f     b */
        { WALL,  WALL,  LINK,  WALL,  PLUG,  WALL},
        { LINK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    },
    {/* 01   n      e      s      w      f     b */
        { WALL,  WALL,  LINK,  WALL,  PLUG,  WALL},
        { LINK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    },
    {/* 01   n      e      s      w      f     b */
        { WALL,  WALL,  LINK,  WALL,  PLUG,  WALL},
        { LINK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    },
    {/* 01   n      e      s      w      f     b */
        { WALL,  WALL,  LINK,  WALL,  PLUG,  WALL},
        { LINK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    },
    {/* 01   n      e      s      w      f     b */
        { WALL,  WALL,  LINK,  WALL,  PLUG,  WALL},
        { LINK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    },
    {/* 01   n      e      s      w      f     b */
        { WALL,  WALL,  LINK,  WALL,  PLUG,  WALL},
        { LINK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    },
    {/* 01   n      e      s      w      f     b */
        { WALL,  WALL,  LINK,  WALL,  PLUG,  WALL},
        { LINK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    },
};

int main() {
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j <2; ++j) {
            printf("%02d %02d n: %d\n", i, j, cubes[i][j].n);
            printf("%02d %02d e: %d\n", i, j, cubes[i][j].e);
            printf("%02d %02d s: %d\n", i, j, cubes[i][j].s);
            printf("%02d %02d w: %d\n", i, j, cubes[i][j].w);
            printf("%02d %02d f: %d\n", i, j, cubes[i][j].f);
            printf("%02d %02d b: %d\n", i, j, cubes[i][j].b);

        }
    }
    printf("Allocating spaces...\n");
    pieces = calloc(nb_pieces, sizeof(Piece *));
    for (int i=0; i<nb_pieces; i++) {
        pieces[i] = piece_create(2);
//        pieces[i]->parts[0].c.n = ;
    }
    spaces = malloc(sizeof(Cube) * X * Y * Z);
    atexit(pieces_free);
    atexit(spaces_free);
    return 0;
}
