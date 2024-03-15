#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct Pipeline {
    Command *command;
    Pipeline *pipeline;
} Pipeline;

typedef struct Redirections {
    char *input;
    char *output;
} Redirections;

typedef struct Command {
    char *name;
    int argc;
    char **argv;
} Command;

typedef struct Chain {
    Pipeline *pipeline;
    Redirections *redirections;
    Command *builtIn;
} Chain;

typedef enum InputLineSep {
    BACKGROUND,
    AND,
    OR,
    SEMICOLON,
    NONE
} InputLineSep;

typedef struct InputLine {
    Chain *chain;
    InputLineSep *sep;
    InputLine *next;
} InputLine;


#endif // STRUCTURES_H
