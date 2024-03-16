#ifndef REDIRECTIONS_H
#define REDIRECTIONS_H

typedef struct Redirections {
    char *input;
    char *output;
} Redirections;

Redirections *redirections_create();
void redirections_destroy(Redirections *redirections);
void redirections_print(Redirections *redirections, int depth);

#endif // REDIRECTIONS_H
