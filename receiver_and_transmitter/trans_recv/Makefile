CC=arm-linux-gnueabihf-gcc
CFLAGS=-mfloat-abi=hard --sysroot=$(HOME)/pluto-0.32.sysroot -std=gnu99 -g -Wall -Wextra
LIBS=-lpthread -liio -lm

all:receiver_v1 transmitter_v1

receiver_v1: receiver_v1
	$(CC) $(CFLAGS) -o receiver_v1 receiver_v1.c $(LIBS)  

receiver_v1.o: receiver_v1.c
	$(CC) $(CFLAGS) -c receiver_v1.c
transmitter_v1: transmitter_v1
	$(CC) $(CFLAGS) -o transmitter_v1 transmitter_v1.c $(LIBS)  

transmitter_v1.o: transmitter_v1.c
	$(CC) $(CFLAGS) -c transmitter_v1.c
clean:
	rm  -f *.o receiver_v1 transmitter_v1
	
#rm  -f *.o  sdr_1 sdr_2 prueba_33