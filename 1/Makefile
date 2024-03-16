all: shell

shell: *.c *.h
	gcc -std=c99 -Wall -pedantic *.c -o shell -g

clean:
	rm -f *~
	rm -f *.o
	rm -f shell
