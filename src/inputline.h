#ifndef INPUTLINE_H
#define INPUTLINE_H

enum InputLineSep {
    BACKGROUND,
    AND,
    OR,
    SEMICOLON,
    NONE
};

struct Chain;

/**
 * <inputline> ::= <chain> & <inputline>
 *              | <chain> && <inputline>
 *              | <chain> || <inputline>
 *              | <chain> ; <inputline>
 *              | <chain>
 *              | <empty>
 */
typedef struct InputLine {
    struct Chain *chain;
    enum InputLineSep sep;
    struct InputLine *next;
} InputLine;

InputLine *inputline_create();
void inputline_destroy(InputLine **inputLine);
void inputline_print(InputLine *inputLine, int depth);
void inputline_execute(InputLine *inputLine);

#endif // INPUTLINE_H
