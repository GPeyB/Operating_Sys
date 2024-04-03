#ifndef PROCESSLIST_H
#define PROCESSLIST_H

#include "process.h"

typedef struct ProcessList {
    Process *process;
    struct ProcessList *next;
} ProcessList;

ProcessList *processlist_create();
void processlist_destroy(ProcessList *list);
int processlist_size(ProcessList *list);
void processlist_add(ProcessList *list, Process *process);
void processlist_remove(ProcessList *list, int pid);
void processlist_print(ProcessList *list);

#endif // PROCESSLIST_H
