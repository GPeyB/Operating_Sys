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

        InputLine *parsedLine = inputline_create();
        bool parsedSuccessfully = parseInputLine(parsedLine, &tokenList);
        if (tokenList == NULL && parsedSuccessfully) {
            printf("Input line parsed successfully!\n");
        } else {
            printf("Error: invalid syntax!\n");
        }

        free(inputLine);
        freeTokenList(tokenList);
        inputline_destroy(parsedLine);
    }
    
    return 0;
}
