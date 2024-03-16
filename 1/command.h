#ifndef COMMAND_H
#define COMMAND_H

#include "options.h"

typedef struct Command {
    char *name;
    Options *options;
} Command;

Command *command_create();
void command_destroy(Command *command);
void command_print(Command *command, int depth);

#endif // COMMAND_H
