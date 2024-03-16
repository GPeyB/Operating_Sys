#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "structures.h"

#define MAX_OPTIONS 10

void printDepth(int depth) {
    for (int i = 0; i < depth; i++) {
        printf(" ");
    }
}

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

Redirections *redirections_create() {
    Redirections *redirections = (Redirections *)malloc(sizeof(Redirections));
    redirections->input = NULL;
    redirections->output = NULL;
    return redirections;
}

void redirections_destroy(Redirections *redirections) {
    // don't free redirections->input and redirections->output,
    // as they're pointers to elements of the token list
    free(redirections);
}

void redirections_print(Redirections *redirections, int depth) {
    printDepth(depth++);
    printf("Redirections:\n");
    if (redirections->input != NULL) {
        printDepth(depth);
        printf("input: %s\n", redirections->input);
    }
    if (redirections->output != NULL) {
        printDepth(depth);
        printf("output: %s\n", redirections->output);
    }
}

Options *options_create() {
    Options *options = (Options *)malloc(sizeof(Options));
    options->nrOptions = 0;
    options->options = (char **)malloc(sizeof(char *) * MAX_OPTIONS);
    return options;
}

void options_destroy(Options *options) {
    // don't free options->options, as they're pointers to elements of the token list
    free(options->options);
    free(options);
}

void options_print(Options *options, int depth) {
    printDepth(depth++);
    printf("Options:");
    for (int i = 0; i < options->nrOptions; i++) {
        printf(" %s", options->options[i]);
    }
    printf("\n");
}

Command *command_create() {
    Command *command = (Command *)malloc(sizeof(Command));
    command->name = NULL;
    command->options = NULL;
    return command;
}

void command_destroy(Command *command) {
    // don't free command->name, as it's a pointer to an element of the token list
    if (command->options != NULL)
        options_destroy(command->options);
    free(command);
}

void command_print(Command *command, int depth) {
    printDepth(depth++);
    printf("Command:\n");
    printDepth(depth);
    printf("name: %s\n", command->name);
    if (command->options != NULL)
        options_print(command->options, depth);
}

Chain *chain_create() {
    Chain *chain = (Chain *)malloc(sizeof(Chain));
    chain->pipeline = NULL;
    chain->redirections = NULL;
    chain->builtIn = NULL;
    return chain;
}

void chain_destroy(Chain *chain) {
    if (chain->pipeline != NULL)
        pipeline_destroy(chain->pipeline);
    if (chain->redirections != NULL)
        redirections_destroy(chain->redirections);
    if (chain->builtIn != NULL)
        command_destroy(chain->builtIn);
    free(chain);
}

void chain_print(Chain *chain, int depth) {
    printDepth(depth++);
    printf("Chain:\n");
    if (chain->pipeline != NULL)
        pipeline_print(chain->pipeline, depth);
    if (chain->redirections != NULL)
        redirections_print(chain->redirections, depth);
    if (chain->builtIn != NULL)
        command_print(chain->builtIn, depth);
}

InputLine *inputline_create() {
    InputLine *inputLine = (InputLine *)malloc(sizeof(InputLine));
    inputLine->chain = NULL;
    inputLine->sep = NONE;
    inputLine->next = NULL;
    return inputLine;
}

void inputline_destroy(InputLine *inputLine) {
    if (inputLine->chain != NULL)
        chain_destroy(inputLine->chain);
    if (inputLine->next != NULL)
        inputline_destroy(inputLine->next);
    free(inputLine);
}

void inputline_print(InputLine *inputLine, int depth) {
    printDepth(depth++);
    printf("InputLine:\n");
    if (inputLine->chain != NULL)
        chain_print(inputLine->chain, depth);
    printDepth(depth);
    printf("separator: ");
    switch (inputLine->sep) {
    case BACKGROUND:
        printf("&\n");
        break;
    case AND:
        printf("&&\n");
        break;
    case OR:
        printf("||\n");
        break;
    case SEMICOLON:
        printf(";\n");
        break;
    case NONE:
        printf("NONE\n");
        break;
    }
    if (inputLine->next != NULL)
        inputline_print(inputLine->next, depth);
}
