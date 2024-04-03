#include "processlist.h"

#include <stdio.h>
#include <stdlib.h>

ProcessList *processlist_create() {
    ProcessList *list = (ProcessList *)malloc(sizeof(ProcessList));
    list->process = NULL;
    list->next = NULL;
    return list;
}

void processlist_destroy(ProcessList *list) {
    ProcessList *current = list;
    while (current != NULL) {
        ProcessList *next = current->next;
        free(current);
        current = next;
    }
}

int processlist_size(ProcessList *list) {
    int size = 0;
    ProcessList *current = list;
    while (current->next != NULL) {
        current = current->next;
        size++;
    }
    return size;
}

Process *processlist_get(ProcessList *list, int idx) {
    ProcessList *current = list;
    while (current->next != NULL) {
        current = current->next;
        if (current->process->idx == idx) {
            return current->process;
        }
    }
    return NULL;
}

void processlist_add(ProcessList *list, Process *process) {
    if (process == NULL)
        return;

    ProcessList *current = list;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = (ProcessList *)malloc(sizeof(ProcessList));
    current->next->process = process;
    current->next->next = NULL;
}

void processlist_remove(ProcessList *list, Process *process) {
    if (process == NULL)
        return;

    ProcessList *current = list;
    while (current->next != NULL) {
        if (current->next->process == process) {
            ProcessList *next = current->next->next;
            free(current->next);
            current->next = next;
            return;
        }
        current = current->next;
    }
}

static void print_recursive(ProcessList *list) {
    if (list->next == NULL)
        return;
    print_recursive(list->next);
    printf("Process running with index %d\n", list->next->process->idx);
}

void processlist_print(ProcessList *list) {
    ProcessList *current = list;

    if (current->next == NULL) {
        printf("No background processes!\n");
        return;
    }

    // Recursive to easily print in reverse order
    print_recursive(current);
}
