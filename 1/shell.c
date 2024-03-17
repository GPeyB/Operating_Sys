#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "inputline.h"
#include "scanner.h"

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
    if (o->nrOptions == 0)
        options_destroy(options);
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

    if (!parseCommand(&p->command, tokens)) {
        pipeline_destroy(pipeline);
        return false;
    }

    if (acceptToken(tokens, "|"))
        if (!parsePipeline(&p->pipeline, tokens)) {
            pipeline_destroy(pipeline);
            return false;
        }

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
    if (isEmpty(*tokens))
        return true;

    *redirections = redirections_create();
    Redirections *r = *redirections;

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

    redirections_destroy(redirections);
    return true;
}

bool parseBuiltIn(BuiltIn **builtIn, List *tokens) {
    *builtIn = builtin_create();
    BuiltIn *b = *builtIn;

    for (int i = 0; BUILTINS[i] != NULL; i++) {
        if (acceptToken(tokens, BUILTINS[i])) {
            b->name = BUILTINS[i];
            return acceptOptions(&b->options, tokens);
        }
    }

    builtin_destroy(builtIn);
    return false;
}

/**
 * <chain> ::= <pipeline> <redirections>
 *          |  <builtin> <options>
 */
bool parseChain(Chain **chain, List *tokens) {
    *chain = chain_create();

    bool parsed = parseBuiltIn(&(*chain)->builtIn, tokens)
        || (parsePipeline(&(*chain)->pipeline, tokens)
            && parseRedirections(&(*chain)->redirections, tokens));
    if (!parsed)
        chain_destroy(chain);
    return parsed;
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
