#include <stdio.h>
#include <stdlib.h>

#include "command.h"
#include "options.h"
#include "shared.h"
#include "util.h"

Command *command_create() {
    Command *command = (Command *)malloc(sizeof(Command));
    command->name = NULL;
    command->options = NULL;
    return command;
}

void command_destroy(Command **command) {
    Command *c = *command;
    // don't free name, as it's a pointer to an element of the token list
    c->name = NULL;
    if (c->options != NULL)
        options_destroy(&c->options);
    free(c);
    *command = NULL;
}

void command_print(Command *command, int depth) {
    printDepth(depth++);
    printf("Command:\n");
    printDepth(depth);
    printf("name: %s\n", command->name);
    if (command->options != NULL)
        options_print(command->options, depth);
}

void command_execute(Command *command) {
    printf("Executing command: %s\n", command->name);
    // TODO: Implement command execution
    g_status = 0;
}
