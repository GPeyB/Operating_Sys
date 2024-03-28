#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
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

// child process signal handler
static void sigchld_handler(int signum) {
    puts("got SIGCHLD");
}

void command_execute(Command *command, pid_t *pid, int *prevPipe, int *pipe, int *infd, int *outfd) {
    *pid = fork();

    if (*pid < 0) {
        perror("fork");
        return;
    }

    if (*pid == 0) {
        // only child process gets here
        if (prevPipe != NULL) {
            // there is a previous command, connect to its output
            close(prevPipe[WRITE_END]);
            dup2(prevPipe[READ_END], STDIN_FILENO);
            close(prevPipe[READ_END]);
        } else {
            // no previous command, connect to the input file
            if (*infd != STDIN_FILENO) {
                dup2(*infd, STDIN_FILENO);
                close(*infd);
            }
        }
        if (pipe != NULL) {
            // there is a next command, connect to its input
            close(pipe[READ_END]);
            dup2(pipe[WRITE_END], STDOUT_FILENO);
            close(pipe[WRITE_END]);
        } else {
            // no next command, connect to the output file
            if (*outfd != STDOUT_FILENO) {
                dup2(*outfd, STDOUT_FILENO);
                close(*outfd);
            }
        }
        execCommand(command);
    } else {
        // only parent process gets here

        // register signal handler for child process
        struct sigaction sa;
        sa.sa_handler = sigchld_handler;
        if (sigaction(SIGCHLD, sa, NULL) == -1) {
            perror("sigaction");
            return;
        }

        if (prevPipe != NULL) {
            close(prevPipe[READ_END]);
            close(prevPipe[WRITE_END]);
        }
    }
}
