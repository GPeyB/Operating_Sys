all: shell

# exclude test.c from shell compilation
SHELL_FILES = $(filter-out test.c, $(wildcard *.c))

shell: *.c *.h
	gcc -std=c99 -Wall -pedantic $(SHELL_FILES) -o shell -g -DBONUS -D_POSIX_C_SOURCE

test: test.c
	gcc -std=c99 -Wall -pedantic test.c -o test -g -D_POSIX_C_SOURCE

clean:
	rm -f *~
	rm -f *.o
	rm -f shell
	rm -f test
