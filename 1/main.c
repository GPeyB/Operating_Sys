#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "scanner.h"
#include "shell.h"
#include "inputline.h"

int main(int argc, char *argv[]) {
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    char *inputLine;
    List tokenList;

    bool exitShell = false;
    while (!exitShell) {
        printf("shell> ");
        inputLine = readInputLine();
        tokenList = getTokenList(inputLine);
        printList(tokenList);
        // copy token list pointer to avoid losing the original pointer
        List tokenListCopy = tokenList;

        InputLine *line = NULL;
        bool parsedSuccessfully = parseInputLine(&line, &tokenList);
        if (tokenList == NULL && parsedSuccessfully) {
            printf("Parsed successfully!\n");
        } else {
            printf("Error: invalid syntax!\n");
        }

        inputline_destroy(line);
        freeTokenList(tokenListCopy);
        free(inputLine);
    }

    return 0;
}
