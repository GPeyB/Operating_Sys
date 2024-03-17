#ifndef CHAIN_H
#define CHAIN_H

#include "builtin.h"
#include "pipeline.h"
#include "redirections.h"

/**
 * <chain> ::= <pipeline> <redirections>
 *          |  <builtin> <options>
 */
typedef struct Chain {
    Pipeline *pipeline;
    Redirections *redirections;
    BuiltIn *builtIn;
} Chain;

Chain *chain_create();
void chain_destroy(Chain **chain);
void chain_print(Chain *chain, int depth);
int chain_execute(Chain *chain);

#endif // CHAIN_H
