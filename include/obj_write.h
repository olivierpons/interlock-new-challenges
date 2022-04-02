//
// © Olivier Pons / HQF Development - 16/03/2022.
//

#ifndef INTERLOCK_OBJ_WRITE_H
#define INTERLOCK_OBJ_WRITE_H

/* S_W() = "Sprintf() then Write = fputs() "*/
#include "custom_types.h"
#include "cube.h"
#include "3d.h"

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

/* one-liner for a (Coord *) pointer to assign x, y, z: */
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

extern void rotate(
    long double pitch, long double roll, long double yaw, Coord *m, uint nb
);

extern void calcRadiusPoints(
    Coord *m, long double x, long double y, long double z,
    long double radius, uint totalPoints);
extern void objWritePoints(FILE *fOut, Coord *m, ulong start, ulong end);
extern Links *linksAlloc(ulong total);
extern void linksCreate(Links **q, ulong idx, int total, ... );
extern void objLinksWrite(FILE *fOut, Links *l);
extern ulong closestPoint(Coord p, Coord *pPos, ulong nbPoints, ulong base);
extern void objWriteLinks(FILE *fOut, Links **pLinks, ulong nbLinks);
extern void objWriteSimpleFace(
    FILE *fOut, ulong *pRef,
    long double offX, long double offY, long double offZ,
    long double rotX, long double rotY, long double rotZ);
extern void objWriteFaceWithPlug(
    FILE *fOut, ulong *pRef,
    long double offX, long double offY, long double offZ,
    long double rotX, long double rotY, long double rotZ,
    bool goOutside);
extern void objWriteFace(
    FILE *fOut, ulong *pRef, Face f,
    long double x, long double y, long double z,
    long double rotX, long double rotY, long double rotZ
);
extern void objWriteCube(
    FILE *fOut, ulong *pRef, Cube* c,
    long double x, long double y, long double z);
extern void objWriteFullWorld(Cube* world);

#endif //INTERLOCK_OBJ_WRITE_H
