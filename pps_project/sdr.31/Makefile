CC=arm-linux-gnueabihf-gcc
CFLAGS=-mfloat-abi=hard --sysroot=$(HOME)/pluto-0.32.sysroot -std=gnu99 -g -Wall -Wextra
LIBS=-lpthread -liio -lm

all: tx_rx_pulse

tx_rx_pulse: tx_rx_pulse
	$(CC) $(CFLAGS) -o tx_rx_pulse tx_rx_pulse.c $(LIBS)

tx_rx_pulse.o: tx_rx_pulse.c
	$(CC) $(CFLAGS) -c tx_rx_pulse.c 

clean:
	rm  -f *.o tx_rx_pulse output_pulse.txt
	
#rm  -f *.o  sdr_1 sdr_2 prueba_33