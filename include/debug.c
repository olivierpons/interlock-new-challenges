//
// Created by olivier on 08/04/2022.
//

#ifdef __linux__
#include <linux/limits.h>
#endif
#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>
#include "debug.h"

static int indent = 0;

static void dbIndent()
{
    for (int i = 0; i < indent; ++i) {
        printf("    ");
    }
}

void dbStart(char *str)
{
    db(str);
    indent++;
}
void dbEnd(char *str)
{
    indent--;
    db(str);
}

void db(char *str)
{
    dbIndent();
    printf("%s", str);
    fflush(stdout);
}

