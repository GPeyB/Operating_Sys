#include <stdio.h>
#include <stdlib.h>

#include "redirections.h"
#include "util.h"

Redirections *redirections_create() {
    Redirections *redirections = (Redirections *)malloc(sizeof(Redirections));
    redirections->input = NULL;
    redirections->output = NULL;
    return redirections;
}

void redirections_destroy(Redirections **redirections) {
    Redirections *r = *redirections;
    // don't free input and output, as they're pointers to elements of the token list
    r->input = NULL;
    r->output = NULL;
    free(r);
    *redirections = NULL;
}

void redirections_print(Redirections *redirections, int depth) {
    printDepth(depth++);
    printf("Redirections:\n");
    if (redirections->input != NULL) {
        printDepth(depth);
        printf("input: %s\n", redirections->input);
    }
    if (redirections->output != NULL) {
        printDepth(depth);
        printf("output: %s\n", redirections->output);
    }
}
