calc: integer.o main.c
	gcc -ansi -pedantic -Wall -o calc integer.o main.c -lm

integer.o: integer.c
	gcc -ansi -pedantic -Wall -c integer.c -lm

clean: 
	rm *.o
	rm calc


