#include <stdio.h>
#include <stdlib.h>

#include "chain.h"
#include "inputline.h"
#include "shared.h"
#include "util.h"

InputLine *inputline_create() {
    InputLine *inputLine = (InputLine *)malloc(sizeof(InputLine));
    inputLine->chain = NULL;
    inputLine->sep = NONE;
    inputLine->next = NULL;
    return inputLine;
}

void inputline_destroy(InputLine **inputLine) {
    InputLine *i = *inputLine;
    if (i->chain != NULL)
        chain_destroy(&i->chain);
    if (i->next != NULL)
        inputline_destroy(&i->next);
    free(i);
    *inputLine = NULL;
}

void inputline_print(InputLine *inputLine, int depth) {
    printDepth(depth++);
    printf("InputLine:\n");
    if (inputLine->chain != NULL)
        chain_print(inputLine->chain, depth);
    if (inputLine->sep != NONE) {
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
            break;
        }
    }
    if (inputLine->next != NULL)
        inputline_print(inputLine->next, depth);
}

void inputline_execute(InputLine *inputLine) {
    if (inputLine->chain != NULL)
        chain_execute(inputLine->chain);

    switch (inputLine->sep) {
    case BACKGROUND:
        break;
    case AND:
        if (g_status == 0)
            inputline_execute(inputLine->next);
        break;
    case OR:
        if (g_status != 0)
            inputline_execute(inputLine->next);
        break;
    case SEMICOLON:
        inputline_execute(inputLine->next);
        break;
    case NONE:
        break;
    }
}
