all: shell

SHELL_FILES = $(filter-out test.c, $(wildcard *.c)) # exclude test.c from shell compilation

shell: *.c *.h
	gcc -std=c99 -Wall -pedantic $(SHELL_FILES) -o shell -g

test: test.c
	gcc -std=c99 -Wall -pedantic test.c -o test -g

clean:
	rm -f *~
	rm -f *.o
	rm -f shell
