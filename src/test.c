#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <limits.h>

int main() {

    printf("INT_MAX: %d\n", INT_MAX);
    printf("INT_MIN: %d\n", INT_MIN);
    printf("LONG_MAX: %ld\n", LONG_MAX);
    printf("LONG_MIN: %ld\n", LONG_MIN);

    return 0;
}
