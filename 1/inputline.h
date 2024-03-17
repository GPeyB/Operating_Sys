#ifndef INPUTLINE_H
#define INPUTLINE_H

#include "chain.h"

enum InputLineSep {
    BACKGROUND,
    AND,
    OR,
    SEMICOLON,
    NONE
};

/**
 * <inputline> ::= <chain> & <inputline>
 *              | <chain> && <inputline>
 *              | <chain> || <inputline>
 *              | <chain> ; <inputline>
 *              | <chain>
 *              | <empty>
 */
typedef struct InputLine {
    Chain *chain;
    enum InputLineSep sep;
    struct InputLine *next;
} InputLine;

InputLine *inputline_create();
void inputline_destroy(InputLine **inputLine);
void inputline_print(InputLine *inputLine, int depth);
int inputline_execute(InputLine *inputLine);

#endif // INPUTLINE_H
