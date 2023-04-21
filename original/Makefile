CC=arm-linux-gnueabihf-gcc
CFLAGS=-mfloat-abi=hard --sysroot=$(HOME)/pluto-0.32.sysroot -std=gnu99 -g -Wall -Wextra
LIBS=-lpthread -liio -lm
all: ejemplo


ejemplo :ejemplo.o
	$(CC) $(CFLAGS) -o ejemplo ejemplo.o $(LIBS)	
ejemplo.o:ejemplo.c
	$(CC) $(CFLAGS) -c ejemplo.c

clean:
	rm  -f *.o ejemplo