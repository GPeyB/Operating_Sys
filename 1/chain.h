#ifndef CHAIN_H
#define CHAIN_H

#include "builtin.h"
#include "pipeline.h"
#include "inputline.h"
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
void chain_execute(Chain *chain, enum InputLineSep sep);

#endif // CHAIN_H
