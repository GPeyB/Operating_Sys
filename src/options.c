#include <stdio.h>
#include <stdlib.h>

#include "options.h"
#include "util.h"

Options *options_create() {
    Options *options = (Options *)malloc(sizeof(Options));
    options->size = 0;
    options->capacity = INITIAL_NR_OPTIONS;
    options->options = (char **)malloc(sizeof(char *) * INITIAL_NR_OPTIONS);
    return options;
}

void options_destroy(Options **options) {
    Options *o = *options;
    // don't free options, as they're pointers to elements of the token list
    for (int i = 0; i < o->size; i++) {
        o->options[i] = NULL;
    }
    free(o->options);
    free(o);
    *options = NULL;
}

void options_insert(Options *options, char *option, int index) {
    if (options->size == options->capacity) {
        options->capacity *= 2;
        options->options = (char **)realloc(options->options, sizeof(char *) * options->capacity);
    }
    for (int i = options->size; i > index; i--) {
        options->options[i] = options->options[i - 1];
    }
    options->options[index] = option;
    options->size++;
}

void options_append(Options *options, char *option) {
    options_insert(options, option, options->size);
}

void options_print(Options *options, int depth) {
    printDepth(depth++);
    printf("Options:");
    for (int i = 0; i < options->size; i++) {
        printf(" %s", options->options[i]);
    }
    printf("\n");
}
