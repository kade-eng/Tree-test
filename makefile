CC = gcc
CFLAGS = -std=c99 -Wall -pedantic

all: tree.o ttest

tree.o: tree.c
	$(CC) $(CFLAGS) -c tree.c -o tree.o

ttest.o: ttest.c
	$(CC) $(CFLAGS) -c ttest.c -o ttest.o

ttest: ttest.o tree.o tree.h
	$(CC) $(CFLAGS) ttest.o tree.o -o ttest

clean:
	rm *.o tree.o ttest
