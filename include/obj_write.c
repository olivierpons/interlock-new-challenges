/**
 * Author: Olivier Pons / HQF Development
 * Creation: 16/03/2022
 */
#ifdef __linux__
#include <linux/limits.h>
#endif

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "obj_write.h"
#include "3d.h"
#include "cube.h"
#include "world.h"

void rotate(
    long double pitch, long double roll, long double yaw, Coord *m, uint nb
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
    Coord *m, long double x, long double y, long double z,
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

void objWritePoints(FILE *fOut, Coord *m, ulong start, ulong end)
{
    char s[255];
    for (ulong i = start; i <end; ++i) {
        // S_W(fOut, s, "# point %lu\n", i + 1)
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

ulong closestPoint(Coord p, Coord *pPos, ulong nbPoints, ulong base) {
    ulong result = base;  // default = first point = base + 0 = 0

    long double x2 = (pPos[0].x - p.x); x2 = x2 * x2;
    long double y2 = (pPos[0].y - p.y); y2 = y2 * y2;
    long double z2 = (pPos[0].z - p.z); z2 = z2 * z2;
    long double distance = sqrtl(x2+y2+z2);
    long double maxDistance = distance;
    for (ulong i = 1; i < nbPoints; ++i) {
        x2 = (pPos[i].x - p.x); x2 = x2 * x2;
        y2 = (pPos[i].y - p.y); y2 = y2 * y2;
        z2 = (pPos[i].z - p.z); z2 = z2 * z2;
        distance = sqrtl(x2+y2+z2);
        if (distance < maxDistance) {
            maxDistance = distance;
            result = base + i;
        }
    }
//    printf("result = %lu\n", result);
    return result;
}

void objWriteLinks(FILE *fOut, Links **pLinks, ulong nbLinks)
{
    char s[255];
    // Loop on links and write them:
    for (ulong i = 0; i < nbLinks; ++i) {
        if (pLinks[i]) {
            if (strlen(pLinks[i]->comment) &&
                strlen(pLinks[i]->name) &&
                strlen(pLinks[i]->material)) {
                L_W(fOut, s, pLinks[i])
            }
            objLinksWrite(fOut, pLinks[i]);
        } else {  /* should never happen */
            printf("?? WTF write? %lu\n", i);
        }
    }
}
void objWriteSimpleFace(
    FILE *fOut, ulong *pRef,
    long double offX, long double offY, long double offZ,
    long double rotX, long double rotY, long double rotZ)
{
    const ulong NB_POINTS = 8; // inner + outer square
    Coord *m = malloc(NB_POINTS * sizeof(Coord));
    if (!m) { /* should never happen, I should handle this better... */
        exit(-1);
    }
    // region - Points computations -
    /* Hard-coded points of the squares (offsets done later, AFTER rotation): */
    /* Central square: */
    SET_XYZ(m, 0, -0.45, -0.45, +0.55)
    SET_XYZ(m, 1, -0.45, +0.45, +0.55)
    SET_XYZ(m, 2, +0.45, +0.45, +0.55)
    SET_XYZ(m, 3, +0.45, -0.45, +0.55)
    /* Bigger square: */
    SET_XYZ(m, 4, -0.50, -0.50, +0.50)
    SET_XYZ(m, 5, -0.50, +0.50, +0.50)
    SET_XYZ(m, 6, +0.50, +0.50, +0.50)
    SET_XYZ(m, 7, +0.50, -0.50, +0.50)
    // endregion
    // inner square

    const ulong NB_LINKS = 5;
    Links **q = calloc(NB_LINKS, sizeof(Links *));
    if (!q) { /* should never happen, I should handle this better... */
        exit(-1);
    }

    // region - Links: square -
    ulong r = *pRef;
    linksCreate(q, 0, 4, r + 1, r + 2, r + 3, r + 4);
    L_OBJ(q[0], "Inner square", "InnerSquare%ld", r, "Orange")

    // outer square
    linksCreate(q, 1, 4, r + 1, r + 2, r + 6, r + 5);
    linksCreate(q, 2, 4, r + 2, r + 3, r + 7, r + 6);
    linksCreate(q, 3, 4, r + 3, r + 4, r + 8, r + 7);
    linksCreate(q, 4, 4, r + 1, r + 5, r + 8, r + 4);
    L_OBJ(q[1], "Outer square", "OuterSquare%ld", r, "Green")
    // endregion - Links: square -

    // All points and links are calculated -> rotate all:
    rotate(rotX, rotZ, rotY, m, NB_POINTS);
    /* After rotation, apply offsets to all points */
    Coord *tmp = m;
    for (ulong i = 0; i < NB_POINTS; ++i) {
        tmp->x += offX;
        tmp->y += offY;
        tmp->z += offZ;
        tmp++;
    }
    // first, write *all* points
    objWritePoints(fOut, m, 0, NB_POINTS);
    // then loop on links and write them:
    objWriteLinks(fOut, q, NB_LINKS);
    // region - Free memory -
    for (ulong i = 0; i < NB_LINKS; ++i) {
        if (q[i]) {
            free(q[i]);
        } else {
            printf("?? %lu\n", i);
        }
    }
    free(q);
    free(m);
    // endregion - Free memory -
    *pRef += 8;
}
void objWriteFaceWithPlug(
    FILE *fOut, ulong *pRef,
    long double offX, long double offY, long double offZ,
    long double rotX, long double rotY, long double rotZ,
    bool goOutside)
{
    ulong circlePoints = 50;
    long double radius = 0.20;
    const ulong NB_POINTS = (
        8 // inner + outer square
        + circlePoints // circle "on" the square
        + circlePoints // same circle but far the square
        + circlePoints // smaller circle inside the previous
    );

    Coord *m = calloc(NB_POINTS, sizeof(Coord));
    assert(m);

    // region - Points computations -
    /* Hard-coded points of the squares (offsets done later, AFTER rotation): */
    Coord *tmp = m + 8;
    if (goOutside) {
        /* Central square: */
        SET_XYZ(m, 0, -0.45, -0.45, +0.55)
        SET_XYZ(m, 1, -0.45, +0.45, +0.55)
        SET_XYZ(m, 2, +0.45, +0.45, +0.55)
        SET_XYZ(m, 3, +0.45, -0.45, +0.55)
        // Points of the inner circle: (m+8):
        calcRadiusPoints(tmp, 0, 0, 1.0, radius, circlePoints);
        // Points of the outer circle = add circlePoints:
        tmp += circlePoints;
        calcRadiusPoints(tmp, 0, 0, 0.9, radius + 0.1, circlePoints);
        // Points of the circle close to the square = add circlePoints:
        tmp += circlePoints;
        calcRadiusPoints(tmp, 0, 0, 0.65, radius + 0.1, circlePoints);
    } else {
        /* Central square: */
        SET_XYZ(m, 0, -0.45, -0.45, +0.45)
        SET_XYZ(m, 1, -0.45, +0.45, +0.45)
        SET_XYZ(m, 2, +0.45, +0.45, +0.45)
        SET_XYZ(m, 3, +0.45, -0.45, +0.45)
        // Points of the inner circle: (m+8):
        calcRadiusPoints(tmp, 0, 0, 0, radius, circlePoints);
        // Points of the outer circle = add circlePoints:
        tmp += circlePoints;
        calcRadiusPoints(tmp, 0, 0, 0.1, radius + 0.1, circlePoints);
        // Points of the circle close to the square = add circlePoints:
        tmp += circlePoints;
        calcRadiusPoints(tmp, 0, 0, 0.45, radius + 0.1, circlePoints);
    }
    /* Bigger square: */
    SET_XYZ(m, 4, -0.50, -0.50, +0.50)
    SET_XYZ(m, 5, -0.50, +0.50, +0.50)
    SET_XYZ(m, 6, +0.50, +0.50, +0.50)
    SET_XYZ(m, 7, +0.50, -0.50, +0.50)
    // endregion

    /**
     * NB_LINKS to do:
     *
     * 0     - 1 list of 4 points for the central square
     *
     * 1 ..4 - 4 lists of 4 points for the square around the central square
     *
     * 5     - 1 list linking all points of the central circle
     *
     * 6 .. (6 + circlePoints) - [circlePoints] lists of 4 points:
     *                                          2 inner circle -> 2 outer circle
     *
     * (6 + circlePoints) .. (6 + (circlePoints * 2)) - same as above but:
     *                         2 outer circle -> 2 on circle against the square
     *
     * (6 + (circlePoints * 2)) .. (6 + (circlePoints * 2) + (circlePoints + 4))
     *    - for each point, 1 list connects 3 points: 1 circle point (a)
     *      with the next circle point (b) and 1 point of the square which
     *      is the closest point of the middle of (a) and (b)
     *    - 4 lists: 4 times 1 side of the square (2 points) with 1 circle point
     */

    const ulong NB_LINKS = 6 + (circlePoints*2) + (circlePoints + 4);
    Links **q;
    q = calloc(NB_LINKS, sizeof(Links *));
    assert(q);

    // region - Links: square: 0..4 -
    ulong r = *pRef;

    // inner square
//    linksCreate(q, 0, 4, r + 1, r + 2, r + 3, r + 4);
    linksCreate(q, 0, 4, r + 1, r + 1, r + 1, r + 1);
    L_OBJ(q[0], "Inner square", "InnerSquare%ld", r, "Orange")

    // outer square
    linksCreate(q, 1, 4, r + 1, r + 2, r + 6, r + 5);
    linksCreate(q, 2, 4, r + 2, r + 3, r + 7, r + 6);
    linksCreate(q, 3, 4, r + 3, r + 4, r + 8, r + 7);
    linksCreate(q, 4, 4, r + 1, r + 5, r + 8, r + 4);
    L_OBJ(q[1], "Outer square", "OuterSquare%ld", r, "Green")
    // endregion - Links: square -
    // region - Links: central circle (one list): 5 -
    // Central circle links = all points
    q[5] = linksAlloc(circlePoints);
    for (ulong i = 0; i < circlePoints; ++i) {
        q[5]->links[i] = r + 9 + i;
    }
    L_OBJ(q[5], "Central circle", "CentralCircle%ld", r, "Orange")
    // endregion - Links: central circle -
    // region - Links: from inner to outer circle: 6..6 + c.Points -
    /* - linking points: from inner to outer - */
    ulong base = 6;
    for (ulong i = 0; i < circlePoints; ++i) {
        ulong p1 = r + 1 + i + 8;
        ulong p2 = r + 2 + i + 8;
        if ((1 + i) >= circlePoints) {
            // last point = special case:
            // case for 16 points:   f 17//1 16//1 32//1 33//1
            // has to be changed to: f  1//1 16//1 32//1 17//1
            p2 = p2 - circlePoints;
        }
        linksCreate(q, base+i, 4, p2, p1, p1 + circlePoints, p2 + circlePoints);
    }
    L_OBJ(q[base], "Linking inner to outer", "Circle%ld", r, "Purple")
    // endregion
    // region - Links: outer circle -> circle on the square: 6 + c.Points .. 6 + (c.Points*2) -
    /* - linking points: from inner to circle on the square (same as above) - */
    base = 6 + circlePoints;
    for (ulong i = 0; i < circlePoints; ++i) {
        ulong p1 = r + 1 + i + 8 + circlePoints;
        ulong p2 = r + 2 + i + 8 + circlePoints;
        if ((1 + i) >= circlePoints) {
            p2 = p2 - circlePoints;
        }
        linksCreate(q, base+i, 4, p2, p1, p1 + circlePoints, p2 + circlePoints);
    }
    L_OBJ(q[base], "Linking outer to circle on the square", "Circle%ld", r, "Red")
    // endregion
    // region - Links: circle on the square -> square
    base = 6 + (circlePoints*2);
    ulong cStart = 8 + (circlePoints * 2);
    ulong current = base;
    ulong oldClosest, firstClosest;
    for (ulong i = 0; i < circlePoints; ++i) {
        Coord p1 = m[cStart + i];
        Coord p2 = i != (circlePoints - 1) ? m[cStart + i + 1] : m[cStart];
        Coord p3;
        p3.x = (p1.x + p2.x)/2;
        p3.y = (p1.y + p2.y)/2;
        p3.z = (p1.z + p2.z)/2;
        ulong newClosest = closestPoint(p3, m, 4, 0);
        if (i == 0) {
            firstClosest = newClosest;
        }
        linksCreate(q, current++, 3,
                    r + cStart + i + 1,
                    i != (circlePoints-1) ? r + cStart + i + 2 : r + cStart +  1,
                    r + newClosest + 1);
        if (i && oldClosest != newClosest) {
            linksCreate(q, current++, 3,
                        r + cStart + i + 1,
                        r + newClosest + 1,
                        r + oldClosest + 1);
        }
        oldClosest = newClosest;
        if (i == (circlePoints-1)) {
            linksCreate(q, current++, 3,
                        r + cStart + 1,
                        r + firstClosest + 1,
                        r + newClosest + 1);
        }
    }
    L_OBJ(q[base], "Join circle to square!", "JoinCircleSquare%ld", r, "Blue")
    // endregion

    // All points and links are calculated -> rotate all:
    rotate(rotX, rotZ, rotY, m, NB_POINTS);
    /* After rotation, apply offsets to all points */
    tmp = m;
    for (ulong i = 0; i < NB_POINTS; ++i) {
        tmp->x += offX;
        tmp->y += offY;
        tmp->z += offZ;
        tmp++;
    }

    // first, write *all* points
    objWritePoints(fOut, m, 0, NB_POINTS);
    // then loop on links and write them:
    objWriteLinks(fOut, q, NB_LINKS);
    // region - Free memory -
    for (ulong i = 0; i < NB_LINKS; ++i) {
        if (q[i]) {
            free(q[i]);
        } else {
            printf("?? %lu\n", i);
        }
    }
    free(q);
    free(m);
    // endregion - Free memory -

    *pRef += (8 + (circlePoints*3));
}

void objWriteFace(
    FILE *fOut, ulong *pRef, Face f,
    long double x, long double y, long double z,
    long double rotX, long double rotY, long double rotZ
) {
    switch (f) {
        case F_WALL:
//            printf(
//                "objWriteSimpleFace: "
//                "(%.2Lf, %.2Lf, %.2Lf) "
//                "rot (%.2Lf, %.2Lf, %.2Lf)\n", x, y, z, rotX, rotY, rotZ
//            );
            objWriteSimpleFace(fOut, pRef, x, y, z, rotX, rotY, rotZ);
            break;
        case F_HOLE:
        case F_PLUG:
//            printf(
//                "objWriteFaceWithPlug: "
//                "(%.2Lf, %.2Lf, %.2Lf) "
//                "rot (%.2Lf, %.2Lf, %.2Lf) (%i)\n",
//                x, y, z, rotX, rotY, rotZ, f == F_PLUG
//            );
            objWriteFaceWithPlug(
                fOut, pRef, x, y, z, rotX, rotY, rotZ, f == F_PLUG
            );
            break;
        case F_LINK:
        case F_EMPTY:
        default:
            break;
    }
}
void objWriteCube(
    FILE *fOut, ulong *pRef, Cube* c,
    long double x, long double y, long double z)
{
    objWriteFace(fOut, pRef, c->f, x, y, z, 0.0, 0.0, 0); // front
    objWriteFace(fOut, pRef, c->b, x, y, z, M_PI, 0, 0); // back
    objWriteFace(fOut, pRef, c->s, x, y, z, 0.0, 0.0, M_PI / 2); // bottom
    objWriteFace(fOut, pRef, c->n, x, y, z, 0.0, 0.0, -M_PI / 2); // top
    objWriteFace(fOut, pRef, c->e, x, y, z, M_PI/2, 0, 0.0); // right
    objWriteFace(fOut, pRef, c->w, x, y, z, -M_PI/2, 0, 0.0); // left
}

void objWriteFullWorld(Cube* world)
{
    char *dst_path = "../3d-obj/";
    char dstExpandedPath[PATH_MAX + 1];
    char *ptr;
#ifdef __linux__
    ptr = realpath(dst_path, dstExpandedPath);
#elif _WIN32
    ptr = _fullpath(dstExpandedPath, dst_path, PATH_MAX);
#else
#error "OS not supported!"
#endif
    if (ptr[strlen(ptr)-1] != os_char_separator) {
        strcat(ptr, os_str_separator);
    }
    strcat(ptr, "chest.obj");
    printf("Destination file: %s\n", ptr);

    FILE *f_out;
    f_out = fopen(ptr, "w");
    if (f_out) {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        char s[255];
        S_W(f_out, s,
            "# © Olivier Pons / HQF Development - "
            "%d-%02d-%02d %02d:%02d:%02d\n",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec)
        S_W(f_out, s, "mtllib model.mtl\n")
        S_W(f_out, s, "vn 0 0 -1\n")

        // Obj file output: loop to write all pieces:
        ulong ref = 0;
        for (int zIdx = 0; zIdx < WORLD_SIZE_Z; ++zIdx) {
            for (int yIdx = 0; yIdx < WORLD_SIZE_Y; ++yIdx) {
                for (int xIdx = 0; xIdx < WORLD_SIZE_X; ++xIdx) {
                    long long a = XYZ(xIdx, yIdx, zIdx);
                    Cube c = world[a];
                    if (!c.b) {
                        continue;
                    }
                    /*
                    if (c.b) {
                        printf("%lld (%d/%d/%d): ", a, xIdx, yIdx, zIdx);
                        cubeToStr(c);
                    } */
                    objWriteCube(
                        f_out, &ref, &c,
                        (long double)xIdx, (long double)yIdx, (long double)zIdx
                    );
                }
            }
        }
        fclose(f_out);
    } else {
        printf("? can't write to file -> aborting.\n");
    }
}

