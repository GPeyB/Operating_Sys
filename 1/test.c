#include <stdio.h>
#include <stdlib.h>

typedef struct Int {
    int a;
    int b;
} Int;

void update(Int **p) {
    *p = malloc(sizeof(Int));
    Int *q = *p;
    q->a = 1;
    q->b = 2;
}

int main() {

    Int *a = NULL;
    update(&a);
    printf("%d %d\n", a->a, a->b);
    free(a);

    return 0;
}
