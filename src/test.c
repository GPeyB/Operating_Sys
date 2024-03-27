#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

// #include "command.h"
// #include "pipeline.h"

#define READ_END 0
#define WRITE_END 1

int g_status = 0;

/*
void pipeline_execute(Pipeline *pipeline) {

}

int main() {

    // Create a pipeline containing two commands
    Pipeline *pipeline = pipeline_create();
    Command *command1 = command_create();
    command1->name = "ls";
    command1->options = options_create();
    options_append(command1->options, "-l");
    pipeline->command = command1;
    Command *command2 = command_create();
    command2->name = "wc";
    command2->options = options_create();
    options_append(command2->options, "-l");
    pipeline->pipeline = pipeline_create();
    pipeline->pipeline->command = command2;

    //pipeline_print(pipeline, 0);

    int pipefd[2];
    pipe(pipefd);

    pid_t pid = fork();

    if (pid == 0) {
        close(pipefd[READ_END]);
        dup2(pipefd[WRITE_END], STDOUT_FILENO);
        close(pipefd[WRITE_END]);
        execlp("ls", "ls", NULL);
    } else {
        close(pipefd[WRITE_END]);
        dup2(pipefd[READ_END], STDIN_FILENO);
        close(pipefd[READ_END]);
        execlp("wc", "wc", "-l", NULL);
    }

    pipeline_destroy(&pipeline);

    return 0;
}
*/

void pipetest1() {
    // pipe
    int pipefd[2];
    pipe(pipefd);

    // fork
    pid_t pid1 = fork();
    if (pid1 == 0) { // child process 1
        close(pipefd[READ_END]);
        dup2(pipefd[WRITE_END], STDOUT_FILENO);
        // close(pipefd[WRITE_END]);
        execlp("ls", "ls", "-l", NULL);
    }

    // fork again
    pid_t pid2 = fork();
    if (pid2 == 0) {
        close(pipefd[WRITE_END]);
        dup2(pipefd[READ_END], STDIN_FILENO);
        // close(pipefd[READ_END]);
        execlp("wc", "wc", "-l", NULL);
    } else {
        close(pipefd[READ_END]);
        close(pipefd[WRITE_END]);
    }
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}

void pipetest2() {
    int pipe1[2], pipe2[2];
    pipe(pipe1);
    pipe(pipe2);

    pid_t pid1 = fork();
    if (pid1 == 0) {
        // child process 1 writes to pipe 1
        close(pipe1[READ_END]);
        dup2(pipe1[WRITE_END], STDOUT_FILENO);
        close(pipe1[WRITE_END]);
        execlp("ls", "ls", "-l", NULL);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        // child process 2 reads from pipe 1 and writes to pipe 2
        close(pipe1[WRITE_END]);
        dup2(pipe1[READ_END], STDIN_FILENO);
        close(pipe1[READ_END]);

        close(pipe2[READ_END]);
        dup2(pipe2[WRITE_END], STDOUT_FILENO);
        close(pipe2[WRITE_END]);

        execlp("wc", "wc", "-l", NULL);
    }
    close(pipe1[READ_END]);
    close(pipe1[WRITE_END]);

    pid_t pid3 = fork();
    if (pid3 == 0) {
        // child process 3 reads from pipe 2
        close(pipe2[WRITE_END]);
        dup2(pipe2[READ_END], STDIN_FILENO);
        close(pipe2[READ_END]);
        execlp("wc", "wc", "-c", NULL);
    }

    close(pipe2[READ_END]);
    close(pipe2[WRITE_END]);
}

int main() {
    pipetest2();
    return 0;
}
