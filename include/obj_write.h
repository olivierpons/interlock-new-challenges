//
// © Olivier Pons / HQF Development - 16/03/2022.
//

#ifndef INTERLOCK_OBJ_WRITE_H
#define INTERLOCK_OBJ_WRITE_H

/* S_W() = "Sprintf() then Write = fputs() "*/
#define S_W(h, st, ...)  sprintf((st), __VA_ARGS__); fputs((st), (h));

/* W_P() = "Write Point()"*/
#define W_P(h, st, x, y, z)  S_W(h, st, "v %Lf %Lf %Lf\n", (x), (y), (z))

/* W_O() = "Write Object()"*/
#define W_O(h, st, comment, name, ref, material) \
        S_W(h, st, "\n# "comment":\n") \
        S_W(h, st, "g "name"\n", ref)     \
        S_W(h, st, "usemtl "material"\n")

/* W_O() = "Write Link()"*/
#define W_LINK(h, tmp, r1, r2, r4, r3) \
        S_W(h, tmp, "f %lu//1 %lu//1 %lu//1 %lu//1\n", r1, r2, r4, r3)

        /* one-liner for a (Pos *) pointer to assign x, y, z: */
#define SET_XYZ(ptr, i, vx, vy, vz) \
    (ptr)[(i)].x = (vx); \
    (ptr)[(i)].y = (vy); \
    (ptr)[(i)].z = (vz);

extern void objWriteFaceRadius(
    FILE *fOut, ulong *ref,
    long double x, long double y, long double z,
    long double radius, long double thick, uint totalPoints
);

extern void objWriteFaceSimple(
    FILE *fOut, ulong *pRef,
    long double x, long double y, long double z,
    long double offX, long double offY, long double rotZ
);

#endif //INTERLOCK_OBJ_WRITE_H
