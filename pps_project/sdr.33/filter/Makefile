CC=arm-linux-gnueabihf-gcc
CFLAGS=-mfloat-abi=hard --sysroot=$(HOME)/pluto-0.32.sysroot -std=gnu99 -g -Wall -Wextra
LIBS=-lpthread -liio -lm

all: filter

filter: filter
	$(CC) $(CFLAGS) -o filter filter.c $(LIBS)  

filter.o: filter.c
	$(CC) $(CFLAGS) -c filter.c
clean:
	rm  -f *.o filter