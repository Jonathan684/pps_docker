CC=arm-linux-gnueabihf-gcc
CFLAGS=-mfloat-abi=hard --sysroot=$(HOME)/pluto-0.32.sysroot -std=gnu99 -g -Wall -Wextra
LIBS=-lpthread -liio -lm
all: sdr_control
#all:device input_output description attrs_channel input_output

sdr_control :sdr_control.o
	$(CC) $(CFLAGS) -o sdr_control sdr_control.o $(LIBS)	
sdr_control.o:sdr_control.c
	$(CC) $(CFLAGS) -c sdr_control.c

clean:
	rm  -f *.o sdr_control