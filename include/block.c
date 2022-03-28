//
// © Olivier Pons / HQF Development - 12/03/2022.
//
#include <malloc.h>
#include <memory.h>
#include <stdarg.h>
#include <sys/types.h>
#include "block.h"
#include "custom_types.h"

Block *blockCreate(int total) {
    Block *b = malloc(sizeof(*b) + sizeof(Part[total]));
    if (b) {
        b->total = (ubyte)total;
        memset(b->parts, 0, total * sizeof(Part));
        b->parts[0].isMain = true;
    }
    return b;
}
Block *blockCreateWithParts(int total, ... )
{
    Block *b = blockCreate(total);
    va_list arguments;
    /* Initializing arguments to store all values after total */
    va_start(arguments, total);
    for (ulong i = 0; i < total; i++ ) {
        b->parts[i].c.n = va_arg(arguments, Face);
        b->parts[i].c.e = va_arg(arguments, Face);
        b->parts[i].c.s = va_arg(arguments, Face);
        b->parts[i].c.w = va_arg(arguments, Face);
        b->parts[i].c.f = va_arg(arguments, Face);
        b->parts[i].c.b = va_arg(arguments, Face);
        b->parts[i].isMain  = (bool)va_arg(arguments, int);
        b->parts[i].offsetX = (char)va_arg(arguments, int);
        b->parts[i].offsetY = (char)va_arg(arguments, int);
        b->parts[i].offsetZ = (char)va_arg(arguments, int);

    }
    va_end (arguments);
    return b;
}
