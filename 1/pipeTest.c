#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to execute the pipeline (super simplified)
void execute_pipeline(char *command1, char *command2) {
    int pipefd[2];  // File descriptors for the pipe
    pid_t pid;

    // Create the pipe
    if (pipe(pipefd) == -1) { // If pipe creation fails
        perror("pipe"); // Print error message
        exit(EXIT_FAILURE); // Exit with failure status
    }

    // Fork a new process
    pid = fork();

    if (pid == -1) { // If fork fails
        perror("fork"); // Print error message
        exit(EXIT_FAILURE); // Exit with failure status
    } else if (pid == 0) {  // Child process
        // Close the write end of the pipe
        close(pipefd[1]); // Close the write end of the pipe to avoid blocking

        // Redirect stdin to the read end of the pipe
        dup2(pipefd[0], STDIN_FILENO); // Redirect standard input to the read end of the pipe

        // Execute command 2
        execlp("/bin/sh", "/bin/sh", "-c", command2, NULL); // Execute the second command using /bin/sh

        // Error handling for execlp
        perror("execlp"); // Print error message
        exit(EXIT_FAILURE); // Exit with failure status
    } else {  // Parent process
        // Close the read end of the pipe
        close(pipefd[0]); // Close the read end of the pipe to avoid blocking

        // Redirect stdout to the write end of the pipe
        dup2(pipefd[1], STDOUT_FILENO); // Redirect standard output to the write end of the pipe

        // Execute command 1
        execlp("/bin/sh", "/bin/sh", "-c", command1, NULL); // Execute the first command using /bin/sh

        // Error handling for execlp
        perror("execlp"); // Print error message
        exit(EXIT_FAILURE); // Exit with failure status
    }
}

// Parsing
void parse_pipeline(char *input, char **command1, char **command2) {
    // Remove trailing newline character, if any
    if (input[strlen(input) - 1] == '\n') {
        input[strlen(input) - 1] = '\0'; // Replace newline with null terminator
    }

    // Parse the input to extract the two commands
    *command1 = strtok(input, "|"); // Tokenize input string using "|" delimiter, extract first command
    *command2 = strtok(NULL, "|"); // Extract second command

    // Trim leading and trailing spaces from commands
    while (*command1 && **command1 == ' ') { // Loop to remove leading spaces from first command
        (*command1)++; // Move pointer forward
    }
    if (*command1) { // If first command is not NULL
        char *end = *command1 + strlen(*command1) - 1; // Pointer to end of command string
        while (end >= *command1 && *end == ' ') { // Loop to remove trailing spaces from first command
            *end-- = '\0'; // Replace trailing spaces with null terminator
        }
    }
    while (*command2 && **command2 == ' ') { // Loop to remove leading spaces from second command
        (*command2)++; // Move pointer forward
    }
    if (*command2) { // If second command is not NULL
        char *end = *command2 + strlen(*command2) - 1; // Pointer to end of command string
        while (end >= *command2 && *end == ' ') { // Loop to remove trailing spaces from second command
            *end-- = '\0'; // Replace trailing spaces with null terminator
        }
    }
}

int main() {
    char input[100];  // Buffer to store input string
    char *command1, *command2; // Pointers to store parsed commands

    printf(">"); // Prompt user for input
    if (fgets(input, sizeof(input), stdin) == NULL) { // Read input from stdin
        printf("Error reading input.\n"); // Print error message if reading fails
        exit(EXIT_FAILURE); // Exit with failure status
    }

    // Parse the input to extract the two commands
    parse_pipeline(input, &command1, &command2); // Call parsing function

    // Execute the pipeline
    execute_pipeline(command1, command2); // Call function to execute the pipeline

    return 0; // Exit successfully
}
