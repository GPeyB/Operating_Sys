#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>    

#include "scanner.h"
#include "shell.h"

int main(int argc, char *argv[]) {
    char *inputLine;
    List tokenList;

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);




    printf("Shell initialized \n");

    

    //TODO: Signal back that the loop must stop when "exit" has been encountered (or EOF)
    while (true) {
        
        printf("My-Shell %% ");
        inputLine = readInputLine();
        tokenList = getTokenList(inputLine);
        //printList(tokenList);

        bool parsedSuccessfully = parseInputLine(&tokenList);
        if (tokenList == NULL && parsedSuccessfully) {
            // Input was parsed successfully and can be accessed in "tokenList"
            if(strcmp(inputLine, "exit") == 0) {
                printf("exiting... \n");
                exit(0);
            }
            
            pid_t pid;
            pid = fork();
            if(pid < 0) {
                fprintf(stderr, "fork() could not create a child process!\n");
                exit(0);
            }
            if(pid == 0){ // Only child process gets here
                printf("Child proccess is executing... \n");
                execlp("/bin/sh", "/bin/sh", "-c", inputLine, 0);
                /*if(execlp(argv[0], argv[0], inputLine, NULL) == -1) {
                    printf("execution error");
                }*/
            } else { // Only parent process gets here
                wait(NULL);
                printf("Child process terminated! Parent process executing...\n");
            }
            



            // However, this is still a simple list of strings, it might be convenient
            // to build some intermediate structure representing the input line or a
            // command that you then construct in the parsing logic. It's up to you
            // to determine how to approach this!
        } else {
            printf("Error: invalid syntax!\n");
        }

        free(inputLine);
        freeTokenList(tokenList);
    }
    
    return 0;
}
