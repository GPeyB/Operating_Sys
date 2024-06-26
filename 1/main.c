#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "inputline.h"
#include "scanner.h"
#include "shell.h"

#define BLUE "\033[1;34m"
#define RESET "\033[0m"

bool g_exitShell = false;
int g_status = 0;

int main(int argc, char *argv[]) {
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    char *inputLine;
    List tokenList;

    while (!g_exitShell) {
#if BONUS
        // custom prompt
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf(BLUE "%s" RESET "> ", cwd);
        } else {
            perror("getcwd");
            return 1;
        }
#endif // BONUS

        inputLine = readInputLine();
        if (inputLine == NULL)
            break;
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
