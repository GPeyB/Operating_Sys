all: shell

shell: *.c
	gcc -std=c99 -Wall -pedantic main.c scanner.c shell.c -o shell -g

clean:
	rm -f *~
	rm -f *.o
	rm -f shell
