CC=arm-linux-gnueabihf-gcc
CFLAGS=-mfloat-abi=hard --sysroot=$(HOME)/pluto-0.32.sysroot -std=gnu99 -g -Wall -Wextra
LIBS=-lpthread -liio -lm

all:receiver transmitter

receiver: receiver
	$(CC) $(CFLAGS) -o receiver receiver.c $(LIBS)  

receiver.o: receiver.c
	$(CC) $(CFLAGS) -c receiver.c
transmitter: transmitter
	$(CC) $(CFLAGS) -o transmitter transmitter.c $(LIBS)  

transmitter.o: transmitter.c
	$(CC) $(CFLAGS) -c transmitter.c
clean:
	rm  -f *.o receiver transmitter
	
#rm  -f *.o  sdr_1 sdr_2 prueba_33