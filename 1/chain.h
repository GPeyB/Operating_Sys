#ifndef CHAIN_H
#define CHAIN_H

#include "command.h"
#include "pipeline.h"
#include "redirections.h"

typedef struct Chain {
    Pipeline *pipeline;
    Redirections *redirections;
    Command *builtIn;
} Chain;

Chain *chain_create();
void chain_destroy(Chain *chain);
void chain_print(Chain *chain, int depth);

#endif // CHAIN_H
