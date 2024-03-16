#include <stdbool.h>
#include <string.h>

#include "scanner.h"
#include "structures.h"

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

bool acceptExecutable(char *name, List *tokens) {
    if (isEmpty(*tokens) || isOperator((*tokens)->t))
        return false;
    name = (*tokens)->t;
    *tokens = (*tokens)->next;
    return true;
}

bool acceptOptions(Options *options, List *tokens) {
    while (*tokens != NULL && !isOperator((*tokens)->t)) {
        options->options[options->nrOptions++] = (*tokens)->t;
        (*tokens) = (*tokens)->next;
    }
    return true;
}

/**
 * <command> ::= <executable> <options>
 */
bool parseCommand(Command *command, List *tokens) {
    return acceptExecutable(command->name, tokens) && acceptOptions(command->options, tokens);
}

/**
 * <pipeline> ::= <command> "|" <pipeline>
 *             |  <command>
 */
bool parsePipeline(Pipeline *pipeline, List *tokens) {
    if (!parseCommand(pipeline->command, tokens))
        return false;

    if (acceptToken(tokens, "|"))
        return parsePipeline(pipeline->pipeline, tokens);

    return true;
}

bool parseFileName(char *name, List *tokens) {
    if (isEmpty(*tokens) || isOperator((*tokens)->t))
        return false;
    name = (*tokens)->t;
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
bool parseRedirections(Redirections *redirections, List *tokens) {
    if (isEmpty(*tokens))
        return true;

    if (acceptToken(tokens, "<")) {
        if (!parseFileName(redirections->input, tokens))
            return false;
        if (acceptToken(tokens, ">"))
            return parseFileName(redirections->output, tokens);
    } else if (acceptToken(tokens, ">")) {
        if (!parseFileName(redirections->output, tokens))
            return false;
        if (acceptToken(tokens, "<"))
            return parseFileName(redirections->input, tokens);
    }

    return true;
}

bool parseBuiltIn(Command *command, List *tokens) {
    for (int i = 0; BUILTINS[i] != NULL; i++) {
        if (acceptToken(tokens, BUILTINS[i])) {
            command->name = BUILTINS[i];
            return acceptOptions(command->options, tokens);
        }
    }

    return false;
}

/**
 * <chain> ::= <pipeline> <redirections>
 *          |  <builtin> <options>
 */
bool parseChain(Chain *chain, List *tokens) {
    return (parsePipeline(chain->pipeline, tokens)
               && parseRedirections(chain->redirections, tokens))
        || parseBuiltIn(chain->builtIn, tokens);
}

/**
 * <inputline> ::= <chain> & <inputline>
 *              | <chain> && <inputline>
 *              | <chain> || <inputline>
 *              | <chain> ; <inputline>
 *              | <chain>
 *              | <empty>
 */
bool parseInputLine(InputLine *inputLine, List *tokens) {
    if (isEmpty(*tokens))
        return true;

    if (!parseChain(inputLine->chain, tokens))
        return false;

    if (acceptToken(tokens, "&"))
        inputLine->sep = BACKGROUND;
    else if (acceptToken(tokens, "&&"))
        inputLine->sep = AND;
    else if (acceptToken(tokens, "||"))
        inputLine->sep = OR;
    else if (acceptToken(tokens, ";"))
        inputLine->sep = SEMICOLON;
    else
        return false;

    return parseInputLine(inputLine->next, tokens);
}

#include <stdio.h>

void printTokenList(List l) {
    if (l == NULL) {
        printf("NULL\n");
        return;
    }
    printf("%s ", l->t);
    printTokenList(l->next);
}
