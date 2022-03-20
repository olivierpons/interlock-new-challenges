//
// © Olivier Pons / HQF Development - 16/03/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "obj_write.h"
#include "3d.h"

void rotate(double pitch, double roll, double yaw, Pos m[], uint nb) {
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
        double px = m[i].x;
        double py = m[i].y;
        double pz = m[i].z;
        m[i].x = Axx*px + Axy*py + Axz*pz;
        m[i].y = Ayx*px + Ayy*py + Ayz*pz;
        m[i].z = Azx*px + Azy*py + Azz*pz;
    }
}

ulong objWriteCirclePoints(
    FILE *f_out, ulong ref,
    double center_x, double center_y, double center_z, double radius,
    uint nb_points)
{
    char s[255];
    double x, y, step = 360.0/nb_points, angle = 0.0;
    while (angle < 360.0) {
        x = center_x + radius * cos(angle / DEG2RAD);
        y = center_y + radius * sin(angle / DEG2RAD);
        s_w(f_out, s, "v %lf %lf %lf\n", x, y, center_z)
        angle += step;
        ref++;
    }
    return ref;
}

ulong objWriteFaceRadius(
    FILE *f_out, ulong ref,
    double centerX, double centerY, double centerZ, double radius,
    uint totalPoints)
{
    Pos *m;
    char s[255];
    double x, y, step = 360.0 / totalPoints, angle;
    ulong done = 0;
    {
        uint nbPoints = 0;

        m = malloc(totalPoints * sizeof(Pos) * 2);
        // First points = points of the inner circle:
        angle = 0.0;
        while (angle < 360.0) {
            x = centerX + radius * cos(angle / DEG2RAD);
            y = centerY + radius * sin(angle / DEG2RAD);
            m[nbPoints].x = x;
            m[nbPoints].y = y;
            m[nbPoints].z = centerZ;
            angle += step;
            nbPoints++;
        }
        // First points = points of the outer circle:
        radius += 0.2;
        angle = 0.0;
        while (angle < 360.0) {
            x = centerX + radius * cos(angle / DEG2RAD);
            y = centerY + radius * sin(angle / DEG2RAD);
            m[nbPoints].x = x;
            m[nbPoints].y = y;
            m[nbPoints].z = centerZ;
            angle += step;
            nbPoints++;
        }
//        for (ulong i = 0; i < nbPoints; ++i) {
//            printf("v %lf %lf %lf\n", m[i].x,m[i].y, m[i].z);
//            s_w(f_out, s, "v %lf %lf %lf\n", m[i].x,m[i].y, m[i].z)
//        }
        free(m);
    }

    printf("------------------------------------------------------\n");
    s_w(f_out, s, "# Central circle:\n")
    s_w(f_out, s, "g Circle%ld\n", ref)
    s_w(f_out, s, "usemtl Purple\n")
    done = objWriteCirclePoints(
        f_out, done, centerX, centerY, centerZ, radius, totalPoints
    );

    s_w(f_out, s, "# Round circle:\n")
    s_w(f_out, s, "g CircleRound%ld\n", ref)
    s_w(f_out, s, "usemtl Orange\n")
    done = objWriteCirclePoints(
        f_out, done, centerX, centerY, centerZ, radius + 0.2, totalPoints
    );
    /* - linking points - */
    s_w(f_out, s, "# Central circle:\n")
    s_w(f_out, s, "g Circle%ld\n", ref)
    s_w(f_out, s, "usemtl Purple\n")
    ulong half = done / 2;
    for (ulong i = 0; i < half; ++i) {
        ulong p1 = ref + 1 + i;
        ulong p2 = ref + 2 + i;
        if ((1 + i) == half) {
            // last point: special case
            // bad old case for 16 points: f 17//1 16//1 32//1 33//1
            // should be changed to:       f 1//1 16//1 32//1 17//1
            p2 = p2 - half;
        }
        s_w(f_out, s, "f %lu//1 %lu//1 %lu//1 %lu//1\n",
            p2, p1, p1 + half, p2 + half)
    }
    s_w(f_out, s, "\n")
    return ref + done;
}

ulong objWriteFaceSimple(
    FILE *f_out, ulong *ref,
    double x, double y, double z,
    double off_x, double off_y, double rot_z)
{
    char s[255];
    Pos m[] = {
        {-0.5, -0.5, 0.5},
        {-0.5, +0.5, 0.5},
        {+0.5, +0.5, 0.5},
        {+0.5, -0.5, 0.5},
        {-0.45, -0.45, 0.55},
        {-0.45, +0.45, 0.55},
        {+0.45, +0.45, 0.55},
        {+0.45, -0.45, 0.55},
    };
    rotate(off_x / DEG2RAD, rot_z / DEG2RAD, off_y / DEG2RAD, m, 8);

    ulong i = *ref;
    s_w(f_out, s, "g Borders%ld\n", i)
    s_w(f_out, s, "usemtl Green\n")

    s_w(f_out, s, "# ref = %ld\n", i)
    s_w(f_out, s, "v %lf %lf %lf\n", x + m[0].x, y  + m[0].z, z + m[0].z)
    s_w(f_out, s, "v %lf %lf %lf\n", x + m[1].x, y  + m[1].z, z + m[1].z)
    s_w(f_out, s, "v %lf %lf %lf\n", x + m[2].x, y  + m[2].z, z + m[2].z)
    s_w(f_out, s, "v %lf %lf %lf\n", x + m[3].x, y  + m[3].z, z + m[3].z)

    s_w(f_out, s, "v %lf %lf %lf\n", x + m[4].x, y  + m[4].y, z + m[4].z)
    s_w(f_out, s, "v %lf %lf %lf\n", x + m[5].x, y  + m[5].y, z + m[5].z)
    s_w(f_out, s, "v %lf %lf %lf\n", x + m[6].x, y  + m[6].y, z + m[6].z)
    s_w(f_out, s, "v %lf %lf %lf\n", x + m[7].x, y  + m[7].y, z + m[7].z)

    s_w(f_out, s, "f %lu//1 %lu//1 %lu//1 %lu//1\n", i+1, i+2, i+6, i+5)
    s_w(f_out, s, "f %lu//1 %lu//1 %lu//1 %lu//1\n", i+2, i+3, i+7, i+6)
    s_w(f_out, s, "f %lu//1 %lu//1 %lu//1 %lu//1\n", i+3, i+4, i+8, i+7)
//    s_w(f_out, s, "f %lu//1 %lu//1 %lu//1 %lu//1\n", i+1, i+5, i+8, i+4)

    s_w(f_out, s, "g Center%ld\n", i)
    s_w(f_out, s, "usemtl Orange\n")
    s_w(f_out, s, "f %lu//1 %lu//1 %lu//1 %lu//1\n", i+5, i+6, i+7, i+8)

    return *ref += 8;
}
