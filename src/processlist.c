#include "processlist.h"

#include <stddef.h>

ProcessList *processlist_create() {
    ProcessList *list = (ProcessList *)malloc(sizeof(ProcessList));
    list->pid = -1;
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

void processlist_add(ProcessList *list, int pid) {
    ProcessList *current = list;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = (ProcessList *)malloc(sizeof(ProcessList));
    current->next->pid = pid;
    current->next->next = NULL;
}

void processlist_remove(ProcessList *list, int pid) {
    ProcessList *current = list;
    while (current->next != NULL) {
        if (current->next->pid == pid) {
            ProcessList *next = current->next->next;
            free(current->next);
            current->next = next;
            return;
        }
        current = current->next;
    }
}

void processlist_print(ProcessList *list) {
    ProcessList *current = list;
    while (current->next != NULL) {
        current = current->next;
        printf("%d\n", current->pid);
    }
}
