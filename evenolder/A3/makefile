CC = mpicc
CFLAGS= -O3 -std=c99 -g 
LDFLAGS= -lm 
OBJS = qsArray.o qsMPI.o Quicksort.o 

BIN = quicksort

Quicksort: $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJS) $(LDFLAGS)

Quicksort.o: Quicksort.c
	$(CC) $(CFLAGS) -c Quicksort.c $(LDFLAGS)

qsArray.o: qsArray.c
	$(CC) $(CFLAGS) -c qsArray.c $(LDFLAGS)

qsMPI.o: qsMPI.c
	$(CC) $(CFLAGS) -c qsMPI.c $(LDFLAGS)

clean:
	rm -f $(BIN) checkSorted *.o

