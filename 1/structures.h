#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct Pipeline Pipeline;
typedef struct Redirections Redirections;
typedef struct Options Options;
typedef struct Command Command;
typedef struct Chain Chain;
typedef struct InputLine InputLine;

struct Pipeline {
    Command *command;
    Pipeline *pipeline;
};

struct Redirections {
    char *input;
    char *output;
};

struct Options {
    int nrOptions;
    char **options;
};

struct Command {
    char *name;
    Options *options;
};

Command *command_create();
void command_destroy(Command *command);

struct Chain {
    Pipeline *pipeline;
    Redirections *redirections;
    Command *builtIn;
};

Chain *chain_create();
void chain_destroy(Chain *chain);

enum InputLineSep {
    BACKGROUND,
    AND,
    OR,
    SEMICOLON,
    NONE
};

struct InputLine {
    Chain *chain;
    enum InputLineSep sep;
    InputLine *next;
};

InputLine *inputline_create();
void inputline_destroy(InputLine *inputLine);

#endif // STRUCTURES_H
