#include "signalhandler.h"
#include "processlist.h"

#include <signal.h>
#include <stdio.h>
#include <wait.h>

extern ProcessList *g_processList;

static void sigchld_handler(int signum, siginfo_t *info, void *context) {
    Process *process = processlist_get(g_processList, info->si_pid);
    processlist_remove(g_processList, process);
    waitpid(info->si_pid, NULL, 0);
}

static void registerSigchldHandler() {
    struct sigaction sa;
    sa.sa_sigaction = sigchld_handler;
    sa.sa_flags = SA_SIGINFO | SA_RESTART; // SIGINFO to get PID, RESTART for getc
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        return;
    }
}

static void registerSigintHandler() {
}

void registerSignalHandlers() {
    registerSigchldHandler();
    registerSigintHandler();
}
