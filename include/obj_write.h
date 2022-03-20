//
// © Olivier Pons / HQF Development - 16/03/2022.
//

#ifndef INTERLOCK_OBJ_WRITE_H
#define INTERLOCK_OBJ_WRITE_H

/* S_W() = "Sprintf() then Write = fputs() "*/
#define S_W(h, st, ...)  sprintf((st), __VA_ARGS__); fputs((st), (h));
/* W_P() = "Write Point()"*/
#define W_P(h, st, x, y, z)  S_W(h, st, "v %lf %lf %lf\n", (x), (y), (z))
/* W_O() = "Write Object()"*/
#define W_O(h, st, comment, name, ref, material) \
        S_W(h, st, "\n# "comment":\n") \
        S_W(h, st, "g "name"\n", ref)     \
        S_W(h, st, "usemtl "material"\n")
#define DEG2RAD (360.0/M_PI/2.0)

extern ulong objWriteFaceRadius(
    FILE *fOut, ulong ref,
    double centerX, double centerY, double centerZ, double radius,
    uint totalPoints
);

extern ulong objWriteFaceSimple(
    FILE *fOut, ulong *pRef,
    double x, double y, double z,
    double offX, double offY, double rotZ
);

#endif //INTERLOCK_OBJ_WRITE_H
