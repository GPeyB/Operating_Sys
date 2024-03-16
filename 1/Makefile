all: shell

shell: *.c *.h
	gcc -std=c99 -Wall -pedantic main.c scanner.c shell.c structures.c -o shell -g

test: test.c
	gcc -std=c99 -Wall -pedantic test.c -o test -g

clean:
	rm -f *~
	rm -f *.o
	rm -f shell
