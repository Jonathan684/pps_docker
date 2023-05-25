CC=arm-linux-gnueabihf-gcc
CFLAGS=-mfloat-abi=hard --sysroot=$(HOME)/pluto-0.32.sysroot -std=gnu99 -g -Wall -Wextra
LIBS=-lpthread -liio -lm

all:transmitter # 2_sdr  #sdr_1 sdr_2 prueba_33 # buffer 

transmitter: transmitter
	$(CC) $(CFLAGS) -o transmitter transmitter.c $(LIBS)  

transmitter.o: transmitter.c
	$(CC) $(CFLAGS) -c transmitter.c

clean:
	rm  -f *.o transmitter
	
#rm  -f *.o  sdr_1 sdr_2 prueba_33