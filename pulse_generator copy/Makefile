CC=arm-linux-gnueabihf-gcc
CFLAGS=-mfloat-abi=hard --sysroot=$(HOME)/pluto-0.32.sysroot -std=gnu99 -g -Wall -Wextra
LIBS=-lpthread -liio -lm

all:transmit_pulse receive_pulse

transmit_pulse: transmit_pulse
	$(CC) $(CFLAGS) -o transmit_pulse transmit_pulse.c $(LIBS)

transmit_pulse.o: transmit_pulse.c
	$(CC) $(CFLAGS) -c transmit_pulse.c 

receive_pulse: receive_pulse
	$(CC) $(CFLAGS) -o receive_pulse receive_pulse.c $(LIBS)

receive_pulse.o: receive_pulse.c
	$(CC) $(CFLAGS) -c receive_pulse.c 

clean:
	rm  -f *.o transmit_pulse receive_pulse
	
#rm  -f *.o  sdr_1 sdr_2 prueba_33