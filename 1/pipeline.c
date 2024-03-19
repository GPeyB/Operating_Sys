#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

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

void pipeline_execute(Pipeline *pipeline) {
    pid_t pid;
    command_execute(pipeline->command, &pid);
    if (pipeline->pipeline != NULL) {
        pipeline_execute(pipeline->pipeline);
    } else {
        // the command run above is the last command in the pipeline
        // get its status and set it as the global status
        int status = 0;
        waitpid(pid, &status, 0);
        g_status = WEXITSTATUS(status);
        return;
    }
    waitpid(pid, NULL, 0);
}
