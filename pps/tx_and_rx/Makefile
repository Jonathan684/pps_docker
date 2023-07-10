CC=arm-linux-gnueabihf-gcc
CFLAGS=-mfloat-abi=hard --sysroot=$(HOME)/pluto-0.32.sysroot -std=gnu99 -g -Wall -Wextra
LIBS=-lpthread -liio -lm
all: tx_rx 
tx_rx: tx_rx
	$(CC) $(CFLAGS) -o tx_rx tx_rx.c $(LIBS)
tx_rx.o: tx_rx.c
	$(CC) $(CFLAGS) -c tx_rx.c 

prueba_tx: prueba_tx
	$(CC) $(CFLAGS) -o prueba_tx prueba_tx.c $(LIBS)
prueba_tx.o: prueba_tx.c
	$(CC) $(CFLAGS) -c prueba_tx.c 
clean:
	rm  -f *.o tx_rx prueba_tx
#rm  -f *.o  sdr_1 sdr_2 prueba_33