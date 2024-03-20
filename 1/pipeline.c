#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "command.h"
#include "pipeline.h"
#include "shared.h"
#include "util.h"

Pipeline *pipeline_create() {
    Pipeline *pipeline = (Pipeline *)malloc(sizeof(Pipeline));
    pipeline->command = NULL;
    pipeline->pipeline = NULL;
    return pipeline;
}

void pipeline_destroy(Pipeline **pipeline) {
    Pipeline *p = *pipeline;
    if (p->command != NULL)
        command_destroy(&p->command);
    if (p->pipeline != NULL)
        pipeline_destroy(&p->pipeline);
    free(p);
    *pipeline = NULL;
}

void pipeline_print(Pipeline *pipeline, int depth) {
    printDepth(depth++);
    printf("Pipeline:\n");
    if (pipeline->command != NULL)
        command_print(pipeline->command, depth);
    if (pipeline->pipeline != NULL)
        pipeline_print(pipeline->pipeline, depth);
}

void pipeline_execute(Pipeline *pipeline, int *prevfd) {
    pid_t pid;
    if (pipeline->pipeline != NULL) {
        // there is a next command, create pipe to connect current and next
        int pipefd[2];
        if (pipe(pipefd) == -1) {
            perror("pipe");
            return;
        }

        command_execute(pipeline->command, &pid, prevfd, pipefd);
        pipeline_execute(pipeline->pipeline, pipefd);
    } else {
        // this was the last command in the pipeline
        command_execute(pipeline->command, &pid, prevfd, NULL);
        int status = 0;
        waitpid(pid, &status, 0);
        g_status = WEXITSTATUS(status);
        return;
    }
    waitpid(pid, NULL, 0);
}
