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

void command_execute(Command *command, pid_t *pid, int fdIn, int fdOut) {
    pid_t localPid = fork();
    
    if (localPid < 0) {
        perror("fork");
        return;
    }
    
    if (localPid == 0) {
        // only child process gets here
        dup2(fdIn, STDIN_FILENO); // replace stdin with the read end of the pipe
        dup2(fdOut, STDOUT_FILENO); // replace stdout with the write end of the pipe
        
        execCommand(command);
        
        close(fdIn);
        close(fdOut);
    } else {
        // only parent process gets here
        *pid = localPid;
    }
}
