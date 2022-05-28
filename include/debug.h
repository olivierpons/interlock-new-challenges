/**
 * Author: Olivier Pons / HQF Development
 * Creation: 08/04/2022
 */

#ifndef INTERLOCK_DEBUG_H
#define INTERLOCK_DEBUG_H

#ifdef __linux__
#include <linux/limits.h>
#endif

#define dbs(...) { char str[PATH_MAX]; sprintf(str, __VA_ARGS__); db(str); }

extern void dbStart(char *str);
extern void dbEnd(char *str);
extern void db(char *str);

#endif //INTERLOCK_DEBUG_H
