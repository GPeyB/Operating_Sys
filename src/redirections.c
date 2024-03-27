#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "redirections.h"
#include "util.h"

#define OPEN_PERMISSIONS 0644

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

bool redirections_execute(Redirections *redirections, int *infd, int *outfd) {
    if (redirections->input != NULL && redirections->output != NULL
        && strcmp(redirections->input, redirections->output) == 0) {
        printf("Error: input and output files cannot be equal!\n");
        return false;
    }

    if (redirections->input != NULL) {
        *infd = open(redirections->input, O_RDONLY);
        if (*infd == -1) {
            perror("open");
            return false;
        }
    }
    if (redirections->output != NULL) {
        *outfd = open(redirections->output, O_CREAT | O_WRONLY | O_TRUNC, OPEN_PERMISSIONS);
        if (*outfd == -1) {
            perror("open");
            return false;
        }
    }
    return true;
}
