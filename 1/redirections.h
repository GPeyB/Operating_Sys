#ifndef REDIRECTIONS_H
#define REDIRECTIONS_H

/**
 * <redirections> ::= "<" <filename> ">" <filename>
 *                 |  ">" <filename> "<" <filename>
 *                 |  "<" <filename>
 *                 |  ">" <filename>
 *                 |  <empty>
 */
typedef struct Redirections {
    char *input;
    char *output;
} Redirections;

Redirections *redirections_create();
void redirections_destroy(Redirections **redirections);
void redirections_print(Redirections *redirections, int depth);
void redirections_execute(Redirections *redirections, int *infd, int *outfd);

#endif // REDIRECTIONS_H
