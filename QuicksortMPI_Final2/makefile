CC = mpicc
CFLAGS= -std=c99 -g 
LDFLAGS= -lm 
OBJS = qsArray.o qsMPI.o Quicksort.o 

BIN = quicksort

Quicksort: $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJS) $(LDFLAGS)

Quicksort.o: Quicksort.c
	$(CC) $(CFLAGS) -c Quicksort.c $(LDFLAGS)

qsArray.o: qsArray.c
	$(CC) $(CFLAGS) -c qsArray.c $(LDFLAGS)

qsAlgorithm.o: qsAlgorithm.c
	$(CC) $(CFLAGS) -c qsAlgorithm.c $(LDFLAGS)

qsMPI.o: qsMPI.c
	$(CC) $(CFLAGS) -c qsMPI.c $(LDFLAGS)

run: Quicksort
	./Quicksort

checkSorted: qsArray.o checkSorted.o
	gcc -O3 -Wall -o checkSorted checkSorted.o qsArray.o -lm

checkSorted.o: checkSorted.c
	gcc -O3 -Wall -c checkSorted.c -lm

debug:
	gdb ./Quicksort

valgrind:
	valgrind --leak-check=full ./Quicksort

clean:
	rm -f $(BIN) checkSorted *.o

tfp: tfp.o qsMPI.o qsArray.o
	$(CC) $(CFLAGS) -o tfp tfp.o qsMPI.o qsArray.o $(LDFLAGS)

tfp.o: tfp.c
	$(CC) $(CFLAGS) -c tfp.c $(LDFLAGS)
