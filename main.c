#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>
#include <time.h>
#include "include/custom_types.h"
#include "include/cube.h"
#include "include/piece.h"
#include "include/obj_write.h"

#define ARRAY_LEN(a) (sizeof(a) / sizeof(a[0]))

const ubyte X = 10;
const ubyte Y = 10;
const ubyte Z = 10;

// #define xyz(x, y, z) ((x) + ((y)*X) + ((z)*X*Y))

Cube* spaces = NULL;
Piece **pieces = NULL;

const Part refs[][2] ={
    { /* 01 : array of 2 Parts: */
        { /* Part 0 : */
            { /* cube: */
                HOLE,  WALL,  LINK,  WALL,  PLUG,  WALL
            },
            true, 0, 0, 0, // isMain / offsets: x y z
        },
        { /* Part 1 : */
            {  /* cube: */
                LINK,  WALL,  WALL,  WALL,  WALL,  WALL
            },
            false, 0, -1, 0, // isMain / offsets: x y z
        },
    },
    { /* 02 : array of 2 Parts: */
        { /* Part 0 : */
            { /* cube: */
                WALL,  HOLE,  LINK,  WALL,  PLUG,  WALL
            },
            true, 0, 0, 0, // isMain / offsets: x y z
        },
        { /* Part 1 : */
            {  /* cube: */
                LINK,  WALL,  WALL,  WALL,  WALL,  WALL
            },
            false, 0, -1, 0, // isMain / offsets: x y z
        },
    },
    { /* 03 : array of 2 Parts: */
        { /* Part 0 : */
            { /* cube: */
                PLUG,  WALL,  LINK,  WALL,  HOLE,  WALL
            },
            true, 0, 0, 0, // isMain / offsets: x y z
        },
        { /* Part 1 : */
            {  /* cube: */
                LINK,  WALL,  WALL,  WALL,  WALL,  WALL
            },
            false, 0, -1, 0, // isMain / offsets: x y z
        },
    },
    { /* 04 : array of 2 Parts: */
        { /* Part 0 : */
            { /* cube: */
                PLUG,  WALL,  LINK,  WALL,  WALL,  WALL
            },
            true, 0, 0, 0, // isMain / offsets: x y z
        },
        { /* Part 1 : */
            {  /* cube: */
                LINK,  WALL,  WALL,  WALL,  HOLE,  WALL
            },
            false, 0, -1, 0, // isMain / offsets: x y z
        },
    },
    { /* 05 : array of 2 Parts: */
        { /* Part 0 : */
            { /* cube: */
                WALL,  WALL,  LINK,  WALL,  PLUG,  WALL
            },
            true, 0, 0, 0, // isMain / offsets: x y z
        },
        { /* Part 1 : */
            {  /* cube: */
                LINK,  WALL,  HOLE,  WALL,  WALL,  WALL
            },
            false, 0, -1, 0, // isMain / offsets: x y z
        },
    },
    { /* 06 : array of 2 Parts: */
        { /* Part 0 : */
            { /* cube: */
                WALL,  WALL,  LINK,  WALL,  PLUG,  HOLE
            },
            true, 0, 0, 0, // isMain / offsets: x y z
        },
        { /* Part 1 : */
            {  /* cube: */
                LINK,  WALL,  WALL,  WALL,  WALL,  WALL
            },
            false, 0, -1, 0, // isMain / offsets: x y z
        },
    },
    { /* 07 : array of 2 Parts: */
        { /* Part 0 : */
            { /* cube: */
                WALL,  WALL,  LINK,  WALL,  PLUG,  WALL
            },
            true, 0, 0, 0, // isMain / offsets: x y z
        },
        { /* Part 1 : */
            {  /* cube: */
                LINK,  WALL,  WALL,  WALL,  HOLE,  WALL
            },
            false, 0, -1, 0, // isMain / offsets: x y z
        },
    },
    { /* 08 : array of 2 Parts: */
        { /* Part 0 : */
            { /* cube: */
                WALL,  WALL,  LINK,  WALL,  PLUG,  WALL
            },
            true, 0, 0, 0, // isMain / offsets: x y z
        },
        { /* Part 1 : */
            {  /* cube: */
                LINK,  WALL,  WALL,  WALL,  WALL,  HOLE
            },
            false, 0, -1, 0, // isMain / offsets: x y z
        },
    },
    { /* 09 : array of 2 Parts: */
        { /* Part 0 : */
            { /* cube: */
                PLUG,  WALL,  LINK,  WALL,  WALL,  WALL
            },
            true, 0, 0, 0, // isMain / offsets: x y z
        },
        { /* Part 1 : */
            {  /* cube: */
                LINK,  WALL,  HOLE,  WALL,  WALL,  WALL
            },
            false, 0, -1, 0, // isMain / offsets: x y z
        },
    },
    { /* 10 : array of 2 Parts: */
        { /* Part 0 : */
            { /* cube: */
                WALL,  WALL,  LINK,  HOLE,  PLUG,  WALL
            },
            true, 0, 0, 0, // isMain / offsets: x y z
        },
        { /* Part 1 : */
            {  /* cube: */
                LINK,  WALL,  WALL,  WALL,  WALL,  WALL,
            },
            false, 0, -1, 0, // isMain / offsets: x y z
        },
    },
    { /* 11 : array of 2 Parts: */
        { /* Part 0 : */
            { /* cube: */
                WALL,  WALL,  LINK,  WALL,  PLUG,  WALL
            },
            true, 0, 0, 0, // isMain / offsets: x y z
        },
        { /* Part 1 : */
            {  /* cube: */
                LINK,  WALL,  WALL,  HOLE,  WALL,  WALL
            },
            false, 0, -1, 0, // isMain / offsets: x y z
        },
    },
    { /* 12 : array of 2 Parts: */
        { /* Part 0 : */
            { /* cube: */
                WALL,  WALL,  LINK,  WALL,  PLUG,  WALL
            },
            true, 0, 0, 0, // isMain / offsets: x y z
        },
        { /* Part 1 : */
            {  /* cube: */
                LINK,  HOLE,  WALL,  WALL,  WALL,  WALL
            },
            false, 0, -1, 0, // isMain / offsets: x y z
        },
    },
};

const int NB_PIECES = 12;

void piecesFree() {
    if (pieces) {
        printf("Freeing blocks...\n");
        for (int i = 0; i < NB_PIECES; ++i) {
            free(pieces[i]);
        }
        free(pieces);
        pieces = NULL;
    }
}
void spacesFree() {
    if (spaces) {
        printf("Freeing spaces...\n");
        free(spaces);
        spaces = NULL;
    }
}

int main() {
    pieces = calloc(NB_PIECES, sizeof(Piece *));
    spaces = malloc(sizeof(Cube) * X * Y * Z);
    atexit(piecesFree);
    atexit(spacesFree);
    for (int i=0; i < NB_PIECES; ++i) {
        /**
         * pieces
         *   |
         *   + Piece
         *   + ...
         *   + Piece
         *       |
         *       +-- Part
         *             |
         *             +-- Cube +
         *             +-- offset_(n e s w f b)
         *       +-- Part
         *             |
         *             +-- Cube +
         *             +-- offset_(n e s w f b)
         *       +-- ...
         *       +-- Part
         */
        int nbPiecesToCreate = ARRAY_LEN(refs[i]);
        pieces[i] = pieceCreate(nbPiecesToCreate);
        for (int j = 0; j < nbPiecesToCreate; ++j) {
            pieces[i]->parts[j] = refs[i][j];
            // char *desc =cubeToStr(pieces[i]->parts[j].c);
            // printf("piece %d, part: %d: %s\n", i+1, j, desc);
            // free(desc);
        }
    }

    char *dst_path = "../3d-obj/";
    char dstExpandedPath[PATH_MAX + 1];
    char *ptr;
    ptr = realpath(dst_path, dstExpandedPath);
    if (!ptr) {
        printf("? Can't expand %s\n", dst_path);
        return -1;
    }
    strcat(ptr, "/chest.obj");
    printf("Destination file: %s\n", ptr);

    FILE *f_out;
    f_out = fopen(ptr, "w");
    if (f_out) {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        char s[255];
        S_W(f_out, s,
            "# © Olivier Pons / HQF Development - "
            "%d-%02d-%02d %02d:%02d:%02d\n",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec)
        S_W(f_out, s, "mtllib model.mtl\n")
        S_W(f_out, s, "vn 0 0 1\n")
        double x = 0.0;
        double y = 0.0;
        double z = 0.0;
        ulong ref = 0;
        objWriteFaceSimple(f_out, &ref, x, y, z, 0.0, 0.0, 0); // front
        objWriteFaceSimple(f_out, &ref, x, y, z, 180, 0, 0);  // back
        objWriteFaceSimple(f_out, &ref, x, y, z, 0.0, 0.0, 90); // bottom
        objWriteFaceSimple(f_out, &ref, x, y, z, 0.0, 0.0, -90);  // top
        objWriteFaceSimple(f_out, &ref, x, y, z, 90, 0, 0.0);  // right
        objWriteFaceSimple(f_out, &ref, x, y, z, -90, 0, 0.0); // left
        objWriteFaceRadius(f_out, ref, x, y, z, 1.5, 32);
        fclose(f_out);
    } else {
        printf("? cant write to file ?\n");
    }
    return 0;
}
