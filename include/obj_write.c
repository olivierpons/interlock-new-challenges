//
// © Olivier Pons / HQF Development - 16/03/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>
#include <time.h>
#include <math.h>
#include "obj_write.h"

void rotate(double pitch, double roll, double yaw, double m[][3], uint nb) {
    double cos_a = cos(yaw);
    double sin_a = sin(yaw);

    double cos_b = cos(pitch);
    double sin_b = sin(pitch);

    double cos_c = cos(roll);
    double sin_c = sin(roll);

    double Axx = cos_a*cos_b;
    double Axy = cos_a*sin_b*sin_c - sin_a*cos_c;
    double Axz = cos_a*sin_b*cos_c + sin_a*sin_c;

    double Ayx = sin_a*cos_b;
    double Ayy = sin_a*sin_b*sin_c + cos_a*cos_c;
    double Ayz = sin_a*sin_b*cos_c - cos_a*sin_c;

    double Azx = -sin_b;
    double Azy = cos_b*sin_c;
    double Azz = cos_b*cos_c;

    for (uint i = 0; i <nb; i++) {
        double px = m[i][0];
        double py = m[i][1];
        double pz = m[i][2];
        m[i][0] = Axx*px + Axy*py + Axz*pz;
        m[i][1] = Ayx*px + Ayy*py + Ayz*pz;
        m[i][2] = Azx*px + Azy*py + Azz*pz;
    }
}

ulong obj_write_face(
    FILE *f_out, ulong ref,
    double x, double y, double z,
    double off_x, double off_y, double rot_z
) {
    char s[255];
    double m[][3] = {
        {-0.5, -0.5, 0.5},
        {-0.5, +0.5, 0.5},
        {+0.5, +0.5, 0.5},
        {+0.5, -0.5, 0.5},
        {-0.45, -0.45, 0.55},
        {-0.45, +0.45, 0.55},
        {+0.45, +0.45, 0.55},
        {+0.45, -0.45, 0.55},
    };
    printf("Before:\n");
    for (int i = 0; i < 4; ++i) {
        printf("%lf/%lf/%lf\n", m[i][0], m[i][1], m[i][2]);
    }
    rotate(0, rot_z/DEG2RAD, 0, m, 8);
    printf("After:\n");
    for (int i = 0; i < 4; ++i) {
        printf("%lf/%lf/%lf\n", m[i][0], m[i][1], m[i][2]);
    }

    s_w(f_out, s, "g Borders%ld\n", ref);
    s_w(f_out, s, "usemtl Green\n");

    s_w(f_out, s, "v %lf %lf %lf\n", x + m[0][0], y  + m[0][1], z + m[0][2])
    s_w(f_out, s, "v %lf %lf %lf\n", x + m[1][0], y  + m[1][1], z + m[1][2])
    s_w(f_out, s, "v %lf %lf %lf\n", x + m[2][0], y  + m[2][1], z + m[2][2])
    s_w(f_out, s, "v %lf %lf %lf\n", x + m[3][0], y  + m[3][1], z + m[3][2])

    s_w(f_out, s, "v %lf %lf %lf\n", x + m[4][0], y  + m[4][1], z + m[4][2])
    s_w(f_out, s, "v %lf %lf %lf\n", x + m[5][0], y  + m[5][1], z + m[5][2])
    s_w(f_out, s, "v %lf %lf %lf\n", x + m[6][0], y  + m[6][1], z + m[6][2])
    s_w(f_out, s, "v %lf %lf %lf\n", x + m[7][0], y  + m[7][1], z + m[7][2])

    s_w(f_out, s, "f %lu//1 %lu//1 %lu//1 %lu//1\n", ref + 1, ref + 2, ref + 6, ref + 5)
    s_w(f_out, s, "f %lu//1 %lu//1 %lu//1 %lu//1\n", ref + 2, ref + 3, ref + 7, ref + 6)
    s_w(f_out, s, "f %lu//1 %lu//1 %lu//1 %lu//1\n", ref + 3, ref + 4, ref + 8, ref + 7)
    s_w(f_out, s, "f %lu//1 %lu//1 %lu//1 %lu//1\n", ref + 1, ref + 5, ref + 8, ref + 4)

    s_w(f_out, s, "g Center%ld\n", ref);
    s_w(f_out, s, "usemtl Orange\n");
    s_w(f_out, s, "f %lu//1 %lu//1 %lu//1 %lu//1\n", ref + 5, ref + 6, ref + 7, ref + 8)

    return ref + 8;
}
