#include "signalhandler.h"
#include "processlist.h"

#include <signal.h>
#include <stdio.h>

extern ProcessList *g_processList;

static void sigchld_handler(int signum, siginfo_t *info, void *context) {
    printf("Signal %d received from process with PID %d\n", signum, info->si_pid);
    // processlist_remove(g_processList);
}

void registerSignalHandler() {
    struct sigaction sa;
    sa.sa_sigaction = sigchld_handler;
    sa.sa_flags = SA_SIGINFO;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        return;
    }
}
