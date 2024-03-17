#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtin.h"
#include "options.h"
#include "shared.h"
#include "util.h"

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

void builtin_execute(BuiltIn *builtIn) {
    if (strcmp(builtIn->name, "exit") == 0) {
        g_status = 0;
        g_exitShell = true;
    } else if (strcmp(builtIn->name, "status") == 0) {
        printf("The most recent exit code is: %d\n", g_status);
    } else if (strcmp(builtIn->name, "cd") == 0) {
        if (builtIn->options->size > 0) {
            if (chdir(builtIn->options->options[0]) != 0) {
                fprintf(stderr, "cd: %s: No such file or directory\n", builtIn->options->options[0]);
                g_status = 1;
            } else {
                g_status = 0;
            }
        } else {
            fprintf(stderr, "cd: missing argument\n");
            g_status = 1;
        }
    }
}
