#include "process.h"

#include <stdlib.h>
#include <string.h>

static int processIdx = 1;

// Duplicates the name string
Process *process_create(int pid, char *name) {
    Process *process = (Process *)malloc(sizeof(Process));
    process->idx = processIdx++;
    process->pid = pid;
    process->name = strdup(name);
    return process;
}

void process_destroy(Process *process) {
    if (process == NULL)
        return;
    free(process->name);
    free(process);
}
