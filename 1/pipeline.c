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

void pipeline_destroy(Pipeline *pipeline) {
    if (pipeline->command != NULL)
        command_destroy(pipeline->command);
    if (pipeline->pipeline != NULL)
        pipeline_destroy(pipeline->pipeline);
    free(pipeline);
}

void pipeline_print(Pipeline *pipeline, int depth) {
    printDepth(depth++);
    printf("Pipeline:\n");
    if (pipeline->command != NULL)
        command_print(pipeline->command, depth);
    if (pipeline->pipeline != NULL)
        pipeline_print(pipeline->pipeline, depth);
}

int pipeline_execute(Pipeline *pipeline) {
    int status = 0;

    if (pipeline->command != NULL) {
        status = command_execute(pipeline->command);
    } else if (pipeline->pipeline != NULL) {
        status = pipeline_execute(pipeline->pipeline);
    }

    return status;
}
