#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iio.h>

#define TX_BUF_LEN 1024

int main()
{
    struct iio_context *ctx;
    struct iio_device *dev;
    struct iio_buffer *txbuf;
    char *txbuf_data;
    ssize_t nbytes;

    // Create a context for communicating with the AD9361
    ctx = iio_create_context_from_uri("ip:192.168.2.1");

    // Get a handle to the TX device
    dev = iio_context_find_device(ctx, "cf-ad9361-dds-core-lpc");

    // Configure the TX device
    struct iio_channel *tx0_i = iio_device_find_channel(dev, "voltage0", true);
    struct iio_channel *tx0_q = iio_device_find_channel(dev, "voltage1", true);

    // Enable cyclic mode
    // iio_channel_attr_write_bool(tx0_i, "cyclic", true);
    // iio_channel_attr_write_bool(tx0_q, "cyclic", true);
    
    // Create a TX buffer
    txbuf = iio_device_create_buffer(dev, 1024*1024, false);
    if (!txbuf) {
        perror( "Unable to create TX buffer.\n"); // Unable to create TX buffer.: Invalid argument
        return EXIT_FAILURE;
    }

    // Get a pointer to the TX buffer data
    txbuf_data = (char *)iio_buffer_start(txbuf);

    // Generate some test data to send
    int16_t data[TX_BUF_LEN];
    for (int i = 0; i < TX_BUF_LEN; i++) {
        data[i] = i % 65535;
    }

    // Copy the test data into the TX buffer
    for (int i = 0; i < TX_BUF_LEN; i += 2) {
        ((int16_t *)txbuf_data)[i] = data[i];
        ((int16_t *)txbuf_data)[i + 1] = data[i + 1];
    }

    // Send the data
    nbytes = iio_buffer_push(txbuf);
    if (nbytes < 0) {
        //fprintf(stderr, "Error sending data: %s\n", strerror(-nbytes));
        perror("Error sending data: \n");
		return 1;
    }

    // Cleanup
    iio_buffer_destroy(txbuf);
    iio_context_destroy(ctx);

    return EXIT_SUCCESS;
}






















// // SPDX-License-Identifier: GPL-2.0-or-later
// /*
//  * libiio - AD9361 IIO streaming example
//  *
//  * Copyright (C) 2014 IABG mbH
//  * Author: Michael Feilen <feilen_at_iabg.de>
//  **/
//  #include "program.h"
// /* simple configuration and streaming */
// /* usage:
//  * Default context, assuming local IIO devices, i.e., this script is run on ADALM-Pluto for example
//  * $./a.out
//  * URI context, find out the uri by typing `iio_info -s` at the command line of the host PC
//  $ ./a.out usb:x.x.x
//  */

// int main (int argc, char **argv)
// {

// 	load_structure(ctx,rx0_i ,rx0_q,tx0_i,tx0_q,rxbuf,txbuf);
// 	// Streaming devices
// 	struct iio_device *tx;
// 	struct iio_device *rx;

// 	// RX and TX sample counters
// 	//size_t nrx = 0;
// 	size_t ntx = 0;

// 	// Stream configurations
// 	struct stream_cfg rxcfg;
// 	struct stream_cfg txcfg;

// 	// Listen to ctrl+c and IIO_ENSURE
// 	signal(SIGINT, handle_sig);

// 	// RX stream config
// 	rxcfg.bw_hz = MHZ(2);   // 2 MHz rf bandwidth
// 	rxcfg.fs_hz = MHZ(2.5);   // 2.5 MS/s rx sample rate
// 	rxcfg.lo_hz = GHZ(2.5); // 2.5 GHz rf frequency
// 	rxcfg.rfport = "A_BALANCED"; // port A (select for rf freq.)

// 	// TX stream config
// 	txcfg.bw_hz = MHZ(1.5); // 1.5 MHz rf bandwidth
// 	txcfg.fs_hz = MHZ(2.5);   // 2.5 MS/s tx sample rate
// 	txcfg.lo_hz = GHZ(2.5); // 2.5 GHz rf frequency
// 	txcfg.rfport = "A"; // port A (select for rf freq.)

// 	printf("* Acquiring IIO context\n");
// 	if (argc == 1) {
// 		IIO_ENSURE((ctx = iio_create_default_context()) && "No context");
// 	}
// 	else if (argc == 2) {
// 		IIO_ENSURE((ctx = iio_create_context_from_uri(argv[1])) && "No context");
// 	}
// 	IIO_ENSURE(iio_context_get_devices_count(ctx) > 0 && "No devices");

// 	printf("* Acquiring AD9361 streaming devices\n");
// 	IIO_ENSURE(get_ad9361_stream_dev(TX, &tx) && "No tx dev found");
// 	IIO_ENSURE(get_ad9361_stream_dev(RX, &rx) && "No rx dev found");

// 	printf("* Configuring AD9361 for streaming\n");
// 	IIO_ENSURE(cfg_ad9361_streaming_ch(&rxcfg, RX, 0) && "RX port 0 not found");
// 	IIO_ENSURE(cfg_ad9361_streaming_ch(&txcfg, TX, 0) && "TX port 0 not found");

// 	printf("* Initializing AD9361 IIO streaming channels\n");
// 	IIO_ENSURE(get_ad9361_stream_ch(RX, rx, 0, &rx0_i) && "RX chan i not found");
// 	IIO_ENSURE(get_ad9361_stream_ch(RX, rx, 1, &rx0_q) && "RX chan q not found");
// 	IIO_ENSURE(get_ad9361_stream_ch(TX, tx, 0, &tx0_i) && "TX chan i not found");
// 	IIO_ENSURE(get_ad9361_stream_ch(TX, tx, 1, &tx0_q) && "TX chan q not found");

// 	printf("* Enabling IIO streaming channels\n");
// 	iio_channel_enable(rx0_i);
// 	iio_channel_enable(rx0_q);
// 	iio_channel_enable(tx0_i);
// 	iio_channel_enable(tx0_q);

// 	printf("* Creating non-cyclic IIO buffers with 1 MiS\n");
// 	rxbuf = iio_device_create_buffer(rx, 1024*1024, false);
// 	if (!rxbuf) {
// 		perror("Could not create RX buffer");
// 		shutdown();
// 	}
// 	txbuf = iio_device_create_buffer(tx, 1024*1024, false);
// 	if (!txbuf) {
// 		perror("Could not create TX buffer");
// 		shutdown();
// 	}

// 	/// Set the loopback mode to RF loopback//////////////////
// 	// struct iio_device *dev =get_ad9361_phy()
//     // const char *loopback_mode = "rf";
    
// 	// // Set the loopback mode to RF loopback
//     // const char *loopback_mode = "rf";
//     // int ret = iio_device_attr_write(dev, "loopback", loopback_mode);
//     // if (ret < 0) {
//     //     printf("Failed to set loopback mode: %s\n", strerror(-ret));
//     //     shutdown();
//     // }
// 	////////////////////////////////////////////////////////

// 	printf("* Starting IO streaming (press CTRL+C to cancel)\n");
// 	while (!stop)
// 	{
// 		ssize_t nbytes_rx, nbytes_tx;
// 		char *p_dat, *p_end;
// 		ptrdiff_t p_inc;

// 		// Schedule TX buffer
// 		nbytes_tx = iio_buffer_push(txbuf);
// 		if (nbytes_tx < 0) { printf("Error pushing buf %d\n", (int) nbytes_tx); 
// 		shutdown();
// 		 }

// 		// Refill RX buffer
// 		nbytes_rx = iio_buffer_refill(rxbuf);
// 		if (nbytes_rx < 0) 
// 		{ printf("Error refilling buf %d\n",(int) nbytes_rx); 
// 			shutdown();
// 		 }

// 		// // READ: Get pointers to RX buf and read IQ from RX buf port 0
// 		// p_inc = iio_buffer_step(rxbuf);
// 		// p_end = iio_buffer_end(rxbuf);
// 		// for (p_dat = (char *)iio_buffer_first(rxbuf, rx0_i); p_dat < p_end; p_dat += p_inc) {
// 		// 	// Example: swap I and Q
// 		// 	const int16_t i = ((int16_t*)p_dat)[0]; // Real (I)
// 		// 	const int16_t q = ((int16_t*)p_dat)[1]; // Imag (Q)
// 		// 	((int16_t*)p_dat)[0] = q;
// 		// 	((int16_t*)p_dat)[1] = i;
// 		// }

// 		// WRITE: Get pointers to TX buf and write IQ to TX buf port 0
// 		p_inc = iio_buffer_step(txbuf);
// 		p_end = iio_buffer_end(txbuf);
// 		for (p_dat = (char *)iio_buffer_first(txbuf, tx0_i); p_dat < p_end; p_dat += p_inc) {
// 			// Example: fill with zeros
// 			// 12-bit sample needs to be MSB aligned so shift by 4
// 			// https://wiki.analog.com/resources/eval/user-guides/ad-fmcomms2-ebz/software/basic_iq_datafiles#binary_format
// 			((int16_t*)p_dat)[0] = 0 << 4; // Real (I)
// 			((int16_t*)p_dat)[1] = 0 << 4; // Imag (Q)
// 		}

// 		// Sample counter increment and status output
// 		//nrx += nbytes_rx / iio_device_get_sample_size(rx);
// 		ntx += nbytes_tx / iio_device_get_sample_size(tx);
		
//         printf("TX %8.2f MSmp\n", ntx/1e6);
// 	}

// 	shutdown();

// 	return 0;
// }
