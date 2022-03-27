//
// © Olivier Pons / HQF Development - 12/03/2022.
//

#ifndef INTERLOCK_CUSTOM_TYPES_H
#define INTERLOCK_CUSTOM_TYPES_H

typedef unsigned char ubyte;

char os_char_separator =
#ifdef _WIN32
    '\\';
#else
    '/';
#endif

char *os_str_separator =
#ifdef _WIN32
    "\\";
#else
    "/";
#endif

#ifdef _WIN32
typedef unsigned long ulong;
typedef unsigned int uint;
#endif

#endif //INTERLOCK_CUSTOM_TYPES_H
