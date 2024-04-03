#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtin.h"
#include "options.h"
#include "shared.h"
#include "util.h"

BuiltIn *builtin_create() {
    BuiltIn *builtIn = (BuiltIn *)malloc(sizeof(BuiltIn));
    builtIn->name = NULL;
    builtIn->options = NULL;
    return builtIn;
}

void builtin_destroy(BuiltIn **builtIn) {
    BuiltIn *b = *builtIn;
    // don't free name, as it's a pointer to an element of the token list
    b->name = NULL;
    if (b->options != NULL)
        options_destroy(&b->options);
    free(b);
    *builtIn = NULL;
}

void builtin_print(BuiltIn *builtIn, int depth) {
    printDepth(depth++);
    printf("BuiltIn:\n");
    printDepth(depth);
    printf("name: %s\n", builtIn->name);
    if (builtIn->options != NULL)
        options_print(builtIn->options, depth);
}

static void execExit() {
    if (processlist_size(g_processList) > 0) {
        printf("Error: there are still background processes running!\n");
        g_status = 2;
        return;
    }
    g_status = 0;
    g_exitShell = true;
}

static void execStatus() {
    printf("The most recent exit code is: %d\n", g_status);
}

static void execCd(Options *options) {
    // Check if folder is provided
    if (options->size == 0) {
        printf("Error: cd requires folder to navigate to!\n");
        g_status = 2;
        return;
    }

    // Try to change directory
    if (chdir(options->options[0]) != 0) {
        printf("Error: cd directory not found!\n");
        g_status = 2;
        return;
    }

    g_status = 0;
}

static void execJobs() {
    processlist_print(g_processList);
    g_status = 0;
}

static int str2i(char *str) {
    long value = strtol(str, NULL, 10);
    if (value > INT_MAX) {
        errno = ERANGE;
        return INT_MAX;
    } else if (value < INT_MIN) {
        errno = ERANGE;
        return INT_MIN;
    }
    return (int)value;
}

static void execKill(Options *options) {
    // Check if index is provided
    if (options->size < 1) {
        printf("Error: command requires an index!\n");
        g_status = 2;
        return;
    }

    // Check if index is valid
    errno = 0;
    int idx = str2i(options->options[0]);
    if (errno != 0) {
        printf("Error: invalid index provided!\n");
        g_status = 2;
        return;
    }

    // Check if index is in list
    Process *process = processlist_get(g_processList, idx);
    if (process == NULL) {
        printf("Error: this index is not a background process!\n");
        g_status = 2;
        return;
    }

    // Check if signal is valid
    int signal = SIGTERM;
    if (options->size >= 2) {
        errno = 0;
        signal = str2i(options->options[1]);
        if (errno != 0) {
            printf("Error: invalid signal provided!\n");
            g_status = 2;
            return;
        }
    }

    // Send signal to process
    if (kill(process->pid, signal) == -1) {
        perror("kill");
        g_status = 2;
        return;
    }

    g_status = 0;
}

void builtin_execute(BuiltIn *builtIn) {
    char *name = builtIn->name;
    if (strcmp(name, "exit") == 0) {
        execExit();
    } else if (strcmp(name, "status") == 0) {
        execStatus();
    } else if (strcmp(name, "cd") == 0) {
        execCd(builtIn->options);
    } else if (strcmp(name, "jobs") == 0) {
        execJobs();
    } else if (strcmp(name, "kill") == 0) {
        execKill(builtIn->options);
    }
}
