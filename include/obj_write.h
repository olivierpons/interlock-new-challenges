//
// © Olivier Pons / HQF Development - 16/03/2022.
//

#ifndef INTERLOCK_OBJ_WRITE_H
#define INTERLOCK_OBJ_WRITE_H

/* S_W() = "Sprintf() then Write = fputs() "*/
#include "custom_types.h"

#define S_W(h, st, ...)  sprintf((st), __VA_ARGS__); fputs((st), (h));

/* W_P() = "Write Point()"*/
#define W_P(h, st, x, y, z)  S_W(h, st, "v %Lf %Lf %Lf\n", (x), (y), (z))

/* L_W() = "Link Write() " */
#define L_W(h, st, pLink) \
        S_W(h, st, "%s", pLink->comment) \
        S_W(h, st, "%s", pLink->name)     \
        S_W(h, st, "%s", pLink->material)

/* W_OR() = "Write Object() with Ref" */
#define W_OR(h, st, comment, name, ref, material) \
        S_W(h, st, "\n# "comment":\n") \
        S_W(h, st, "g "name"\n", ref)     \
        S_W(h, st, "usemtl "material"\n")

/* L_OBJ() = Link Object with comment, name, and material: */
#define L_OBJ(h, c, n, r, m) \
        sprintf((h)->comment, "\n# "c":\n"); \
        sprintf((h)->name, "g "n"\n", r); \
        sprintf((h)->material, "usemtl "m"\n");

/* W_LINK() = "Write Link()"*/
#define W_LINK(h, tmp, r1, r2, r4, r3) \
        S_W(h, tmp, "f %lu//1 %lu//1 %lu//1 %lu//1\n", r1, r2, r4, r3)

/* one-liner for a (Pos *) pointer to assign x, y, z: */
#define SET_XYZ(ptr, i, vx, vy, vz) \
    (ptr)[(i)].x = (vx); \
    (ptr)[(i)].y = (vy); \
    (ptr)[(i)].z = (vz);


typedef struct Links {
    char comment[255];
    char name[255];
    char material[255];

    ulong total;
    ulong links[];
} Links;

extern void objWriteFaceSimple(
    FILE *fOut, ulong *pRef,
    long double x, long double y, long double z,
    long double offX, long double offY, long double rotZ
);

extern Links *linksAlloc(ulong total);

#endif //INTERLOCK_OBJ_WRITE_H
