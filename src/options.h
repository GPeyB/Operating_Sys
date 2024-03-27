#ifndef OPTIONS_H
#define OPTIONS_H

#define INITIAL_NR_OPTIONS 10

typedef struct Options {
    int size;
    int capacity;
    char **options;
} Options;

Options *options_create();
void options_destroy(Options **options);
void options_insert(Options *options, char *option, int index);
void options_append(Options *options, char *option);
void options_print(Options *options, int depth);

#endif // OPTIONS_H
