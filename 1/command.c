#include <stdio.h>
#include <stdlib.h>

#include "command.h"
#include "options.h"
#include "util.h"

Command *command_create() {
    Command *command = (Command *)malloc(sizeof(Command));
    command->name = NULL;
    command->options = NULL;
    return command;
}

void command_destroy(Command *command) {
    // don't free command->name, as it's a pointer to an element of the token list
    if (command->options != NULL)
        options_destroy(command->options);
    free(command);
}

void command_print(Command *command, int depth) {
    printDepth(depth++);
    printf("Command:\n");
    printDepth(depth);
    printf("name: %s\n", command->name);
    if (command->options != NULL)
        options_print(command->options, depth);
}

int command_execute(Command *command) {
    int status = 0;

    return status;
}
