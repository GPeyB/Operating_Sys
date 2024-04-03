#ifndef PROCESS_H
#define PROCESS_H

typedef struct {
    int idx;
    int pid;
} Process;

Process *process_create(int pid);
void process_destroy(Process *process);

#endif // PROCESS_H
