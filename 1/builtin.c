#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builtin.h"
#include "options.h"
#include "util.h"

extern bool g_exitShell;
extern int g_status;

BuiltIn *builtin_create() {
    BuiltIn *builtIn = (BuiltIn *)malloc(sizeof(BuiltIn));
    builtIn->name = NULL;
    builtIn->options = NULL;
    return builtIn;
}

void builtin_destroy(BuiltIn **builtIn) {
    BuiltIn *b = *builtIn;
    // don't free name, as it's a pointer to an element of the token list
    b->name = NULL;
    if (b->options != NULL)
        options_destroy(&b->options);
    free(b);
    *builtIn = NULL;
}

void builtin_print(BuiltIn *builtIn, int depth) {
    printDepth(depth++);
    printf("BuiltIn:\n");
    printDepth(depth);
    printf("name: %s\n", builtIn->name);
    if (builtIn->options != NULL)
        options_print(builtIn->options, depth);
}

int builtin_execute(BuiltIn *builtIn) {
    int status = 0;

    if (strcmp(builtIn->name, "exit") == 0) {
        g_exitShell = true;
    } else if (strcmp(builtIn->name, "status") == 0) {
        printf("The most recent exit code is: %d\n", g_status);
    }

    return status;
}
