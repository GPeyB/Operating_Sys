#ifndef PIPELINE_H
#define PIPELINE_H

#include "command.h"

/**
 * <pipeline> ::= <command> "|" <pipeline>
 *             |  <command>
 */
typedef struct Pipeline {
    Command *command;
    struct Pipeline *pipeline;
} Pipeline;

Pipeline *pipeline_create();
void pipeline_destroy(Pipeline *pipeline);
void pipeline_print(Pipeline *pipeline, int depth);
int pipeline_execute(Pipeline *pipeline);

#endif // PIPELINE_H
