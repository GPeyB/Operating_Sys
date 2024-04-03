#ifndef PROCESS_H
#define PROCESS_H

typedef struct {
    int idx;
    int pid;
    char *name;
} Process;

Process *process_create(int pid, char *name);
void process_destroy(Process *process);

#endif // PROCESS_H
