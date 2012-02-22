calc: integer.o main.c
	gcc -ansi -pedantic -Wall -o calc integer.o main.c

integer.o: integer.c
	gcc -c integer.c

clean: 
	rm *.o
	rm calc


