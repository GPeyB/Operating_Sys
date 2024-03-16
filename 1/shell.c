#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "scanner.h"
#include "inputline.h"

char *OPERATORS[] = {
    "&",
    "&&",
    "||",
    ";",
    "<",
    ">",
    "|",
    NULL
};

char *BUILTINS[] = {
    "exit",
    "status",
    NULL
};

bool acceptToken(List *tokens, char *ident) {
    if (*tokens != NULL && strcmp(((*tokens)->t), ident) == 0) {
        *tokens = (*tokens)->next;
        return true;
    }
    return false;
}

bool isOperator(char *s) {
    for (int i = 0; OPERATORS[i] != NULL; i++) {
        if (strcmp(s, OPERATORS[i]) == 0)
            return true;
    }
    return false;
}

bool acceptExecutable(char **name, List *tokens) {
    if (isEmpty(*tokens) || isOperator((*tokens)->t))
        return false;
    *name = (*tokens)->t;
    *tokens = (*tokens)->next;
    return true;
}

bool acceptOptions(Options **options, List *tokens) {
    *options = options_create();
    Options *o = *options;

    while (*tokens != NULL && !isOperator((*tokens)->t)) {
        o->options[o->nrOptions++] = (*tokens)->t;
        (*tokens) = (*tokens)->next;
    }
    return true;
}

/**
 * <command> ::= <executable> <options>
 */
bool parseCommand(Command **command, List *tokens) {
    *command = command_create();
    Command *c = *command;

    return acceptExecutable(&c->name, tokens) && acceptOptions(&c->options, tokens);
}

/**
 * <pipeline> ::= <command> "|" <pipeline>
 *             |  <command>
 */
bool parsePipeline(Pipeline **pipeline, List *tokens) {
    *pipeline = pipeline_create();
    Pipeline *p = *pipeline;

    if (!parseCommand(&p->command, tokens))
        return false;

    if (acceptToken(tokens, "|"))
        return parsePipeline(&p->pipeline, tokens);

    return true;
}

bool parseFileName(char **name, List *tokens) {
    if (isEmpty(*tokens) || isOperator((*tokens)->t))
        return false;
    *name = (*tokens)->t;
    *tokens = (*tokens)->next;
    return true;
}

/**
 * <redirections> ::= "<" <filename> ">" <filename>
 *                 |  ">" <filename> "<" <filename>
 *                 |  "<" <filename>
 *                 |  ">" <filename>
 *                 |  <empty>
 */
bool parseRedirections(Redirections **redirections, List *tokens) {
    *redirections = redirections_create();
    Redirections *r = *redirections;

    if (isEmpty(*tokens))
        return true;

    if (acceptToken(tokens, "<")) {
        if (!parseFileName(&r->input, tokens))
            return false;
        if (acceptToken(tokens, ">"))
            return parseFileName(&r->output, tokens);
    } else if (acceptToken(tokens, ">")) {
        if (!parseFileName(&r->output, tokens))
            return false;
        if (acceptToken(tokens, "<"))
            return parseFileName(&r->input, tokens);
    }

    return true;
}

bool parseBuiltIn(Command **command, List *tokens) {
    *command = command_create();
    Command *c = *command;

    for (int i = 0; BUILTINS[i] != NULL; i++) {
        if (acceptToken(tokens, BUILTINS[i])) {
            c->name = BUILTINS[i];
            return acceptOptions(&c->options, tokens);
        }
    }

    return false;
}

/**
 * <chain> ::= <pipeline> <redirections>
 *          |  <builtin> <options>
 */
bool parseChain(Chain **chain, List *tokens) {
    *chain = chain_create();
    Chain *c = *chain;

    return (parsePipeline(&c->pipeline, tokens)
               && parseRedirections(&c->redirections, tokens))
        || parseBuiltIn(&c->builtIn, tokens);
}

/**
 * <inputline> ::= <chain> & <inputline>
 *              | <chain> && <inputline>
 *              | <chain> || <inputline>
 *              | <chain> ; <inputline>
 *              | <chain>
 *              | <empty>
 */
bool parseInputLine(InputLine **inputLine, List *tokens) {
    *inputLine = inputline_create();
    InputLine *line = *inputLine;

    if (isEmpty(*tokens))
        return true;

    if (!parseChain(&line->chain, tokens))
        return false;

    if (acceptToken(tokens, "&"))
        line->sep = BACKGROUND;
    else if (acceptToken(tokens, "&&"))
        line->sep = AND;
    else if (acceptToken(tokens, "||"))
        line->sep = OR;
    else if (acceptToken(tokens, ";"))
        line->sep = SEMICOLON;
    else
        return true;

    return parseInputLine(&line->next, tokens);
}
