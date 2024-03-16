#include <stddef.h>
#include <stdlib.h>

#include "structures.h"

Command *command_create() {
    Command *command = (Command *)malloc(sizeof(Command));
    command->name = NULL;
    command->options = NULL;
    return command;
}

void command_destroy(Command *command) {
    if (command->name != NULL)
        free(command->name);
    if (command->options != NULL)
        free(command->options);
    free(command);
}

Chain *chain_create() {
    Chain *chain = (Chain *)malloc(sizeof(Chain));
    chain->pipeline = NULL;
    chain->redirections = NULL;
    chain->builtIn = NULL;
    return chain;
}

void chain_destroy(Chain *chain) {
    if (chain->pipeline != NULL)
        free(chain->pipeline);
    if (chain->redirections != NULL)
        free(chain->redirections);
    if (chain->builtIn != NULL)
        free(chain->builtIn);
    free(chain);
}

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
