//
// © Olivier Pons / HQF Development - 16/03/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "obj_write.h"
#include "3d.h"

void rotate(
    long double pitch, long double roll, long double yaw, Pos *m, uint nb
)
{
    long double cosA = cosl(yaw);
    long double sin_a = sinl(yaw);

    long double cosB = cosl(pitch);
    long double sinB = sinl(pitch);

    long double cosC = cosl(roll);
    long double sinC = sinl(roll);

    long double Axx = cosA * cosB;
    long double Axy = cosA * sinB * sinC - sin_a * cosC;
    long double Axz = cosA * sinB * cosC + sin_a * sinC;

    long double Ayx = sin_a * cosB;
    long double Ayy = sin_a * sinB * sinC + cosA * cosC;
    long double Ayz = sin_a * sinB * cosC - cosA * sinC;

    long double Azx = -sinB;
    long double Azy = cosB * sinC;
    long double Azz = cosB * cosC;

    for (uint i = 0; i <nb; i++) {
        long double px = m[i].x;
        long double py = m[i].y;
        long double pz = m[i].z;
        m[i].x = Axx*px + Axy*py + Axz*pz;
        m[i].y = Ayx*px + Ayy*py + Ayz*pz;
        m[i].z = Azx*px + Azy*py + Azz*pz;
    }
}

void calcRadiusPoints(
    Pos *m, long double x, long double y, long double z,
    long double radius, uint totalPoints)
{
    long double step = 2*M_PI / totalPoints;
    for (int i = 0; i < totalPoints; ++i) {
        // "Inner" points: points of the inner circle:
        m[i].x = x + radius * cosl(i * step);
        m[i].y = y + radius * sinl(i * step);
        m[i].z = z;
    }
}

void objWriteFaceRadius(
    FILE *fOut, ulong *ref,
    long double x, long double y, long double z,
    long double radius, long double thick, uint totalPoints)
{
    Pos *m;
    char s[255];
    ulong r = *ref;

    m = malloc(totalPoints * sizeof(Pos) * 2);

    // Points of the inner circle:
    calcRadiusPoints(m, x, y, z, radius, totalPoints);
    // Points of the outer circle:
    calcRadiusPoints(&(m[totalPoints]), x, y, z, radius + thick, totalPoints);

    for (ulong i = 0; i < totalPoints*2; ++i) {
        S_W(fOut, s, "v %Lf %Lf %Lf\n", m[i].x,m[i].y, m[i].z)
    }

    /* - linking points: from inner to outer - */
    W_O(fOut, s, "Linking inner to outer", "Circle%ld", r, "Purple")
    for (ulong i = 0; i < totalPoints; ++i) {
        ulong p1 = r + 1 + i;
        ulong p2 = r + 2 + i;
        if ((1 + i) >= totalPoints) {
            // last point = special case:
            // old case for 16 points: f 17//1 16//1 32//1 33//1
            // has to be changed to:   f  1//1 16//1 32//1 17//1
            p2 = p2 - totalPoints;
        }
        S_W(fOut, s, "f %lu//1 %lu//1 %lu//1 %lu//1\n",
            p2, p1, p1 + totalPoints, p2 + totalPoints)
    }

    free(m);
    *ref = r + totalPoints*2;
}

void objWritePoints(FILE *fOut, Pos *m, ulong start, ulong end)
{
    char s[255];
    for (ulong i = start; i <end; ++i) {
        W_P(fOut, s, m[i].x, m[i].y, m[i].z)
    }
}
void objWriteFaceSimple(
    FILE *fOut, ulong *pRef,
    long double x, long double y, long double z,
    long double offX, long double offY, long double rotZ)
{
    char s[255];
    ulong circlePoints = 100;
    long double radius = 0.20;

    Pos *m;
    m = malloc((
        8 // inner + outer square
        + circlePoints // circle "on" the square
        + circlePoints // circle "off" the square
        + circlePoints // smaller circle "off" the square
        ) * sizeof(Pos));

    /* hard-code points of the 2 squares, relative to x/y/z: */
    SET_XYZ(m, 0, x - 0.45, y - 0.45, z + 0.55)
    SET_XYZ(m, 1, x - 0.45, y + 0.45, z + 0.55)
    SET_XYZ(m, 2, x + 0.45, y + 0.45, z + 0.55)
    SET_XYZ(m, 3, x + 0.45, y - 0.45, z + 0.55)
    SET_XYZ(m, 4, x - 0.50, y - 0.50, z + 0.50)
    SET_XYZ(m, 5, x - 0.50, y + 0.50, z + 0.50)
    SET_XYZ(m, 6, x + 0.50, y + 0.50, z + 0.50)
    SET_XYZ(m, 7, x + 0.50, y - 0.50, z + 0.50)

    ulong r = *pRef;
    // Points of the inner circle:
    calcRadiusPoints(m+8, x, y, z+1.0, radius, circlePoints);
    // Points of the outer circle:
    calcRadiusPoints(m+8+circlePoints, x, y, z+0.9, radius + 0.1, circlePoints);
    // Points of the circle "on" the square:
    calcRadiusPoints(m+8+(circlePoints*2), x, y, z+0.56, radius + 0.1, circlePoints);

    // All points are calculated -> rotate them:
    rotate(offX, rotZ, offY, m, 8 + (circlePoints*3));

    // first, write *all* points
    objWritePoints(fOut, m, 0, 8 + (circlePoints*3));

    // make the links = quads here:
//    W_O(fOut, s, "Inner square", "InnerSquare%ld", r, "Orange")
//    W_LINK(fOut, s, r + 1, r + 2, r + 3, r + 4)
    W_O(fOut, s, "Outer square", "OuterSquare%ld", r, "Green")
    W_LINK(fOut, s, r + 1, r + 2, r + 6, r + 5)
    W_LINK(fOut, s, r + 2, r + 3, r + 7, r + 6)
    W_LINK(fOut, s, r + 3, r + 4, r + 8, r + 7)
    W_LINK(fOut, s, r + 1, r + 5, r + 8, r + 4)

    // write whole central circle links:
    S_W(fOut, s, "f")
    for (ulong i = 0; i < circlePoints; ++i) {
        S_W(fOut, s, " %lu//1", r + 9 + i)
    }
    S_W(fOut, s, "\n")

    /* - linking points: from inner to outer - */
    W_O(fOut, s, "Linking inner to outer", "Circle%ld", r, "Purple")
    for (ulong i = 0; i < circlePoints; ++i) {
        ulong p1 = r + 1 + i + 8;
        ulong p2 = r + 2 + i + 8;
        if ((1 + i) >= circlePoints) {
            // last point = special case:
            // case for 16 points:   f 17//1 16//1 32//1 33//1
            // has to be changed to: f  1//1 16//1 32//1 17//1
            p2 = p2 - circlePoints;
        }
        S_W(fOut, s, "f %lu//1 %lu//1 %lu//1 %lu//1\n",
            p2, p1, p1 + circlePoints, p2 + circlePoints)
    }

    /* - linking points: from inner to outer - */
    W_O(fOut, s, "Linking inner to outer", "Circle%ld", r, "Red")
    for (ulong i = 0; i < circlePoints; ++i) {
        ulong p1 = r + 1 + i + 8 + circlePoints;
        ulong p2 = r + 2 + i + 8 + circlePoints;
        if ((1 + i) >= circlePoints) {
            p2 = p2 - circlePoints;
        }
        S_W(fOut, s, "f %lu//1 %lu//1 %lu//1 %lu//1\n",
            p2, p1, p1 + circlePoints, p2 + circlePoints)
    }





    free(m);

    *pRef += (8 + (circlePoints*3));

    /* write a plug */
}
