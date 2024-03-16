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

struct Chain {
    Pipeline *pipeline;
    Redirections *redirections;
    Command *builtIn;
};

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

Pipeline *pipeline_create();
Redirections *redirections_create();
Options *options_create();
Command *command_create();
Chain *chain_create();
InputLine *inputline_create();
void inputline_destroy(InputLine *inputLine);

void inputline_print(InputLine *inputLine, int depth);

#endif // STRUCTURES_H
