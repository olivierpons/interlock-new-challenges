//
// © Olivier Pons / HQF Development - 16/03/2022.
//

#ifndef INTERLOCK_OBJ_WRITE_H
#define INTERLOCK_OBJ_WRITE_H

/* s_w() = "sprintf() then write = fputs() "*/
#define s_w(h, st, ...)  sprintf((st), __VA_ARGS__); fputs((st), (h));
#define DEG2RAD (360.0/M_PI/2.0)

extern ulong obj_write_face(
    FILE *f_out, ulong ref,
    double x, double y, double z,
    double off_x, double off_y, double rot_z
);

#endif //INTERLOCK_OBJ_WRITE_H
