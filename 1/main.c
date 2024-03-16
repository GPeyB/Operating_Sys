#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "scanner.h"
#include "shell.h"
#include "structures.h"

int main(int argc, char *argv[]) {
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    char *inputLine;
    List tokenList;

    // TODO: Signal back that the loop must stop when "exit" has been encountered (or EOF)
    while (true) {
        inputLine = readInputLine();
        tokenList = getTokenList(inputLine);
        printList(tokenList);
        // copy token list pointer to avoid losing the original pointer
        List tokenListCopy = tokenList;

        InputLine *line = NULL;
        bool parsedSuccessfully = parseInputLine(&line, &tokenList);
        if (tokenList == NULL && parsedSuccessfully) {
            printf("Input line parsed successfully!\n");
            inputline_print(line, 0);
        } else {
            printf("Error: invalid syntax!\n");
        }

        inputline_destroy(line);
        freeTokenList(tokenListCopy);
        free(inputLine);
        exit(0);
    }
    
    return 0;
}
