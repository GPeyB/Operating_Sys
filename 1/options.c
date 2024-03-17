#include <stdio.h>
#include <stdlib.h>

#include "options.h"
#include "util.h"

#define MAX_OPTIONS 10

Options *options_create() {
    Options *options = (Options *)malloc(sizeof(Options));
    options->nrOptions = 0;
    options->options = (char **)malloc(sizeof(char *) * MAX_OPTIONS);
    return options;
}

void options_destroy(Options **options) {
    Options *o = *options;
    // don't free options, as they're pointers to elements of the token list
    for (int i = 0; i < o->nrOptions; i++) {
        o->options[i] = NULL;
    }
    free(o->options);
    free(o);
    *options = NULL;
}

void options_print(Options *options, int depth) {
    printDepth(depth++);
    printf("Options:");
    for (int i = 0; i < options->nrOptions; i++) {
        printf(" %s", options->options[i]);
    }
    printf("\n");
}
