CC=arm-linux-gnueabihf-gcc
CFLAGS=-mfloat-abi=hard --sysroot=$(HOME)/pluto-0.32.sysroot -std=gnu99 -g -Wall -Wextra
LIBS=-lpthread -liio -lm
all: sdr_transmitter
#all:device input_output description attrs_channel input_output

sdr_transmitter :sdr_transmitter.o
	$(CC) $(CFLAGS) -o sdr_transmitter sdr_transmitter.o $(LIBS)	
sdr_transmitter.o:sdr_transmitter.c
	$(CC) $(CFLAGS) -c sdr_transmitter.c

clean:
	rm  -f *.o sdr_transmitter