#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "scanner.h"
#include "shell.h"
#include "inputline.h"

bool exitShell = false;
int status = 0;

int main(int argc, char *argv[]) {
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    char *inputLine;
    List tokenList;

    while (!exitShell) {
        printf("shell> ");

        inputLine = readInputLine();
        tokenList = getTokenList(inputLine);
        // copy token list pointer to avoid losing the original pointer
        List tokenListCopy = tokenList;

        InputLine *line = NULL;
        bool parsedSuccessfully = parseInputLine(&line, &tokenList);
        if (tokenList == NULL && parsedSuccessfully) {
            printf("Parsed successfully!\n");
            inputline_print(line, 0);
            status = inputline_execute(line);
        } else {
            printf("Error: invalid syntax!\n");
        }

        inputline_destroy(line);
        freeTokenList(tokenListCopy);
        free(inputLine);
    }

    return 0;
}
