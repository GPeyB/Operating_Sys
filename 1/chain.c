#include <stdlib.h>
#include <stdio.h>

#include "chain.h"
#include "command.h"
#include "pipeline.h"
#include "redirections.h"
#include "util.h"

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
        builtin_destroy(chain->builtIn);
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
        builtin_print(chain->builtIn, depth);
}

int chain_execute(Chain *chain) {
    int status = 0;

    if (chain->builtIn != NULL) {
        status = builtin_execute(chain->builtIn);
    } else if (chain->pipeline != NULL) {
        status = pipeline_execute(chain->pipeline);
    }

    return status;
}
