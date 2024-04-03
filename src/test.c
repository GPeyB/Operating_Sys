#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

void signal_handler(int signum, siginfo_t *info, void *context) {
    printf("Signal %d received from process with PID %d\n", signum, info->si_pid);
}

int main() {
    struct sigaction sa;

    // Set up the signal handler
    sa.sa_sigaction = &signal_handler;
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &sa, NULL);

    printf("My PID is: %d\n", getpid());
    printf("Send SIGUSR1 signal to this process from another terminal to see the sender's PID.\n");

    while(1) {
        sleep(1);
    }

    return 0;
}
