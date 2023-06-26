CC=arm-linux-gnueabihf-gcc
CFLAGS=-mfloat-abi=hard --sysroot=$(HOME)/pluto-0.32.sysroot -std=gnu99 -g -Wall -Wextra
LIBS=-lpthread -liio -lm

all: 2_receive_pulse


2_receive_pulse: 2_receive_pulse
	$(CC) $(CFLAGS) -o 2_receive_pulse 2_receive_pulse.c $(LIBS)

2_receive_pulse.o: 2_receive_pulse.c
	$(CC) $(CFLAGS) -c 2_receive_pulse.c 

clean:
	rm  -f *.o 2_receive_pulse output_pulse.txt
	
#rm  -f *.o  sdr_1 sdr_2 prueba_33