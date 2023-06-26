CC=arm-linux-gnueabihf-gcc
CFLAGS=-mfloat-abi=hard --sysroot=$(HOME)/pluto-0.32.sysroot -std=gnu99 -g -Wall -Wextra
LIBS=-lpthread -liio -lm

all:1_transmit_pulse 2_receive_pulse

1_transmit_pulse: 1_transmit_pulse
	$(CC) $(CFLAGS) -o 1_transmit_pulse 1_transmit_pulse.c $(LIBS)

1_transmit_pulse.o: 1_transmit_pulse.c
	$(CC) $(CFLAGS) -c 1_transmit_pulse.c 

2_receive_pulse: 2_receive_pulse
	$(CC) $(CFLAGS) -o 2_receive_pulse 2_receive_pulse.c $(LIBS)

2_receive_pulse.o: 2_receive_pulse.c
	$(CC) $(CFLAGS) -c 2_receive_pulse.c 

clean:
	rm  -f *.o 1_transmit_pulse 2_receive_pulse output_pulse.txt
	
#rm  -f *.o  sdr_1 sdr_2 prueba_33