#ifndef OPTIONS_H
#define OPTIONS_H

typedef struct Options {
    int nrOptions;
    char **options;
} Options;

Options *options_create();
void options_destroy(Options **options);
void options_print(Options *options, int depth);

#endif // OPTIONS_H
