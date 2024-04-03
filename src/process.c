#include "process.h"

#include <stdlib.h>

static int processIdx = 1;

Process *process_create(int pid) {
    Process *process = (Process *)malloc(sizeof(Process));
    process->idx = processIdx++;
    process->pid = pid;
    return process;
}

void process_destroy(Process *process) {
    free(process);
}
