CC=gcc
CFLAGS=-I.

fiter: filter.o
	$(CC) -o filter filter.o

.PHONY: clean

clean:
	rm -rf *.o
	
