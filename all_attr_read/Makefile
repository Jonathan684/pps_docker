CC=arm-linux-gnueabihf-gcc
CFLAGS=-mfloat-abi=hard --sysroot=$(HOME)/pluto-0.32.sysroot -std=gnu99 -g -Wall -Wextra
LIBS=-lpthread -liio -lm
all: all_attr_read
#all:device input_output description attrs_channel input_output

all_attr_read :all_attr_read.o
	$(CC) $(CFLAGS) -o all_attr_read all_attr_read.o $(LIBS)	
all_attr_read.o:all_attr_read.c
	$(CC) $(CFLAGS) -c all_attr_read.c

clean:
	rm  -f *.o all_attr_read