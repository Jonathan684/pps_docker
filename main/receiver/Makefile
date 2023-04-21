CC=arm-linux-gnueabihf-gcc
CFLAGS=-mfloat-abi=hard --sysroot=$(HOME)/pluto-0.32.sysroot -std=gnu99 -g -Wall -Wextra
LIBS=-lpthread -liio -lm


receiver: program.o shutdown.o
	$(CC) $(CFLAGS) -o receiver program.o shutdown.o $(LIBS)  

program.o: program.c program.h shutdown.h definition.h
	$(CC) $(CFLAGS) -c program.c

shutdown.o:shutdown.c shutdown.h program.h definition.h
	$(CC) $(CFLAGS) -c shutdown.c


clean:
	rm  -f *.o receiver