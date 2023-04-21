CC=arm-linux-gnueabihf-gcc
CFLAGS=-mfloat-abi=hard --sysroot=$(HOME)/pluto-0.32.sysroot -std=gnu99 -g -Wall -Wextra
LIBS=-lpthread -liio -lm

all:sdr_1 sdr_2  # buffer 

sdr_1: sdr_1
	$(CC) $(CFLAGS) -o sdr_1 sdr_1.c $(LIBS)  

sdr_1.o: sdr_1.c
	$(CC) $(CFLAGS) -c sdr_1.c

sdr_2: sdr_2
	$(CC) $(CFLAGS) -o sdr_2 sdr_2.c $(LIBS)  

sdr_2.o: sdr_2.c
	$(CC) $(CFLAGS) -c sdr_2.c

buffer: buffer
	$(CC) $(CFLAGS) -o buffer buffer.c $(LIBS)  

buffer.o: buffer.c
	$(CC) $(CFLAGS) -c buffer.c
clean:
	rm  -f *.o sdr_1 sdr_2