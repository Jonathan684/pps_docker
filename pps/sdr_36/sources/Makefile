CC=arm-linux-gnueabihf-gcc
CFLAGS=-mfloat-abi=hard --sysroot=$(HOME)/pluto-0.32.sysroot -std=gnu99 -g -Wall -Wextra
LIBS=-lpthread -liio -lm
all: tx_rx 
tx_rx: tx_rx
	$(CC) $(CFLAGS) -o tx_rx tx_rx.c $(LIBS)
tx_rx.o: tx_rx.c
	$(CC) $(CFLAGS) -c tx_rx.c 
clean:
	rm  -f *.o tx_rx
#rm  -f *.o  sdr_1 sdr_2 prueba_33