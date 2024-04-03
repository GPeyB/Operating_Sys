#include "processlist.h"

#include <stdio.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 10

ProcessList *processlist_create() {
    ProcessList *list = (ProcessList *)malloc(sizeof(ProcessList));
    list->processes = (Process **)malloc(INITIAL_CAPACITY * sizeof(Process *));
    list->size = 0;
    list->capacity = INITIAL_CAPACITY;
    return list;
}

void processlist_destroy(ProcessList *list) {
    if (list == NULL)
        return;
    for (int i = 0; i < list->size; i++) {
        process_destroy(list->processes[i]);
    }
    free(list->processes);
    free(list);
}

Process *processlist_get(ProcessList *list, int pid) {
    if (list == NULL)
        return NULL;
    for (int i = 0; i < list->size; i++) {
        if (list->processes[i]->pid == pid)
            return list->processes[i];
    }
    return NULL;
}

void processlist_add(ProcessList *list, Process *process) {
    if (process == NULL)
        return;

    if (list->size == list->capacity) {
        list->capacity *= 2;
        list->processes = (Process **)realloc(list->processes, list->capacity * sizeof(Process *));
    }
    list->processes[list->size++] = process;
}

void processlist_remove(ProcessList *list, Process *process) {
    if (process == NULL)
        return;

    for (int i = 0; i < list->size; i++) {
        if (list->processes[i] == process) {
            process_destroy(list->processes[i]);
            for (int j = i; j < list->size - 1; j++) {
                list->processes[j] = list->processes[j + 1];
            }
            list->size--;
            return;
        }
    }
}

void processlist_print(ProcessList *list) {
    if (list->size == 0) {
        printf("No background processes!\n");
        return;
    }

    printf("Index\tPID\tCommand\n");
    for (int i = 0; i < list->size; i++) {
        Process *process = list->processes[i];
        printf("%d\t%d\t%s\n", process->idx, process->pid, process->name);
    }
}
