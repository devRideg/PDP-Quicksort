CFLAGS=-Wall -O3
LDFLAGS=-lm -pthread

Quicksort: Quicksort.o
	gcc -g -o Quicksort Quicksort.o $(LDFLAGS)

Quicksort.o: Quicksort.c
	gcc -g $(CFLAGS) -c Quicksort.c

run: Quicksort
	./Quicksort

debug:
	gdb ./Quicksort

valgrind:
	valgrind --leak-check=full ./Quicksort

clean:
	rm -f ./Quicksort *.o