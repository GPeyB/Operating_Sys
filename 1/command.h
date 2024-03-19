#ifndef COMMAND_H
#define COMMAND_H

#include "options.h"
#include <sys/types.h>

typedef struct Command {
    char *name;
    Options *options;
} Command;

Command *command_create();
void command_destroy(Command **command);
void command_print(Command *command, int depth);
void command_execute(Command *command, pid_t *pid, int fdIn, int fdOut);

#endif // COMMAND_H
