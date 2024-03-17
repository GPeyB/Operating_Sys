#ifndef BUILTIN_H
#define BUILTIN_H

#include <stdbool.h>

#include "options.h"

typedef struct BuiltIn {
    char *name;
    Options *options;
} BuiltIn;

BuiltIn *builtin_create();
void builtin_destroy(BuiltIn **builtIn);
void builtin_print(BuiltIn *builtIn, int depth);
int builtin_execute(BuiltIn *builtIn);

#endif // BUILTIN_H
