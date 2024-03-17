#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "inputline.h"
#include "scanner.h"
#include "shell.h"

bool g_exitShell = false;
int g_status = 0;

int main(int argc, char *argv[]) {
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    char *inputLine;
    List tokenList;

    while (!g_exitShell) {
        //printf("shell> "); comment to match themis output

        inputLine = readInputLine();
        tokenList = getTokenList(inputLine);
        // copy token list pointer to avoid losing the original pointer
        List tokenListCopy = tokenList;

        InputLine *line = NULL;
        bool parsedSuccessfully = parseInputLine(&line, &tokenList);
        if (tokenList == NULL && parsedSuccessfully) {
            //inputline_print(line, 0);
            inputline_execute(line, NONE);
        } else {
            printf("Error: invalid syntax!\n");
        }

        inputline_destroy(&line);
        freeTokenList(tokenListCopy);
        free(inputLine);
    }

    return 0;
}
