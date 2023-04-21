CC=arm-linux-gnueabihf-gcc
CFLAGS=-mfloat-abi=hard --sysroot=$(HOME)/pluto-0.32.sysroot -std=gnu99 -g -Wall -Wextra
LIBS=-lpthread -liio -lm
all: attr_debug
#all:device input_output description attrs_channel input_output
current_config: current_config.o
	$(CC) $(CFLAGS) -o current_config current_config.o $(LIBS)  

current_config.o:current_config.c
	$(CC) $(CFLAGS) -c current_config.c

device :device.o
	$(CC) $(CFLAGS) -o device device.o $(LIBS)	
device.o:device.c
	$(CC) $(CFLAGS) -c device.c

input_output :input_output.o
	$(CC) $(CFLAGS) -o input_output input_output.o $(LIBS)	
input_output.o:input_output.c
	$(CC) $(CFLAGS) -c input_output.c

description :description.o
	$(CC) $(CFLAGS) -o description description.o $(LIBS)	
description.o:description.c
	$(CC) $(CFLAGS) -c description.c

attrs_channel :attrs_channel.o
	$(CC) $(CFLAGS) -o attrs_channel attrs_channel.o $(LIBS)	
attrs_channel.o:attrs_channel.c
	$(CC) $(CFLAGS) -c attrs_channel.c

probando :probando.o
	$(CC) $(CFLAGS) -o probando probando.o $(LIBS)	
probando.o:probando.c
	$(CC) $(CFLAGS) -c probando.c

attr_debug :attr_debug.o
	$(CC) $(CFLAGS) -o attr_debug attr_debug.o $(LIBS)	
attr_debug.o:attr_debug.c
	$(CC) $(CFLAGS) -c attr_debug.c

clean:
	rm  -f *.o current_config device input_output description attrs_channel probando