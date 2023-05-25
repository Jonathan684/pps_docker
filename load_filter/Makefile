CC=arm-linux-gnueabihf-gcc
CFLAGS=-mfloat-abi=hard --sysroot=$(HOME)/pluto-0.32.sysroot -std=gnu99 -g -Wall -Wextra
LIBS=-lpthread -liio -lm

all: filter read_filter#load_filter

load_filter: load_filter
	$(CC) $(CFLAGS) -o load_filter load_filter.c $(LIBS)  

load_filter.o: load_filter.c
	$(CC) $(CFLAGS) -c load_filter.c

filter: filter
	$(CC) $(CFLAGS) -o filter filter.c $(LIBS)  

filter.o: filter.c
	$(CC) $(CFLAGS) -c filter.c

read_filter: read_filter
	$(CC) $(CFLAGS) -o read_filter read_filter.c $(LIBS)  

read_filter.o: read_filter.c
	$(CC) $(CFLAGS) -c read_filter.c

clean:
	rm  -f *.o read_filter filter load_filter
	
#rm  -f *.o  sdr_1 sdr_2 prueba_33