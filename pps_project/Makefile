CC=arm-linux-gnueabihf-gcc
CFLAGS=-mfloat-abi=hard --sysroot=$(HOME)/pluto-0.32.sysroot -std=gnu99 -g -Wall -Wextra
LIBS=-lpthread -liio -lm

all:1_sdr # 2_sdr  #sdr_1 sdr_2 prueba_33 # buffer 

sdr_1: sdr_1
	$(CC) $(CFLAGS) -o sdr_1 sdr_1.c $(LIBS)  

sdr_1.o: sdr_1.c
	$(CC) $(CFLAGS) -c sdr_1.c

sdr_2: sdr_2
	$(CC) $(CFLAGS) -o sdr_2 sdr_2.c $(LIBS)  

sdr_2.o: sdr_2.c
	$(CC) $(CFLAGS) -c sdr_2.c

buffer: buffer
	$(CC) $(CFLAGS) -o buffer buffer.c $(LIBS)  

buffer.o: buffer.c
	$(CC) $(CFLAGS) -c buffer.c

prueba_33: prueba_33
	$(CC) $(CFLAGS) -o prueba_33 prueba_33.c $(LIBS)  

prueba_33.o: prueba_33.c
	$(CC) $(CFLAGS) -c prueba_33.c



2_sdr: 2_sdr
	$(CC) $(CFLAGS) -o 2_sdr 2_sdr.c $(LIBS)  

2_sdr.o: 2_sdr.c
	$(CC) $(CFLAGS) -c 2_sdr.c

1_sdr: 1_sdr
	$(CC) $(CFLAGS) -o 1_sdr 1_sdr.c $(LIBS)  

1_sdr.o: 1_sdr.c
	$(CC) $(CFLAGS) -c 1_sdr.c

clean:
	rm  -f *.o 1_sdr 2_sdr
	
#rm  -f *.o  sdr_1 sdr_2 prueba_33