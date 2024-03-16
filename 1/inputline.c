#include <stdio.h>
#include <stdlib.h>

#include "chain.h"
#include "inputline.h"
#include "util.h"

InputLine *inputline_create() {
    InputLine *inputLine = (InputLine *)malloc(sizeof(InputLine));
    inputLine->chain = NULL;
    inputLine->sep = NONE;
    inputLine->next = NULL;
    return inputLine;
}

void inputline_destroy(InputLine *inputLine) {
    if (inputLine->chain != NULL)
        chain_destroy(inputLine->chain);
    if (inputLine->next != NULL)
        inputline_destroy(inputLine->next);
    free(inputLine);
}

void inputline_print(InputLine *inputLine, int depth) {
    printDepth(depth++);
    printf("InputLine:\n");
    if (inputLine->chain != NULL)
        chain_print(inputLine->chain, depth);
    printDepth(depth);
    printf("separator: ");
    switch (inputLine->sep) {
    case BACKGROUND:
        printf("&\n");
        break;
    case AND:
        printf("&&\n");
        break;
    case OR:
        printf("||\n");
        break;
    case SEMICOLON:
        printf(";\n");
        break;
    case NONE:
        printf("NONE\n");
        break;
    }
    if (inputLine->next != NULL)
        inputline_print(inputLine->next, depth);
}
