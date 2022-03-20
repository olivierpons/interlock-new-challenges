//
// © Olivier Pons / HQF Development - 16/03/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "obj_write.h"
#include "3d.h"

void rotate(double pitch, double roll, double yaw, Pos m[], uint nb) {
    double cosA = cos(yaw);
    double sin_a = sin(yaw);

    double cosB = cos(pitch);
    double sinB = sin(pitch);

    double cosC = cos(roll);
    double sinC = sin(roll);

    double Axx = cosA * cosB;
    double Axy = cosA * sinB * sinC - sin_a * cosC;
    double Axz = cosA * sinB * cosC + sin_a * sinC;

    double Ayx = sin_a * cosB;
    double Ayy = sin_a * sinB * sinC + cosA * cosC;
    double Ayz = sin_a * sinB * cosC - cosA * sinC;

    double Azx = -sinB;
    double Azy = cosB * sinC;
    double Azz = cosB * cosC;

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
    FILE *fOut, ulong ref,
    double centerX, double centerY, double centerZ, double radius,
    uint totalPoints)
{
    char s[255];
    double x, y, step = 360.0 / totalPoints, angle = 0.0;
    while (angle < 360.0) {
        x = centerX + radius * cos(angle / DEG2RAD);
        y = centerY + radius * sin(angle / DEG2RAD);
        W_P(fOut, s, x, y, centerZ)
        angle += step;
        ref++;
    }
    return ref;
}

ulong objWriteFaceRadius(
    FILE *fOut, ulong ref,
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
//            S_W(fOut, s, "v %lf %lf %lf\n", m[i].x,m[i].y, m[i].z)
//        }
        free(m);
    }

    printf("------------------------------------------------------\n");

    W_O(fOut, s, "Central", "Circle%ld", ref, "Purple")
    done = objWriteCirclePoints(
        fOut, done, centerX, centerY, centerZ, radius, totalPoints
    );

    W_O(fOut, s, "Round circle", "CircleRound%ld", ref, "Orange")
    done = objWriteCirclePoints(
        fOut, done, centerX, centerY, centerZ, radius + 0.2, totalPoints
    );
    /* - linking points - */
    W_O(fOut, s, "Central circle", "Circle%ld", ref, "Purple")
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
        S_W(fOut, s, "f %lu//1 %lu//1 %lu//1 %lu//1\n",
            p2, p1, p1 + half, p2 + half)
    }
    S_W(fOut, s, "\n")
    return ref + done;
}

ulong objWriteFaceSimple(
    FILE *fOut, ulong *pRef,
    double x, double y, double z,
    double offX, double offY, double rotZ)
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
    rotate(offX / DEG2RAD, rotZ / DEG2RAD, offY / DEG2RAD, m, 8);

    ulong r = *pRef;
    W_O(fOut, s, "Borders", "Borders%ld", r, "Green")
    S_W(fOut, s, "# pRef = %ld\n", r)

    W_P(fOut, s, x + m[0].x, y + m[0].z, z + m[0].z)
    W_P(fOut, s, x + m[1].x, y + m[1].z, z + m[1].z)
    W_P(fOut, s, x + m[2].x, y + m[2].z, z + m[2].z)
    W_P(fOut, s, x + m[3].x, y + m[3].z, z + m[3].z)

    W_P(fOut, s, x + m[4].x, y + m[4].y, z + m[4].z)
    W_P(fOut, s, x + m[5].x, y + m[5].y, z + m[5].z)
    W_P(fOut, s, x + m[6].x, y + m[6].y, z + m[6].z)
    W_P(fOut, s, x + m[7].x, y + m[7].y, z + m[7].z)

    S_W(fOut, s, "f %lu//1 %lu//1 %lu//1 %lu//1\n", r + 1, r + 2, r + 6, r + 5)
    S_W(fOut, s, "f %lu//1 %lu//1 %lu//1 %lu//1\n", r + 2, r + 3, r + 7, r + 6)
    S_W(fOut, s, "f %lu//1 %lu//1 %lu//1 %lu//1\n", r + 3, r + 4, r + 8, r + 7)
//    S_W(fOut, s, "f %lu//1 %lu//1 %lu//1 %lu//1\n", r + 1, r + 5, r + 8, r + 4)

    W_O(fOut, s, "Center", "Center%ld", r, "Orange")
    S_W(fOut, s, "f %lu//1 %lu//1 %lu//1 %lu//1\n", r + 5, r + 6, r + 7, r + 8)

    return *pRef += 8;
}
