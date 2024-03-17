#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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

void childProcess(Command *command) {
    // first argument is the command name
    options_insert(command->options, command->name, 0);
    // options are NULL-terminated
    options_append(command->options, NULL);

    execvp(command->name, command->options->options);
}

void parentProcess(pid_t childPid) {
    waitpid(childPid, &g_status, 0);
}

void command_execute(Command *command) {
    pid_t pid;
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork() failed\n");
    } else if (pid == 0) {
        // only child process gets here
        childProcess(command);
    } else {
        // only parent process gets here
        parentProcess(pid);
    }
}
