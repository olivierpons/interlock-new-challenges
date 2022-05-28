/**
 * Author: Olivier Pons / HQF Development
 * Creation: 27/03/2022
 */
#include "custom_types.h"

char os_char_separator =
#ifdef _WIN32
    '\\';
#else
    '/';
#endif

char *os_str_separator
#ifdef _WIN32
    = "\\";
#else
    = "/";
#endif
