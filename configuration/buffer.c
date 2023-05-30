
#include <iio.h>
#include <stdio.h>
#include <stdlib.h>
#include <iio.h>
#include <math.h>
#include <string.h>
// int main(){

// const int buffer_size = 1024;

// // Open the IIO context and get the device
// struct iio_context *ctx = NULL;
// ctx = iio_create_default_context();
// //struct iio_context *ctx = iio_create_context_from_uri("ip:pluto.local");
// struct iio_device *tx_dev = iio_context_find_device(ctx, "cf-ad9361-dds-core-lpc");

// // Create the transmit buffer
// struct iio_buffer *txbuf = iio_device_create_buffer(tx_dev, buffer_size, false);

// if (!txbuf) {
//  		perror("Could not create RX buffer 23");
//         return 0;
 		
// }

// // Define the pulse parameters
// double pulse_frequency = 1000000;  // Hz
// double pulse_duration = 0.0001;    // seconds
// double pulse_amplitude = 32767;    // maximum amplitude for 16-bit signed integer

// // Calculate the number of samples in the pulse waveform
// int num_samples = (int)(pulse_duration * pulse_frequency);

// // Generate the pulse waveform and write it to the buffer
// // Generate the pulse waveform and write it to the buffer
// // Generate the pulse waveform and write it to the buffer
// //struct iio_data_format fmt = iio_buffer_get_attrs(txbuf);
// //struct iio_data_format fmt = iio_buffer_get_data_format(txbuf);
// //int bytes_per_sample = iio_buffer_get_bytes_per_sample(txbuf);
// for (int i = 0; i < buffer_size; i++) {
//     int16_t *buf = iio_buffer_start(txbuf);
//     if (i < num_samples) {
//         buf[2*i] = (int16_t)pulse_amplitude;
//         buf[2*i+1] = (int16_t)pulse_amplitude;
//     } else {
//         buf[2*i] = 0;
//         buf[2*i+1] = 0;
//     }
// }

// // Release resources
// // Release resources
// iio_buffer_destroy(txbuf);
// //iio_device_destroy(tx_dev);
// iio_context_destroy(ctx);
// return 0;
// }


int receive(struct iio_context *ctx);
int transmit(struct iio_context *ctx);

int main (int argc, char **argv)
{
	struct iio_context *ctx;
	struct iio_device *phy;
 
	ctx = iio_create_context_from_uri("ip:192.168.2.1");
 
	phy = iio_context_find_device(ctx, "ad9361-phy");
 
	iio_channel_attr_write_longlong(
		iio_device_find_channel(phy, "altvoltage0", true),
		"frequency",
		2400000000); /* RX LO frequency 2.4GHz */
 
	iio_channel_attr_write_longlong(
		iio_device_find_channel(phy, "voltage0", false),
		"sampling_frequency",
		5000000); /* RX baseband rate 5 MSPS */
    transmit(ctx);
	
    receive(ctx);
 
	iio_context_destroy(ctx);
 
	return 0;
} 
int receive(struct iio_context *ctx)
{
	struct iio_device *dev;
	struct iio_channel *rx0_i, *rx0_q;
	struct iio_buffer *rxbuf;
 
	dev = iio_context_find_device(ctx, "cf-ad9361-lpc");
 
	rx0_i = iio_device_find_channel(dev, "voltage0", 0);
	rx0_q = iio_device_find_channel(dev, "voltage1", 0);
 
	iio_channel_enable(rx0_i);
	iio_channel_enable(rx0_q);
 
	rxbuf = iio_device_create_buffer(dev, 4096, false);
	if (!rxbuf) {
		perror("Could not create RX buffer");
//		shutdown();
        iio_context_destroy(ctx);
 
        
	}
 
	while (true) {
		void *p_dat, *p_end, *t_dat;
		ptrdiff_t p_inc;
 
		iio_buffer_refill(rxbuf);
 
		p_inc = iio_buffer_step(rxbuf);
		p_end = iio_buffer_end(rxbuf);
 
		for (p_dat = iio_buffer_first(rxbuf, rx0_i); p_dat < p_end; p_dat += p_inc, t_dat += p_inc) {
			const int16_t i = ((int16_t*)p_dat)[0]; // Real (I)
			const int16_t q = ((int16_t*)p_dat)[1]; // Imag (Q)
 
			/* Process here */
 
		}
	}
 
	iio_buffer_destroy(rxbuf);
 
}

int transmit(struct iio_context *ctx)
{
    struct iio_device *dev;
    struct iio_channel *tx0_i, *tx0_q;
    struct iio_buffer *txbuf;

    dev = iio_context_find_device(ctx, "cf-ad9361-dds-core-lpc");

    tx0_i = iio_device_find_channel(dev, "voltage0", true);
    tx0_q = iio_device_find_channel(dev, "voltage1", true);

    iio_channel_enable(tx0_i);
    iio_channel_enable(tx0_q);

    txbuf = iio_device_create_buffer(dev, 4096, false);
    if (!txbuf) {
        perror("Could not create TX buffer");
        //shutdown();
        iio_context_destroy(ctx);
 
    }
    printf("A transmitir :: \n");
    while (true) {
        void *p_dat, *p_end, *t_dat;
        ptrdiff_t p_inc;

        iio_buffer_refill(txbuf);

        p_inc = iio_buffer_step(txbuf);
        p_end = iio_buffer_end(txbuf);

        for (p_dat = iio_buffer_first(txbuf, tx0_i), t_dat = iio_buffer_first(txbuf, tx0_q);
             p_dat < p_end; p_dat += p_inc, t_dat += p_inc) {
            int16_t *buf_i = p_dat;
            int16_t *buf_q = t_dat;

            /* Construct your signal here */
            buf_i[0] = 1000;
            buf_q[0] = 0;

        }

        iio_buffer_push(txbuf);
    }

    iio_buffer_destroy(txbuf);
}