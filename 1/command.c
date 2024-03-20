#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "command.h"
#include "options.h"
#include "shared.h"
#include "util.h"

#define READ_END 0
#define WRITE_END 1

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

void execCommand(Command *command) {
    // first argument is the command name
    options_insert(command->options, command->name, 0);
    // options are NULL-terminated
    options_append(command->options, NULL);

    if (execvp(command->name, command->options->options) == -1) {
        printf("Error: command not found!\n");
        exit(127);
    }
}

void command_execute(Command *command, pid_t *pid, int *prevfd, int *fd) {
    *pid = fork();
    
    if (*pid < 0) {
        perror("fork");
        return;
    }
    
    if (*pid == 0) {
        // only child process gets here
        if (prevfd != NULL) {
            // there is a previous command, connect to its output
            close(prevfd[WRITE_END]);
            dup2(prevfd[READ_END], STDIN_FILENO);
            close(prevfd[READ_END]);
        }
        if (fd != NULL) {
            // there is a next command, connect to its input
            close(fd[READ_END]);
            dup2(fd[WRITE_END], STDOUT_FILENO);
            close(fd[WRITE_END]);
        }
        execCommand(command);
    } else {
        // only parent process gets here
        if (prevfd != NULL) {
            close(prevfd[READ_END]);
            close(prevfd[WRITE_END]);
        }
    }
}
