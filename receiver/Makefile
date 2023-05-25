CC=arm-linux-gnueabihf-gcc
CFLAGS=-mfloat-abi=hard --sysroot=$(HOME)/pluto-0.32.sysroot -std=gnu99 -g -Wall -Wextra
LIBS=-lpthread -liio -lm

all:receiver 

receiver: receiver
	$(CC) $(CFLAGS) -o receiver receiver.c $(LIBS)  

receiver.o: receiver.c
	$(CC) $(CFLAGS) -c receiver.c

clean:
	rm  -f *.o receiver
	
#rm  -f *.o  sdr_1 sdr_2 prueba_33