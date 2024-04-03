#include "signalhandler.h"
#include "processlist.h"

#include <signal.h>
#include <stdio.h>

extern ProcessList *g_processList;

static void sigchld_handler(int signum, siginfo_t *info, void *context) {
    processlist_remove(g_processList, info->si_pid);
}

void registerSignalHandler() {
    struct sigaction sa;
    sa.sa_sigaction = sigchld_handler;
    sa.sa_flags = SA_SIGINFO | SA_RESTART; // SIGINFO to get PID, RESTART for getc
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        return;
    }
}
