CC=arm-linux-gnueabihf-gcc
CFLAGS=-mfloat-abi=hard --sysroot=$(HOME)/pluto-0.32.sysroot -std=gnu99 -g -Wall -Wextra
LIBS=-lpthread -liio -lm

all: filter filter_1

filter: filter
	$(CC) $(CFLAGS) -o filter filter.c $(LIBS)  

filter.o: filter.c
	$(CC) $(CFLAGS) -c filter.c
filter_1: filter_1
	$(CC) $(CFLAGS) -o filter_1 filter_1.c $(LIBS)  

filter_1.o: filter_1.c
	$(CC) $(CFLAGS) -c filter_1.c
clean:
	rm  -f *.o filter filter_1