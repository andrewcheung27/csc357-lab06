CC = gcc
CFLAGS = -Wall -pedantic -g -O3


all: timeit


timeit: timeit.o
	$(CC) $(CFLAGS) -o timeit timeit.o


timeit.o: timeit.c
	$(CC) $(CFLAGS) -c -o timeit.o timeit.c


clean:
	rm -f *.o

server:
	ssh acheun29@unix1.csc.calpoly.edu

upload:
	scp -r ../csc357-lab06 acheun29@unix1.csc.calpoly.edu:csc357

download:
	scp -r acheun29@unix1.csc.calpoly.edu:csc357/csc357-lab06 /Users/andrewcheung/Documents/Cal\ Poly\ Stuff/csc357

