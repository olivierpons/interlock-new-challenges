#include <stdio.h>
#include <stdlib.h>
#include "include/custom_types.h"
#include "include/cube.h"
#include "include/piece.h"

#define ARRAY_LEN(a) (sizeof(a) / sizeof(a[0]))

const ubyte X = 10;
const ubyte Y = 10;
const ubyte Z = 10;

#define xyz(x, y, z) ((x) + ((y)*X) + ((z)*X*Y))

Cube* spaces = NULL;
Piece **pieces = NULL;

const Part refs[][2] ={
        { /* 01 : array of 2 Parts: */
                { /* Part 0 : */
                        { /* cube: */
                                HOLE,  WALL,  LINK,  WALL,  PLUG,  WALL
                        },
                        true, 0, 0, 0, // is_main / offsets: x y z
                },
                { /* Part 1 : */
                        {  /* cube: */
                                LINK,  WALL,  WALL,  WALL,  WALL,  WALL
                        },
                        false, 0, -1, 0, // is_main / offsets: x y z
                },
        },
        { /* 02 : array of 2 Parts: */
                { /* Part 0 : */
                        { /* cube: */
                                WALL,  HOLE,  LINK,  WALL,  PLUG,  WALL
                        },
                        true, 0, 0, 0, // is_main / offsets: x y z
                },
                { /* Part 1 : */
                        {  /* cube: */
                                LINK,  WALL,  WALL,  WALL,  WALL,  WALL
                        },
                        false, 0, -1, 0, // is_main / offsets: x y z
                },
        },
        { /* 03 : array of 2 Parts: */
                { /* Part 0 : */
                        { /* cube: */
                                PLUG,  WALL,  LINK,  WALL,  HOLE,  WALL
                        },
                        true, 0, 0, 0, // is_main / offsets: x y z
                },
                { /* Part 1 : */
                        {  /* cube: */
                                LINK,  WALL,  WALL,  WALL,  WALL,  WALL
                        },
                        false, 0, -1, 0, // is_main / offsets: x y z
                },
        },
        { /* 04 : array of 2 Parts: */
                { /* Part 0 : */
                        { /* cube: */
                                PLUG,  WALL,  LINK,  WALL,  WALL,  WALL
                        },
                        true, 0, 0, 0, // is_main / offsets: x y z
                },
                { /* Part 1 : */
                        {  /* cube: */
                                LINK,  WALL,  WALL,  WALL,  HOLE,  WALL
                        },
                        false, 0, -1, 0, // is_main / offsets: x y z
                },
        },
        { /* 05 : array of 2 Parts: */
                { /* Part 0 : */
                        { /* cube: */
                                WALL,  WALL,  LINK,  WALL,  PLUG,  WALL
                        },
                        true, 0, 0, 0, // is_main / offsets: x y z
                },
                { /* Part 1 : */
                        {  /* cube: */
                                LINK,  WALL,  HOLE,  WALL,  WALL,  WALL
                        },
                        false, 0, -1, 0, // is_main / offsets: x y z
                },
        },
        { /* 06 : array of 2 Parts: */
                { /* Part 0 : */
                        { /* cube: */
                                WALL,  WALL,  LINK,  WALL,  PLUG,  HOLE
                        },
                        true, 0, 0, 0, // is_main / offsets: x y z
                },
                { /* Part 1 : */
                        {  /* cube: */
                                LINK,  WALL,  WALL,  WALL,  WALL,  WALL
                        },
                        false, 0, -1, 0, // is_main / offsets: x y z
                },
        },
        { /* 07 : array of 2 Parts: */
                { /* Part 0 : */
                        { /* cube: */
                                WALL,  WALL,  LINK,  WALL,  PLUG,  WALL
                        },
                        true, 0, 0, 0, // is_main / offsets: x y z
                },
                { /* Part 1 : */
                        {  /* cube: */
                                LINK,  WALL,  WALL,  WALL,  HOLE,  WALL
                        },
                        false, 0, -1, 0, // is_main / offsets: x y z
                },
        },
        { /* 08 : array of 2 Parts: */
                { /* Part 0 : */
                        { /* cube: */
                                WALL,  WALL,  LINK,  WALL,  PLUG,  WALL
                        },
                        true, 0, 0, 0, // is_main / offsets: x y z
                },
                { /* Part 1 : */
                        {  /* cube: */
                                LINK,  WALL,  WALL,  WALL,  WALL,  HOLE
                        },
                        false, 0, -1, 0, // is_main / offsets: x y z
                },
        },
        { /* 09 : array of 2 Parts: */
                { /* Part 0 : */
                        { /* cube: */
                                PLUG,  WALL,  LINK,  WALL,  WALL,  WALL
                        },
                        true, 0, 0, 0, // is_main / offsets: x y z
                },
                { /* Part 1 : */
                        {  /* cube: */
                                LINK,  WALL,  HOLE,  WALL,  WALL,  WALL
                        },
                        false, 0, -1, 0, // is_main / offsets: x y z
                },
        },
        { /* 10 : array of 2 Parts: */
                { /* Part 0 : */
                        { /* cube: */
                                WALL,  WALL,  LINK,  HOLE,  PLUG,  WALL
                        },
                        true, 0, 0, 0, // is_main / offsets: x y z
                },
                { /* Part 1 : */
                        {  /* cube: */
                                LINK,  WALL,  WALL,  WALL,  WALL,  WALL,
                        },
                        false, 0, -1, 0, // is_main / offsets: x y z
                },
        },
        { /* 11 : array of 2 Parts: */
                { /* Part 0 : */
                        { /* cube: */
                                WALL,  WALL,  LINK,  WALL,  PLUG,  WALL
                        },
                        true, 0, 0, 0, // is_main / offsets: x y z
                },
                { /* Part 1 : */
                        {  /* cube: */
                                LINK,  WALL,  WALL,  HOLE,  WALL,  WALL
                        },
                        false, 0, -1, 0, // is_main / offsets: x y z
                },
        },
        { /* 12 : array of 2 Parts: */
                { /* Part 0 : */
                        { /* cube: */
                                WALL,  WALL,  LINK,  WALL,  PLUG,  WALL
                        },
                        true, 0, 0, 0, // is_main / offsets: x y z
                },
                { /* Part 1 : */
                        {  /* cube: */
                                LINK,  HOLE,  WALL,  WALL,  WALL,  WALL
                        },
                        false, 0, -1, 0, // is_main / offsets: x y z
                },
        },
};

const int NB_PIECES = 12;

void pieces_free() {
    if (pieces) {
        printf("Freeing blocks...\n");
        for (int i = 0; i < NB_PIECES; ++i) {
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

int main() {
    pieces = calloc(NB_PIECES, sizeof(Piece *));
    for (int i=0; i < NB_PIECES; ++i) {
        // pieces
        //   |
        //   + Piece
        //   + ...
        //   + Piece
        //       |
        //       +-- Part
        //             +-- Cube + offset_(n e s w f b)
        //       +-- Part
        //             +-- Cube + offset_(n e s w f b)
        //       +-- ...
        //       +-- Part
        int nb_pieces_to_create = ARRAY_LEN(refs[i]);
        pieces[i] = piece_create(nb_pieces_to_create);
        for (int j = 0; j < nb_pieces_to_create; ++j) {
            pieces[i]->parts[j] = refs[i][j];
            char *desc =cube_to_str(pieces[i]->parts[j].c);
            printf("piece %d, part: %d: %s\n", i+1, j, desc);
            free(desc);
        }
    }
    spaces = malloc(sizeof(Cube) * X * Y * Z);
    atexit(pieces_free);
    atexit(spaces_free);
    return 0;
}
