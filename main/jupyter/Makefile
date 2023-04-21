CC=arm-linux-gnueabihf-gcc
CFLAGS=-mfloat-abi=hard --sysroot=$(HOME)/pluto-0.32.sysroot -std=gnu99 -g -Wall -Wextra
LIBS=-lpthread -liio -lm

all: jupyter2

jupyter2: jupyter2
	$(CC) $(CFLAGS) -o jupyter2 jupyter2.c $(LIBS)  

jupyter2.o: jupyter2.c
	$(CC) $(CFLAGS) -c jupyter2.c


clean:
	rm  -f *.o jupyter2