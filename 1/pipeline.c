#include <stdio.h>
#include <stdlib.h>

#include "command.h"
#include "pipeline.h"
#include "util.h"

Pipeline *pipeline_create() {
    Pipeline *pipeline = (Pipeline *)malloc(sizeof(Pipeline));
    pipeline->command = NULL;
    pipeline->pipeline = NULL;
    return pipeline;
}

void command_destroy(Command *command);

void pipeline_destroy(Pipeline *pipeline) {
    if (pipeline->command != NULL)
        command_destroy(pipeline->command);
    if (pipeline->pipeline != NULL)
        pipeline_destroy(pipeline->pipeline);
    free(pipeline);
}

void command_print(Command *command, int depth);

void pipeline_print(Pipeline *pipeline, int depth) {
    printDepth(depth++);
    printf("Pipeline:\n");
    if (pipeline->command != NULL)
        command_print(pipeline->command, depth);
    if (pipeline->pipeline != NULL)
        pipeline_print(pipeline->pipeline, depth);
}
