#include <stdio.h>
#include <stdlib.h>

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

void chain_destroy(Chain **chain) {
    Chain *c = *chain;
    if (c->pipeline != NULL)
        pipeline_destroy(&c->pipeline);
    if (c->redirections != NULL)
        redirections_destroy(&c->redirections);
    if (c->builtIn != NULL)
        builtin_destroy(&c->builtIn);
    free(c);
    *chain = NULL;
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

void chain_execute(Chain *chain) {
    if (chain->builtIn != NULL) {
        builtin_execute(chain->builtIn);
    } else if (chain->pipeline != NULL) {
        pipeline_execute(chain->pipeline);
    }
}
