#ifndef PROCESSLIST_H
#define PROCESSLIST_H

#include "process.h"

typedef struct ProcessList {
    Process **processes;
    int size;
    int capacity;
} ProcessList;

ProcessList *processlist_create();
void processlist_destroy(ProcessList *list);
Process *processlist_get(ProcessList *list, int pid);
void processlist_add(ProcessList *list, Process *process);
void processlist_remove(ProcessList *list, Process *process);
void processlist_print(ProcessList *list);

#endif // PROCESSLIST_H
