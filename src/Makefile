all: shell

# exclude test.c from shell compilation
SHELL_FILES = $(filter-out test.c, $(wildcard *.c))

shell: *.c *.h
	gcc -std=c99 -Wall -pedantic $(SHELL_FILES) -o shell -g -DBONUS

test: test.c
	gcc -std=c99 -Wall -pedantic test.c -o test -g

clean:
	rm -f *~
	rm -f *.o
	rm -f shell
	rm -f test
