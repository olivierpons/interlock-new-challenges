//
// © Olivier Pons / HQF Development - 16/03/2022.
//

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
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

void objWritePoints(FILE *fOut, Pos *m, ulong start, ulong end)
{
    char s[255];
    for (ulong i = start; i <end; ++i) {
        W_P(fOut, s, m[i].x, m[i].y, m[i].z)
    }
}

Links *linksAlloc(ulong total) {
    Links *b = malloc(sizeof(*b) + sizeof(Links[total]));
    if (b) {
        b->comment[0] = '\0';
        b->name[0] = '\0';
        b->material[0] = '\0';
        b->total = total;
        memset(b->links, 0, total * sizeof(Links));
    }
    return b;
}

void linksCreate(Links **q, ulong idx, int total, ... )
{
    q[idx] = linksAlloc(total);
    Links *l = q[idx];
    va_list arguments;
    /* Initializing arguments to store all values after total */
    va_start (arguments, total );
    for (ulong i = 0; i < total; i++ ) {
        l->links[i] = va_arg(arguments, ulong);
    }
    va_end (arguments);
}

void objLinksWrite(FILE *fOut, Links *l) {
    char s[255];
    S_W(fOut, s, "f")
    for (ulong i = 0; i < l->total; ++i) {
        S_W(fOut, s, " %lu//1", l->links[i])
    }
    S_W(fOut, s, "\n")
}

void objWriteFaceSimple(
    FILE *fOut, ulong *pRef,
    long double x, long double y, long double z,
    long double offX, long double offY, long double rotZ)
{
    char s[255];
    ulong circlePoints = 8;
    long double radius = 0.20;

    Pos *m;
    m = malloc((
        8 // inner + outer square
        + circlePoints // circle "on" the square
        + circlePoints // circle "off" the square
        + circlePoints // smaller circle "off" the square
        ) * sizeof(Pos));

    const ulong NB_LINKS = 6 + circlePoints;
    Links **q;
    q = malloc(NB_LINKS * sizeof(Links *));

    /* hard-code the 8 points of the 2 squares, relative to x/y/z: */
    SET_XYZ(m, 0, x - 0.45, y - 0.45, z + 0.55)
    SET_XYZ(m, 1, x - 0.45, y + 0.45, z + 0.55)
    SET_XYZ(m, 2, x + 0.45, y + 0.45, z + 0.55)
    SET_XYZ(m, 3, x + 0.45, y - 0.45, z + 0.55)
    SET_XYZ(m, 4, x - 0.50, y - 0.50, z + 0.50)
    SET_XYZ(m, 5, x - 0.50, y + 0.50, z + 0.50)
    SET_XYZ(m, 6, x + 0.50, y + 0.50, z + 0.50)
    SET_XYZ(m, 7, x + 0.50, y - 0.50, z + 0.50)

    // Points of the inner circle: (m+8) = (m + 8 points for the 2 squares) :
    calcRadiusPoints(m+8, x, y, z+1.0, radius, circlePoints);
    // Points of the outer circle:
    calcRadiusPoints(m+8+circlePoints, x, y, z+0.9, radius + 0.1, circlePoints);
    // Points of the circle "on" the square:
    calcRadiusPoints(m+8+(circlePoints*2), x, y, z+0.55, radius + 0.1, circlePoints);

    ulong r = *pRef;

    // All points are calculated -> rotate them:
    rotate(offX, rotZ, offY, m, 8 + (circlePoints*3));

    // first, write *all* points
    objWritePoints(fOut, m, 0, 8 + (circlePoints*3));

    // inner square
    linksCreate(q, 0, 4, r + 1, r + 2, r + 3, r + 4);
    L_OBJ(q[0], "Inner square", "InnerSquare%ld", r, "Orange")
    // outer square
    linksCreate(q, 1, 4, r + 1, r + 2, r + 6, r + 5);
    linksCreate(q, 2, 4, r + 2, r + 3, r + 7, r + 6);
    linksCreate(q, 3, 4, r + 3, r + 4, r + 8, r + 7);
    linksCreate(q, 4, 4, r + 1, r + 5, r + 8, r + 4);
    L_OBJ(q[1], "Outer square", "OuterSquare%ld", r, "Green")

    // Central circle links = all points:
    q[5] = linksAlloc(circlePoints);
    for (ulong i = 0; i < circlePoints; ++i) {
        q[5]->links[i] = r + 9 + i;
    }

    for (int i = 0; i < 6; ++i) {
        if (strlen(q[i]->comment) &&
            strlen(q[i]->name) &&
            strlen(q[i]->material)) {
            L_W(fOut, s, q[i])
        }
        objLinksWrite(fOut, q[i]);
    }

    /* - linking points: from inner to outer - */
    W_OR(fOut, s, "Linking inner to outer", "Circle%ld", r, "Purple")
    for (ulong i = 0; i < circlePoints; ++i) {
        ulong p1 = r + 1 + i + 8;
        ulong p2 = r + 2 + i + 8;
        if ((1 + i) >= circlePoints) {
            // last point = special case:
            // case for 16 points:   f 17//1 16//1 32//1 33//1
            // has to be changed to: f  1//1 16//1 32//1 17//1
            p2 = p2 - circlePoints;
        }
        linksCreate(q, 6+i, 4, p2, p1, p1 + circlePoints, p2 + circlePoints);
    }
    for (ulong i = 0; i < circlePoints; ++i) {
        objLinksWrite(fOut, q[6+i]);
    }

    /* - linking points: from inner to circle on the square (same as above) - */
    W_OR(fOut, s, "Linking outer to circle on the square", "Circle%ld", r, "Red")
    for (ulong i = 0; i < circlePoints; ++i) {
        ulong p1 = r + 1 + i + 8 + circlePoints;
        ulong p2 = r + 2 + i + 8 + circlePoints;
        if ((1 + i) >= circlePoints) {
            p2 = p2 - circlePoints;
        }
        S_W(fOut, s, "f %lu//1 %lu//1 %lu//1 %lu//1\n",
            p2, p1, p1 + circlePoints, p2 + circlePoints)
    }

    Pos *startCircle = m+8+(circlePoints*2);
    printf("--------------------------------------------\n");
    for (ulong i = 0;i < circlePoints; ++i) {
        ulong p1 = r + 1 + i + 8 + (circlePoints * 2);
        ulong p2 = r + 2 + i + 8 + (circlePoints * 2);
        long double sX = startCircle[i].x;
        long double sY = startCircle[i].y;
        long double angle = 2 * atanl(sY / ( x + sqrtl( (sX*sX) + (sY*sY) )));
//        printf("%lu, sX=%.2Lf, sY=%.2Lf\t:\t", i, sX, sY);
        if (sX>=0) { // 4-1 ou 1-2
            if (sY>=0) { // 4-1 ou 1-2 ou 2-3
                if (sX > sY) {  // 1-2
                    printf("1-2 (cond.1)\n");
                    S_W(fOut, s, "f %lu//1 %lu//1 %lu//1\n",
                        p1, r + 1, r + 2)
                } else {
                    printf("4-1 (cond.1)\n");
                }
            } else { // 1-2 ou 2-3
                if (sX > fabsl(sY)) {  // 1-2
                    printf("1-2 (cond.2)\n");
                } else {
                    printf("2-3 (cond.2)\n");
                }
            }
        } else { // sX < 0
            if (sY >= 0) {
                if (sY >= fabsl(sX)) {
                    printf("4-1 (cond.3)\n");
                } else {
                    printf("3-4 (cond.3)\n");
                }
            } else { // sX < 0 and sY < 0
                if (sY >= sX) {
                    printf("3-4 (cond.4)\n");
                } else {
                    printf("2-3 (cond.4)\n");
                }
            }
        }
        /**
         * ┌───────────────────────┬───────────────────────┐ *
         * │ 4                     │                     1 │ *
         * │                       │                   /   │ *
         * │                      PI/2               /     │ *
         * │                       │               /       │ *
         * │                       │     PI + PI/2 + PI/4  │ *
         * │                       │           or          │ *
         * │                       │      2PI - PI/4       │ *
         * │                       │       /               │ *
         * │                       │     /                 │ *
         * │                       │   /                   │ *
         * │                       │ /                     │ *
         * │                center │                       │ *
         * ├─── 0 ─────────────────┼──────────────── 0 ────┤ *
         * │                       │                       │ *
         * │                       │                       │ *
         * │                   /   │                       │ *
         * │                 /     │                       │ *
         * │               /       │                       │ *
         * │             /         │                       │ *
         * │           /           │                       │ *
         * │         /             │                       │ *
         * │                    -PI/2                      │ *
         * │     /                 │                       │ *
         * │   /                   │                       │ *
         * │ 3                     │                     2 │ *
         * └───────────────────────┴───────────────────────┘ *
         */
    }

    for (ulong i = 0; i < NB_LINKS; ++i) {
        free(q[i]);
    }
    free(q);

    free(m);

    *pRef += (8 + (circlePoints*3));

    /* write a plug */
}
